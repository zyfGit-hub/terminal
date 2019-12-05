// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#pragma once

#include "winrt/Microsoft.UI.Xaml.Controls.h"

#include "../../cascadia/inc/cppwinrt_utils.h"
#include "TabSwitcherControl.g.h"
#include "Tab.h"

namespace winrt::TerminalApp::implementation
{
    struct TabSwitcherControl : TabSwitcherControlT<TabSwitcherControl>
    {
        TabSwitcherControl();

        void Initialize(std::vector<std::weak_ptr<Tab>>& tabs);

        void ToggleVisibility();

    private:
        std::vector<std::weak_ptr<Tab>> TabList;
    };
}

namespace winrt::TerminalApp::factory_implementation
{
    struct TabSwitcherControl : TabSwitcherControlT<TabSwitcherControl, implementation::TabSwitcherControl>
    {
    };
}
