#include "AdapterReader.h"

std::vector<AdapterData> AdapterReader::mAdapters;
std::vector<AdapterData> AdapterReader::GetAdapters()
{
    if (mAdapters.size() > 0) // already initialized
    {
        return mAdapters;
    }

    Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;


    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf()));
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "fucked up creating DXGIFactory for enumerating adapters");
        exit(-1);
    }

    IDXGIAdapter* pAdapter;
    UINT index = 0;
    while (SUCCEEDED(pFactory->EnumAdapters(index, &pAdapter)))
    {
        mAdapters.push_back(AdapterData(pAdapter));
        index += 1;
    }
    return mAdapters;

    return std::vector<AdapterData>();
}

AdapterData::AdapterData(IDXGIAdapter* pAdapter)
    :mAdapter(pAdapter)
{
    HRESULT hr = mAdapter->GetDesc(&this->mDescription);
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "fucked up getting the description for IDXGIAdapter");
    }
}