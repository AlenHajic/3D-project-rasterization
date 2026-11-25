// Texture och Sampler
Texture2D shaderTexture : register(t0);
SamplerState samplerState : register(s0);

//Constant buffer for light and material properties
cbuffer ConstBuffer : register(b0)
{
    float4 lightPosition;
    float4 lightColor;
    float4 cameraPosition;
    float ambientLightIntensity;
    float shininess;
}

//Input
struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float4 worldPosition : WORLD_POSITION;
    float2 uv : UV;
};

//Light/Shader outputs
float4 main(PixelShaderInput input) : SV_TARGET
{
    float3 normalizedNormal = normalize(input.normal.xyz);
    float3 lightDirection = normalize(lightPosition.xyz - input.worldPosition.xyz);

    float diffuseIntensity = max(dot(normalizedNormal, lightDirection), 0.0f);
    float4 diffuseLight = lightColor * diffuseIntensity;

    float3 reflection = reflect(-lightDirection, normalizedNormal);
    float3 vectorCamera = normalize(cameraPosition.xyz - input.worldPosition.xyz);
    float specularIntensity = pow(max(dot(reflection, vectorCamera), 0.0f), shininess);
    float4 specularLight = lightColor * specularIntensity;

    float4 ambientLight = lightColor * ambientLightIntensity;

    float4 texColor = shaderTexture.Sample(samplerState, input.uv);

    return (ambientLight + diffuseLight) * texColor + specularLight;
}
