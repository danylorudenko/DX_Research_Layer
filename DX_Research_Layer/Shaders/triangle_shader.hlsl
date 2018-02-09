#pragma pack_matrix(row_major)

static const float PI = 3.14159265359f;
static const float Lambertian = 1.0f / PI;

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
    float3 ambient = pow(float3(0.2f, 0.3f, 0.5f), 2.2f) * Lambertian * 0.5f * (1.0f + dot(float3(0.0f, 1.0f, 0.0f), normalize(input.NormW)));

    const float3 lightDir = normalize(float3(-1.0f, 0.0f, 0.0f));

    const float3 baseColor = float3(1.0f, 1.0f, 1.0f);

    float3 linBaseColor = pow(baseColor, 2.2f);

    float ndotl = saturate(dot(lightDir, normalize(input.NormW)));

    float3 fragment = linBaseColor * Lambertian * ndotl + ambient;
    
    fragment = pow(fragment, 0.45454545454f);

    return float4(fragment, 1.0f);
}