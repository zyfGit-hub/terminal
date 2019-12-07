// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#include "pch.h"
#include "TabSwitcherControl.h"
#include "TabSwitcherControl.g.cpp"

using namespace winrt;
using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::System;

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

    void TabSwitcherControl::KeyDownHandler(Windows::Foundation::IInspectable const& /*sender*/,
                                            Windows::UI::Xaml::Input::KeyRoutedEventArgs const& e)
    {
        auto key = e.OriginalKey();
        if (key == VirtualKey::Up)
        {
            auto selected = _TabList().SelectedIndex();
            selected = (selected - 1) % _TabList().Items().Size();
            _TabList().SelectedIndex(selected);
            e.Handled(true);
        }
        else if (key == VirtualKey::Down)
        {
            auto selected = _TabList().SelectedIndex();
            selected = (selected + 1) % _TabList().Items().Size();
            _TabList().SelectedIndex(selected);
            e.Handled(true);
        }
        else if (key == VirtualKey::Enter)
        {
            auto selected = _TabList().SelectedItem();
            if (selected)
            {
                // TODO: Do the thing here.
            }
        }
        else if (key == VirtualKey::Escape)
        {
            _Close();
        }
    }

    void TabSwitcherControl::ToggleVisibility()
    {
        const bool isVisible = Visibility() == Visibility::Visible;
        if (!isVisible)
        {
            // Become visible
            Visibility(Visibility::Visible);
            _TabList().Focus(FocusState::Programmatic);
            _TabList().SelectedIndex(0);
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
