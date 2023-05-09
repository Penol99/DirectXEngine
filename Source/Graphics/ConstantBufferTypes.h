#pragma once
#include <DirectXMath.h>

struct CB_VS_VertexShader
{
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMFLOAT3 gModelRotation;
	float padding0;
	DirectX::XMFLOAT3 gModelPosition;
	float padding1;
};

struct CB_PS_PixelShader
{
	float alpha = 1.0f;
};
