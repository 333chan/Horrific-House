#pragma once
#include<type_traits>

//�L�[�̎��
enum class KeyID
{
	Up,			//���Ɉړ�
	Down,		//��O�Ɉړ�
	Left,		//���Ɉړ�
	Right,		//�E�Ɉړ�
	Decide,		//����
	Cancel,		//�L�����Z��
	Dash,		//�_�b�V��
	Inventory,	//�C���x���g��
	Pause,		//�|�[�Y�{�^��
	Interact,	//�C���^���N�g(�A�C�e���𒲂ׂ���E�����肷��)
	Shoot,		//�B�e
	Attack,		//�X�}�z�t���b�V��
	ShootForAndroid,		//�X�}�z����
	DashForAndroid,		//�X�}�z�_�b�V��
	Max
};

static KeyID begin(KeyID) { return KeyID::Up; };
static KeyID end(KeyID) { return KeyID::Max; };
static KeyID operator++(KeyID& state) { return state = static_cast<KeyID>(std::underlying_type<KeyID>::type(state) + 1); };
static KeyID operator*(const KeyID& state) { return state; };

