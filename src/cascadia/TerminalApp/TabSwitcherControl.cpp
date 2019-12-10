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

        _TabList().KeyDown({ this, &TabSwitcherControl::KeyDownHandler });
        _TabList().KeyUp({ this, &TabSwitcherControl::KeyUpHandler });
    }

    void TabSwitcherControl::SetDispatch(const winrt::TerminalApp::ShortcutActionDispatch& dispatch)
    {
        _dispatch = dispatch;
    }

    void TabSwitcherControl::KeyUpHandler(Windows::Foundation::IInspectable const& /*sender*/,
                                          Windows::UI::Xaml::Input::KeyRoutedEventArgs const& e)
    {
        auto key = e.OriginalKey();
        if (key == VirtualKey::Control)
        {
            _Close();
        }
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
            e.Handled(true);
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
        }
    }

    void TabSwitcherControl::_Close()
    {
        Visibility(Visibility::Collapsed);
        _TabList().SelectedIndex(0);
        _closeHandlers(*this, RoutedEventArgs{});
    }

    DEFINE_EVENT_WITH_TYPED_EVENT_HANDLER(TabSwitcherControl, Closed, _closeHandlers, TerminalApp::TabSwitcherControl, winrt::Windows::UI::Xaml::RoutedEventArgs);
}
