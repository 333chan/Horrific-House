struct VS_INPUT
{
    float3 pos : POSITION;
    float3 norm : NORMAL;
    float4 diffuse : COLOR0;
    float4 specular : COLOR1;
    float4 uv0 : TEXCOORD0;
    float4 uv1 : TEXCOORD1;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};

struct VS_OUTPUT
{
    float4 svpos : SV_POSITION;
    float3 pos : POSITION0;
    float3 norm : NORMAL;
    float3 tan : TANGENT;
    float3 bin : BINORMAL;
    float2 uv : TEXCOORD0;
    float4 posInLVP : TEXCOORD1;
    float3 col : COLOR;
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

cbuffer CameraBuffer : register(b6)
{
    //float3 eyePos;
    float4x4 vmat;
    float4x4 Provmat;
}
VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    float3 worldPosition;
    float4 viewPosition;
    float4 pos = float4(input.pos, 1);
    worldPosition = mul(pos, LocalM);
    output.pos = worldPosition;
    viewPosition = float4(mul(worldPosition, ViewM),1.0f);
    
    pos = mul(viewPosition, ProjectionM);
	
    //法線に回転成分を乗算
    float3 norm = mul(input.norm, LocalM);
    float3 tan = mul(input.tangent, LocalM);
    float3 bin = mul(input.binormal, LocalM);
    
    //出力にデータを格納
    output.svpos = pos;
    
    output.uv = input.uv0.xy;
    output.norm = normalize(norm);
    output.tan = normalize(tan);
    output.bin = normalize(bin);
    output.col = input.diffuse.rgb;
    float4 LVP;
    output.posInLVP = viewPosition;
    //LVP = mul(vmat, float4(output.pos, 0.0f));
    //output.posInLVP = mul(Provmat, LVP);
    //output.posInLVP.z = LVP.z;
    return output;
}