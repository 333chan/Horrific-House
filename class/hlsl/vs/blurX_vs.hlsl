struct VS_INPUT
{
	float3 pos		 : POSITION0;	//���W
	float4 spos		 : POSITION1;	// �\�����W
	float3 norm		 : NORMAL0;		//�@��
	float3 tangent	 : TANGENT0;	//�ڃx�N�g��
	float3 binorm	 : BINORMAL0;	//�]�@���x�N�g��
	float4 diffuse	 : COLOR0;		//�f�B�t���[�Y
	float4 specular  : COLOR1;		//�X�y�L����
	float2 uv0		 : TEXCOORD0;	//�e�N�X�`�����W
	float2 uv1		 : TEXCOORD1;	//�T�u�e�N�X�`�����W
};

cbuffer BaseCBuffer  : register(b1) {
	float4x4	AntiViewportM;				//4x4�r���[�|�[�g�t�s��
	float4x4	ProjectionM;				//4x4�v���W�F�N�V�����s��
	float4x3	ViewM;						//4x3(�r���[�s��)
	float4x3	LocalM;						//4x3(��]�g�k���s�ړ�)
	float4		ToonOutLineSize;			// �g�D�[���̗֊s���̑傫��
	float		DiffuseSource;				// �f�B�t���[�Y�J���[( 0.0f:�}�e���A��  1.0f:���_ )
	float		SpecularSource;				// �X�y�L�����J���[(   0.0f:�}�e���A��  1.0f:���_ )
	float		MulSpecularColor;			// �X�y�L�����J���[�l�ɏ�Z����l( �X�y�L�������������Ŏg�p )
	float		Padding;					//�l�ߕ�(����)
}

//�o�[�W�����ɑΉ����Ă��Ȃ��̂�ps��input���W�܂ł����g���Ȃ�
//uv���W������炵�Ă���
struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 uv0 : TEXCOORD0;
	float4 uv1 : TEXCOORD1;
	float4 uv2 : TEXCOORD2;
	float4 uv3 : TEXCOORD3;
	float4 uv4 : TEXCOORD4;
	float4 uv5 : TEXCOORD5;
	float4 uv6 : TEXCOORD6;
	float4 uv7 : TEXCOORD7;
};

Texture2D<float4> tex : register(t0);

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;

	float4 pos = float4(input.pos, 1);

	pos.xyz = mul(pos, LocalM);		//���[���h
	pos.xyz = mul(pos, ViewM);		//�r���[
	pos = mul(pos, ProjectionM);	//�v���W�F�N�V����

	float2 textureSize = float2(960.0f, 1080.0f);
	float level;

	//���UV���W
	float2 textureUV = input.uv0;

	//�@1�e�N�Z�����炷
	output.uv0.xy = float2(1.0f / textureSize.x, 0.0f);
	//�@3�e�N�Z�����炷
	output.uv1.xy = float2(3.0f / textureSize.x, 0.0f);
	//�@5�e�N�Z�����炷
	output.uv2.xy = float2(5.0f / textureSize.x, 0.0f);
	//�@7�e�N�Z�����炷
	output.uv3.xy = float2(7.0f / textureSize.x, 0.0f);
	//�@9�e�N�Z�����炷
	output.uv4.xy = float2(9.0f / textureSize.x, 0.0f);
	//�@11�e�N�Z�����炷
	output.uv5.xy = float2(11.0f / textureSize.x, 0.0f);
	//�@13�e�N�Z�����炷
	output.uv6.xy = float2(13.0f / textureSize.x, 0.0f);
	//	15�e�N�Z�����炷
	output.uv7.xy = float2(15.0f / textureSize.x, 0.0f);

	//�I�t�Z�b�g��-1�������ċt�������v�Z
	output.uv0.zw = output.uv0.xy * -1.0f;
	output.uv1.zw = output.uv1.xy * -1.0f;
	output.uv2.zw = output.uv2.xy * -1.0f;
	output.uv3.zw = output.uv3.xy * -1.0f;
	output.uv4.zw = output.uv4.xy * -1.0f;
	output.uv5.zw = output.uv5.xy * -1.0f;
	output.uv6.zw = output.uv6.xy * -1.0f;
	output.uv7.zw = output.uv7.xy * -1.0f;

	//��̈ʒu��������
	output.uv0 += float4(textureUV, textureUV);
	output.uv1 += float4(textureUV, textureUV);
	output.uv2 += float4(textureUV, textureUV);
	output.uv3 += float4(textureUV, textureUV);
	output.uv4 += float4(textureUV, textureUV);
	output.uv5 += float4(textureUV, textureUV);
	output.uv6 += float4(textureUV, textureUV);
	output.uv7 += float4(textureUV, textureUV);

	output.pos = pos;

	return output;
}