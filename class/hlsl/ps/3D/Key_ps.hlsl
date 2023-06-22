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
    float3 position; //座標
    float3 color; //色
    float range; //範囲
};

struct SpotLight
{
    float3 position; //座標
    float3 color; //色
    float range; //範囲
    float3 Direction;
    float angle;
};

float3 CalcLambertDiffuse(float3 ligDir, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 ligDir, float3 lightColor, float3 worldPos, float3 normal);
float3 CalcPointLight();

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

static const int DIRECTION_LIGHT_NUM = 2; // ディレクションライトの数
static const int POINT_LIGHT_NUM = 3; // ポイントライトの数

cbuffer CameraBuffer : register(b8)
{
    float3 eyePos;
    matrix vmat;
}

cbuffer LightBuffer : register(b9)
{
    DirectionLight directionLights[DIRECTION_LIGHT_NUM];
}

cbuffer LightBuffer : register(b10)
{
    PointLight pointLights[POINT_LIGHT_NUM];
}

cbuffer LightBuffer : register(b11)
{
    SpotLight spotLights;
}

cbuffer LightBuffer : register(b12)
{
    float3 ambientLight; // アンビエントライト
    float specPow;
}

SamplerState sam : register(s0);
Texture2D<float4> tex : register(t0);
Texture2D<float4> norm : register(t1);

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    output.color = float4(1, 1, 1, 1);
    return output;
    float3 normal = norm.Sample(sam, input.uv).xyz;

    float3 pointLig = 0.0f;
    float3 worldPos = input.pos;
    float3 toEye = eyePos - worldPos;
    float linerPos = length(toEye);
    toEye = normalize(toEye);

    for (int ligNo = 0; ligNo < POINT_LIGHT_NUM; ligNo++)
    {
        // 拡散反射を計算
        // 1. 光源からサーファイスに入射するベクトルを計算
        float3 ligDir = normalize(worldPos - pointLights[ligNo].position);
        // 2. 光源からサーフェイスまでの距離を計算
        float distance = length(worldPos - pointLights[ligNo].position);
        // 3. 影響率を計算する影響率は0.0～1.0の範囲で、
        //    指定した距離（pointsLights[i].range）を超えたら、影響率は0.0になる
        float affect = 1.0f - min(1.0f, distance / pointLights[ligNo].range);
        // 4. 拡散反射光を加算
        pointLig += CalcLambertDiffuse(
            ligDir,
            pointLights[ligNo].color,
            normal) * affect;

        // スペキュラ反射を加算
        pointLig += CalcPhongSpecular(
            ligDir,
            pointLights[ligNo].color,
            normal,
            toEye) * affect;
    }

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
    float3 finalLig = pointLig + ambientLight;
    finalLig += diffSpotLight + specSpotLight;
    float4 finalColor = tex.Sample(sam, input.uv);

    // 環境光を加算
    finalColor.xyz *= finalLig;
    finalColor.rgb = saturate(finalColor.rgb - float3(0.5f, 0.5f, 0.5f));

    output.color = finalColor;

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
