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

cbuffer CbufDOF : register(b13)
{
    float3 Focus;
    float3 Area;
}

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    
	// ��ʊE�[�x�͈͓̔��� 0.0f �` 1.0f �ɕϊ�
    if (input.pos.z < Area.x)
    {
        output.color.r = 0.0f;
    }
    else if (input.pos.z > Area.y)
    {
        output.color.r = 1.0f;
    }
    else
    {
        output.color.r = (input.pos.z - Area.x) * Area.z;
    }
    //Output.color.r = 1.0f;
    output.color.g = output.color.r;
    output.color.b = output.color.r;
    output.color.a = 1.0f;
    
    //Output.color.r = PSInput.pos.z;
    //Output.color.g = PSInput.pos.z;
    //Output.color.b = PSInput.pos.z;
    //Output.color.a = 1.0f;
    return output;
}