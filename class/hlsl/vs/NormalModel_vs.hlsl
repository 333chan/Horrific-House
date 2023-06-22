struct VS_INPUT
{
    float3 pos      : POSITION;
    float3 norm     : NORMAL;
    float4 diffuse  : COLOR0;
    float4 specular : COLOR1;
    float4 uv0      : TEXCOORD0;
    float4 uv1      : TEXCOORD1;
    float3 tangent  : TANGENT;
    float3 binormal : BINORMAL;
};

struct VS_OUTPUT
{
    float4 svpos : SV_POSITION;
    float3 pos   : POSITION0;
    float3 norm  : NORMAL;
    float3 tan   : TANGENT;
    float3 bin   : BINORMAL;
    float2 uv    : TEXCOORD0;
    float4 posInLVP : TEXCOORD1;
    float3 col   : COLOR;
};

cbuffer BaseCBuffer : register(b1)
{
    matrix   AntiViewportM; //4x4�r���[�|�[�g�t�s��
    matrix   ProjectionM; //4x4�v���W�F�N�V�����s��
    float4x3 ViewM; //4x3(�r���[�s��)
    float4x3 LocalM; //4x3(��]�g�k���s�ړ�)
    float4   ToonOutLineSize; // �g�D�[���̗֊s���̑傫��
    float    DiffuseSource; // �f�B�t���[�Y�J���[( 0.0f:�}�e���A��  1.0f:���_ )
    float    SpecularSource; // �X�y�L�����J���[(   0.0f:�}�e���A��  1.0f:���_ )
    float    MulSpecularColor; // �X�y�L�����J���[�l�ɏ�Z����l( �X�y�L�������������Ŏg�p )
    float    Padding; //�l�ߕ�(����)
}

cbuffer CameraBuffer : register(b6)
{
    float4x4 vmat;
    float4x4 Provmat;
}
VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    float4 pos = float4(input.pos, 1);
    pos.xyz    = mul(pos, LocalM);
    output.pos = pos;
    pos.xyz    = mul(pos, ViewM);
    pos        = mul(pos, ProjectionM);
	
    //�@���ɉ�]��������Z
    float3 norm = mul(input.norm, LocalM);
    float3 tan  = mul(input.tangent, LocalM);
    float3 bin  = mul(input.binormal, LocalM);
    
    //�o�͂Ƀf�[�^���i�[
    output.svpos = pos;
    
    output.uv    = input.uv0.xy;
    output.norm  = normalize(norm);
    output.tan   = normalize(tan);
    output.bin   = normalize(bin);
    output.col   = input.diffuse.rgb;

    return output;
}