/*++
Copyright (c) Microsoft Corporation
Licensed under the MIT license.

Module Name:
- JsonUtils.h

Abstract:
- Helpers for the TerminalApp project
Author(s):
- Mike Griese - August 2019

--*/
#pragma once

#include <windef.h>
#include <string>
#include <optional>
#include <json.h>

#include "../types/inc/utils.hpp"

namespace winrt
{
    struct guid;
}

namespace TerminalApp::JsonUtils
{
    namespace Details
    {
        template<typename T>
        struct ConversionTrait
        {
            // FromJson is not defined so as to cause a compile error (which forces a specialization)
            /*
            static Json::Value ToJson(T&& value) {
                Json::Value val{ value };
                return val;
            }
        */
        };

        template<>
        struct ConversionTrait<std::string>
        {
            static std::string FromJson(const Json::Value& json)
            {
                return json.asString();
            }
        };

        template<>
        struct ConversionTrait<std::wstring>
        {
            static std::wstring FromJson(const Json::Value& json)
            {
                return GetWstringFromJson(json);
            }
        };

        template<>
        struct ConversionTrait<bool>
        {
            static bool FromJson(const Json::Value& json)
            {
                return json.asBool();
            }
        };

        template<>
        struct ConversionTrait<COLORREF>
        {
            static COLORREF FromJson(const Json::Value& json)
            {
                return ::Microsoft::Console::Utils::ColorFromHexString(json.asString());
            }
        };

        template<>
        struct ConversionTrait<int>
        {
            static int FromJson(const Json::Value& json)
            {
                return json.asInt();
            }
        };

        template<>
        struct ConversionTrait<unsigned int>
        {
            static unsigned int FromJson(const Json::Value& json)
            {
                return json.asUInt();
            }
        };

        template<>
        struct ConversionTrait<float>
        {
            static float FromJson(const Json::Value& json)
            {
                return json.asFloat();
            }
        };

        template<>
        struct ConversionTrait<double>
        {
            static double FromJson(const Json::Value& json)
            {
                return json.asFloat();
            }
        };

        template<>
        struct ConversionTrait<GUID>
        {
            static GUID FromJson(const Json::Value& json)
            {
                return ::Microsoft::Console::Utils::GuidFromString(GetWstringFromJson(json.asString()));
            }
        };

        template<>
        struct ConversionTrait<winrt::guid>
        {
            static winrt::guid FromJson(const Json::Value& json)
            {
                return ConversionTrait<GUID>::FromJson(json);
            }
        };
    }

    template<typename T, typename TBase>
    struct KeyValueMapper
    {
        using pair_type = std::pair<std::string_view, T>;
        static T FromJson(const Json::Value& json)
        {
            auto name = json.asString();
            for (const auto& pair : TBase::mappings)
            {
                if (pair.first == name)
                {
                    return pair.second;
                }
            }
            // the first mapping is the "Default"
            return TBase::mappings[0].second;
        }
    };

    // Method Description:
    // - Helper that can be used for retrieving an optional value from a json
    //   object, and parsing it's value to layer on a given target object.
    //    - If the key we're looking for _doesn't_ exist in the json object,
    //      we'll leave the target object unmodified.
    //    - If the key exists in the json object, but is set to `null`, then
    //      we'll instead set the target back to nullopt.
    // - Each caller should provide a conversion function that takes a
    //   Json::Value and returns an object of the same type as target.
    // Arguments:
    // - json: The json object to search for the given key
    // - key: The key to look for in the json object
    // - target: the optional object to recieve the value from json
    // - conversion: a std::function<T(const Json::Value&)> which can be used to
    //   convert the Json::Value to the appropriate type.
    // Return Value:
    // - <none>
    template<typename T, typename F>
    void GetOptionalValue(const Json::Value& json,
                          std::string_view key,
                          std::optional<T>& target,
                          F&& conversion)
    {
        if (json.isMember(JsonKey(key)))
        {
            if (auto jsonVal{ json[JsonKey(key)] })
            {
                target = conversion(jsonVal);
                return;
            }
        }
        // fall through: both cases should nullopt
        target = std::nullopt;
    }

    template<typename T>
    void GetOptionalValue(const Json::Value& json,
                          std::string_view key,
                          std::optional<T>& target)
    {
        GetOptionalValue(json, key, target, Details::ConversionTrait<T>::FromJson);
    }

    // notes to self
    /*
    consider GetValue<T>(..., json, key, target) with optional conversion fn
    consider tag-based dispatch (Required_t, Optional_t) for whether the failed lookup is throwing
    Look at specializing GetValue<optional<T>> to simply be GetValue<T>(Optional_t)?
    Does the optional specialization need a temporary local to move about?
    *do we even need custom conversion functions* or can we rely on specializations.
    the benefit of relying on specializations is that it simplifies our specialization logic a great deal.
    */

    /*
    namespace Details2
    {
        template<typename T>
        struct Getter
        {
            bool Get(const Json::Value& json, std::string_view key, T& target)
            {
            }
        };
    }
    */

    template<typename T>
    bool GetValueForKey(const Json::Value& json, std::string_view key, T& target)
    {
        if (auto found{ json.find(&*key.cbegin(), (&*key.cbegin()) + key.size()) })
        {
            if (*found) // take care to filter out JSON null
            {
                target = Details::ConversionTrait<T>::FromJson(*found);
                return true;
            }
        }
        return false;
    }

    template<typename T>
    void GetRequiredValueForKey(const Json::Value& json, std::string_view key, T& target)
    {
        if (!GetValueForKey(json, key, target))
        {
            throw "WE REALLY FUCKED IT UP THIS TIME EH";
        }
    }

    template<typename TOpt>
    void GetValueForKey(const Json::Value& json, std::string_view key, std::optional<TOpt>& target)
    {
        TOpt local{};
        target = std::nullopt;
        if (GetValueForKey(json, key, local))
        {
            target = std::move(local);
        }
    }

    constexpr void GetValuesForKeys(const Json::Value& /*json*/) {}

    template<typename T, typename... Args>
    void GetValuesForKeys(const Json::Value& json, std::string_view key1, T&& val1, Args&&... args)
    {
        GetValuesForKey(json, key1, val1);
        GetValuesForKeys(json, std::forward<Args>(args)...);
    }
};
