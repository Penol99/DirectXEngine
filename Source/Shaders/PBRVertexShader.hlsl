cbuffer cBuffer : register(b0)
{
    float4x4 mat;
};

struct VertexInput
{
    float3 Position : POSITION0;
    float3 Normal : NORMAL0;
    float2 TexCoord : TEXCOORD0;
};

struct VertexOutput
{
    float4 Position : SV_POSITION;
    float3 Normal : NORMAL0;
    float2 TexCoord : TEXCOORD0;
};

VertexOutput main(VertexInput input)
{
    VertexOutput output;

    // Apply the transformations with the world matrix "mat"
    float4 position = float4(input.Position, 1);
    position = mul(position, mat);

    output.Position = position;
    output.Normal = mul(input.Normal, (float3x3) mat); // Normal also should be transformed but without translation
    output.TexCoord = input.TexCoord;

    return output;
}