//入力
struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 diffuse : COLOR0;
    float2 uv : TEXCOORD0;
};
//出力
struct PS_OUTPUT
{
    float4 color : SV_TARGET;
};

struct DirectionLight
{
    float3 color; // ライトのカラー
    float3 direction; // ライトの方向
};

//struct PointLight
//{
//    float3 position; //座標
//    float3 color; //色
//    float range; //範囲
//};

struct PointLight
{
    float3 position; //座標
    float3 color; //色
    float range; //範囲
    
    //float3 position2; //座標
    //float3 color2; //色
    //float range2; //範囲
};

struct SpotLight
{
    float3 position; //座標
    float3 color; //色
    float range; //範囲
    float3 Direction;
    float angle;
};

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

float3 CalcLambertDiffuse(float3 ligDir, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 ligDir, float3 lightColor, float3 worldPos, float3 normal);

SamplerState sam : register(s0);

Texture2D<float4> albedoTexture : register(t0);
Texture2D<float4> normalTexture : register(t1);
Texture2D<float4> depthTexture  : register(t2);

PS_OUTPUT main(PS_INPUT input)
{
	//// G-Bufferの内容を使ってライティング
 //   float4 albedo = albedoTexture.Sample(sam, input.uv);
 //   float3 normal = normalTexture.Sample(sam, input.uv).xyz;

 //   // step-7 射影空間の深度値からワールド座標を復元する
 //   float z = depthTexture.Sample(sam, input.uv);

 //   // 射影空間の深度値からワールド座標を復元する
 //   float3 worldPos = CalcWorldPosFromUVZ(input.uv, z, mViewProjInv);

 //   float3 lig = 0.0f;

 //   // 視点に向かって伸びるベクトルを計算
 //   float3 toEye = normalize(eyePos - worldPos);

 //   // ディレクションライトを計算
 //   //for (int ligNo = 0; ligNo < NUM_DIRECTION_LIGHT; ligNo++)
 //   //{
 //   //    // 拡散反射光を計算
 //   //    lig += CalcLambertReflection(
 //   //        directionLights[ligNo].direction,
 //   //        directionLights[ligNo].color,
 //   //        normal);

 //   //    // スペキュラ反射を計算
 //   //    lig += CalcSpecularReflection(
 //   //        directionLights[ligNo].direction,
 //   //        directionLights[ligNo].color,
 //   //        normal,
 //   //        toEye);
 //   //}

 //   // step-8 ポイントライトを計算する
 //   for (int ligNo = 0; ligNo < POINT_LIGHT_NUM; ligNo++)
 //   {
 //       // 拡散反射を計算
 //       // 1. 光源からサーファイスに入射するベクトルを計算
 //       float3 ligDir = normalize(worldPos - pointLights[ligNo].position);
 //       // 2. 光源からサーフェイスまでの距離を計算
 //       float distance = length(worldPos - pointLights[ligNo].position);
 //       // 3. 影響率を計算する影響率は0.0～1.0の範囲で、
 //       //    指定した距離（pointsLights[i].range）を超えたら、影響率は0.0になる
 //       float affect = 1.0f - min(1.0f, distance / pointLights[ligNo].range);
 //       // 4. 拡散反射光を加算
 //       lig += CalcLambertDiffuse(
 //           ligDir,
 //           pointLights[ligNo].color,
 //           normal) * affect;

 //       // スペキュラ反射を加算
 //       lig += CalcPhongSpecular(
 //           ligDir,
 //           pointLights[ligNo].color,
 //           normal,
 //           toEye) * affect;
 //   }

 //   float4 finalColor = albedo;
 //   finalColor.xyz *= lig;
 //   return finalColor;
    PS_OUTPUT output;
    output.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
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