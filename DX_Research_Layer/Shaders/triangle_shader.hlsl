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
    float4 cameraPosition;
	float4 lightDir;

};

Texture2D albedoMap				: register(t0);
Texture2D normalMap				: register(t1);
Texture2D metalnessMap			: register(t2);
Texture2D roughnessMap			: register(t3);

SamplerState samplr : register(s0);

cbuffer TransformBuffer : register(b1)
{
    float4x4 worldMatrix;
};

struct PSInput
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3x3 tbn : VERTEXSPACE;
	float2 uv : TEXCOORD;
};

PSInput VS(float3 position : POSITION, float3 normal : NORMAL, float3 tangent : TANGENT, float3 bitangent : BITANGENT, float2 uv : TEXCOORD)
{
    PSInput result;

    float4 positionW = mul(float4(position, 1.0f), worldMatrix);
    result.PosW = positionW.xyz;
    result.PosH = mul(mul(positionW, viewMatrix), projectionMatrix);
	result.uv = uv;

	float3 tW = mul(normalize(float4(tangent, 0.0f)), worldMatrix).xyz;
	float3 bW = mul(normalize(float4(bitangent, 0.0f)), worldMatrix).xyz;
    float3 nW = mul(normalize(float4(normal, 0.0f)), worldMatrix).xyz;
	result.tbn = float3x3(tW, bW, nW);

    return result;
}

float3 ShlickFresnel(float3 Rf0, float cosTheta)
{
	float r = 1.0f - cosTheta;
	return (pow(r, 5.0f) * (1 - Rf0)) + Rf0;
}

float GeometryGGX(float ndotv, float roughness)
{
	float k = pow(roughness + 1, 2) / 8;

	float nom = ndotv;
	float denom = ndotv * (1.0f - k) + k;
	return nom / denom;
}

float SmithGGX(float ndotv, float ndotl, float roughness)
{
	float vGGX = GeometryGGX(ndotv, roughness);
	float lGGX = GeometryGGX(ndotl, roughness);

	return lGGX * vGGX;
}

float TrowbridgeReitzNDF(float ndoth, float roughness)
{
	float r2 = roughness * roughness;
	float ndoth2 = ndoth * ndoth;

	float nom = r2;
	float denom = ndoth2 * (r2 - 1.0f) + 1.0f;
	denom = denom * denom * PI;

	return nom / denom;
}

float3 MetallicFresnelReflectance(float3 diffuse, float metalness)
{
	const float3 dRf0 = float3(0.04f, 0.04f, 0.04f);
	const float3 result = lerp(dRf0, diffuse, metalness);

	return result;
}

float3 CookTorranceSpecular(float3 fresnel, float geometry, float ndf, float ndotv, float ndotl)
{
	float3 nom = ndf * geometry * fresnel;
	float denom = 4 * ndotv * ndotl;

	return nom / denom;
}

float3 CookTorranceMix(float3 specular, float3 diffuse, float metalness)
{
	float3 kS = specular;
	float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
	kD *= 1.0f - metalness;

	float3 result = max(specular, 0.0f) + diffuse * kD;
	return result;
}

float4 PS(PSInput input) : SV_TARGET
{
	const float3 ambient = float3(0.01f, 0.01f, 0.015f);
	const float3 baseDiffColor = albedoMap.Sample(samplr, input.uv).xyz + float3(0.5f, 0.0f, 0.0f);
    const float3 baseLinDiffColor = pow(baseDiffColor, TO_LINEAR);

	float  textureMetalness = metalnessMap.Sample(samplr, input.uv).r;
	float  textureRoughness = roughnessMap.Sample(samplr, input.uv).r;
	float3 textureNormal	= normalMap.Sample(samplr, input.uv).xyz;

	const float3 nA = float3(textureNormal * 2) - 1;
	const float3 n = normalize(mul(float3(nA), input.tbn));
    //const float3 l = normalize(float3(1.0f, 1.0f, 0.0f));
	const float3 l = normalize(lightDir.xyz);
	const float3 v = normalize(cameraPosition.xyz - input.PosW);
	const float3 h = normalize(l + v);
	const float  ndotl = max(dot(n, l), 0.0f);
	const float  ndoth = max(dot(n, h), 0.0f);
	const float  ndotv = max(dot(n, v), 0.0f);

	float3 R0 = MetallicFresnelReflectance(baseLinDiffColor, textureMetalness);

	float3 fresnel = ShlickFresnel(R0, ndotv) * 0.5f;
	float  geometry = SmithGGX(ndotv, ndotl, textureRoughness);
	float  ndf = TrowbridgeReitzNDF(ndoth, textureRoughness);

	float3 spec = CookTorranceSpecular(fresnel, geometry, ndf, ndotv + 0.01f, ndotl) * (ndotl + 0.01f);
	float3 diffuse = baseLinDiffColor * LAMBERTIAN * (ndotl + 0.01f);

	float3 resultColor = CookTorranceMix(spec, diffuse, textureMetalness) + ambient;
    //return float4(input.NormW, 1.0f);
	//return float4(input.tbn[0], 1.0f);
	//return float4(n, 0.0f);
	return float4(pow(resultColor, TO_GAMMA), 1.0f);

}