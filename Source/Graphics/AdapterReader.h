#pragma once

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"DXGI.lib")

#include "..\ErrorLog.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>


class AdapterData
{
public:
	AdapterData(IDXGIAdapter* pAdapter);
	IDXGIAdapter* mAdapter = nullptr;
	DXGI_ADAPTER_DESC mDescription;
};
class AdapterReader
{
public:
	static std::vector<AdapterData> GetAdapters();
private:
	static std::vector<AdapterData> mAdapters;
};