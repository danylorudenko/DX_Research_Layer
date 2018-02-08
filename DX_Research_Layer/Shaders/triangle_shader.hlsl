#pragma pack_matrix(row_major)

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
    float3 NormW : NORMAL;
};

PSInput VS(float3 position : POSITION, float3 normal : NORMAL)
{
    PSInput result;

    float4 positionW = mul(float4(position, 1.0f), worldMatrix);
    result.PosW = positionW.xyz;
    result.NormW = mul(float4(normal, 1.0f), worldMatrix).xyz;
    result.PosH = mul(mul(positionW, viewMatrix), projectionMatrix);

    return result;
}

float4 PS(PSInput input) : SV_TARGET
{
    const float3 lightDir = float3(-1.0f, 0.0f, 0.0f);
    
    float3 litColor = dot(lightDir, normalize(input.NormW));
    
    return float4(litColor, 1.0f);
    //return float4(1.0f, 1.0f, 1.0f, 1.0f);
}