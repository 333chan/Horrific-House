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
static const int SPOT_LIGHT_NUM = 2; // �X�|�b�g���C�g�̐�
static const int far = 10000;
static const int near = 3000;

cbuffer CameraBuffer : register(b8)
{
    matrix vmat;    //�J�����̃}�g���b�N�X
}
cbuffer DirectionLightBuffer : register(b9)
{
    DirectionLight directionLights[DIRECTION_LIGHT_NUM]; //�f�B���N�V�������C�g
}
cbuffer PointLightBuffer : register(b10)
{
    PointLight pointLights[POINT_LIGHT_NUM]; //�|�C���g���C�g
}
cbuffer SpotLightBuffer : register(b11)
{
    SpotLight spotLights[SPOT_LIGHT_NUM]; // �X�|�b�g���C�g
}
cbuffer AmbientLightBuffer : register(b12)
{
    float3 ambientLight; // �A���r�G���g���C�g
    float specPow;
}

SamplerState sam : register(s0);
Texture2D<float4> tex : register(t0);       //�e�N�X�`��
Texture2D<float4> norm : register(t1);      //�m�[�}���}�b�v
Texture2D<float4> depth : register(t10);    //�[�x�}�b�v

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    //�^���W�F���g�X�y�[�X�̖@���ŃT���v�����O����
    float3 texNormal;
    texNormal = norm.Sample(sam, input.uv).xyz;
    texNormal = (texNormal - 0.5f) * 2.0f;
    //�^���W�F���g�X�y�[�X�@�������[���h�X�y�[�X�ɕϊ�����
    float3 normal = input.tan * texNormal.x+ input.bin * texNormal.y + input.norm * texNormal.z;
    //�J�����̈ʒu
    float3 EyePos = spotLights[0].position;
    float3 pointLig = 0.0f;
    float3 worldPos = input.pos;
    //�J�����Ƃ̃x�N�g���쐬
    float3 toEye = EyePos - worldPos;
    //�������擾
    float linerPos = length(EyePos - worldPos);
    //�t�H�O��������͈͂��쐬
    float fogfactor = clamp((far - linerPos) / (far - near), 0.0, 1.0);
    
    toEye = normalize(toEye);
    
    //�|�C���g���C�g�̏���
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
            EyePos,
            normal) * affect;
    }
    
   //�X�|�b�g���C�g�̏���
    float3 ligDir[SPOT_LIGHT_NUM];
    float3 diffSpotLight[SPOT_LIGHT_NUM];
    float3 specSpotLight[SPOT_LIGHT_NUM];
    float3 finalLig = pointLig + ambientLight;
    for (int spLigNo = 0; spLigNo < SPOT_LIGHT_NUM; spLigNo++)
    {
        ligDir[spLigNo] = input.pos - spotLights[spLigNo].position;
        ligDir[spLigNo] = normalize(ligDir[spLigNo]);

    // �����Ȃ���Lambert�g�U���ˌ����v�Z����
        
        diffSpotLight[spLigNo] = CalcLambertDiffuse(
        ligDir[spLigNo], // ���C�g�̕���
        spotLights[spLigNo].color, // ���C�g�̃J���[
        normal // �T�[�t�F�C�X�̖@��
    );

    // �����Ȃ���Phong���ʔ��ˌ����v�Z����
        specSpotLight[spLigNo] = CalcPhongSpecular(
        ligDir[spLigNo], // ���C�g�̕���
        spotLights[spLigNo].color, // ���C�g�̃J���[
        worldPos, // �T�[�t�F�C�X�̃��[���h���W
        normal // �T�[�t�F�C�X�̖@��
    );
    
    // �����ɂ��e�������v�Z����
    // �X�|�b�g���C�g�Ƃ̋������v�Z����
        float3 distance = length(input.pos - spotLights[spLigNo].position);

    // �e�����͋����ɔ�Ⴕ�ď������Ȃ��Ă���
        float affect = 1.0f - 1.0f / spotLights[spLigNo].range * distance;

    // �e���͂��}�C�i�X�ɂȂ�Ȃ��悤�ɕ␳��������
        if (affect < 0.0f)
        {
            affect = 0.0f;
        }

    // �e���̎d�����w���֐��I�ɂ���B����̃T���v���ł�3�悵�Ă���
        affect = pow(affect, 3.0f);

    // �e��������Z���ĉe������߂�
        diffSpotLight[spLigNo] *= affect;
        specSpotLight[spLigNo] *= affect;

    // ���ˌ��Ǝˏo�����̊p�x�����߂�
    // dot()�𗘗p���ē��ς����߂�
        float angle = dot(ligDir[spLigNo], spotLights[spLigNo].Direction);

    // dot()�ŋ��߂��l��acos()�ɓn���Ċp�x�����߂�
        angle = abs(acos(angle));

    // �p�x�ɂ��e���������߂�
    // �p�x�ɔ�Ⴕ�ď������Ȃ��Ă����e�������v�Z����
        affect = 1.0f - 1.0f / spotLights[spLigNo].angle * angle;
    
    // �e�������}�C�i�X�ɂȂ�Ȃ��悤�ɕ␳��������
        if (affect < 0.0f)
        {
            affect = 0.0f;
        }

    // �e���̎d�����w���֐��I�ɂ���B����̃T���v���ł�0.5�悵�Ă���
        affect = pow(affect, 0.5f);

    // �p�x�ɂ��e�����𔽎ˌ��ɏ�Z���āA�e������߂�
        diffSpotLight[spLigNo] *= affect;
        specSpotLight[spLigNo] *= affect;
    
    // �X�|�b�g���C�g�̔��ˌ����ŏI�I�Ȕ��ˌ��ɑ����Z����
        finalLig += diffSpotLight[spLigNo] + specSpotLight[spLigNo];
        
    }
    //�e�N�X�`�����T���v�����O����
    float4 finalColor = tex.Sample(sam, input.uv);
    //�F�Ɛ^�����̐��`��Ԃ��s���t�H�O���쐬���Ă���
    finalColor = lerp(float4(0, 0, 0, 1), finalColor, fogfactor);
     // ���������Z
    finalColor.xyz *= finalLig;
    //���������F����
    output.color = finalColor ;
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
    float3 EyePos = spotLights[0].position;
    float3 toEye = EyePos - worldPos;
    toEye = normalize(toEye);

    // ���ʔ��˂̋��������߂�
    float t = dot(refVec, toEye);

    // ���ʔ��˂̋�����0�ȏ�̐��l�ɂ���
    t = max(0.0f, t);

    // ���ʔ��˂̋������i��
    t = pow(t, 1.0f);

    // ���ʔ��ˌ������߂�
    return lightColor * t;
}