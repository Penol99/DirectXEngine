cbuffer cBuffer : register(b0)
{
    float4x4 mat;
    float3 modelRotation;
    float padding0;
    float3 modelPosition;
    float padding1;
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
    float3 WorldPos : TEXCOORD2;
    float3 Rotation : TEXCOORD6;
    float3 Normal : NORMAL0;
    float2 TexCoord : TEXCOORD0;
};

VertexOutput main(VertexInput input)
{
    VertexOutput output;
    
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

    float4 position = float4(input.Position, 1);
    position = mul(position, rotationMatrix);

    // Apply model position
    position.xyz += modelPosition;

    output.Position = mul(position, mat);
    output.WorldPos = input.Position;
    output.Normal = input.Normal;
    output.TexCoord = input.TexCoord;
    output.Rotation = modelRotation; // Pass the player's rotation as output
    
    return output;
}