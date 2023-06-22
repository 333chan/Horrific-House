#pragma once
#include <string>
#include <map>
#include <vector>
#include "../common/Geometry.h"
//�Đ����@
enum class PLAY_TYPE
{
	NORMAL,		//�ʏ�
	REVERSE		//�t�Đ�
};

//�A�j���[�V�����̍Đ����
enum class ANIM_STATE {
	PLAYING,		//�Đ���
	STOPPING		//��~��
};

class AnimationController
{
public:
	//�A�j���[�V�������
	struct Animation
	{
		int model = 0;					//���f���n���h��
		int attachNo = 0;				//�A�j���[�V�����ԍ�
		float speed = 0.0f;				//�Đ����x
		float totalTime = 0.0f;			//���Đ�����
		float frame = 0.0f;				//�t���[���J�E���^�[
	};

	//�R���X�g���N�^�i���f���n���h���j
	AnimationController(int modelId);
	~AnimationController();

	//�A�j���[�V�����ǉ�
	//����:���,�t�@�C���p�X,�Đ����x
	void Add(int type, std::string path, float speed);

	//����:���,���f���n���h��,�Đ����x
	void Add(int type, int handle, float speed);
	
	//�A�j���[�V�����Đ�
	//�A�j���[�V������ށA���[�v�Đ��̗L��
	void Play(int type, bool isLoop);

	//�A�j���[�V�����̎�ށA�A�j���[�V�����ԍ��A���[�v�Đ��̗L��
	void Play(int type, int num, bool isLoop);

	//�t�Đ�
	//�A�j���[�V������ށA���[�v�Đ��̗L��
	void PlayBack(int type, bool isLoop);

	//�X�V����
	void Update(double delta);

	//�J��
	void Release(void);

	//�A�j���[�V������Ԃ�Ԃ�
	ANIM_STATE GetPlayState(void);

private:
	//���f��ID
	int model_;
	//�A�j���[�V�����R���e�i
	std::map<int, Animation> animations_;
	//�A�j���[�V�����̎��
	int animType_;
	//�A�j���[�V�������
	Animation playAnim_;

	//�Đ����@
	PLAY_TYPE playType;
	// �A�j���[�V���������[�v���邩���Ȃ���
	bool mIsLoop;

	// �A�j���[�V�������~�߂��܂܂ɂ���
	bool mIsStop;

	//�Đ����
	ANIM_STATE animState_;
};

