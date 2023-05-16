struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
};


float4 main(PS_INPUT input) : SV_TARGET
{
    
    float4 pixelColor = float4(0.0,0.0,1.0,1.0);
    return float4(pixelColor);
}