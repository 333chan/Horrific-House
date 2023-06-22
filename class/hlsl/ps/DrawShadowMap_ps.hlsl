struct PS_INPUT
{
    float4 svpos : SV_POSITION;
    float3 pos : POSITION;
    float3 norm : NORMAL;
    float3 tan : TANGENT;
    float3 bin : BINORMAL;
    float2 uv : TECOORD;
    float4 posInLVP : TEXCOORD1;
    float3 col : COLOR;
};

struct PS_OUTPUT
{
    float4 color : SV_TARGET;
};
static const int SPOT_LIGHT_NUM = 2; // スポットライトの数

struct SpotLight
{
    float3 position; //座標
    float3 color; //色
    float range; //範囲
    float3 Direction;
    float angle;
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

cbuffer LightBuffer : register(b11)
{
    SpotLight spotLights[SPOT_LIGHT_NUM];
}
PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    float near = 0.1f;
    float far = 100000.0f;
    //float3 depth = spotLights[0].position - input.pos;
    //depth = normalize(depth);

    float linerDepth = 1.0f / (far - near);
  
    linerDepth *= length(float3(input.pos.x, input.pos.y, input.pos.z));
    //linerDepth *= length(float3(3620.0f, 0.0f, 3830.0f));

    //linerDepth *= length(spotLights[0].position);
    //linerDepth *= length(float3(0, 0, 0));
    //linerDepth = 1 -linerDepth;
    output.color = float4(linerDepth, linerDepth, linerDepth, 1.0f);
    //output.color = float4(input.svpos.z, input.svpos.z, input.svpos.z, 1.0f);
    //output.color = float4(input.posInLVP.z, input.posInLVP.z, input.posInLVP.z, 1.0f);
    //output.color = float4(1 - input.posInLVP.z, 1 - input.posInLVP.z, 1 - input.posInLVP.z, 1.0f);

    //output.color = float4(-input.pos.z, -input.pos.z, -input.pos.z, 1.0f);

    return output;
}