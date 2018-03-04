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
    result.NormW = mul(float4(normal, 0.0f), worldMatrix).xyz;
    result.PosH = mul(mul(positionW, viewMatrix), projectionMatrix);

    return result;
}

float4 PS(PSInput input) : SV_TARGET
{
    //float3 ambient = pow(float3(0.2f, 0.3f, 0.5f), 2.2f) * Lambertian * 0.5f * (1.0f + dot(float3(0.0f, 1.0f, 0.0f), normalize(input.NormW)));

	const float m = 256.0f;

    const float3 baseDiffColor = float3(0.5f, 0.5f, 1.0f);
    const float3 baseSpecColor = float3(0.5f, 0.5f, 0.5f);

    const float3 baseLinDiffColor = pow(baseDiffColor, TO_LINEAR);
    const float3 baseLinSpecColor = pow(baseSpecColor, TO_LINEAR);

    const float3 l = normalize(float3(1.0f, 1.0f, 0.0f));
	const float3 n = normalize(input.NormW);
	const float ndotl = max(dot(n, l), 0.0f);
	const float3 v = normalize(cameraPosition - input.PosW);
	const float3 h = normalize((l + v) / 2);

	const float nF = (m + 8) / (8 * PI);

	const float3 specLin = pow(max(dot(n, h), 0.0f), m) * nF * baseLinSpecColor;


	const float3 diffLin = ndotl * baseLinDiffColor * LAMBERTIAN;
	const float3 resultColor = /*pow(diffLin, TO_GAMMA) + */pow(specLin, TO_GAMMA);

    return float4(resultColor, 1.0f);

}