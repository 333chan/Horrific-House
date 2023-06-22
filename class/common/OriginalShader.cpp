#include <DxLib.h>
#include "OriginalShader.h"


OriginalShader::OriginalShader(const int pso)
{
	vertexShader_ = -1;
	pixelShader_ = pso;
}

OriginalShader::OriginalShader(const int vso, const int pso)
{
	//�V�F�[�_�[��ݒ�
	vertexShader_ = vso;
	pixelShader_  = pso;
}

OriginalShader::~OriginalShader()
{
	//shader�̍폜
	DeleteShader(vertexShader_);
	DeleteShader(pixelShader_ );
}

void OriginalShader::Draw(int model)const
{
	//�I���W�i���V�F�[�_�[���g�p
	MV1SetUseOrigShader(true);

	//�g���V�F�[�_�[�̌���
	SetUseVertexShader(vertexShader_);
	SetUsePixelShader(pixelShader_);

	//���f����draw
	MV1DrawModel(model);

	//���ʂ̃V�F�[�_�[�ɖ߂�
	MV1SetUseOrigShader(false);
}

void OriginalShader::Draw(int model,int tex)const
{
	//�I���W�i���V�F�[�_�[���g�p
	MV1SetUseOrigShader(true);

	//�g���V�F�[�_�[�̌���
	SetUseVertexShader(vertexShader_);
	SetUsePixelShader (pixelShader_	);

	//�e�N�X�`�����̐ݒ�
	SetUseTextureToShader(10,tex);

	//���f����draw
	MV1DrawModel(model);

	//�e�N�X�`�����̃��Z�b�g
	SetUseTextureToShader(10, -1);

	//���ʂ̃V�F�[�_�[�ɖ߂�
	MV1SetUseOrigShader(false);
}

void OriginalShader::Draw(int model, int tex,int norm)const
{
	//�I���W�i���V�F�[�_�[���g�p
	MV1SetUseOrigShader(true);

	//�g���V�F�[�_�[�̌���
	SetUseVertexShader(vertexShader_);
	SetUsePixelShader(pixelShader_);

	//�e�N�X�`�����̐ݒ�
	SetUseTextureToShader(0, tex);
	SetUseTextureToShader(1, norm);
	//���f����draw
	MV1DrawModel(model);

	//�e�N�X�`�����̃��Z�b�g
	SetUseTextureToShader(0, -1);
	SetUseTextureToShader(1, -1);

	//���ʂ̃V�F�[�_�[�ɖ߂�
	MV1SetUseOrigShader(false);
}

void OriginalShader::Draw(int model, int tex, int norm, int noise) const
{
	//�I���W�i���V�F�[�_�[���g�p
	MV1SetUseOrigShader(true);

	//�g���V�F�[�_�[�̌���
	SetUseVertexShader(vertexShader_);
	SetUsePixelShader(pixelShader_);

	//�e�N�X�`�����̐ݒ�
	SetUseTextureToShader(0, tex);
	SetUseTextureToShader(1, norm);
	SetUseTextureToShader(2, noise);
	//���f����draw
	MV1DrawModel(model);

	//�e�N�X�`�����̃��Z�b�g
	SetUseTextureToShader(0, -1);
	SetUseTextureToShader(1, -1);
	SetUseTextureToShader(2, -1);

	//���ʂ̃V�F�[�_�[�ɖ߂�
	MV1SetUseOrigShader(false);
}

void OriginalShader::Draw(int model, int tex, int norm, int rough, int metallic, int sphmap)const
{
	//�I���W�i���V�F�[�_�[���g�p
	MV1SetUseOrigShader(true);

	//�g���V�F�[�_�[�̌���
	SetUseVertexShader(vertexShader_);
	SetUsePixelShader(pixelShader_);

	//�e�N�X�`�����̐ݒ�
	SetUseTextureToShader(0, tex);
	SetUseTextureToShader(1, norm);
	SetUseTextureToShader(2, rough);
	SetUseTextureToShader(3, metallic);
	SetUseTextureToShader(4, sphmap);

	//���f����draw
	MV1DrawModel(model);

	//�e�N�X�`�����̐ݒ�
	SetUseTextureToShader(0, -1);
	SetUseTextureToShader(1, -1);
	SetUseTextureToShader(2, -1);
	SetUseTextureToShader(3, -1);
	SetUseTextureToShader(4, -1);

	//���ʂ̃V�F�[�_�[�ɖ߂�
	MV1SetUseOrigShader(false);

}

//�S��.�O������G(�J�n)---------------------------------------------------------------------------
void OriginalShader::Draw(int model, bool outline,int ps,int vs) const
{
	//�I���W�i���V�F�[�_�[���g�p
	MV1SetUseOrigShader(true);

	//�g���V�F�[�_�[�̌���(���C�e�B���O)
	SetUseVertexShader(vs);
	SetUsePixelShader(ps);
	//���b�V�������擾
	auto num = MV1GetMeshNum(model);
	for (int i = 0; i < num; ++i) {
		//�o�b�N�J�����O�ݒ�
		MV1SetMeshBackCulling(model, i, DX_CULLING_LEFT);
	}
	//���f���`��
	MV1DrawModel(model);

	if (outline) {
		//�g���V�F�[�_�[�̌���(�A�E�g���C��)
		SetUseVertexShader(vertexShader_);
		SetUsePixelShader(pixelShader_);
		for (int i = 0; i < num; ++i) {
			//�o�b�N�J�����O�ݒ�
			MV1SetMeshBackCulling(model, i, DX_CULLING_RIGHT);
		}
		//�A�E�g���C���`��
		MV1DrawModel(model);
	}

	//���ʂ̃V�F�[�_�[�ɖ߂�
	MV1SetUseOrigShader(false);
}
//�S��.�O������G(�I��)---------------------------------------------------------------------------

void OriginalShader::DrawPrimitive3D(Vector2 pos,Vector2 vertsSize, int img)
{
	std::array<VERTEX3DSHADER, 4> verts{};

	for (auto& v : verts)
	{
		v.dif = GetColorU8(255, 255, 255, 255);
	}
	//����
	verts[0].pos.x = pos.x;
	verts[0].pos.y = pos.y + vertsSize.y;
	//�E��
	verts[1].pos.x = pos.x + vertsSize.x;
	verts[1].pos.y = pos.y + vertsSize.y;
	//����
	verts[2].pos.x = pos.x;
	verts[2].pos.y = pos.y;
	//�E��
	verts[3].pos.x = pos.x + vertsSize.x;
	verts[3].pos.y = pos.y;

	//UV���W
	verts[0].u = 0.0f;
	verts[0].v = 0.0f;

	verts[1].u = 1.0f;
	verts[1].v = 0.0f;

	verts[2].u = 0.0f;
	verts[2].v = 1.0f;

	verts[3].u = 1.0f;
	verts[3].v = 1.0f;

	int x, y;
	GetGraphSize(img, &x, &y);
	SetUseVertexShader(vertexShader_);
	SetUsePixelShader(pixelShader_);

	SetUseTextureToShader(0, img);

	DrawPrimitive3DToShader(verts.data(), static_cast<int>(verts.size()), DX_PRIMTYPE_TRIANGLESTRIP);

	SetUseTextureToShader(0, -1);

}

void OriginalShader::DrawOnlyPixel(float x, float y,int img, int tex1, int tex2, int tex3)
{
	std::array<VERTEX2DSHADER, 4> verts{};

	float width, height;

	GetGraphSizeF(img, &width, &height);

	for (auto& v : verts)
	{
		v.dif = GetColorU8(255, 255, 255, 255);
		v.spc = GetColorU8(255, 255, 255, 255);
		v.pos.z = 0.0f;
		v.rhw = 1.0f;
		v.su = 0.0f;
		v.sv = 0.0f;
	}

	verts[0].pos.x = x;
	verts[0].pos.y = y;

	verts[1].pos.x = x + width;
	verts[1].pos.y = y;

	verts[2].pos.x = x;
	verts[2].pos.y = y + height;

	verts[3].pos.x = x + width;
	verts[3].pos.y = y + height;

	//UV���W
	verts[0].u = 0.0f;
	verts[0].v = 0.0f;

	verts[1].u = 1.0f;
	verts[1].v = 0.0f;

	verts[2].u = 0.0f;
	verts[2].v = 1.0f;

	verts[3].u = 1.0f;
	verts[3].v = 1.0f;

	SetUseTextureToShader(0, img);
	SetUseTextureToShader(1, tex1);
	SetUseTextureToShader(2, tex2);
	SetUseTextureToShader(3, tex3);

	SetUsePixelShader(pixelShader_);

	DrawPrimitive2DToShader(verts.data(), static_cast<int>(verts.size()), DX_PRIMTYPE_TRIANGLESTRIP);

	SetUseTextureToShader(0, -1);
	SetUseTextureToShader(1, -1);
	SetUseTextureToShader(2, -1);
	SetUseTextureToShader(3, -1);
}

void OriginalShader::DrawOnlyPixel(const std::array<Vector3, 4>& pos,int img)
{
	std::array<VERTEX2DSHADER, 4> verts{};

	for (auto& v : verts)
	{
		v.dif = GetColorU8(255, 255, 255, 255);
		v.spc = GetColorU8(255, 255, 255, 255);
		v.pos.z = 0.0f;
		v.rhw = 1.0f;
		v.su = 0.0f;
		v.sv = 0.0f;
	}

	for (int i = 0; i != pos.size(); i++)
	{
		verts[i].pos.x = pos[i].x;
		verts[i].pos.y = pos[i].y;
	}

	//UV���W
	verts[0].u = 0.0f;
	verts[0].v = 0.0f;

	verts[1].u = 1.0f;
	verts[1].v = 0.0f;

	verts[2].u = 0.0f;
	verts[2].v = 1.0f;

	verts[3].u = 1.0f;
	verts[3].v = 1.0f;

	SetUseTextureToShader(0, img);

	SetUsePixelShader(pixelShader_);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	DrawPrimitive2DToShader(verts.data(), static_cast<int>(verts.size()), DX_PRIMTYPE_TRIANGLESTRIP);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	SetUseTextureToShader(0, -1);
}

void OriginalShader::SetUseTexture(int num, int tex)
{
	SetUseTextureToShader(num, tex);
}
