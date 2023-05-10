cbuffer cBuffer : register(b0)
{
    float4x4 mat;
    float3 modelRotation;
    float padding0;
    float3 modelPosition;
    float padding1;
};

struct VS_INPUT
{
    float3 inPos : POSITION;
    float2 inTexCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 outPosition : SV_POSITION;
    float2 outTexCoord : TEXCOORD0;
    
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    // Apply model rotation
    float4x4 rotationMatrix = float4x4(
        float4(cos(modelRotation.y) * cos(modelRotation.z),
               cos(modelRotation.y) * sin(modelRotation.z),
              -sin(modelRotation.y),
               0),
        float4(sin(modelRotation.x) * sin(modelRotation.y) * cos(modelRotation.z) - cos(modelRotation.x) * sin(modelRotation.z),
               sin(modelRotation.x) * sin(modelRotation.y) * sin(modelRotation.z) + cos(modelRotation.x) * cos(modelRotation.z),
               sin(modelRotation.x) * cos(modelRotation.y),
               0),
        float4(cos(modelRotation.x) * sin(modelRotation.y) * cos(modelRotation.z) + sin(modelRotation.x) * sin(modelRotation.z),
               cos(modelRotation.x) * sin(modelRotation.y) * sin(modelRotation.z) - sin(modelRotation.x) * cos(modelRotation.z),
               cos(modelRotation.x) * cos(modelRotation.y),
               0),
        float4(0, 0, 0, 1)
    );

    float4 position = float4(input.inPos, 1);
    position = mul(position, rotationMatrix);

    // Apply model position
    position.xyz += modelPosition;

    output.outPosition = mul(position, mat);
    output.outTexCoord = input.inTexCoord;
    
    return output;
}