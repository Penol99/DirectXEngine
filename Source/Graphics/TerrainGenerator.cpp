//#include "TerrainGenerator.h"
//#include <iostream>
//
//TerrainGenerator::TerrainGenerator() : myWidth(0), myHeight(0), myPerlinScale(0.0f), myOctaves(0),
//myPersistence(0.0f), myLacunarity(0.0f), mySeed(0)
//{
//}
//
//TerrainGenerator::TerrainGenerator(const TerrainGenerator& other)
//    : myWidth(other.myWidth),
//    myHeight(other.myHeight),
//    myPerlinScale(other.myPerlinScale),
//    myOctaves(other.myOctaves),
//    myPersistence(other.myPersistence),
//    myLacunarity(other.myLacunarity),
//    mySeed(other.mySeed),
//    myCamera(other.myCamera),
//    myRotationAngles(other.myRotationAngles),
//    myPosition(other.myPosition),
//    myScale(other.myScale)
//{
//    myVertexShader = other.myVertexShader;
//    myPixelShader = other.myPixelShader;
//    myCBVSVertexShader = other.myCBVSVertexShader;
//    myVertexBuffer = other.myVertexBuffer;
//    myIndexBuffer = other.myIndexBuffer;
//}
//
//bool TerrainGenerator::Init(ComPtr<ID3D11Device>& aDevice,
//    ComPtr<ID3D11DeviceContext>& aDeviceContext,
//    Camera& aCamera,
//    int aWidth,
//    int aHeight,
//    float aPerlinScale,
//    int someOctaves,
//    float aPersistence,
//    float aLacunarity,
//    int aSeed)
//{
//    myWidth = aWidth;
//    myHeight = aHeight;
//    myPerlinScale = aPerlinScale;
//    myOctaves = someOctaves;
//    myPersistence = aPersistence;
//    myLacunarity = aLacunarity;
//    mySeed = aSeed;
//
//    myCamera = &aCamera;
//    myPerlinNoise = PerlinNoise(aSeed);
//    std::vector<float> terrain = GenerateTerrain();
//
//    std::vector<Vertex> vertices;
//    vertices.reserve(myWidth * myHeight);
//
//    for (int y = 0; y < myHeight; ++y)
//    {
//        for (int x = 0; x < myWidth; ++x)
//        {
//            Vertex vertex;
//            vertex.myPos = DirectX::XMFLOAT3(x, terrain[y * myWidth + x], y);
//            std::cout << terrain[y * myWidth + x] << std::endl;
//            vertex.myTexCoord = DirectX::XMFLOAT2(x / static_cast<float>(myWidth), y / static_cast<float>(myHeight));
//            vertices.push_back(vertex);
//        }
//    }
//
//    GenerateTextureValues(aDevice, terrain);
//
//    HRESULT hr = myVertexBuffer.Init(aDevice.Get(), vertices.data(), vertices.size());
//    if (FAILED(hr))
//    {
//        ErrorLog::Log(hr, "Failed to create vertex buffer.");
//        return false;
//    }
//
//    std::vector<DWORD> indices;
//    indices.reserve((myWidth - 1) * (myHeight - 1) * 6);
//
//    for (int y = 0; y < myHeight - 1; ++y)
//    {
//        for (int x = 0; x < myWidth - 1; ++x)
//        {
//            unsigned int topLeftIndex = y * myWidth + x;
//            unsigned int topRightIndex = y * myWidth + x + 1;
//            unsigned int bottomLeftIndex = (y + 1) * myWidth + x;
//            unsigned int bottomRightIndex = (y + 1) * myWidth + x + 1;
//
//            indices.push_back(topLeftIndex);
//            indices.push_back(bottomLeftIndex);
//            indices.push_back(topRightIndex);
//
//            indices.push_back(topRightIndex);
//            indices.push_back(bottomLeftIndex);
//            indices.push_back(bottomRightIndex);
//        }
//    }
//
//    hr = myIndexBuffer.Init(aDevice.Get(), indices.data(), indices.size());
//    if (FAILED(hr))
//    {
//        ErrorLog::Log(hr, "Failed to create index buffer.");
//        return false;
//    }
//
//    D3D11_INPUT_ELEMENT_DESC layout[] =
//    {
//        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
//        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
//    };
//
//    UINT numElements = ARRAYSIZE(layout);
//
//    if (!myVertexShader.Init(aDevice, L"../bin/TerrainVertexShader.cso", layout, numElements))
//    {
//        return false;
//    }
//
//    if (!myPixelShader.Init(aDevice, L"../bin/TerrainPixelShader.cso"))
//    {
//        return false;
//    }
//
//    hr = myCBVSVertexShader.Init(aDevice.Get(), aDeviceContext.Get());
//    if (FAILED(hr))
//    {
//        ErrorLog::Log(hr, "Failed to create constant buffer.");
//        return false;
//    }
//
//    return true;
//}
//
//std::vector<float> TerrainGenerator::GenerateTerrain()
//{
//    std::vector<float> terrain;
//    terrain.reserve(myWidth * myHeight);
//
//    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
//
//    float halfWidth = myWidth / 2.0f;
//    float halfHeight = myHeight / 2.0f;
//
//    float maxHeight = -std::numeric_limits<float>::infinity();
//    float minHeight = std::numeric_limits<float>::infinity();
//
//    for (int y = 0; y < myHeight; ++y)
//    {
//        for (int x = 0; x < myWidth; ++x)
//        {
//            float amplitude = 1.0f;
//            float frequency = 1.0f;
//            float noiseHeight = 0.0f;
//
//            for (int octave = 0; octave < myOctaves; ++octave)
//            {
//                float sampleX = (x - halfWidth) / myPerlinScale * frequency;
//                float sampleY = (y - halfHeight) / myPerlinScale * frequency;
//
//                float perlinValue = myPerlinNoise.noise(sampleX, sampleY);
//                noiseHeight += perlinValue * amplitude;
//
//                amplitude *= myPersistence;
//                frequency *= myLacunarity;
//            }
//
//            terrain.push_back(noiseHeight);
//
//            if (noiseHeight > maxHeight)
//                maxHeight = noiseHeight;
//            if (noiseHeight < minHeight)
//                minHeight = noiseHeight;
//        }
//    }
//
//    float heightRange = maxHeight - minHeight;
//    for (float& height : terrain)
//    {
//        height = (height - minHeight) / heightRange;
//        height *= myScale.y;
//    }
//
//    return terrain;
//}
//
//bool TerrainGenerator::GenerateTextureValues(ComPtr<ID3D11Device>& aDevice, const std::vector<float>& terrain)
//{
//    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> snowyTexture;
//    HRESULT hr = DirectX::CreateWICTextureFromFile(aDevice.Get(), L"snowy_texture.jpg", nullptr, snowyTexture.GetAddressOf());
//    if (FAILED(hr))
//    {
//        ErrorLog::Log(hr, "Failed to load snowy texture.");
//        return false;
//    }
//
//    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> rockyTexture;
//    hr = DirectX::CreateWICTextureFromFile(aDevice.Get(), L"rocky_texture.jpg", nullptr, rockyTexture.GetAddressOf());
//    if (FAILED(hr))
//    {
//        ErrorLog::Log(hr, "Failed to load rocky texture.");
//        return false;
//    }
//
//    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> grassyTexture;
//    hr = DirectX::CreateWICTextureFromFile(aDevice.Get(), L"grassy_texture.jpg", nullptr, grassyTexture.GetAddressOf());
//    if (FAILED(hr))
//    {
//        ErrorLog::Log(hr, "Failed to load grassy texture.");
//        return false;
//    }
//
//    // Determine the texture for each vertex based on the terrain height
//    std::vector<TextureCoordinate> textureCoordinates;
//    textureCoordinates.reserve(myWidth * myHeight);
//
//    for (int y = 0; y < myHeight; ++y)
//    {
//        for (int x = 0; x < myWidth; ++x)
//        {
//            float height = terrain[y * myWidth + x];
//            TextureCoordinate textureCoordinate;
//            textureCoordinate.x = x / static_cast<float>(myWidth);
//            textureCoordinate.y = y / static_cast<float>(myHeight);
//
//            if (height >= 0.8f)
//            {
//                textureCoordinate.textureIndex = snowyTexture.Get();
//            }
//            else if (height >= 0.4f)
//            {
//                textureCoordinate.textureIndex = rockyTexture.Get();
//            }
//            else
//            {
//                textureCoordinate.textureIndex = grassyTexture.Get();
//            }
//
//            textureCoordinates.push_back(textureCoordinate);
//        }
//    }
//
//    // Store the texture coordinates in a vertex buffer
//    HRESULT hr = myTextureCoordinateBuffer.Init(aDevice.Get(), textureCoordinates.data(), textureCoordinates.size());
//    if (FAILED(hr))
//    {
//        ErrorLog::Log(hr, "Failed to create texture coordinate buffer.");
//        return false;
//    }
//
//    // Set the texture coordinate buffer in the input layout
//    D3D11_INPUT_ELEMENT_DESC layout[] =
//    {
//        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
//        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0} // Assuming TEXCOORD is the second input element
//    };
//
//    UINT numElements = ARRAYSIZE(layout);
//
//    if (!myVertexShader.Init(aDevice, L"../bin/TerrainVertexShader.cso", layout, numElements))
//    {
//        return false;
//    }
//
//    // Set the texture coordinate buffer in the vertex shader
//    aDeviceContext->IASetVertexBuffers(1, 1, myTextureCoordinateBuffer.GetAddressOf(), myTextureCoordinateBuffer.GetStridePtr(), &offset);
//
//    return true;
//}
//
//void TerrainGenerator::Render(ID3D11DeviceContext* aDeviceContext)
//{
//    UINT offset = 0;
//    aDeviceContext->IASetInputLayout(myVertexShader.GetInputLayout());
//    aDeviceContext->VSSetShader(myVertexShader.GetShader(), NULL, 0);
//    aDeviceContext->
// Shader(myPixelShader.GetShader(), NULL, 0);
//
//    aDeviceContext->IASetVertexBuffers(0, 1, myVertexBuffer.GetAddressOf(), myVertexBuffer.GetStridePtr(), &offset);
//    aDeviceContext->IASetIndexBuffer(myIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
//    aDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//    aDeviceContext->VSSetConstantBuffers(0, 1, myCBVSVertexShader.GetAddressOf());
//
//    aDeviceContext->DrawIndexed(myIndexBuffer.GetIndexCount(), 0, 0);
//}