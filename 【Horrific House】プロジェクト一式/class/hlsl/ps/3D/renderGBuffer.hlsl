struct PS_INPUT
{
    float4 svpos : SV_POSITION;
    float3 pos   : POSITION;
    float3 norm  : NORMAL;
    float3 tan   : TANGENT;
    float3 bin   : BINORMAL;
    float2 uv    : TECOORD;
    float3 col   : COLOR;
};

struct PS_OUTPUT
{
    float4 albedo : SV_TARGET0;
    float4 normal : SV_TARGET1;
    float4 depth  : SV_TARGET2;
};

SamplerState sam : register(s0);
Texture2D<float4> tex : register(t0);

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    
    output.albedo = tex.Sample(sam, input.uv);
    
    output.normal.xyz = input.norm;
    
    output.normal.w = 1.0f;
    
    output.depth = input.pos.z;
    output.depth = input.pos.z;
    output.depth = input.pos.z;
    output.depth = 1.0f;
    return output;
    
	//return float4(1.0f, 1.0f, 1.0f, 1.0f);
}