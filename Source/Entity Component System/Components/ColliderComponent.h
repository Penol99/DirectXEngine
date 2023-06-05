#pragma once
#include "Component.h"
#include "../../Ray.h"
#include <wrl/client.h>
#include <d3d11.h>
#include "../../ErrorLog.h"
#ifdef DRAW_COLLIDERS
#include "../../Graphics/Vertex.h"
#include <vector>
#include "../../Shaders/shaders.h"
#include "../../Graphics/VertexBuffer.h"
#include "../../Graphics/IndexBuffer.h"
#include "../../Graphics/ConstantBuffer.h"
#include "../../Graphics/Camera.h"
#endif
using namespace Microsoft::WRL;
class ColliderComponent : public Component
{
public:
	virtual void Init() override {};
	virtual void RenderImGui() override {};
    virtual bool Intersects(const Ray& ray) const = 0; 
#ifdef DRAW_COLLIDERS
    virtual void Render() = 0;
protected:
	ConstantBuffer<CB_VS_StandardShader> myConstantBuffer;
	VertexBuffer<Vertex> myVertexBuffer;
	IndexBuffer myIndexBuffer;
	VertexShader myVertexShader;
	PixelShader myPixelShader;
#endif
};

