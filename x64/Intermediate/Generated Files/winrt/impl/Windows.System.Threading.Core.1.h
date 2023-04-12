// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.230225.1

#pragma once
#ifndef WINRT_Windows_System_Threading_Core_1_H
#define WINRT_Windows_System_Threading_Core_1_H
#include "winrt/impl/Windows.System.Threading.Core.0.h"
WINRT_EXPORT namespace winrt::Windows::System::Threading::Core
{
    struct WINRT_IMPL_EMPTY_BASES IPreallocatedWorkItem :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IPreallocatedWorkItem>
    {
        IPreallocatedWorkItem(std::nullptr_t = nullptr) noexcept {}
        IPreallocatedWorkItem(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IPreallocatedWorkItemFactory :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IPreallocatedWorkItemFactory>
    {
        IPreallocatedWorkItemFactory(std::nullptr_t = nullptr) noexcept {}
        IPreallocatedWorkItemFactory(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISignalNotifier :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISignalNotifier>
    {
        ISignalNotifier(std::nullptr_t = nullptr) noexcept {}
        ISignalNotifier(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ISignalNotifierStatics :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<ISignalNotifierStatics>
    {
        ISignalNotifierStatics(std::nullptr_t = nullptr) noexcept {}
        ISignalNotifierStatics(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
}
#endif