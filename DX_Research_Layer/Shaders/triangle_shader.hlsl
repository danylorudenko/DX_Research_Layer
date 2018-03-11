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

float TrowbridgeReitzGGXNDF(float ndoth, float roughness)
{
	float nominator = roughness * roughness;
	float denominator = ndoth * ndoth * (nominator - 1.0f) + 1.0f;
	denominator = PI * denominator * denominator;

	return nominator / max(denominator, 0.001f);
}

float ShlickGGX(float ndotv, float roughness)
{
	float k = pow(roughness + 1, 2) / 8.0f;

	float denominator = (ndotv * (1 - k)) + k;

	return ndotv / max(denominator, 0.001f);
}

float3 ShlickFresnel(float3 Rf0, float angleCos)
{
	float coef = 1 - angleCos;
	float3 reflectance = ((coef * coef * coef * coef * coef) * (1 - Rf0)) + Rf0;

	return reflectance;
}

float3 CookTorranceBRDF(float ndf, float schlickGGX, float3 fresnel, float ndotv, float ndotl)
{
	float3 nominator = ndf * schlickGGX * fresnel;
	float denominator = 4 * (ndotv) * (ndotl);

	return nominator / max(denominator, 0.001f);
}

float3 CombineDiffuseSpecular(float3 BRDF, float3 diffuse, float ndotl, float metalness)
{
	float3 kS = BRDF;
	float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
	kD *= 1.0f - metalness;

	float3 result = (diffuse * kD) + (BRDF * ndotl);
	return result;
}

float4 PS(PSInput input) : SV_TARGET
{
    //float3 ambient = pow(float3(0.2f, 0.3f, 0.5f), 2.2f) * Lambertian * 0.5f * (1.0f + dot(float3(0.0f, 1.0f, 0.0f), normalize(input.NormW)));

    const float3 baseDiffColor = float3(0.0f, 215.0f, 255.0f ) * CLAMP_256;
    const float3 baseSpecColor = float3(0.5f, 0.5f, 0.5f);

    const float3 baseLinDiffColor = pow(baseDiffColor, TO_LINEAR);
    const float3 baseLinSpecColor = pow(baseSpecColor, TO_LINEAR);


	const float  roughness = 0.01f;
	const float  metalness = 0.9f;
		  float3 R0 = float3(0.04f, 0.04f, 0.04f);
				 R0 = lerp(R0, baseLinSpecColor, metalness);


    const float3 l = normalize(float3(1.0f, 1.0f, 0.0f));
	const float3 n = normalize(input.NormW);
	const float3 v = normalize(cameraPosition - input.PosW);
	const float3 h = normalize((l + v) / 2);
	const float  ndotl = max(dot(n, l), 0.0f);
	const float  ndoth = max(dot(n, h), 0.0f);
	const float  ndotv = max(dot(n, v), 0.0f);

	const float3 fresnel	= ShlickFresnel(R0, ndotv);
	const float  ndf		= TrowbridgeReitzGGXNDF(ndoth, roughness);
	const float  g			= ShlickGGX(ndotv, roughness);
	const float3 specLin	= CookTorranceBRDF(ndf, g, fresnel, ndotv, ndotl);


	const float3 resultLinDiffColor = ndotl * baseLinDiffColor * LAMBERTIAN;
	//const float3 resultLinDiffColor = float3(0.0f, 0.0f, 0.0f);

	const float3 resultLin = CombineDiffuseSpecular(resultLinDiffColor, specLin, ndotl, metalness);
	const float3 resultColor = pow(resultLin, TO_GAMMA);

    return float4(resultColor, 1.0f);

}