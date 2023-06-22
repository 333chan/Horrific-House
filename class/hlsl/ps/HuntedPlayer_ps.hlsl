struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 diffuse : COLOR0;
    float2 uv : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 color : SV_TARGET;
};

//定数
cbuffer buff : register(b8)
{
    float none;
    float count;
}

float2 Frame(float2 uv)
{
    float2 uvN = uv * 2.0f - 1.0f;
    float2 offset = abs(uvN.xy) / float2(19.2f, 10.8f);
    uvN += uvN * offset;

    return uvN;
}

//乱数
float rand(float2 uv)
{
    return frac(sin(dot(uv, float2(12.9898, 78.233)) + count) * 43758.5453);
}

//ノイズ
float4 noise(float2 uv)
{
    float n = rand(uv);
    return float4(n, n, n, 0.3f);
}

//テクスチャ
SamplerState sam : register(s0);
Texture2D screen : register(t0);

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    float4 n = noise(input.uv);

    float4 color = screen.Sample(sam, input.uv);

    float2 frame = abs(Frame(input.uv));
    frame = pow(frame, 15.0f);

    color.rgb = lerp(color.rgb + float3(n.y * 0.1f, n.y * 0.1f, n.z * 0.1f), float3(n.x, 0.0f, 0.0f), frame.x + frame.y);
    output.color = color;

    return output;
}
