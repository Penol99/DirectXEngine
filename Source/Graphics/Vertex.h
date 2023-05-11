#pragma once
#include <DirectXMath.h>

struct Vertex
{
	Vertex() = default;
	Vertex(float x, float y,float z, float u, float v)
		:myPos(x,y,z),
		myTexCoord(u,v)
	{ 
	}
	DirectX::XMFLOAT3 myPos;
	DirectX::XMFLOAT2 myTexCoord;
};