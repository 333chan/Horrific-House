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
    float3 color; // ���C�g�̃J���[
    float3 direction; // ���C�g�̕���
};

struct PointLight
{
    float3 position; //���W
    float3 color; //�F
    float range; //�͈�
};

struct SpotLight
{
    float3 position; //���W
    float3 color; //�F
    float range; //�͈�
    float3 Direction;
    float angle;
};

float3 CalcLambertDiffuse(float3 ligDir, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 ligDir, float3 lightColor, float3 worldPos, float3 normal);
float3 CalcPointLight();

cbuffer BaseCBuffer : register(b1)
{
    matrix AntiViewportM; //4x4�r���[�|�[�g�t�s��
    matrix ProjectionM; //4x4�v���W�F�N�V�����s��
    float4x3 ViewM; //4x3(�r���[�s��)
    float4x3 LocalM; //4x3(��]�g�k���s�ړ�)
    float4 ToonOutLineSize; // �g�D�[���̗֊s���̑傫��
    float DiffuseSource; // �f�B�t���[�Y�J���[( 0.0f:�}�e���A��  1.0f:���_ )
    float SpecularSource; // �X�y�L�����J���[(   0.0f:�}�e���A��  1.0f:���_ )
    float MulSpecularColor; // �X�y�L�����J���[�l�ɏ�Z����l( �X�y�L�������������Ŏg�p )
    float Padding; //�l�ߕ�(����)
}

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
        // �g�U���˂��v�Z
        // 1. ��������T�[�t�@�C�X�ɓ��˂���x�N�g�����v�Z
        float3 ligDir = normalize(worldPos - pointLights[ligNo].position);
        // 2. ��������T�[�t�F�C�X�܂ł̋������v�Z
        float distance = length(worldPos - pointLights[ligNo].position);
        // 3. �e�������v�Z����e������0.0�`1.0�͈̔͂ŁA
        //    �w�肵�������ipointsLights[i].range�j�𒴂�����A�e������0.0�ɂȂ�
        float affect = 1.0f - min(1.0f, distance / pointLights[ligNo].range);
        // 4. �g�U���ˌ������Z
        pointLig += CalcLambertDiffuse(
            ligDir,
            pointLights[ligNo].color,
            normal) * affect;

        // �X�y�L�������˂����Z
        pointLig += CalcPhongSpecular(
            ligDir,
            pointLights[ligNo].color,
            normal,
            toEye) * affect;
    }

    float3 ligDir = input.pos - spotLights.position;
    ligDir = normalize(ligDir);

    // �����Ȃ���Lambert�g�U���ˌ����v�Z����
    float3 diffSpotLight = CalcLambertDiffuse(
        ligDir, // ���C�g�̕���
        spotLights.color, // ���C�g�̃J���[
        input.norm // �T�[�t�F�C�X�̖@��
    );

    // �����Ȃ���Phong���ʔ��ˌ����v�Z����
    float3 specSpotLight = CalcPhongSpecular(
        ligDir, // ���C�g�̕���
        spotLights.color, // ���C�g�̃J���[
        input.pos, // �T�[�t�F�C�X�̃��[���h���W
        input.norm // �T�[�t�F�C�X�̖@��
    );

    // �����ɂ��e�������v�Z����
    // �X�|�b�g���C�g�Ƃ̋������v�Z����
    float3 distance = length(input.pos - spotLights.position);

    // �e�����͋����ɔ�Ⴕ�ď������Ȃ��Ă���
    float affect = 1.0f - 1.0f / spotLights.range * distance;

    // �e���͂��}�C�i�X�ɂȂ�Ȃ��悤�ɕ␳��������
    if (affect < 0.0f)
    {
        affect = 0.0f;
    }

    // �e���̎d�����w���֐��I�ɂ���B����̃T���v���ł�3�悵�Ă���
    affect = pow(affect, 3.0f);

    // �e��������Z���ĉe������߂�
    diffSpotLight *= affect;
    specSpotLight *= affect;

    // ���ˌ��Ǝˏo�����̊p�x�����߂�
    // dot()�𗘗p���ē��ς����߂�
    float angle = dot(ligDir, spotLights.Direction);

    // dot()�ŋ��߂��l��acos()�ɓn���Ċp�x�����߂�
    angle = abs(acos(angle));

    // �p�x�ɂ��e���������߂�
    // �p�x�ɔ�Ⴕ�ď������Ȃ��Ă����e�������v�Z����
    affect = 1.0f - 1.0f / spotLights.angle * angle;

    // �e�������}�C�i�X�ɂȂ�Ȃ��悤�ɕ␳��������
    if (affect < 0.0f)
    {
        affect = 0.0f;
    }

    // �e���̎d�����w���֐��I�ɂ���B����̃T���v���ł�0.5�悵�Ă���
    affect = pow(affect, 0.5f);

    // �p�x�ɂ��e�����𔽎ˌ��ɏ�Z���āA�e������߂�
    diffSpotLight *= affect;
    specSpotLight *= affect;

    // �X�|�b�g���C�g�̔��ˌ����ŏI�I�Ȕ��ˌ��ɑ����Z����
    float3 finalLig = pointLig + ambientLight;
    finalLig += diffSpotLight + specSpotLight;
    float4 finalColor = tex.Sample(sam, input.uv);

    // ���������Z
    finalColor.xyz *= finalLig;
    finalColor.rgb = saturate(finalColor.rgb - float3(0.5f, 0.5f, 0.5f));

    output.color = finalColor;

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
