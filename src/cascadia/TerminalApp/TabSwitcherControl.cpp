// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#include "pch.h"
#include "TabSwitcherControl.h"

#include "TabSwitcherControl.g.cpp"

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;

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
}
