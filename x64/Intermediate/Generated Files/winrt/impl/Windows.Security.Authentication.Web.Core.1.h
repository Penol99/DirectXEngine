// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.230225.1

#pragma once
#ifndef WINRT_Windows_Security_Authentication_Web_Core_1_H
#define WINRT_Windows_Security_Authentication_Web_Core_1_H
#include "winrt/impl/Windows.Security.Authentication.Web.Core.0.h"
WINRT_EXPORT namespace winrt::Windows::Security::Authentication::Web::Core
{
    struct WINRT_IMPL_EMPTY_BASES IFindAllAccountsResult :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IFindAllAccountsResult>
    {
        IFindAllAccountsResult(std::nullptr_t = nullptr) noexcept {}
        IFindAllAccountsResult(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IWebAccountEventArgs :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IWebAccountEventArgs>
    {
        IWebAccountEventArgs(std::nullptr_t = nullptr) noexcept {}
        IWebAccountEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IWebAccountMonitor :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IWebAccountMonitor>
    {
        IWebAccountMonitor(std::nullptr_t = nullptr) noexcept {}
        IWebAccountMonitor(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IWebAccountMonitor2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IWebAccountMonitor2>
    {
        IWebAccountMonitor2(std::nullptr_t = nullptr) noexcept {}
        IWebAccountMonitor2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IWebAuthenticationCoreManagerStatics :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IWebAuthenticationCoreManagerStatics>
    {
        IWebAuthenticationCoreManagerStatics(std::nullptr_t = nullptr) noexcept {}
        IWebAuthenticationCoreManagerStatics(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IWebAuthenticationCoreManagerStatics2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IWebAuthenticationCoreManagerStatics2>,
        impl::require<winrt::Windows::Security::Authentication::Web::Core::IWebAuthenticationCoreManagerStatics2, winrt::Windows::Security::Authentication::Web::Core::IWebAuthenticationCoreManagerStatics>
    {
        IWebAuthenticationCoreManagerStatics2(std::nullptr_t = nullptr) noexcept {}
        IWebAuthenticationCoreManagerStatics2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
        using impl::consume_t<IWebAuthenticationCoreManagerStatics2, IWebAuthenticationCoreManagerStatics2>::FindAccountProviderAsync;
        using impl::consume_t<IWebAuthenticationCoreManagerStatics2, winrt::Windows::Security::Authentication::Web::Core::IWebAuthenticationCoreManagerStatics>::FindAccountProviderAsync;
    };
    struct WINRT_IMPL_EMPTY_BASES IWebAuthenticationCoreManagerStatics3 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IWebAuthenticationCoreManagerStatics3>,
        impl::require<winrt::Windows::Security::Authentication::Web::Core::IWebAuthenticationCoreManagerStatics3, winrt::Windows::Security::Authentication::Web::Core::IWebAuthenticationCoreManagerStatics>
    {
        IWebAuthenticationCoreManagerStatics3(std::nullptr_t = nullptr) noexcept {}
        IWebAuthenticationCoreManagerStatics3(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IWebAuthenticationCoreManagerStatics4 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IWebAuthenticationCoreManagerStatics4>,
        impl::require<winrt::Windows::Security::Authentication::Web::Core::IWebAuthenticationCoreManagerStatics4, winrt::Windows::Security::Authentication::Web::Core::IWebAuthenticationCoreManagerStatics>
    {
        IWebAuthenticationCoreManagerStatics4(std::nullptr_t = nullptr) noexcept {}
        IWebAuthenticationCoreManagerStatics4(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IWebProviderError :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IWebProviderError>
    {
        IWebProviderError(std::nullptr_t = nullptr) noexcept {}
        IWebProviderError(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IWebProviderErrorFactory :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IWebProviderErrorFactory>
    {
        IWebProviderErrorFactory(std::nullptr_t = nullptr) noexcept {}
        IWebProviderErrorFactory(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IWebTokenRequest :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IWebTokenRequest>
    {
        IWebTokenRequest(std::nullptr_t = nullptr) noexcept {}
        IWebTokenRequest(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IWebTokenRequest2 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IWebTokenRequest2>
    {
        IWebTokenRequest2(std::nullptr_t = nullptr) noexcept {}
        IWebTokenRequest2(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IWebTokenRequest3 :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IWebTokenRequest3>
    {
        IWebTokenRequest3(std::nullptr_t = nullptr) noexcept {}
        IWebTokenRequest3(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IWebTokenRequestFactory :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IWebTokenRequestFactory>
    {
        IWebTokenRequestFactory(std::nullptr_t = nullptr) noexcept {}
        IWebTokenRequestFactory(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IWebTokenRequestResult :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IWebTokenRequestResult>
    {
        IWebTokenRequestResult(std::nullptr_t = nullptr) noexcept {}
        IWebTokenRequestResult(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IWebTokenResponse :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IWebTokenResponse>
    {
        IWebTokenResponse(std::nullptr_t = nullptr) noexcept {}
        IWebTokenResponse(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IWebTokenResponseFactory :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IWebTokenResponseFactory>
    {
        IWebTokenResponseFactory(std::nullptr_t = nullptr) noexcept {}
        IWebTokenResponseFactory(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
}
#endif