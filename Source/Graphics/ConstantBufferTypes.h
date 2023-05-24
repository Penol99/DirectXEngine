#pragma once
#include <DirectXMath.h>

struct CB_VS_StandardShader
{
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMFLOAT3 modelRotation;
	float padding0;
	DirectX::XMFLOAT3 modelPosition;
	float padding1;
};

struct CB_PS_StandardShader
{
	float alpha = 1.0f;
};

struct CB_PS_PBRShader
{
	DirectX::XMFLOAT3 direction;
	float padding0;
	DirectX::XMFLOAT3 cameraPosition;
	float padding1;
	DirectX::XMFLOAT4 ambientColor;
	DirectX::XMFLOAT4 diffuseColor;
};
