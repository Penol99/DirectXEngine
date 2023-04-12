// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.230225.1

#pragma once
#ifndef WINRT_Windows_Phone_StartScreen_2_H
#define WINRT_Windows_Phone_StartScreen_2_H
#include "winrt/impl/Windows.Phone.StartScreen.1.h"
WINRT_EXPORT namespace winrt::Windows::Phone::StartScreen
{
    struct WINRT_IMPL_EMPTY_BASES DualSimTile : winrt::Windows::Phone::StartScreen::IDualSimTile
    {
        DualSimTile(std::nullptr_t) noexcept {}
        DualSimTile(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Phone::StartScreen::IDualSimTile(ptr, take_ownership_from_abi) {}
        DualSimTile();
        static auto GetTileForSim2();
        static auto UpdateDisplayNameForSim1Async(param::hstring const& name);
        static auto CreateTileUpdaterForSim1();
        static auto CreateTileUpdaterForSim2();
        static auto CreateBadgeUpdaterForSim1();
        static auto CreateBadgeUpdaterForSim2();
        static auto CreateToastNotifierForSim1();
        static auto CreateToastNotifierForSim2();
    };
}
#endif