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
static const int SPOT_LIGHT_NUM = 2; // �X�|�b�g���C�g�̐�

struct SpotLight
{
    float3 position; //���W
    float3 color; //�F
    float range; //�͈�
    float3 Direction;
    float angle;
};
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