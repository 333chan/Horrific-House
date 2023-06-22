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

struct DirectionLight
{
    float3 color; // ライトのカラー
    float3 direction; // ライトの方向
};

struct PointLight
{
    float3 position;    //座標
    float3 color;       //色
    float range;        //範囲
};

struct SpotLight
{
    float3 position; //座標
    float3 color; //色
    float range; //範囲
    float3 Direction;
    float angle;
};

//float3 CalcLigFromPointLight(PS_Input psIn);
//float3 CalcLigFromDirectionLight(PS_Input psIn);

float3 CalcLambertDiffuse(float3 ligDir, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 ligDir, float3 lightColor, float3 worldPos, float3 normal);


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

cbuffer CameraBuffer : register(b8)
{
    float3 eyePos;
    matrix vmat;
}

cbuffer LightBuffer : register(b9)
{
    SpotLight spotLights; //スポットライト配列
}
SamplerState      sam  : register(s0);
Texture2D<float4> tex  : register(t0);
Texture2D<float4> norm : register(t1);

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    //float3 normal = norm.Sample(sam, input.uv).xyz;

    float3 ligDir = input.pos - spotLights.position;
    ligDir = normalize(ligDir);

    // 減衰なしのLambert拡散反射光を計算する
    float3 diffSpotLight = CalcLambertDiffuse(
        ligDir, // ライトの方向
        spotLights.color, // ライトのカラー
        input.norm // サーフェイスの法線
    );

    // 減衰なしのPhong鏡面反射光を計算する
    float3 specSpotLight = CalcPhongSpecular(
        ligDir, // ライトの方向
        spotLights.color, // ライトのカラー
        input.pos, // サーフェイスのワールド座標
        input.norm // サーフェイスの法線
    );
    
    // 距離による影響率を計算する
    // スポットライトとの距離を計算する
    float3 distance = length(input.pos - spotLights.position);

    // 影響率は距離に比例して小さくなっていく
    float affect = 1.0f - 1.0f / spotLights.range * distance;

    // 影響力がマイナスにならないように補正をかける
    if (affect < 0.0f)
    {
        affect = 0.0f;
    }

    // 影響の仕方を指数関数的にする。今回のサンプルでは3乗している
    affect = pow(affect, 3.0f);

    // 影響率を乗算して影響を弱める
    diffSpotLight *= affect;
    specSpotLight *= affect;

    // 入射光と射出方向の角度を求める
    // dot()を利用して内積を求める
    float angle = dot(ligDir, spotLights.Direction);

    // dot()で求めた値をacos()に渡して角度を求める
    angle = abs(acos(angle));

    // 角度による影響率を求める
    // 角度に比例して小さくなっていく影響率を計算する
    affect = 1.0f - 1.0f / spotLights.angle * angle;

    // 影響率がマイナスにならないように補正をかける
    if (affect < 0.0f)
    {
        affect = 0.0f;
    }

    // 影響の仕方を指数関数的にする。今回のサンプルでは0.5乗している
    affect = pow(affect, 0.5f);

    // 角度による影響率を反射光に乗算して、影響を弱める
    diffSpotLight *= affect;
    specSpotLight *= affect;

    // スポットライトの反射光を最終的な反射光に足し算する
    float3 finalLig = float3(0.1f, 0.1f,0.1f);
    finalLig += diffSpotLight + specSpotLight;
    float4 finalColor = tex.Sample(sam, input.uv);
    
     // 環境光を加算
    finalColor.xyz *= finalLig;
    output.color = finalColor;
    //normalize(ligDir);
    //output.color = float4(spotLights.position, 1);
    //output.color = float4(ligDir, 1);

    //output.color = float4(spotLights.position, 1);

    return output;
}
/// <summary>
/// Lambert拡散反射光を計算する
/// </summary>
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
    // ピクセルの法線とライトの方向の内積を計算する
    float t = dot(normal, lightDirection) * -1.0f;

    // 内積の値を0以上の値にする
    t = max(0.0f, t);

    // 拡散反射光を計算する
    return lightColor * t;
}

/// <summary>
/// Phong鏡面反射光を計算する
/// </summary>
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal)
{
    // 反射ベクトルを求める
    float3 refVec = reflect(lightDirection, normal);

    // 光が当たったサーフェイスから視点に伸びるベクトルを求める
    float3 toEye = eyePos - worldPos;
    toEye = normalize(toEye);

    // 鏡面反射の強さを求める
    float t = dot(refVec, toEye);

    // 鏡面反射の強さを0以上の数値にする
    t = max(0.0f, t);

    // 鏡面反射の強さを絞る
    t = pow(t, 5.0f);

    // 鏡面反射光を求める
    return lightColor * t;
}

/// <summary>
/// ポイントライトによる反射光を計算
/// </summary>
/// <param name="psIn">ピクセルシェーダーに渡されている引数</param>
//float3 CalcLigFromPointLight(PS_Input psIn)
//{
//    // このサーフェイスに入射しているポイントライトの光の向きを計算する
//    float3 ligDir = psIn.worldPos - ptPosition;

//    // 正規化して大きさ1のベクトルにする
//    ligDir = normalize(ligDir);

//    // 減衰なしのLambert拡散反射光を計算する
//    float3 diffPoint = CalcLambertDiffuse(
//        ligDir, // ライトの方向
//        ptColor, // ライトのカラー
//        psIn.normal // サーフェイスの法線
//    );

//    // 減衰なしのPhong鏡面反射光を計算する
//    float3 specPoint = CalcPhongSpecular(
//        ligDir, // ライトの方向
//        ptColor, // ライトのカラー
//        psIn.worldPos, // サーフェイズのワールド座標
//        psIn.normal     // サーフェイズの法線
//    );

//    // 距離による影響率を計算する
//    // ポイントライトとの距離を計算する
//    float3 distance = length(psIn.worldPos - ptPosition);

//    // 影響率は距離に比例して小さくなっていく
//    float affect = 1.0f - 1.0f / ptRange * distance;

//    // 影響力がマイナスにならないように補正をかける
//    if (affect < 0.0f)
//    {
//        affect = 0.0f;
//    }

//    // 影響の仕方を指数関数的にする。今回のサンプルでは3乗している
//    affect = pow(affect, 3.0f);

//    // 拡散反射光と鏡面反射光に減衰率を乗算して影響を弱める
//    diffPoint *= affect;
//    specPoint *= affect;

//    return diffPoint + specPoint;
//}

/// <summary>
/// ディレクションライトによる反射光を計算
/// </summary
/// <param name="psIn">ピクセルシェーダーからの入力。</param>
//float3 CalcLigFromDirectionLight(PS_Input psIn)
//{
//    // ディレクションライトによるLambert拡散反射光を計算する
//    float3 diffDirection = CalcLambertDiffuse(dirDirection, dirColor, psIn.normal);

//    // ディレクションライトによるPhong鏡面反射光を計算する
//    float3 specDirection = CalcPhongSpecular(
//            dirDirection, dirColor, psIn.worldPos, psIn.normal);
//    return diffDirection + specDirection;
//}

