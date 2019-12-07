// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#pragma once

#include "winrt/Windows.UI.Xaml.h"
#include "winrt/Microsoft.UI.Xaml.Controls.h"
#include "../../cascadia/inc/cppwinrt_utils.h"

#include "TabSwitcherControl.g.h"
#include "ShortcutActionDispatch.h"

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

        void KeyDownHandler(Windows::Foundation::IInspectable const& /*sender*/,
                            Windows::UI::Xaml::Input::KeyRoutedEventArgs const& e);
    };
}

namespace winrt::TerminalApp::factory_implementation
{
    struct TabSwitcherControl : TabSwitcherControlT<TabSwitcherControl, implementation::TabSwitcherControl>
    {
    };
}
