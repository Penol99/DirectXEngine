cbuffer alphaBuffer : register(b0)
{
    float alpha;
}

struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD0;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float2 uv = input.inTexCoord.xy;
    
    // Apply wave effect
    float time = alpha; 
    float waveSpeed = 0.5; 
    float waveHeight = 0.1; 
    float waveOffset = sin(time * waveSpeed); 
    uv.x += waveOffset * waveHeight; 
    
    float4 pixelColor = float4(objTexture.Sample(objSamplerState, uv).rgb, alpha);

    return pixelColor;
}