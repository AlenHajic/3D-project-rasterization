//Constant buffer for the matrices
cbuffer ConstantBuffer : register(b0)
{
    float4x4 world;
    float4x4 viewProjectionMatrix;
};

//Input
struct VertexShaderInput
{
    float3 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

//Output 
struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float4 worldPosition : WORLD_POSITION;
    float2 uv : UV;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;

    // Transform vertex position with World, then ViewProjection
    output.worldPosition = mul(float4(input.position, 1.0f), world);
    output.position = mul(output.worldPosition, viewProjectionMatrix);

    // Transform normal to world space (no projection)
    output.normal = normalize(mul(input.normal, (float3x3)world));

    // Pass UV as is
    output.uv = input.uv;

    return output;
}
