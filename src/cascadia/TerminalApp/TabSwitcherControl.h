// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#pragma once

#include "winrt/Microsoft.UI.Xaml.Controls.h"

#include "ShortcutActionDispatch.h"
#include "../../cascadia/inc/cppwinrt_utils.h"
#include "TabSwitcherControl.g.h"

namespace winrt::TerminalApp::implementation
{
    struct TabSwitcherControl : TabSwitcherControlT<TabSwitcherControl>
    {
        TabSwitcherControl();

        void SetDispatch(const winrt::TerminalApp::ShortcutActionDispatch& dispatch);

        void ToggleVisibility();

        void _Close();

    private:
        winrt::TerminalApp::ShortcutActionDispatch _dispatch;
    };
}

namespace winrt::TerminalApp::factory_implementation
{
    struct TabSwitcherControl : TabSwitcherControlT<TabSwitcherControl, implementation::TabSwitcherControl>
    {
    };
}
