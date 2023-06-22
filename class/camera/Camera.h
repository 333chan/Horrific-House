#pragma once
#include<DxLib.h>
#include<memory>
#include"../Common/Geometry.h"
#include"../common/Quaternion.h"

//�J�����̍���(��)
constexpr float CAMERA_HEIGHT = 1250.0f;

enum class LiftType
{
	UP,
	DOWN,
	MAX,
};

class Camera
{
public:
	Camera();
	Camera(Vector3 cameraAngle);
	~Camera();

	//������
	void Init(void);

	//�X�V
	void Update(bool dashFlag);
	void Update(Vector3 targetPos);//�Q�[���I�[�o�[���̉��o�p
	//�Ǐ]�J����(�����������̂ŃI�u�W�F�N�g�`��O�ɂ���K�v������)
	void SetBeforeDraw(Vector3 playerpos);
	void SetBeforeDraw_dead(Vector3 playerpos,Vector3 targetpos);//targetpos�����Â���

	//�`��
	void Draw(void);
	void SetSensitivity(float sens);
	//�J�������W��Ԃ�
	Vector3 GetPos();

	//�����_��Ԃ�
	Vector3 GetTargetPos();

	//�p�x��Ԃ�
	Vector3 GetAngles();

	//��ʗh��
	float SwingWidth(void);

private:
	void FallHead(void);//����ꂽ�񂪗�����\��
	bool BreakHead(void);//�������\��
	bool WaitTime(double time);//�w�肳�ꂽ���ԑ҂�
	//�J�����̈ʒu
	Vector3 pos_;
	Vector3 offset_;//�J�����̃I�t�Z�b�g�i�Q�[���I�[�o�[�ŃJ������ɂ����Ă�j

	//�J�����p�x(rad)
	Vector3 angle_;
	float defaultangle_y;
	float roll_;
	float sensitivity_;
	//�p�x(�N�H�[�^�j�I��)
	Quaternion quaRot_;

	//�����_
	Vector3 targetPos_;
	Vector3 t_offset_;//�����_��G�̊炪�����邭�炢�ɒ�������悤�i�Q�[���I�[�o�[�j

	Vector2Int mousePos_;
	Vector3 padPos_;

	int sinCnt_;
	double fallCnt_;//�����̌o�ߎ���
	bool liftFlg_;//�㉺�ړ��t���O
	double waitTime_;//���o���̑҂�����
	double fallAccel_ = 0.0;

	float sinCount_;
};
