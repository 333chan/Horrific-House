struct PS_INPUT
{
    float4 svpos : SV_POSITION;
    float3 pos   : POSITION;
    float3 norm  : NORMAL;
    float3 tan   : TANGENT;
    float3 bin   : BINORMAL;
    float2 uv : TEXCOORD0;
    float4 posInLVP : TEXCOORD1;
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
static const int SPOT_LIGHT_NUM = 2; // スポットライトの数
static const int far = 10000;
static const int near = 3000;

cbuffer CameraBuffer : register(b8)
{
    matrix vmat;    //カメラのマトリックス
}
cbuffer DirectionLightBuffer : register(b9)
{
    DirectionLight directionLights[DIRECTION_LIGHT_NUM]; //ディレクションライト
}
cbuffer PointLightBuffer : register(b10)
{
    PointLight pointLights[POINT_LIGHT_NUM]; //ポイントライト
}
cbuffer SpotLightBuffer : register(b11)
{
    SpotLight spotLights[SPOT_LIGHT_NUM]; // スポットライト
}
cbuffer AmbientLightBuffer : register(b12)
{
    float3 ambientLight; // アンビエントライト
    float specPow;
}

SamplerState sam : register(s0);
Texture2D<float4> tex : register(t0);       //テクスチャ
Texture2D<float4> norm : register(t1);      //ノーマルマップ
Texture2D<float4> depth : register(t10);    //深度マップ

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    //タンジェントスペースの法線でサンプリングする
    float3 texNormal;
    texNormal = norm.Sample(sam, input.uv).xyz;
    texNormal = (texNormal - 0.5f) * 2.0f;
    //タンジェントスペース法線をワールドスペースに変換する
    float3 normal = input.tan * texNormal.x+ input.bin * texNormal.y + input.norm * texNormal.z;
    //カメラの位置
    float3 EyePos = spotLights[0].position;
    float3 pointLig = 0.0f;
    float3 worldPos = input.pos;
    //カメラとのベクトル作成
    float3 toEye = EyePos - worldPos;
    //長さを取得
    float linerPos = length(EyePos - worldPos);
    //フォグをかける範囲を作成
    float fogfactor = clamp((far - linerPos) / (far - near), 0.0, 1.0);
    
    toEye = normalize(toEye);
    
    //ポイントライトの処理
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
            EyePos,
            normal) * affect;
    }
    
   //スポットライトの処理
    float3 ligDir[SPOT_LIGHT_NUM];
    float3 diffSpotLight[SPOT_LIGHT_NUM];
    float3 specSpotLight[SPOT_LIGHT_NUM];
    float3 finalLig = pointLig + ambientLight;
    for (int spLigNo = 0; spLigNo < SPOT_LIGHT_NUM; spLigNo++)
    {
        ligDir[spLigNo] = input.pos - spotLights[spLigNo].position;
        ligDir[spLigNo] = normalize(ligDir[spLigNo]);

    // 減衰なしのLambert拡散反射光を計算する
        
        diffSpotLight[spLigNo] = CalcLambertDiffuse(
        ligDir[spLigNo], // ライトの方向
        spotLights[spLigNo].color, // ライトのカラー
        normal // サーフェイスの法線
    );

    // 減衰なしのPhong鏡面反射光を計算する
        specSpotLight[spLigNo] = CalcPhongSpecular(
        ligDir[spLigNo], // ライトの方向
        spotLights[spLigNo].color, // ライトのカラー
        worldPos, // サーフェイスのワールド座標
        normal // サーフェイスの法線
    );
    
    // 距離による影響率を計算する
    // スポットライトとの距離を計算する
        float3 distance = length(input.pos - spotLights[spLigNo].position);

    // 影響率は距離に比例して小さくなっていく
        float affect = 1.0f - 1.0f / spotLights[spLigNo].range * distance;

    // 影響力がマイナスにならないように補正をかける
        if (affect < 0.0f)
        {
            affect = 0.0f;
        }

    // 影響の仕方を指数関数的にする。今回のサンプルでは3乗している
        affect = pow(affect, 3.0f);

    // 影響率を乗算して影響を弱める
        diffSpotLight[spLigNo] *= affect;
        specSpotLight[spLigNo] *= affect;

    // 入射光と射出方向の角度を求める
    // dot()を利用して内積を求める
        float angle = dot(ligDir[spLigNo], spotLights[spLigNo].Direction);

    // dot()で求めた値をacos()に渡して角度を求める
        angle = abs(acos(angle));

    // 角度による影響率を求める
    // 角度に比例して小さくなっていく影響率を計算する
        affect = 1.0f - 1.0f / spotLights[spLigNo].angle * angle;
    
    // 影響率がマイナスにならないように補正をかける
        if (affect < 0.0f)
        {
            affect = 0.0f;
        }

    // 影響の仕方を指数関数的にする。今回のサンプルでは0.5乗している
        affect = pow(affect, 0.5f);

    // 角度による影響率を反射光に乗算して、影響を弱める
        diffSpotLight[spLigNo] *= affect;
        specSpotLight[spLigNo] *= affect;
    
    // スポットライトの反射光を最終的な反射光に足し算する
        finalLig += diffSpotLight[spLigNo] + specSpotLight[spLigNo];
        
    }
    //テクスチャをサンプリングする
    float4 finalColor = tex.Sample(sam, input.uv);
    //色と真っ黒の線形補間を行いフォグを作成している
    finalColor = lerp(float4(0, 0, 0, 1), finalColor, fogfactor);
     // 環境光を加算
    finalColor.xyz *= finalLig;
    //完成した色を代入
    output.color = finalColor ;
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
    float3 EyePos = spotLights[0].position;
    float3 toEye = EyePos - worldPos;
    toEye = normalize(toEye);

    // 鏡面反射の強さを求める
    float t = dot(refVec, toEye);

    // 鏡面反射の強さを0以上の数値にする
    t = max(0.0f, t);

    // 鏡面反射の強さを絞る
    t = pow(t, 1.0f);

    // 鏡面反射光を求める
    return lightColor * t;
}