cbuffer SceneConstantBuffer : register(b0)
{
    float4 offset;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

PSInput VS(float3 position : POSITION, float4 color : COLOR)
{
    PSInput result;

    result.position = float4(position, 1.0) + offset;
    result.color = color;

    return result;
}

float4 PS(PSInput input) : SV_TARGET
{
    return input.color;
}