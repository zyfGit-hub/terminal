// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#include "pch.h"
#include "TabSwitcherControl.h"

#include "TabSwitcherControl.g.cpp"

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Windows::UI::Xaml;

namespace winrt::TerminalApp::implementation
{
    TabSwitcherControl::TabSwitcherControl()
    {
        InitializeComponent();
    }

    void TabSwitcherControl::SetDispatch(const winrt::TerminalApp::ShortcutActionDispatch& dispatch)
    {
        _dispatch = dispatch;
    }

    void TabSwitcherControl::ToggleVisibility()
    {
        const bool isVisible = Visibility() == Visibility::Visible;
        if (!isVisible)
        {
            // Become visible
            Visibility(Visibility::Visible);
        }
        else
        {
            _Close();
        }
    }

    void TabSwitcherControl::_Close()
    {
        Visibility(Visibility::Collapsed);
        _TabList().SelectedIndex(0);
    }
}
