#include "LockDoor.h"
#include "../../common/AnimationController.h"
#include "../../scene/SceneManager.h"
#include "../Item/Item.h"
#include "../../common/OriginalShader.h"
#include "../../common/manager/SoundMng.h"
constexpr double UI_DRAW_TIME = 1.3;		//UI�\������
constexpr int OFFSET_Y = 370;		//UI�\������
constexpr float OPEN_SPEED = 18.0f;
constexpr float CLOSE_SPEED = 17.0f;

LockDoor::LockDoor(std::string name, Vector3 pos, Vector3 angle, Vector3 scale, int num, ItemID itemID)
{
	//�I�u�W�F�N�g��
	name_ = name;
	//���W
	pos_ = pos;
	//�p�x
	angle_ = angle;
	//�g��l
	scale_ = scale;
	//���f��������
	modelNum_ = num;
	//�J���邽�߂ɕK�v�ȃA�C�e��
	itemID_ = itemID;
	//������
	Init();
}

LockDoor::~LockDoor()
{
	Release();
}

void LockDoor::Init(void)
{
	//�ϐ�������
	VariableInit();

	//���W��ݒ�
	MV1SetPosition(lpModelMng.GetID(modelStr_)[modelNum_], pos_.toVECTOR());

	//�g��l��ݒ�
	MV1SetScale(lpModelMng.GetID(modelStr_)[modelNum_], scale_.toVECTOR());

	//�x�����烉�W�A���ɕϊ�
	angle_ = { Deg2RadF(angle_.x),Deg2RadF(angle_.y),Deg2RadF(angle_.z) };

	//��]
	MV1SetRotationXYZ(lpModelMng.GetID(modelStr_)[modelNum_], angle_.toVECTOR());

	//���C�e�B���O
	vsLight_ = LoadVertexShader("./resource/shader/vs/NormalModel_vs.vso");
	psLight_ = LoadPixelShader("./resource/shader/ps/GameModel_ps.pso");
	//�A�E�g���C��
	int outL_vs = LoadVertexShader("./resource/shader/vs/OutLine_vs.vso");
	int outL_ps = LoadPixelShader("./resource/shader/ps/OutLine_ps.pso");
	origShader_ = std::make_unique<OriginalShader>(outL_vs, outL_ps);
}

void LockDoor::Update(void)
{
	//�C���^���N�g���ꂽ��
	if (interactFlg_)
	{
		//���b�N��
		if (isLock_)
		{
			//�K�`���K�`�������Đ�
			lpSoundMng.PlayingSound("resource/sound/LockDoorSE.mp3");
		}
		else	//���b�N��������Ă�����
		{
			//�A�j���[�V�����Đ�
			animFlg_ = true;
		}
		//UI�\��
		uiFlg_ = true;
	}
	//�A�j���[�V��������
	Animation();
	//�`�掞�Ԍv�Z
	DrawTimeCount();
	interactFlg_ = false;
}

void LockDoor::Draw(void)
{
	//�`��
	origShader_->Draw(lpModelMng.GetID(modelStr_)[modelNum_], hitFlg_,psLight_,vsLight_);
	//UI�`��
	DrawUI();
}

void LockDoor::Release(void)
{
	//�V�F�[�_�[�폜
	DeleteShader(psLight_);
	DeleteShader(vsLight_);
}

int LockDoor::GetModelNum(void)
{
	return 	modelNum_;
}

std::string LockDoor::GetCollModelStr(void)
{
	return modelStr_;
}


void LockDoor::Animation(void)
{
	//���b�N������
	if (!isLock_)
	{
		//�Đ���
		if (animFlg_)
		{
			//���Ă���Ƃ�
			if (animState_ == GIMMICK_ANIM::CLOSE)
			{
				lpSoundMng.PlayingSound("resource/sound/DoorSE.mp3");
				//�J����
				animController_->Play((int)GIMMICK_ANIM::OPEN, false);
				//�A�j���[�V��������~������
				if (animController_->GetPlayState() == ANIM_STATE::STOPPING)
				{
					//�J���Ă����Ԃɐ؂�ւ���
					animState_ = GIMMICK_ANIM::OPEN;
					//�Đ��t���O�����~�߂�
					animFlg_ = false;
				}
			}
			else if (animState_ == GIMMICK_ANIM::OPEN)	//�J���Ă���Ƃ�
			{
				//
				lpSoundMng.PlayingSound("resource/sound/DoorSE.mp3");
				//����
				animController_->Play((int)GIMMICK_ANIM::CLOSE, false);
				//�A�j���[�V��������~������
				if (animController_->GetPlayState() == ANIM_STATE::STOPPING)
				{
					//���Ă����Ԃɐ؂�ւ���
					animState_ = GIMMICK_ANIM::CLOSE;
					//�Đ��t���O�����~�߂�
					animFlg_ = false;
					lpSoundMng.PlayingSound("resource/sound/DoorCloseSE.mp3");
				}
			}
		}
	}
	//�����蔻����X�V
	MV1RefreshCollInfo(lpModelMng.GetID(modelStr_)[modelNum_]);
	//�A�j���[�V�����X�V����
	animController_->Update(lpSceneMng.GetDeltaTime());
}

void LockDoor::DrawTimeCount(void)
{
	//�\����
	if (uiFlg_)
	{
		delta_ += lpSceneMng.GetDeltaTime();
	}
	else	//��\����
	{
		//���Ԃ�������
		delta_ = 0.0;
	}

	//�o�ߎ��Ԃ��w�肵�����Ԃ𒴂�����
	if (delta_ >= UI_DRAW_TIME)
	{
		//UI������
		uiFlg_ = false;
		//���b�N����������Ă�����
		if (!isLock_)
		{
			//����UI��1�񂾂��\�����邽�߃t���O��false��
			firstTimeFlg_ = false;
		}
	}
}

void LockDoor::DrawUI(void)
{	
	if (uiFlg_ && firstTimeFlg_)
	{
		//�摜�̃T�C�Y��ݒ�
		SetGraphSize();
		DrawGraph(drawPos.x - graphSize_.x, drawPos.y, lpImageMng.GetID(uiKey_)[0], true);
	}
}

void LockDoor::SetGraphSize(void)
{
	if (!isLock_ && firstTimeFlg_)
	{
		if (name_ == "White_A_Double")
		{
			uiKey_ = "UseKitchenUI";
		}
		else if (name_ == "Dark_B" || name_ == "Dark_B_Double")
		{
			uiKey_ = "UseMusicRoomUI";
		}
		else if (name_ == "Red_A")
		{
			uiKey_ = "UseFoodBankUI";
		}
		else if (name_ == "Dark_C")
		{
			uiKey_ = "UseChildRoomUI";
		}
		else if (name_ == "Red_C")
		{
			uiKey_ = "UseCellerUI";
		}
	}
	if (isLock_)
	{
		if (name_ == "White_A_Double")
		{
			uiKey_ = "KitchenUI";
		}
		else if (name_ == "Dark_B" || name_ == "Dark_B_Double")
		{
			uiKey_ = "MusicRoomUI";
		}
		else if (name_ == "Red_A")
		{
			uiKey_ = "FoodBankUI";
		}
		else if (name_ == "Dark_C")
		{
			uiKey_ = "ChildRoomUI";
		}
		else if (name_ == "Red_C")
		{
			uiKey_ = "CellerUI";
		}
		else if (name_ == "White_C_Double")
		{
			uiKey_ = "EntranceUI";
		}
	}
	GetGraphSize(lpImageMng.GetID(uiKey_)[0], &graphSize_.x, &graphSize_.y);
	graphSize_.x /= 2;
	graphSize_.y /= 2;
}

void LockDoor::VariableInit(void)
{
	//�C���^���N�g�t���O
	interactFlg_ = false;
	//�_�C�A���i���o�[
	dialNumber_ = 0;
	//���b�N���
	isLock_ = true;
	//�����蔻��t���O
	hitFlg_ = false;
	//�A�����b�N�i���o�[
	unlockNum_ = "0000";
	//�����蔻��p�@��
	hitNormal_ = { 0.0f,0.0f,0.0f };
	//UI�摜���ʗp
	uiKey_ = "";
	//�o�ߎ���
	delta_ = 0.0;
	//UI�\������p�t���O
	uiFlg_ = false;
	//�A�j���[�V�������
	animState_ = GIMMICK_ANIM::CLOSE;
	//�M�~�b�N�^�C�v
	type_ = GIMMICK_TYPE::DOOR;
	//����m�F�p�t���O
	firstTimeFlg_ = true;
	//�摜�T�C�Y�̔����̒l
	graphSize_ = { 0,0 };
	//�`��ʒu
	drawPos = { lpSceneMng.GetScreenSize().x / 2 ,(lpSceneMng.GetScreenSize().y / 2) + OFFSET_Y };
	std::string directory = "resource/model/gimmick/";
	//�g�p���郂�f����ݒ�
	if (name_ == "White_A_Double")
	{
		modelStr_ = directory + "Door_White1_Double.mv1";
		//�A�j���[�V�����ǉ�
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(modelStr_)[modelNum_]);
		animController_->Add((int)GIMMICK_ANIM::OPEN, directory +"anim/DoorDoubleOpen.mv1", OPEN_SPEED);
		animController_->Add((int)GIMMICK_ANIM::CLOSE, directory + "anim/DoorDoubleClose.mv1", CLOSE_SPEED);
	}
	else if (name_ == "Red_A")
	{
		modelStr_ = directory + "Door_Red1.mv1";
		//�A�j���[�V�����ǉ�
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(modelStr_)[modelNum_]);
		animController_->Add((int)GIMMICK_ANIM::OPEN, directory + "anim/DoorOpen.mv1", OPEN_SPEED);
		animController_->Add((int)GIMMICK_ANIM::CLOSE, directory + "anim/DoorClose.mv1", CLOSE_SPEED);
	}
	else if (name_ == "Dark_B")
	{
		modelStr_ = directory + "Door_Dark2.mv1";
		//�A�j���[�V�����ǉ�
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(modelStr_)[modelNum_]);
		animController_->Add((int)GIMMICK_ANIM::OPEN, directory + "anim/DoorOpen.mv1", OPEN_SPEED);
		animController_->Add((int)GIMMICK_ANIM::CLOSE, directory + "anim/DoorClose.mv1", CLOSE_SPEED);
	}
	else if (name_ == "Dark_B_Double")
	{
		modelStr_ = directory + "Door_Dark2_Double.mv1";
		//�A�j���[�V�����ǉ�
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(modelStr_)[modelNum_]);
		animController_->Add((int)GIMMICK_ANIM::OPEN, directory + "anim/DoorDoubleOpen.mv1", OPEN_SPEED);
		animController_->Add((int)GIMMICK_ANIM::CLOSE, directory + "anim/DoorDoubleClose.mv1", CLOSE_SPEED);
	}
	else if (name_ == "Dark_C")
	{
		modelStr_ = directory + "Door_Dark3.mv1";
		//�A�j���[�V�����ǉ�
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(modelStr_)[modelNum_]);
		animController_->Add((int)GIMMICK_ANIM::OPEN, directory + "anim/DoorOpen.mv1", OPEN_SPEED);
		animController_->Add((int)GIMMICK_ANIM::CLOSE, directory + "anim/DoorClose.mv1", CLOSE_SPEED);
	}
	else if (name_ == "Red_C")
	{
		modelStr_ = directory + "Door_Red3.mv1";
		//�A�j���[�V�����ǉ�
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(modelStr_)[modelNum_]);
		animController_->Add((int)GIMMICK_ANIM::OPEN, directory + "anim/DoorOpen.mv1", OPEN_SPEED);
		animController_->Add((int)GIMMICK_ANIM::CLOSE, directory + "anim/DoorClose.mv1", CLOSE_SPEED);
	}
	else if(name_ == "White_C_Double")
	{
		modelStr_ = directory + "Door_White3_Double.mv1";
		//�A�j���[�V�����ǉ�
		animController_ = std::make_unique<AnimationController>(lpModelMng.GetID(modelStr_)[modelNum_]);
		animController_->Add((int)GIMMICK_ANIM::OPEN, directory + "anim/DoorDoubleOpen.mv1", OPEN_SPEED);
		animController_->Add((int)GIMMICK_ANIM::CLOSE, directory + "anim/DoorDoubleClose.mv1", CLOSE_SPEED);

	}

}
