#pragma once
#include <DirectXMath.h>

struct Vertex
{
	Vertex()
		:pos(0.f,0.f)
	{
	}
	Vertex(float x, float y)
		:pos(x,y)
	{ 
	}
	DirectX::XMFLOAT2 pos;
};