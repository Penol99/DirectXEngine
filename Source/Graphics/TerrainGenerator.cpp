#include "TerrainGenerator.h"
#include <iostream>
TerrainGenerator::TerrainGenerator() : myWidth(0), myHeight(0), myPerlinScale(0.0f), myOctaves(0),
myPersistence(0.0f), myLacunarity(0.0f), mySeed(0)
{
}

TerrainGenerator::TerrainGenerator(const TerrainGenerator& other)
    : myWidth(other.myWidth),
    myHeight(other.myHeight),
    myPerlinScale(other.myPerlinScale),
    myOctaves(other.myOctaves),
    myPersistence(other.myPersistence),
    myLacunarity(other.myLacunarity),
    mySeed(other.mySeed),
    myRandomEngine(other.myRandomEngine),
    myCamera(other.myCamera),
    myRotationAngles(other.myRotationAngles),
    myPosition(other.myPosition),
    myScale(other.myScale)
{
    myVertexShader = other.myVertexShader;
    myPixelShader = other.myPixelShader;
    myCBVSVertexShader = other.myCBVSVertexShader;
    myVertexBuffer = other.myVertexBuffer;
    myIndexBuffer = other.myIndexBuffer;
}


bool TerrainGenerator::Init(ComPtr<ID3D11Device>& aDevice, 
    ComPtr<ID3D11DeviceContext>& aDeviceContext, 
    Camera& aCamera, 
    int aWidth,
    int aHeight,
    float aPerlinScale,
    int someOctaves,
    float aPersistence,
    float aLacunarity,
    int aSeed)
{
    
    myWidth = aWidth;
    myHeight = aHeight;
    myPerlinScale = aPerlinScale;
    myOctaves = someOctaves;
    myPersistence = aPersistence;
    myLacunarity = aLacunarity;
    mySeed = aSeed;

    myCamera = &aCamera;
    myRandomEngine.seed(mySeed);

    std::vector<float> terrain = GenerateTerrain();

    std::vector<Vertex> vertices;
    vertices.reserve(myWidth * myHeight);

    for (int y = 0; y < myHeight; ++y)
    {
        for (int x = 0; x < myWidth; ++x)
        {
            Vertex vertex;
            vertex.myPos = DirectX::XMFLOAT3(x, terrain[y * myWidth + x], y);
            std::cout << terrain[y * myWidth + x] << std::endl;
            //vertex.texCoord = DirectX::XMFLOAT2(x / static_cast<float>(myWidth), y / static_cast<float>(myHeight));
            vertices.push_back(vertex);
        }
    }

    HRESULT hr = myVertexBuffer.Init(aDevice.Get(), vertices.data(), vertices.size());
    if (FAILED(hr))
    {
        ErrorLog::Log(hr, "Failed to create vertex buffer.");
        return false;
    }

    std::vector<DWORD> indices;
    indices.reserve((myWidth - 1) * (myHeight - 1) * 6);

    for (int y = 0; y < myHeight - 1; ++y)
    {
        for (int x = 0; x < myWidth - 1; ++x)
        {
            unsigned int topLeftIndex = y * myWidth + x;
            unsigned int topRightIndex = y * myWidth + x + 1;
            unsigned int bottomLeftIndex = (y + 1) * myWidth + x;
            unsigned int bottomRightIndex = (y + 1) * myWidth + x + 1;

            indices.push_back(topLeftIndex);
            indices.push_back(bottomLeftIndex);
            indices.push_back(topRightIndex);

            indices.push_back(topRightIndex);
            indices.push_back(bottomLeftIndex);
            indices.push_back(bottomRightIndex);
        }
    }

    hr = myIndexBuffer.Init(aDevice.Get(), indices.data(), indices.size());
    if (FAILED(hr))
    {
        ErrorLog::Log(hr, "Failed to create index buffer.");
        return false;
    }

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        {"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0}
    };

    UINT numElements = ARRAYSIZE(layout);

    if (!myVertexShader.Init(aDevice, L"../bin/TerrainVertexShader.cso", layout, numElements))
    {
        return false;
    }

    if (!myPixelShader.Init(aDevice, L"../bin/TerrainPixelShader.cso"))
    {
        return false;
    }

    hr = myCBVSVertexShader.Init(aDevice.Get(), aDeviceContext.Get());
    if (FAILED(hr))
    {
        ErrorLog::Log(hr, "failed creating constant buffer.");
        return false;
    }
    return true;
}

std::vector<float> TerrainGenerator::GenerateTerrain()
{
    std::vector<float> terrain;
    terrain.reserve(myWidth * myHeight);

    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

    float halfWidth = myWidth / 2.0f;
    float halfHeight = myHeight / 2.0f;

    for (int y = 0; y < myHeight; ++y)
    {
        for (int x = 0; x < myWidth; ++x)
        {
            float amplitude = 1.0f;
            float frequency = 1.0f;
            float noiseHeight = 0.0f;

            for (int octave = 0; octave < myOctaves; ++octave)
            {
                float sampleX = (x - halfWidth) / myPerlinScale * frequency;
                float sampleY = (y - halfHeight) / myPerlinScale * frequency;

                float perlinValue = GeneratePerlin(sampleX, sampleY);
                noiseHeight += perlinValue * amplitude;

                amplitude *= myPersistence;
                frequency *= myLacunarity;
            }

            terrain.push_back(noiseHeight);
        }
    }

    return terrain;
}

void TerrainGenerator::Render(ID3D11DeviceContext* aDeviceContext)
{
    UINT offset = 0;
    aDeviceContext->IASetInputLayout(myVertexShader.GetInputLayout());
    aDeviceContext->VSSetShader(myVertexShader.GetShader(), NULL, 0);
    aDeviceContext->PSSetShader(myPixelShader.GetShader(), NULL, 0);

    aDeviceContext->IASetVertexBuffers(0, 1, myVertexBuffer.GetAddressOf(), myVertexBuffer.GetStridePtr(), &offset);
    aDeviceContext->IASetIndexBuffer(myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    aDeviceContext->DrawIndexed(myIndexBuffer.GetBufferSize(), 0, 0);

    static float worldTranslationOffset[3] = { 0,0,0 };
    XMMATRIX scale = XMMatrixScaling(myScale.x, myScale.y, myScale.z);
    XMMATRIX translationOffset = XMMatrixTranslation(worldTranslationOffset[0], worldTranslationOffset[1], worldTranslationOffset[2]);
    XMMATRIX world = scale * translationOffset;

    myCBVSVertexShader.myData.modelPosition = myPosition;
    myCBVSVertexShader.myData.modelRotation = myRotationAngles;
    myCBVSVertexShader.myData.worldMatrix = world * myCamera->GetViewMatrix() * myCamera->GetProjectionMatrix();
    myCBVSVertexShader.myData.worldMatrix = XMMatrixTranspose(myCBVSVertexShader.myData.worldMatrix);


    //aDeviceContext->UpdateSubresource(myCBVSVertexShader.Get(), 0, nullptr, &myCBVSVertexShader.myData, 0, 0);
    myCBVSVertexShader.ApplyChanges();
    aDeviceContext->VSSetConstantBuffers(2, 1, myCBVSVertexShader.GetAddressOf());

}

float TerrainGenerator::GeneratePerlin(float x, float y)
{
    int xi = static_cast<int>(x);
    int yi = static_cast<int>(y);

    float xf = x - xi;
    float yf = y - yi;

    float u = Fade(xf);
    float v = Fade(yf);

    int aaa = Shuffle(Shuffle(xi) + yi);
    int aba = Shuffle(Shuffle(xi) + yi + 1);
    int baa = Shuffle(Shuffle(xi + 1) + yi);
    int bba = Shuffle(Shuffle(xi + 1) + yi + 1);
    int aab = Shuffle(Shuffle(xi) + yi + 1);
    int abb = Shuffle(Shuffle(xi) + yi + 1);
    int bab = Shuffle(Shuffle(xi + 1) + yi + 1);
    int bbb = Shuffle(Shuffle(xi + 1) + yi + 1);

    float x1, x2, y1, y2;
    x1 = Lerp(Grad(aaa, xf, yf), Grad(baa, xf - 1, yf), u);
    x2 = Lerp(Grad(aba, xf, yf - 1), Grad(bba, xf - 1, yf - 1), u);
    y1 = Lerp(x1, x2, v);

    x1 = Lerp(Grad(aab, xf, yf), Grad(bab, xf - 1, yf), u);
    x2 = Lerp(Grad(abb, xf, yf - 1), Grad(bbb, xf - 1, yf - 1), u);
    y2 = Lerp(x1, x2, v);

    return Lerp(y1, y2, Fade(fmod(x, 255.0f)));
}

int TerrainGenerator::Shuffle(int x)
{
    std::uniform_int_distribution<int> distribution(0, 255);
    return distribution(myRandomEngine) + x;
}

float TerrainGenerator::Grad(int hash, float x, float y)
{
    int h = hash & 7;
    float u = h < 4 ? x : y;
    float v = h < 4 ? y : x;
    return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v);
}

float TerrainGenerator::Fade(float t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float TerrainGenerator::Lerp(float a, float b, float t)
{
    return a + t * (b - a);
}
