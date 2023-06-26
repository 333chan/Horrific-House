//����
struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 diffuse : COLOR0;
    float2 uv : TEXCOORD0;
};
//�o��
struct PS_OUTPUT
{
    float4 color : SV_TARGET;
};

struct DirectionLight
{
    float3 color; // ���C�g�̃J���[
    float3 direction; // ���C�g�̕���
};

//struct PointLight
//{
//    float3 position; //���W
//    float3 color; //�F
//    float range; //�͈�
//};

struct PointLight
{
    float3 position; //���W
    float3 color; //�F
    float range; //�͈�
    
    //float3 position2; //���W
    //float3 color2; //�F
    //float range2; //�͈�
};

struct SpotLight
{
    float3 position; //���W
    float3 color; //�F
    float range; //�͈�
    float3 Direction;
    float angle;
};

static const int DIRECTION_LIGHT_NUM = 2; // �f�B���N�V�������C�g�̐�
static const int POINT_LIGHT_NUM = 3; // �|�C���g���C�g�̐�

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
    float3 ambientLight; // �A���r�G���g���C�g
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
	//// G-Buffer�̓��e���g���ă��C�e�B���O
 //   float4 albedo = albedoTexture.Sample(sam, input.uv);
 //   float3 normal = normalTexture.Sample(sam, input.uv).xyz;

 //   // step-7 �ˉe��Ԃ̐[�x�l���烏�[���h���W�𕜌�����
 //   float z = depthTexture.Sample(sam, input.uv);

 //   // �ˉe��Ԃ̐[�x�l���烏�[���h���W�𕜌�����
 //   float3 worldPos = CalcWorldPosFromUVZ(input.uv, z, mViewProjInv);

 //   float3 lig = 0.0f;

 //   // ���_�Ɍ������ĐL�т�x�N�g�����v�Z
 //   float3 toEye = normalize(eyePos - worldPos);

 //   // �f�B���N�V�������C�g���v�Z
 //   //for (int ligNo = 0; ligNo < NUM_DIRECTION_LIGHT; ligNo++)
 //   //{
 //   //    // �g�U���ˌ����v�Z
 //   //    lig += CalcLambertReflection(
 //   //        directionLights[ligNo].direction,
 //   //        directionLights[ligNo].color,
 //   //        normal);

 //   //    // �X�y�L�������˂��v�Z
 //   //    lig += CalcSpecularReflection(
 //   //        directionLights[ligNo].direction,
 //   //        directionLights[ligNo].color,
 //   //        normal,
 //   //        toEye);
 //   //}

 //   // step-8 �|�C���g���C�g���v�Z����
 //   for (int ligNo = 0; ligNo < POINT_LIGHT_NUM; ligNo++)
 //   {
 //       // �g�U���˂��v�Z
 //       // 1. ��������T�[�t�@�C�X�ɓ��˂���x�N�g�����v�Z
 //       float3 ligDir = normalize(worldPos - pointLights[ligNo].position);
 //       // 2. ��������T�[�t�F�C�X�܂ł̋������v�Z
 //       float distance = length(worldPos - pointLights[ligNo].position);
 //       // 3. �e�������v�Z����e������0.0�`1.0�͈̔͂ŁA
 //       //    �w�肵�������ipointsLights[i].range�j�𒴂�����A�e������0.0�ɂȂ�
 //       float affect = 1.0f - min(1.0f, distance / pointLights[ligNo].range);
 //       // 4. �g�U���ˌ������Z
 //       lig += CalcLambertDiffuse(
 //           ligDir,
 //           pointLights[ligNo].color,
 //           normal) * affect;

 //       // �X�y�L�������˂����Z
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
/// Lambert�g�U���ˌ����v�Z����
/// </summary>
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
    // �s�N�Z���̖@���ƃ��C�g�̕����̓��ς��v�Z����
    float t = dot(normal, lightDirection) * -1.0f;

    // ���ς̒l��0�ȏ�̒l�ɂ���
    t = max(0.0f, t);

    // �g�U���ˌ����v�Z����
    return lightColor * t;
}

/// <summary>
/// Phong���ʔ��ˌ����v�Z����
/// </summary>
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal)
{
    // ���˃x�N�g�������߂�
    float3 refVec = reflect(lightDirection, normal);

    // �������������T�[�t�F�C�X���王�_�ɐL�т�x�N�g�������߂�
    float3 toEye = eyePos - worldPos;
    toEye = normalize(toEye);

    // ���ʔ��˂̋��������߂�
    float t = dot(refVec, toEye);

    // ���ʔ��˂̋�����0�ȏ�̐��l�ɂ���
    t = max(0.0f, t);

    // ���ʔ��˂̋������i��
    t = pow(t, 5.0f);

    // ���ʔ��ˌ������߂�
    return lightColor * t;
}