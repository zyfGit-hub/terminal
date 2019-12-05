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

    void TabSwitcherControl::Initialize(std::vector<std::weak_ptr<Tab>>& tabs)
    {
        TabList = tabs;
    }

    void TabSwitcherControl::ToggleVisibility()
    {
        const bool isVisible = Visibility() == Visibility::Visible;
        if (!isVisible)
        {
            // Become visible
            Visibility(Visibility::Visible);
        }
    }
}
