#include <vector>
#include <random>
#include <cmath>
#include <vector>
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "../Shaders/Shaders.h"
#include "Camera.h"
class TerrainGenerator
{
public:
    TerrainGenerator();

    TerrainGenerator(const TerrainGenerator& other);

    std::vector<float> GenerateTerrain();

    bool Init(ComPtr<ID3D11Device>& aDevice, 
        ComPtr<ID3D11DeviceContext>& aDeviceContext, 
        Camera& aCamera, 
        int aWidth, 
        int aHeight, 
        float aScale, 
        int someOctaves, 
        float aPersistence, 
        float aLacunarity, 
        int aSeed);

    void Render(ID3D11DeviceContext* aDeviceContext);
private:

    DirectX::XMFLOAT3 myRotationAngles = { 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT3 myPosition = { 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT3 myScale = { 1.0f,1.0f,1.0f };
    Camera* myCamera = nullptr;


    int myWidth;
    int myHeight;
    float myPerlinScale;
    int myOctaves;
    float myPersistence;
    float myLacunarity;
    int mySeed;
    ConstantBuffer<CB_VS_VertexShader> myCBVSVertexShader;

    VertexBuffer<Vertex> myVertexBuffer;
    IndexBuffer myIndexBuffer;

    VertexShader myVertexShader;
    PixelShader myPixelShader;
    std::default_random_engine myRandomEngine;

    float GeneratePerlin(float x, float y);

    int Shuffle(int x);

    float Grad(int hash, float x, float y);

    float Fade(float t);

    float Lerp(float a, float b, float t);
};