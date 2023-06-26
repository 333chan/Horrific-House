struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 diffuse : COLOR0;
    float2 uv : TEXCOORD0;
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



float CRT(float2 uv)
{
    float2 nu = uv * 2.0f - 1.0f;
    float2 offset = abs(nu.yx) / float2(6.0f, 4.0f);
    nu += nu * offset * offset;
    return nu;
}

struct PS_OUTPUT
{
    float4 color : SV_TARGET;
};



//テクスチャ
SamplerState sam : register(s0);
Texture2D screen : register(t0);
Texture2D noise : register(t1);

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float4 edgeCol = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    //color = screen.Sample(sam, input.uv);//Tex2Dと一緒
    
    //ブラウン管ぽいやつ

    float2 crt = CRT(input.uv);
    //crt = abs(crt);
    //crt = pow(crt, 15.0f);
    //color.rgb = lerp(color.rgb, (0.0f).xxx, crt.x + crt.y);
    
    //ディゾルブ
    float4 dissolve = noise.Sample(sam, input.uv);
    float alpha = dissolve.r * 0.2 + dissolve.g * 0.7 + dissolve.b * 0.1;
    
    if (alpha < count)
    {
        discard;
    }
    if (alpha < count + 0.08 && count > 0)
    {
        edgeCol.gb = 0.0f; //(1.0f,0.0f,0.0f,1.0f);
    }
    color = screen.Sample(sam, input.uv) * edgeCol;
    
    
    
    float2 scrolluv = input.uv;
    //scrolluv.y += frac(count);
    //color = screen.Sample(sam, scrolluv);
    
    //float r = screen.Sample(sam, scrolluv + 0.01 * sin(time)).r;
    //float b = screen.Sample(sam, scrolluv - 0.01 * sin(time)).b;
    //float2 ga = screen.Sample(sam, scrolluv).ga;
    
    //color.r = r;
    //color.b = b;
    //color.ga = ga;
    
     //アナロググリッチ
    float2 scanLineJitter = float2(1.0f, 0.9f); //(displacement,threshold)
    float2 verticalJump = float2(0.01f, time); //(amount,time)
    float horizontalShake = 0.001f;
    float2 colorDrift = float2(0.01f, time); //(amount,time)
    float u = input.uv.x;
    float v = input.uv.y;
    
    float jitter;
    float jump;
    float shake;
    float drift;
    float4 src1;
    float4 src2;
    
    float num = nrand(v, time) * 2 - 1;
    if (0.4f < time && time < 0.5f || 1.0f < time && time < 1.5f || 1.7f < time && time < 2.0 || 2.0f < time && time < 2.2f
        || 2.4f < time && time < 2.5f || 3.0f < time && time < 3.5f || 3.7f < time && time < 4.0 || 4.0f < time && time < 4.2f)
   // if (0.1f < num && num < 0.12f)
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

        src1 = screen.Sample(sam, frac(float2(u + jitter + shake, jump)));
        src2 = screen.Sample(sam, frac(float2(u + jitter + shake + drift, jump)));
        color = float4(src1.r, src2.g, src1.b, 1);
    }
    
    //color.rgb = lerp(color.rgb, (0.0f).xxx, crt.x + crt.y);
    output.color = color;
    return output;
}