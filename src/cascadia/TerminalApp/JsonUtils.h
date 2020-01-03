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

#include <json.h>

#include "../types/inc/utils.hpp"

namespace winrt
{
    struct guid;
}

namespace TerminalApp::JsonUtils
{
    template<typename T>
    struct ConversionTrait
    {
        // FromJson, ToJson are not defined so as to cause a compile error (which forces a specialization)
    };

    template<typename T>
    struct PodConversionBase // implements ToJson for all basic conerverts (converters)
    {
        static Json::Value ToJson(T&& value)
        {
            Json::Value val{ value };
            return val;
        }
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
            return json.asDouble();
        }
    };

    template<>
    struct ConversionTrait<GUID>
    {
        static GUID FromJson(const Json::Value& json)
        {
            auto jsonString{ json.asString() };
            return ::Microsoft::Console::Utils::GuidFromString(json.asString());
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
    // - Helper that will populate a reference with a value converted from a json object.
    // Arguments:
    // - json: the json object to convert
    // - target: the value to populate with the converted result
    // Return Value:
    // - a boolean indicating whether the value existed (in this case, was non-null)
    template<typename T>
    bool GetValue(const Json::Value& json, T& target)
    {
        if (json)
        {
            target = ConversionTrait<T>::FromJson(json);
            return true;
        }
        return false;
    }

    // Method Description:
    // - Overload on GetValue that will populate a std::optional with a value converted from json
    //    - If the json value doesn't exist we'll leave the target object unmodified.
    //    - If the json object is set to `null`, then
    //      we'll instead set the target back to nullopt.
    // Arguments:
    // - json: the json object to convert
    // - target: the value to populate with the converted result
    // Return Value:
    // - a boolean indicating whether the optional was changed
    template<typename TOpt>
    bool GetValue(const Json::Value& json, std::optional<TOpt>& target)
    {
        if (json.isNull())
        {
            target = std::nullopt;
            return true; // null is valid for optionals
        }

        TOpt local{};
        if (GetValue(json, local))
        {
            target = std::move(local);
            return true;
        }
        return false;
    }

    template<typename T>
    bool GetValueForKey(const Json::Value& json, std::string_view key, T& target)
    {
        if (auto found{ json.find(&*key.cbegin(), (&*key.cbegin()) + key.size()) })
        {
            return GetValue(*found, target);
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

    // THIS may be useful?
    constexpr void GetValuesForKeys(const Json::Value& /*json*/) {}

    template<typename T, typename... Args>
    void GetValuesForKeys(const Json::Value& json, std::string_view key1, T&& val1, Args&&... args)
    {
        GetValueForKey(json, key1, val1);
        GetValuesForKeys(json, std::forward<Args>(args)...);
    }
};
