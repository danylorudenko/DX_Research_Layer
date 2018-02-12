#pragma pack_matrix(row_major)

static const float PI = 3.14159265359f;
static const float LAMBERTIAN = 1.0f / PI;
static const float TO_LINEAR = 2.2f;
static const float TO_GAMMA = 0.45454545454f;

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
    //float3 ambient = pow(float3(0.2f, 0.3f, 0.5f), 2.2f) * Lambertian * 0.5f * (1.0f + dot(float3(0.0f, 1.0f, 0.0f), normalize(input.NormW)));

    const float m = 2048.0f;

    const float3 lightDir = normalize(float3(-1.0f, 0.0f, 0.0f));
    const float3 baseDiffColor = float3(1.0f, 1.0f, 0.5f);
    const float3 baseSpecColor = float3(0.5f, 0.5f, 0.1f);

    const float3 baseLinDiffColor = pow(baseDiffColor, TO_LINEAR);
    const float3 baseLinSpecColor = pow(baseSpecColor, TO_LINEAR);

    float3 nv = normalize(input.NormW);
    float ndotl = saturate(dot(lightDir, nv));

    float3 diffuse = baseLinDiffColor * LAMBERTIAN * ndotl;

    float3 vv = normalize(cameraPosition - input.PosW);
    float3 hv = normalize(vv + input.NormW);
    float Kspec = (m + 8) / (8 * PI);
    float3 Mspec = baseLinSpecColor * ndotl;
    float magic = pow(saturate(dot(nv, hv)), m);

    float3 specular = Kspec * magic * Mspec;

    float3 final = pow(diffuse + specular, TO_GAMMA);
    return float4(final, 1.0f);

}