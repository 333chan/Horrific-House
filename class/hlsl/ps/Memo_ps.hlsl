//入力
struct PS_INPUT {
	float4 pos    : SV_POSITION;
	float4 diffuse     : COLOR0;
	float2 uv  : TEXCOORD0;
};
//出力
struct PS_OUTPUT
{
	float4 color      : SV_TARGET;
};

float2 barrel(float2 uv)
{
	float s1 = 0.99f;
	float s2 = 0.125f;
	float2 center = 2.0f * uv - 1.0f;
	
	float barrel = min(1.0f - length(center) * s1, 1.0f) * s2;

	return uv - center * barrel;
}

//テクスチャ
SamplerState sam     : register(s0);
Texture2D    screen  : register(t0);

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;

	float2 p = barrel(input.uv);

	float4 color = screen.Sample(sam,p);

	//float2 vig = abs(Vignette(input.uv));
	//vig = pow(vig, 15.0f);

	//少し赤みを出す
	//color.rgb *= float3(1.25f, 0.95f, 0.7f);
	//color.rgb = saturate(color.rgb);
	//color.rgb = color.rgb * color.rgb * (3.0f - 2.0f * color.rgb);
	//color.rgb = saturate(color.rgb);

	//写真の枠を作成する
	float offset = 1080.0f / 1920.0f;
	if ((0.05f * offset > input.uv.x || input.uv.x > 1.0f - 0.05f * offset) ||
		(0.05f > input.uv.y || input.uv.y > 0.95f))
	{
		color.rgb = input.diffuse.rgb;//float3(1.0f,1.0f,1.0f);
	}

	//color.rgb = lerp(color.rgb, float3(1.0f,1.0f,1.0f), vig.x + vig.y);

	//明るめにする
	//color.rgb = saturate(color.rgb + color.rgb);

	output.color = color;

	return output;
}
