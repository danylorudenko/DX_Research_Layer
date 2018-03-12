#pragma pack_matrix(row_major)

static const float PI = 3.14159265359f;
static const float LAMBERTIAN = 1.0f / PI;
static const float TO_LINEAR = 2.2f;
static const float TO_GAMMA = 0.45454545454f;
static const float CLAMP_256 = 1.0f / 256.0f;

cbuffer SceneBuffer : register(b0)
{
    float4x4 projectionMatrix;
    float4x4 viewMatrix;
    float4 cameraPosition_roughness;
	float metalness;

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

float3 ShlickFresnel(float3 Rf0, float cosTheta)
{
	float r = 1.0f - cosTheta;
	return (pow(r, 5.0f) * (1 - Rf0)) + Rf0;
}

float4 PS(PSInput input) : SV_TARGET
{
    const float3 baseDiffColor = float3(0.0f, 215.0f, 255.0f ) * CLAMP_256;
    const float3 baseSpecColor = float3(0.5f, 0.5f, 0.5f);

    const float3 baseLinDiffColor = pow(baseDiffColor, TO_LINEAR);
    const float3 baseLinSpecColor = pow(baseSpecColor, TO_LINEAR);

	float3 R0 = float3(0.02f, 0.02f, 0.02f);
	//R0 = lerp(R0, baseLinDiffColor, metalness);

    const float3 l = normalize(float3(1.0f, 1.0f, 0.0f));
	const float3 n = normalize(input.NormW);
	const float3 v = normalize(cameraPosition_roughness.xyz - input.PosW);
	const float3 h = normalize(l + v);
	const float  ndotl = max(dot(n, l), 0.0f);
	const float  ndoth = max(dot(n, h), 0.0f);
	const float  ndotv = dot(n, v);

	float3 fresnel = ShlickFresnel(R0, ndotv);
	float3 specular = fresnel * baseLinSpecColor;

	float3 diffuse = baseLinDiffColor * LAMBERTIAN * ndotl;

	float3 resultLin = specular;
	float3 resultColor = pow(resultLin, TO_GAMMA);

    return float4(resultColor, 1.0f);

}