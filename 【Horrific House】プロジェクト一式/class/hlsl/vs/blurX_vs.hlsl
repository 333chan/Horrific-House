struct VS_INPUT
{
	float3 pos		 : POSITION0;	//座標
	float4 spos		 : POSITION1;	// 予備座標
	float3 norm		 : NORMAL0;		//法線
	float3 tangent	 : TANGENT0;	//接ベクトル
	float3 binorm	 : BINORMAL0;	//従法線ベクトル
	float4 diffuse	 : COLOR0;		//ディフューズ
	float4 specular  : COLOR1;		//スペキュラ
	float2 uv0		 : TEXCOORD0;	//テクスチャ座標
	float2 uv1		 : TEXCOORD1;	//サブテクスチャ座標
};

cbuffer BaseCBuffer  : register(b1) {
	float4x4	AntiViewportM;				//4x4ビューポート逆行列
	float4x4	ProjectionM;				//4x4プロジェクション行列
	float4x3	ViewM;						//4x3(ビュー行列)
	float4x3	LocalM;						//4x3(回転拡縮平行移動)
	float4		ToonOutLineSize;			// トゥーンの輪郭線の大きさ
	float		DiffuseSource;				// ディフューズカラー( 0.0f:マテリアル  1.0f:頂点 )
	float		SpecularSource;				// スペキュラカラー(   0.0f:マテリアル  1.0f:頂点 )
	float		MulSpecularColor;			// スペキュラカラー値に乗算する値( スペキュラ無効処理で使用 )
	float		Padding;					//詰め物(無視)
}

//バージョンに対応していないのでpsのinputが８個までしか使えない
//uv座標を一つ減らしている
struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 uv0 : TEXCOORD0;
	float4 uv1 : TEXCOORD1;
	float4 uv2 : TEXCOORD2;
	float4 uv3 : TEXCOORD3;
	float4 uv4 : TEXCOORD4;
	float4 uv5 : TEXCOORD5;
	float4 uv6 : TEXCOORD6;
	float4 uv7 : TEXCOORD7;
};

Texture2D<float4> tex : register(t0);

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;

	float4 pos = float4(input.pos, 1);

	pos.xyz = mul(pos, LocalM);		//ワールド
	pos.xyz = mul(pos, ViewM);		//ビュー
	pos = mul(pos, ProjectionM);	//プロジェクション

	float2 textureSize = float2(960.0f, 1080.0f);
	float level;

	//基準のUV座標
	float2 textureUV = input.uv0;

	//　1テクセルずらす
	output.uv0.xy = float2(1.0f / textureSize.x, 0.0f);
	//　3テクセルずらす
	output.uv1.xy = float2(3.0f / textureSize.x, 0.0f);
	//　5テクセルずらす
	output.uv2.xy = float2(5.0f / textureSize.x, 0.0f);
	//　7テクセルずらす
	output.uv3.xy = float2(7.0f / textureSize.x, 0.0f);
	//　9テクセルずらす
	output.uv4.xy = float2(9.0f / textureSize.x, 0.0f);
	//　11テクセルずらす
	output.uv5.xy = float2(11.0f / textureSize.x, 0.0f);
	//　13テクセルずらす
	output.uv6.xy = float2(13.0f / textureSize.x, 0.0f);
	//	15テクセルずらす
	output.uv7.xy = float2(15.0f / textureSize.x, 0.0f);

	//オフセットに-1をかけて逆方向も計算
	output.uv0.zw = output.uv0.xy * -1.0f;
	output.uv1.zw = output.uv1.xy * -1.0f;
	output.uv2.zw = output.uv2.xy * -1.0f;
	output.uv3.zw = output.uv3.xy * -1.0f;
	output.uv4.zw = output.uv4.xy * -1.0f;
	output.uv5.zw = output.uv5.xy * -1.0f;
	output.uv6.zw = output.uv6.xy * -1.0f;
	output.uv7.zw = output.uv7.xy * -1.0f;

	//基準の位置を加える
	output.uv0 += float4(textureUV, textureUV);
	output.uv1 += float4(textureUV, textureUV);
	output.uv2 += float4(textureUV, textureUV);
	output.uv3 += float4(textureUV, textureUV);
	output.uv4 += float4(textureUV, textureUV);
	output.uv5 += float4(textureUV, textureUV);
	output.uv6 += float4(textureUV, textureUV);
	output.uv7 += float4(textureUV, textureUV);

	output.pos = pos;

	return output;
}