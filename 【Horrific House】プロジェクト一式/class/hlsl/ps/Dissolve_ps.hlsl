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
    float count;
    float time;
}

float nrand(float x, float y)
{
    return frac(sin(dot(float2(x, y), float2(12.9898, 78.233))) * 43758.5453);
}

//テクスチャ
SamplerState sam : register(s0);
Texture2D screen : register(t0);
Texture2D noise : register(t1);
Texture2D bg    : register(t2);

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    float4 color = float4(1.0f, count, 1.0f, 1.0f);
    float4 edgeCol = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    //color = screen.Sample(sam, input.uv);//Tex2Dと一緒
    
    float4 dissolve = noise.Sample(sam, input.uv);
    float alpha = dissolve.r * 0.2 + dissolve.g * 0.7 + dissolve.b * 0.1;
    
    if (alpha > count)
    {
        discard;
    }
    if (alpha < count + 0.08 && count > 0)
    {
        edgeCol.gb = 0.0f; //(1.0f,0.0f,0.0f,1.0f);
    }
    color = bg.Sample(sam, input.uv) * edgeCol;
    
     //アナロググリッチ
    float2 scanLineJitter = float2(1.0f, 0.9f); //(displacement,threshold)
    float2 verticalJump = float2(0.01f, time); //(amount,time)
    float horizontalShake = 0.001f;
    float2 colorDrift = float2(0.001f, time); //(amount,time)
    float u = input.uv.x;
    float v = input.uv.y;
    
    float jitter;
    float jump;
    float shake;
    float drift;
    float4 src1;
    float4 src2;
    
    //if (4.4f < time && time < 4.5f || 5.0f < time && time < 5.5f || 5.7f < time && time < 6.0 || 6.0f < time && time < 6.2f
    //    || 6.4f < time && time < 6.5f || 7.0f < time && time < 7.5f || 7.7f < time && time < 8.0 || 8.0f < time && time < 9.2f || (9.5f < time && (int) time % 2 == 0))
    {
    
       
        // Scan line jitter
        jitter = nrand(v, time) * 2 - 1;
        jitter *= step(scanLineJitter.y, abs(jitter)) * scanLineJitter.x;

        // Vertical jump
        jump = lerp(v, frac(v + verticalJump.y), verticalJump.x);

        // Horizontal shake
        shake = (nrand(time, 2) - 0.5) * horizontalShake;

        // Color drift
        drift = sin(jump + colorDrift.y) * colorDrift.x;

        src1 = bg.Sample(sam, frac(float2(u + jitter + shake, jump)));
        src2 = bg.Sample(sam, frac(float2(u + jitter + shake + drift, jump)));
        color = float4(src1.r, src2.g, src1.b, 1);
    }
    
    //color.rgb = lerp(color.rgb, (0.0f).xxx, crt.x + crt.y);
    output.color = color;
    
    output.color = color;
    return output;
}