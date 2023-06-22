//�o�[�W�����ɑΉ����Ă��Ȃ��̂�ps��input���W�܂ł����g���Ȃ�
//uv���W������炵�Ă���
struct PS_INPUT
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

struct PS_OUTPUT
{
	float4 color : SV_TARGET0;
};

//�萔
cbuffer cbBlur : register(b9)
{
	float weight1x;
	float weight1y;
	float weight1z;
	float weight1w;

	float weight2x;
	float weight2y;
	float weight2z;
	float weight2w;
}

SamplerState sam;	//�T���v��
Texture2D<float4> tex : register(t0);

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;

	//���σu���[(�Y��ɂڂ������߂ɃK�E�V�A���u���[�̗p)-------------------------------------------------------------
	//float4 color = tex.Sample(sam, input.uv);
	//float offsetU = 3.5f / 1280.0f;
	//float offsetV = 3.5f / 720.0f;

	////�^�񒆂𒆐S�ɕ��ς��Ƃ�
	////��
	//color += tex.Sample(sam, input.uv + float2(0.0f, -offsetV));
	////��
	//color += tex.Sample(sam, input.uv + float2(0.0f, offsetV));
	////��
	//color += tex.Sample(sam, input.uv + float2(-offsetU, 0.0f));
	////�E
	//color += tex.Sample(sam, input.uv + float2(offsetU, 0.0f));
	////�E��
	//color += tex.Sample(sam, input.uv + float2(offsetU, offsetV));
	////�E��
	//color += tex.Sample(sam, input.uv + float2(offsetU, -offsetV));
	////����
	//color += tex.Sample(sam, input.uv + float2(-offsetU, offsetV));
	////����
	//color += tex.Sample(sam, input.uv + float2(-offsetU, -offsetV));

	////���ςȂ̂�9�Ŋ���
	//color /= 9.0f;

	//output.color = color;
	//-----------------------------------------------------------------------


	float4 color;

	color  = weight1x * tex.Sample(sam, input.uv0.xy);
	color += weight1y * tex.Sample(sam, input.uv1.xy);
	color += weight1z * tex.Sample(sam, input.uv2.xy);
	color += weight1w * tex.Sample(sam, input.uv3.xy);
	color += weight2x * tex.Sample(sam, input.uv4.xy);
	color += weight2y * tex.Sample(sam, input.uv5.xy);
	color += weight2z * tex.Sample(sam, input.uv6.xy);
	color += weight2w * tex.Sample(sam, input.uv7.xy);

	color += weight1x * tex.Sample(sam, input.uv0.zw);
	color += weight1y * tex.Sample(sam, input.uv1.zw);
	color += weight1z * tex.Sample(sam, input.uv2.zw);
	color += weight1w * tex.Sample(sam, input.uv3.zw);
	color += weight2x * tex.Sample(sam, input.uv4.zw);
	color += weight2y * tex.Sample(sam, input.uv5.zw);
	color += weight2z * tex.Sample(sam, input.uv6.zw);
	color += weight2w * tex.Sample(sam, input.uv7.zw);

	color.a = 1.0f;

	//�኱�Â�����
	color.rgb -= 0.05f;

	output.color = color;

	return output;
}
