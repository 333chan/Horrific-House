#pragma once
#include<DxLib.h>
#include <vector>
#include <string>
#include "../Geometry.h"

#define lpCBufferMng CBufferMng::GetInstance()

class CBufferMng
{
public:

	static CBufferMng& GetInstance()
	{
		static CBufferMng s_Instance;
		return s_Instance;
	}
	
	// Cbuffer�̃A�b�v�f�[�g
	void Update();

	//�`��f�o�b�O
	void Draw()const;
	/// <summary>
	/// �s�N�Z���o�b�t�@��buffernum�̔Ԓn�̒��g�̕\��
	/// </summary>
	/// <param name="str">�f�o�b�O�\���̎��킩��₷�����邽�߂ɐ擪�ɕ\������������</param>
	/// <param name="buffernum">�o�b�t�@�̔Ԓn</param>
	/// <param name="pixelbuffersize">num�̔Ԓn�̑傫��(���̃o�b�t�@����������̑傫��)</param>
	void DebugBufferDraw(std::string str, int x,int buffernum , int pixelbuffersize) const;

	void Create();

	void Remove();

	/// <summary>
	/// ���_�o�b�t�@�̍쐬
	/// </summary>
	/// <param name="vertexbuffersize">���_�o�b�t�@�̃T�C�Y</param>
	void CreateVertexCbuffer(const int vertexbuffersize);

	/// <summary>
	/// �s�N�Z���o�b�t�@�̍쐬
	/// </summary>
	/// <param name="pixelbuffersize">�s�N�Z���o�b�t�@�̃T�C�Y</param>
	void CreatePixelCbuffer( const int pixelbuffersize);

	/// <summary>
	/// ���_�o�b�t�@��num�̔Ԓn�ɍ쐬
	/// </summary>
	/// <param name="num"></param>
	/// <param name="vertexbuffersize"></param>
	void CreateVertexCbuffer(int num, const int vertexbuffersize);

	/// <summary>
	/// �s�N�Z���o�b�t�@��num�̔Ԓn�ɍ쐬
	/// </summary>
	/// <param name="num"></param>
	/// <param name="pixelbuffersize"></param>
	void CreatePixelCbuffer(int num, const int pixelbuffersize);

	//�o�b�t�@�̒l�Z�b�g
	/// <summary>
	/// ���_�V�F�[�_�[��cbuffer�l�Z�b�g
	/// </summary>
	/// <param name="buffernum">�o�b�t�@�[�̔Ԓn</param>
	/// <param name="num">cbuffer�̒��̒l�������Ԓn</param>
	/// <param name="value">�����l</param>
	void SetVertexCbuffer(const int buffernum, const int num, const float value);

	/// <summary>
	/// ���_�V�F�[�_�[��cbuffer�̒l��float4�ŃZ�b�g
	/// </summary>
	/// <param name="buffernum">�o�b�t�@�[�̔Ԓn</param>
	/// <param name="num">cbuffer�̒��̒l�������Ԓn</param>
	/// <param name="value4">�����l(float4)</param>
	void SetVertexCbuffer(const int buffernum, const int num, const FLOAT4 value4);

	void SetVertexCbuffer(int buffernum, int cbufferFloatNum, MATRIX value);



	/// <summary>
	/// �s�N�Z���V�F�[�_�[��cbuffer�̒l�Z�b�g
	/// </summary>
	/// <param name="buffernum">�o�b�t�@�[�̔Ԓn</param>
	/// <param name="num">cbuffer�̒��̒l�������Ԓn</param>
	/// <param name="value">�����l</param>
	void SetPixelCbuffer(const int buffernum, const int num, const float value);

	/// <summary>
	/// �s�N�Z���V�F�[�_�[��cbuffer�̒l��float4�ŃZ�b�g
	/// </summary>
	/// <param name="buffernum">�o�b�t�@�[�̔Ԓn</param>
	/// <param name="num">cbuffer�̒��̒l�������Ԓn</param>
	/// <param name="value4">�����l(float4)</param>
	void SetPixelCbuffer(const int buffernum, const int num, const FLOAT4 value4);

	//void SetPixelCbuffer(const int buffernum, const int num, const Matrix value4x4);

	Vector3 GetALLambientLight();
	void SetALLambientLight(Vector3 value);
private:
	// �R���X�g���N�^
	CBufferMng();
	//�f�X�g���N�^
	~CBufferMng();

	//���_�o�b�t�@�̔z��
	std::vector<int> vertexCbuffer_;
	//�s�N�Z���o�b�t�@�̔z��
	std::vector<int> pixelCbuffer_;

	int VertexCbufferNum_;
	int PixelCbufferNum_;
	//�A���r�G���g���C�g����
	Vector3 ALLambientLight_;
};

