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
    float3 position;    //���W
    float3 color;       //�F
    float range;        //�͈�
};

struct SpotLight
{
    float3 position; //���W
    float3 color; //�F
    float range; //�͈�
    float3 Direction;
    float angle;
};

//float3 CalcLigFromPointLight(PS_Input psIn);
//float3 CalcLigFromDirectionLight(PS_Input psIn);

float3 CalcLambertDiffuse(float3 ligDir, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 ligDir, float3 lightColor, float3 worldPos, float3 normal);


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

cbuffer CameraBuffer : register(b8)
{
    float3 eyePos;
    matrix vmat;
}

cbuffer LightBuffer : register(b9)
{
    SpotLight spotLights; //�X�|�b�g���C�g�z��
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
    float3 finalLig = float3(0.1f, 0.1f,0.1f);
    finalLig += diffSpotLight + specSpotLight;
    float4 finalColor = tex.Sample(sam, input.uv);
    
     // ���������Z
    finalColor.xyz *= finalLig;
    output.color = finalColor;
    //normalize(ligDir);
    //output.color = float4(spotLights.position, 1);
    //output.color = float4(ligDir, 1);

    //output.color = float4(spotLights.position, 1);

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

/// <summary>
/// �|�C���g���C�g�ɂ�锽�ˌ����v�Z
/// </summary>
/// <param name="psIn">�s�N�Z���V�F�[�_�[�ɓn����Ă������</param>
//float3 CalcLigFromPointLight(PS_Input psIn)
//{
//    // ���̃T�[�t�F�C�X�ɓ��˂��Ă���|�C���g���C�g�̌��̌������v�Z����
//    float3 ligDir = psIn.worldPos - ptPosition;

//    // ���K�����đ傫��1�̃x�N�g���ɂ���
//    ligDir = normalize(ligDir);

//    // �����Ȃ���Lambert�g�U���ˌ����v�Z����
//    float3 diffPoint = CalcLambertDiffuse(
//        ligDir, // ���C�g�̕���
//        ptColor, // ���C�g�̃J���[
//        psIn.normal // �T�[�t�F�C�X�̖@��
//    );

//    // �����Ȃ���Phong���ʔ��ˌ����v�Z����
//    float3 specPoint = CalcPhongSpecular(
//        ligDir, // ���C�g�̕���
//        ptColor, // ���C�g�̃J���[
//        psIn.worldPos, // �T�[�t�F�C�Y�̃��[���h���W
//        psIn.normal     // �T�[�t�F�C�Y�̖@��
//    );

//    // �����ɂ��e�������v�Z����
//    // �|�C���g���C�g�Ƃ̋������v�Z����
//    float3 distance = length(psIn.worldPos - ptPosition);

//    // �e�����͋����ɔ�Ⴕ�ď������Ȃ��Ă���
//    float affect = 1.0f - 1.0f / ptRange * distance;

//    // �e���͂��}�C�i�X�ɂȂ�Ȃ��悤�ɕ␳��������
//    if (affect < 0.0f)
//    {
//        affect = 0.0f;
//    }

//    // �e���̎d�����w���֐��I�ɂ���B����̃T���v���ł�3�悵�Ă���
//    affect = pow(affect, 3.0f);

//    // �g�U���ˌ��Ƌ��ʔ��ˌ��Ɍ���������Z���ĉe������߂�
//    diffPoint *= affect;
//    specPoint *= affect;

//    return diffPoint + specPoint;
//}

/// <summary>
/// �f�B���N�V�������C�g�ɂ�锽�ˌ����v�Z
/// </summary
/// <param name="psIn">�s�N�Z���V�F�[�_�[����̓��́B</param>
//float3 CalcLigFromDirectionLight(PS_Input psIn)
//{
//    // �f�B���N�V�������C�g�ɂ��Lambert�g�U���ˌ����v�Z����
//    float3 diffDirection = CalcLambertDiffuse(dirDirection, dirColor, psIn.normal);

//    // �f�B���N�V�������C�g�ɂ��Phong���ʔ��ˌ����v�Z����
//    float3 specDirection = CalcPhongSpecular(
//            dirDirection, dirColor, psIn.worldPos, psIn.normal);
//    return diffDirection + specDirection;
//}

