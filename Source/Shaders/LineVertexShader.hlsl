cbuffer cBuffer : register(b1)
{
    float4x4 mat;
};

struct VS_INPUT
{
    float3 inPos : POSITION;
};

struct VS_OUTPUT
{
    float4 outPosition : SV_POSITION;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.outPosition = mul(float4(input.inPos, 1.0), mat); // Apply transformation matrix
    return output;
}