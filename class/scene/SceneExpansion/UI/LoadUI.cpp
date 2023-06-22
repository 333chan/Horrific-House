#include "LoadUI.h"
#include "../../SceneManager.h"
LoadUI::LoadUI()
{
	Init();
}

LoadUI::~LoadUI()
{
	// �}�X�N�f�[�^���폜���܂�
	DeleteMask(maskHandle_);
	DeleteMaskScreen();
}

void LoadUI::Init()
{
	LoadVol = 0;
	IsLoadingEnd = false;
	CreateMaskScreen();
	//�}�X�N�摜�̓ǂݍ���
	maskHandle_ = LoadMask("resource/image/Load/gageImage3.png");
}

void LoadUI::Update()
{
	//���Ŏ��Ԍo�߂ŏ���ɏオ���Ă���
	//TRACE("�񓯊��ǂݍ��݂̐�:%d\n", GetASyncLoadNum());
	if (GetASyncLoadNum() == 0)//�񓯊��ǂݍ��݂̎c�������������ׂ�
	{
		//�O�Ȃ�
		//TRACE("�񓯊��ǂݍ��ݏI��\n");
		if (LoadVol >= 1000)
		{
			IsLoadingEnd = true;//���[�h�I��
		}
		else//�o�[���}�b�N�X�ɂ���
		{
			LoadVol += 30;
			if (LoadVol > 1000)
			{
				LoadVol = 1000;
			}
		}
	}
	else
	{
		IsLoadingEnd = false;
		//���[�h�̐i�����\�����Ă�
		if (LoadVol < 950)
		{
			LoadVol += 0.4f;

		}
	}
}

void LoadUI::Draw()
{
	//�}�X�N�`��	
	DrawMask(0, 0, maskHandle_, DX_MASKTRANS_NONE);
	//�w�i��
	DrawBox(0, 0, 1920, 1080,
		0x000000,
		true);
	//�Q�[�W�̗�
	DrawGraph(0, 0, lpImageMng.GetID("fram")[0], true);
	//�Ԃ̃Q�[�W
	DrawBox(0,
		0,
		lpSceneMng.GetScreenSize().x - 1200 + LoadVol,
		lpSceneMng.GetScreenSize().y,
		0x881100,
		true);
	
	
}

bool LoadUI::GetIsLoadingEnd()
{
	return IsLoadingEnd;
}
