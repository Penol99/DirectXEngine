cbuffer ConstantBuffer : register(b0)
{
    float3 DirectionalLightDirection;
    float3 CameraPosition;
    float4 DirectionalLightAmbientColor;
    float4 DirectionalLightDiffuseColor;
}

struct PixelInput
{
    float4 Position : SV_POSITION;
    float3 WorldPos : TEXCOORD2;
    float3 Rotation : TEXCOORD6;
    float3 Normal : NORMAL0;
    float2 TexCoord : TEXCOORD0;
};

Texture2D albedoTexture : register(t0);
Texture2D metalnessTexture : register(t1);
Texture2D roughnessTexture : register(t2);

SamplerState defaultSampler : register(s0);

struct PixelOutput
{
    float4 Color : SV_TARGET;
};

float3 RotateVector(float3 v, float3 rotation)
{
    float3 sinRotation = sin(rotation);
    float3 cosRotation = cos(rotation);
    
    float3 rotatedVector;
    rotatedVector.x = v.x * cosRotation.y * cosRotation.z + v.y * (sinRotation.x * sinRotation.y * cosRotation.z - cosRotation.x * sinRotation.z) + v.z * (cosRotation.x * sinRotation.y * cosRotation.z + sinRotation.x * sinRotation.z);
    rotatedVector.y = v.x * cosRotation.y * sinRotation.z + v.y * (sinRotation.x * sinRotation.y * sinRotation.z + cosRotation.x * cosRotation.z) + v.z * (cosRotation.x * sinRotation.y * sinRotation.z - sinRotation.x * cosRotation.z);
    rotatedVector.z = -v.x * sinRotation.y + v.y * sinRotation.x * cosRotation.y + v.z * cosRotation.x * cosRotation.y;
    
    return rotatedVector;
}

PixelOutput main(PixelInput input) : SV_TARGET
{
    PixelOutput output;
    float Shininess = 200.0f;
    float3 SpecularColor = float3(1.0f, 1.0f, 1.0f); // Bright white specular color
    
    // Sample textures
    float4 albedo = albedoTexture.Sample(defaultSampler, input.TexCoord);
    float metalness = metalnessTexture.Sample(defaultSampler, input.TexCoord).r;
    float roughness = roughnessTexture.Sample(defaultSampler, input.TexCoord).r;

    // Rotate the normal vector based on the player's rotation
    float3 rotatedNormal = RotateVector(input.Normal, input.Rotation);

    // Calculate lighting
    float3 normal = normalize(rotatedNormal);
    float3 lightDirection = normalize(DirectionalLightDirection);
    float3 viewDirection = normalize(CameraPosition - input.WorldPos);
    float3 halfwayDirection = normalize(lightDirection + viewDirection);

    float3 diffuseColor = albedo.rgb;

    float3 ambient = DirectionalLightAmbientColor.rgb;
    float3 diffuse = saturate(dot(normal, lightDirection)) * DirectionalLightDiffuseColor.rgb;
    float3 specular = pow(saturate(dot(normal, halfwayDirection)), Shininess) * SpecularColor * DirectionalLightDiffuseColor.rgb;

    float3 finalColor = albedo.rgb * (ambient + diffuse) + specular;

    // Apply metalness and roughness
    finalColor = lerp(finalColor, albedo.rgb, metalness);
    finalColor = lerp(finalColor, float3(1.0, 1.0, 1.0), roughness);

    output.Color = float4(finalColor, albedo.a);
    return output;
}