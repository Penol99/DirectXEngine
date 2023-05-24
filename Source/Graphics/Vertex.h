#pragma once
#include <DirectXMath.h>

struct Vertex
{
    Vertex() = default;
    Vertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
        : myPos(x, y, z),
        myNormal(nx, ny, nz),
        myTexCoord(u, v)
    {
    }
    DirectX::XMFLOAT3 myPos;
    DirectX::XMFLOAT3 myNormal;
    DirectX::XMFLOAT2 myTexCoord;
};