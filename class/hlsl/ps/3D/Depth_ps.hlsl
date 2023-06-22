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
    float4 color : SV_TARGET;
};

cbuffer BaseCBuffer : register(b1)
{
    matrix AntiViewportM; //4x4ビューポート逆行列
    matrix ProjectionM; //4x4プロジェクション行列
    float4x3 ViewM; //4x3(ビュー行列)
    float4x3 LocalM; //4x3(回転拡縮平行移動)
    float4 ToonOutLineSize; // トゥーンの輪郭線の大きさ
    float DiffuseSource; // ディフューズカラー( 0.0f:マテリアル  1.0f:頂点 )
    float SpecularSource; // スペキュラカラー(   0.0f:マテリアル  1.0f:頂点 )
    float MulSpecularColor; // スペキュラカラー値に乗算する値( スペキュラ無効処理で使用 )
    float Padding; //詰め物(無視)
}

cbuffer CbufDOF : register(b13)
{
    float3 Focus;
    float3 Area;
}

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    
	// 被写界深度の範囲内を 0.0f ～ 1.0f に変換
    if (input.pos.z < Area.x)
    {
        output.color.r = 0.0f;
    }
    else if (input.pos.z > Area.y)
    {
        output.color.r = 1.0f;
    }
    else
    {
        output.color.r = (input.pos.z - Area.x) * Area.z;
    }
    //Output.color.r = 1.0f;
    output.color.g = output.color.r;
    output.color.b = output.color.r;
    output.color.a = 1.0f;
    
    //Output.color.r = PSInput.pos.z;
    //Output.color.g = PSInput.pos.z;
    //Output.color.b = PSInput.pos.z;
    //Output.color.a = 1.0f;
    return output;
}