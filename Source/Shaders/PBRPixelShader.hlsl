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
    float3 Normal : NORMAL0;
    float2 TexCoord : TEXCOORD0;
};

Texture2D albedoTexture : register(t0);
Texture2D metalnessTexture : register(t1);
Texture2D roughnessTexture : register(t2);
TextureCube reflectionMap : register(t3);

SamplerState defaultSampler : register(s0);
SamplerState reflectionSampler : register(s1);

struct PixelOutput
{
    float4 Color : SV_TARGET;
};

PixelOutput main(PixelInput input) : SV_TARGET
{
    PixelOutput output;
    float Shininess = .5f;
    float3 SpecularColor = float3(0.1f, 0.1f, 0.1f); // Bright white specular color
    
    // Sample textures
    float4 albedo = albedoTexture.Sample(defaultSampler, input.TexCoord);
    float metalness = metalnessTexture.Sample(defaultSampler, input.TexCoord).r;
    float roughness = roughnessTexture.Sample(defaultSampler, input.TexCoord).r;

    // Calculate lighting
    float3 normal = normalize(input.Normal);
    float3 lightDirection = normalize(DirectionalLightDirection);
    float3 viewDirection = normalize(CameraPosition - input.Position.xyz);
    float3 halfwayDirection = normalize(lightDirection + viewDirection);

    float3 diffuseColor = albedo.rgb;

    float3 ambient = DirectionalLightAmbientColor.rgb;
    float3 diffuse = saturate(dot(normal, lightDirection)) * DirectionalLightDiffuseColor.rgb;
    float3 specular = pow(saturate(dot(normal, halfwayDirection)), Shininess) * SpecularColor * DirectionalLightDiffuseColor.rgb;

    float3 finalColor = albedo.rgb * (ambient + diffuse) + specular;

    // Apply metalness and roughness
    finalColor = lerp(finalColor, albedo.rgb, metalness);
    finalColor = lerp(finalColor, float3(1.0, 1.0, 1.0), roughness);

    // Calculate reflection
    float3 reflectionVector = reflect(-viewDirection, normal);
    float3 reflectedColor = reflectionMap.SampleLevel(reflectionSampler, reflectionVector, 0).rgb;
    finalColor = lerp(finalColor, reflectedColor, metalness);

    output.Color = float4(finalColor, albedo.a);
    return output;
}