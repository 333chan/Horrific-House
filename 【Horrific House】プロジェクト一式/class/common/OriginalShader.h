#pragma once
#include <array>
#include"Geometry.h"

class OriginalShader
{
public:

	/// <summary>
	/// �C���X�^���X2D�p
	/// </summary>
	/// <param name="pso">�s�N�Z���V�F�[�_�[</param>
	OriginalShader(const int pso);
	/// <summary>
	/// �C���X�^���X3D�p
	/// </summary>
	/// <param name="vso">���_�V�F�[�_�[</param>
	/// <param name="pso">�s�N�Z���V�F�[�_�[</param>
	OriginalShader(const int vso, const int pso);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~OriginalShader();



	/// <summary>
	/// �V�F�[�_�[���g�����`�� (���f���P��)
	/// </summary>
	/// <param name="model">���f��</param>
	void Draw(int model)const;

	/// <summary>
	/// �V�F�[�_�[���g�����`��
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="tex">�e�N�X�`���}�b�v</param>
	/// /// <param name="tex">�e�N�X�`���}�b�v</param>
	void Draw(int model, int tex)const;

	/// <summary>
	/// �V�F�[�_�[���g�����`��
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="tex">�e�N�X�`���}�b�v</param>
	/// <param name="norm">�@���}�b�v</param>
	void Draw(int model, int tex, int norm)const;

	/// <summary>
	/// �V�F�[�_�[���g�����`��
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="tex">�e�N�X�`���}�b�v</param>
	/// <param name="norm">�@���}�b�v</param>
	void Draw(int model, int tex, int norm,int noise)const;

	/// <summary>
	/// �V�F�[�_�[���g�����`��
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="tex">�e�N�X�`���}�b�v</param>
	/// <param name="norm">�@���}�b�v</param>
	/// <param name="rough"></param>
	/// <param name="metallic">���^���b�N</param>
	/// <param name="sphmap">�X�t�B�A�}�b�v</param>
	void Draw(int model, int tex, int norm, int rough, int metallic, int sphmap)const;

	//�S��.�O������G(�J�n)---------------------------------------------------------------------------
	/// <summary>
	/// �V�F�[�_�[�A�E�g���C��
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="outline"></param>
	/// <param name="ps">�s�N�Z���V�F�[�_�[</param>
	/// <param name="vs">���_�V�F�[�_�[</param>
	void Draw(int model, bool outline, int ps, int vs)const;
	//�S��.�O������G(�I��)---------------------------------------------------------------------------

	/// <summary>
	/// �v���~�e�B�u�ŕ`��
	/// </summary>
	/// <param name="pos">�ʒu</param>
	/// <param name="vertsSize">�摜�̑傫��</param>
	/// <param name="img">�摜</param>
	void DrawPrimitive3D(Vector2 pos,Vector2 vertsSize, int img);

	/// <summary>
	/// �|�X�g�G�t�F�N�g�`��p
	/// </summary>
	/// <param name="x">�摜�̍����X</param>
	/// <param name="y">�摜�̍����Y</param>
	/// <param name="img">�摜�n���h��</param>
	/// <param name="tex1">�V�F�[�_�Ŏg���e�N�X�`���P</param>
	/// <param name="tex2">�V�F�[�_�Ŏg���e�N�X�`���Q</param>
	/// <param name="tex3">�V�F�[�_�Ŏg���e�N�X�`���R</param>
	void DrawOnlyPixel(float x, float y, int img, int tex1, int tex2, int tex3);

	//�����p�ʐ^�֐�
	void DrawOnlyPixel(const std::array<Vector3, 4>& pos, int img);
	/// <summary>
	/// �e�N�X�`�����ʂɃZ�b�g����i�����[�X�͂��Ȃ��j
	/// </summary>
	/// <param name="num">�Z�b�g����Ԓn</param>
	/// <param name="tex">�Z�b�g����e�N�X�`��</param>
	void SetUseTexture(int num ,int tex);
private:

	//�ϐ�
	//�g�����_�V�F�[�_�[�̊i�[�ꏊ
	int vertexShader_;
	//�g���s�N�Z���V�F�[�_�[�̊i�[�ꏊ
	int pixelShader_;
};