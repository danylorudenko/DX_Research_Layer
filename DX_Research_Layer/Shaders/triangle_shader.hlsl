cbuffer SceneBuffer : register(b0)
{
    float4x4 projectionMatrix;
    float4x4 viewMatrix;
    float3 cameraPosition;
};

cbuffer TransformBuffer : register(b1)
{
    float4x4 worldMatrix;
};

struct PSInput
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
};

PSInput VS(float3 position : POSITION)
{
    PSInput result;

    float4 positionW = mul(float4(position, 1.0f), viewMatrix);
    result.PosW = positionW.xyz;
    result.PosH = mul(positionW, projectionMatrix);

    return result;
}

float4 PS(PSInput input) : SV_TARGET
{
    const float3 lightDir = float3(0.0f, -1.0f, 0.0f);
    
    float3 viewDir = normalize(input.PosW - cameraPosition);
    float3 litColor = dot(lightDir, viewDir);
    
    return float4(litColor, 1.0f);
}