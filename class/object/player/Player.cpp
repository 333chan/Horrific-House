#include <iostream>
#include <algorithm>

#include "../../common/manager/ModelMng.h"
#include"../../common/manager/TutorialMng.h"
#include "../../common/MouseIcon.h"
#include "../../common/SocketServer.h"
#include "../../common/manager/ImageMng.h"
#include "../../common/manager/ModelMng.h"
#include "../../common/manager/SoundMng.h"
#include"../../common/OriginalShader.h"
#include "../../scene/SceneManager.h"

#include "../../input/KeyInput.h"
#include "../../input/PadInput.h"

#include "../../common/Capsule.h"
#include "Player.h"
#include "Stamina.h"
#include "MinCamera.h"

#include "../gimmick/Gimmick.h"
#include "../Item/Item.h"
#include"../../camera/Camera.h"

#include"../../../_debug/_DebugDispOut.h"
#include"../../../_debug/_DebugConOut.h"

//�����ʒu
constexpr VECTOR INIT_POS = { 500.0f,0.0f,500.0f };

//�ړ��֌W
constexpr float MOVE_SPEED = 30.0f;				//�ړ��X�s�[�h
constexpr float FALL_SPEED = 30.0f;				//�����X�s�[�h
constexpr float FALL_ACCEL = 3.0f;			//�d��
constexpr float MOVE_COEFFICIENT = 1.0f;		//�ړ��W��
constexpr float HALF_ROOT_TWO = 0.70710678118f;	//��Q���̂P

//�J��������I�u�W�F�N�g�̓����蔻��̒���(��)
constexpr float INTERACT_LINE_LENGTH = 1500.0f;
constexpr float WALL_INTERACT_LINE_LENGTH = 5000.0f;

//�t���b�V���֌W
constexpr float FLASH_LINE_LENGTH = 7000.0f;				//�t���b�V���̋���
constexpr float FLASH_ANGLE = 20.0f * (DX_PI_F / 180.0f);	//�t���b�V���̊p�x

//�J�v�Z���֌W
constexpr float CAPSULE_RELATIVE_TOP_Y  = CAMERA_HEIGHT;//��̋��̑��Έʒu
constexpr float CAPSULE_RELATIVE_DOWN_Y = 450.0f;		//���̋��̑��Έʒu
constexpr float CAPSULE_RADIUS		    = 400.0f;		//���a

//�R���W�����֌W
constexpr int WALL_EXTRUSION_NUM = 24;			//�ǂ̉����o����
constexpr float SLIDE_DISTANCE = 5.0f;			//�����o������
constexpr float COLLISION_LINE_LENGTH = 300.0f;	//�������C���̒���

//�z�u�~�X�œ����蔻��̃I�t�Z�b�g���K�v
constexpr float COLLSION_STAGE_OFFSET_X = 16990.02f;
constexpr float COLLSION_STAGE_OFFSET_Y = -1323.776f;
constexpr float COLLSION_STAGE_OFFSET_Z = -20603.69f;

constexpr int MAX_DIAL_NUM = 4;

Player::Player(ControllerType type, const std::vector<std::shared_ptr<Gimmick>>& gList ,const std::vector<std::shared_ptr<Item>>& iList)
{
	collisionHandle_ = lpModelMng.GetID("resource/model/stage/Coll4.mv1")[0];
	MV1SetPosition(collisionHandle_, VGet(COLLSION_STAGE_OFFSET_X, COLLSION_STAGE_OFFSET_Y, COLLSION_STAGE_OFFSET_Z));
	//�R���g���[������
	if (type == ControllerType::Pad)
	{
		controller_ = std::make_unique<PadInput>();
	}
	else
	{
		controller_ = std::make_unique<KeyInput>();
	}
	gimmickList_ = gList;
	itemList_    = iList;
	pickUpFlag_  = false;
	catchFlg_ = false;
	Init();
}

Player::~Player()
{
	Release();
}

void Player::Init(void)
{
	Vector3 vectorZero = {0.0f,0.0f,0.0f};
	capsule_ = Capsule(VGet(0.0f, CAPSULE_RELATIVE_TOP_Y, 0.0f), VGet(0.0f, CAPSULE_RELATIVE_DOWN_Y, 0.0f), CAPSULE_RADIUS);

	stamina_ = std::make_unique<Stamina>(controller_->GetControllerType());
	minCamera_ = std::make_unique<MinCamera>(controller_->GetControllerType());
	state_ = State::Walk;
	fallGravity_ = 0.0f;

	dashFlag_ = false;
	lowSensFlg_ = false;
	itemID_ = ItemID::Non;
	pos_ = { INIT_POS.x,INIT_POS.y,INIT_POS.z };
	lpSoundMng.PlayingSound("./resource/sound/playerWalkSE.mp3");
}

void Player::Release(void)
{
}

void Player::Update(void)
{
	controller_->Update();
}

void Player::Update(Vector3 cameraAngle,Vector3 targetPos)
{
	quaRot_ = Quaternion::Euler(cameraAngle.toVECTOR());
	itemID_ = ItemID::Non;
	//�e�A�b�v�f�[�g
	Update();

	//�ړ�����
	MovePosition(cameraAngle);
	Sound();
	minCamera_->Update(dashFlag_);

	if (minCamera_->GetIsOnCamera() && controller_->CheckLongInputKey(KeyID::Dash))
	{
		lowSensFlg_ = true;
	}
	else
	{
		lowSensFlg_ = false;
	}
	//��������
	SlideLinePosition();

	//�C���^���N�g����(���u)
	AddInventory(targetPos);

	//�C���^���N�g����(�M�~�b�N)
	InteractGimmick(targetPos);	

	//�M�~�b�N�̃C�x���g����
	GimmickEvent(targetPos);
	ConnectingDrawerToItem();
	//�S�[���Ƃ̔���
	HitCapsuleAndGoal();
	//�}�E�X�A�C�R���̕ω��t���O��ݒ�
	lpMouseIcon.SetFlg(catchFlg_);

}

void Player::Draw(void)
{
	//�J����
	minCamera_->Draw();
	
	//�X�^�~�i
	stamina_->Draw();
}

void Player::Draw(int screenID)
{
	Draw();
}

Vector3 Player::GetPosition(void)
{
	return pos_;
}

bool Player::GetminCameraFlag()
{
	return minCamera_->GetIsOnCamera();
}

bool Player::GetIsGameClear()
{
	return isGameClear_;
}

bool Player::GetDashFlag(void)
{
	return dashFlag_;
}

void Player::SetMinCameraRenderTarget(int screenID)
{
	minCamera_->SetRenderTarget(screenID);
}

bool Player::CheckHitFlash(Vector3 targetPos, Vector3 cameraAngle)
{
	//�t���b�V�������Ă��Ȃ��ꍇ��false��Ԃ�
	if (!minCamera_->GetFlashFlag())
	{
		return false;
	}

	bool hitFlag = false;

	//�n�_
	VECTOR pPos = Vector3(pos_.x, pos_.y + CAMERA_HEIGHT, pos_.z).toVECTOR();

	//�I�_
	VECTOR centerEndPos = Vector3(pos_.x, pos_.y + CAMERA_HEIGHT, pos_.z).toVECTOR();
	VECTOR leftEndPos = Vector3(pos_.x, pos_.y + CAMERA_HEIGHT, pos_.z).toVECTOR();
	VECTOR rightEndPos = Vector3(pos_.x, pos_.y + CAMERA_HEIGHT, pos_.z).toVECTOR();

	//���S�͈�
	centerEndPos.x += sinf(cameraAngle.y) * FLASH_LINE_LENGTH;
	centerEndPos.z += cosf(cameraAngle.y) * FLASH_LINE_LENGTH;
	//���͈�
	leftEndPos.x += sinf(cameraAngle.y - FLASH_ANGLE) * FLASH_LINE_LENGTH;
	leftEndPos.z += cosf(cameraAngle.y - FLASH_ANGLE) * FLASH_LINE_LENGTH;
	//�E�͈�
	rightEndPos.x += sinf(cameraAngle.y + FLASH_ANGLE) * FLASH_LINE_LENGTH;
	rightEndPos.z += cosf(cameraAngle.y + FLASH_ANGLE) * FLASH_LINE_LENGTH;

	MV1_COLL_RESULT_POLY flashHit{};

	//�X�e�[�W�̃R���W�����ɍ��킹�đ傫���ύX
	//����
	flashHit = MV1CollCheck_Line(lpModelMng.GetID("resource/model/stage/Coll4.mv1")[0], -1, pPos, centerEndPos);
	if (flashHit.HitFlag)
	{
		centerEndPos = flashHit.HitPosition;
	}
	//�E
	flashHit = MV1CollCheck_Line(lpModelMng.GetID("resource/model/stage/Coll4.mv1")[0], -1, pPos, leftEndPos);
	if (flashHit.HitFlag)
	{
		leftEndPos = flashHit.HitPosition;
	}
	//��
	flashHit = MV1CollCheck_Line(lpModelMng.GetID("resource/model/stage/Coll4.mv1")[0], -1, pPos, rightEndPos);
	if (flashHit.HitFlag)
	{
		rightEndPos = flashHit.HitPosition;
	}

	//���̎O�p�`����
	auto leftTrinagleHits = MV1CollCheck_Triangle(lpModelMng.GetID("./resource/model/enemy/ghost3.mv1")[0], -1, pPos, leftEndPos, centerEndPos);
	if (leftTrinagleHits.HitNum > 0)
	{
		hitFlag = true;
	}
	//��n��
	MV1CollResultPolyDimTerminate(leftTrinagleHits);

	//�E�̎O�p�`����
	auto rightTrinagleHits = MV1CollCheck_Triangle(lpModelMng.GetID("./resource/model/enemy/ghost3.mv1")[0], -1, pPos, rightEndPos, centerEndPos);
	if (leftTrinagleHits.HitNum > 0)
	{
		hitFlag = true;
	}
	//��n��
	MV1CollResultPolyDimTerminate(rightTrinagleHits);

	return hitFlag;
}

std::vector<int>& Player::GetMinCameraPhotoVector(void)
{
	return minCamera_->GetPhotoVector();
}

std::vector<Collider>& Player::GetColliders(void)
{
	return colliders_;
}

std::vector<InventoryObj>& Player::GetInventoryList(void)
{
	return inventoryList_;
}

Capsule Player::GetCapsule(void)
{
	return capsule_;
}

ItemID Player::GetItemID(void)
{
	return itemID_;
}

void Player::MovePosition(Vector3 cameraAngle)
{
	//�ړ��ʌW��
	float moveCoefficient = MOVE_COEFFICIENT;

	//�΂߈ړ����Ă��邩�ǂ����`�F�b�N
	bool diagonalFlag = CheckDiagonalMove();

	//X���ړ��p
	auto movePowerX = [](Vector3 cAngle, float rot, float moveCoefficient_,bool diagonalFlag) {
		float movePower = MOVE_SPEED * moveCoefficient_;	//�ړ���
		float halfRoot2 = diagonalFlag ? HALF_ROOT_TWO : 1.0f;
		return sinf(cAngle.y + rot) * movePower * halfRoot2;
	};

	//Z���ړ��p
	auto movePowerZ = [](Vector3 cAngle, float rot, float moveCoefficient_,bool diagonalFlag) {
		float movePower = MOVE_SPEED * moveCoefficient_;	//�ړ���
		float halfRoot2 = diagonalFlag ? HALF_ROOT_TWO : 1.0f;
		return cosf(cAngle.y + rot) * movePower * halfRoot2;
	};

	dashFlag_ = false;
	//�ړ�����
	//�_�b�V������(�X�^�~�i�Ǘ�)
	moveCoefficient *= stamina_->Update(minCamera_->GetIsOnCamera());

	if (moveCoefficient > 1.0f)
	{
		//�_�b�V�����Ă���ꍇ�̓t���O�𗧂Ă�
		dashFlag_ = true;
		//����_�b�V�����ATutorialflg��true
		lpTutorialMng.SetTutorialFlg(TutorialProgress::Dash,true);
	}
	oldpos_ = pos_;
	//�O�i
	if (controller_->CheckLongInputKey(KeyID::Up))
	{
		//����ړ����ATutorialflg��true
		lpTutorialMng.SetTutorialFlg(TutorialProgress::Move, true);
		float powerX = movePowerX(cameraAngle, Deg2RadF(0.0f), moveCoefficient, diagonalFlag);
		float powerZ = movePowerZ(cameraAngle, Deg2RadF(0.0f), moveCoefficient, diagonalFlag);

		
		if (!HitLineX(powerX, collisionHandle_) && !HitLineXGimmick(powerX))
		{
			pos_.x += powerX;

		}
		 
		if (!HitLineZ(powerZ, collisionHandle_) && !HitLineZGimmick(powerZ))
		{
			pos_.z += powerZ;
		}
	}
	//���
	if (controller_->CheckLongInputKey(KeyID::Down))
	{
		//����ړ����ATutorialflg��true
		lpTutorialMng.SetTutorialFlg(TutorialProgress::Move, true);
		//��ގ��͈ړ��ʂ�����������(�ړ���0.6�{)
		moveCoefficient *= 0.6f;

		float powerX = movePowerX(cameraAngle, Deg2RadF(180.0f), moveCoefficient, diagonalFlag);
		float powerZ = movePowerZ(cameraAngle, Deg2RadF(180.0f), moveCoefficient, diagonalFlag);

		if (!HitLineX(powerX, collisionHandle_) && !HitLineXGimmick(powerX))
		{
			pos_.x += powerX;

		}

		if (!HitLineZ(powerZ, collisionHandle_) && !HitLineZGimmick(powerZ))
		{
			pos_.z += powerZ;
		}
	}
	//�E��
	if (controller_->CheckLongInputKey(KeyID::Right))
	{
		//����ړ����ATutorialflg��true
		lpTutorialMng.SetTutorialFlg(TutorialProgress::Move, true);
		float powerX = movePowerX(cameraAngle, Deg2RadF(90.0f), moveCoefficient, diagonalFlag);
		float powerZ = movePowerZ(cameraAngle, Deg2RadF(90.0f), moveCoefficient, diagonalFlag);

		if (!HitLineX(powerX, collisionHandle_) && !HitLineXGimmick(powerX))
		{
			pos_.x += powerX;

		}

		if (!HitLineZ(powerZ, collisionHandle_) && !HitLineZGimmick(powerZ))
		{
			pos_.z += powerZ;
		}
	}
	//����
	if (controller_->CheckLongInputKey(KeyID::Left))
	{
		//����ړ����ATutorialflg��true
		lpTutorialMng.SetTutorialFlg(TutorialProgress::Move, true);
		float powerX = movePowerX(cameraAngle, Deg2RadF(-90.0f), moveCoefficient, diagonalFlag);
		float powerZ = movePowerZ(cameraAngle, Deg2RadF(-90.0f), moveCoefficient, diagonalFlag);

		if (!HitLineX(powerX, collisionHandle_) && !HitLineXGimmick(powerX))
		{
			pos_.x += powerX;

		}

		if (!HitLineZ(powerZ, collisionHandle_) && !HitLineZGimmick(powerZ))
		{
			pos_.z += powerZ;
		}
	}
	
	// �������v�Z
	
	PlayerRot_.x = sin(cameraAngle.y);
	PlayerRot_.y = 0.0f;
	PlayerRot_.z = cos(cameraAngle.y);

	// ���X�i�[�̈ʒu��������ݒ�
	Set3DSoundListenerPosAndFrontPos_UpVecY(pos_.toVECTOR(), VAdd(pos_.toVECTOR(), PlayerRot_));
}

bool Player::CheckDiagonalMove(void)
{
	//�΂߈ړ����Ă���ꍇ��true��Ԃ�
	if (controller_->CheckLongInputKey(KeyID::Up)||
		controller_->CheckLongInputKey(KeyID::Down))
	{
		if (controller_->CheckLongInputKey(KeyID::Right) ||
			controller_->CheckLongInputKey(KeyID::Left))
		{
			return true;
		}
	}
	return false;
}

bool Player::HitLineX(float movePower, int& collision)
{
	Vector3 from = Vector3(pos_.x, pos_.y + 500, pos_.z);
	Vector3 player2TargetPosVector = (Vector3(pos_.x + movePower
		, pos_.y + 500, pos_.z) - from).Normalized();
	Vector3 to = from + player2TargetPosVector * 250;
	auto stageHits = MV1CollCheck_Line(collision, -1, from.toVECTOR(), to.toVECTOR());
	return stageHits.HitFlag;
}

bool Player::HitLineZ(float movePower, int& collision)
{
	Vector3 from = Vector3(pos_.x, pos_.y + 500, pos_.z);
	Vector3 player2TargetPosVector = (Vector3(pos_.x 
		, pos_.y + 500, pos_.z + movePower) - from).Normalized();
	Vector3 to = from + player2TargetPosVector * 250;
	auto stageHits = MV1CollCheck_Line(collision, -1, from.toVECTOR(), to.toVECTOR());
	return stageHits.HitFlag;
}

bool Player::HitLineXGimmick(float movePower)
{
	for (auto& gimmick : gimmickList_)
	{
		auto gType = gimmick->GetType();
		switch (gType)
		{
			//�����蔻����s��
		case GIMMICK_TYPE::FRAME:
		case GIMMICK_TYPE::WALL:
		case GIMMICK_TYPE::DOOR:
		case GIMMICK_TYPE::DRAWER:
		case GIMMICK_TYPE::DIAL:
		case GIMMICK_TYPE::DL_DOOR:
			break;
		default:
			continue;
			break;
		}
		//�ǂ̎�
		if (gType == GIMMICK_TYPE::WALL)
		{
			//���b�N��Ԃ���������Ă�����
			bool flg = gimmick->GetIsLockFlg();
			if (!flg)
			{
				//�����蔻����s��Ȃ�
				continue;
			}
		}
		//���̎�
		if (gType == GIMMICK_TYPE::DOOR || 
			gType == GIMMICK_TYPE::DL_DOOR || 
			gType==GIMMICK_TYPE::DRAWER)
		{
			auto aState_ = gimmick->GetAnimState_();
			//�J���Ă����Ԃ�������
			if (aState_ == GIMMICK_ANIM::OPEN)
			{
				//�����蔻����s��Ȃ�
				continue;
			}
		}
		Vector3 from = Vector3(pos_.x, pos_.y + 500, pos_.z);
		Vector3 player2TargetPosVector = (Vector3(pos_.x + movePower
			, pos_.y + 500, pos_.z) - from).Normalized();
		Vector3 to = from + player2TargetPosVector * 250;
		auto stageHits = MV1CollCheck_Line(lpModelMng.GetID(gimmick->GetCollModelStr())[gimmick->GetModelNum()], -1, from.toVECTOR(), to.toVECTOR());
		if (stageHits.HitFlag)
		{
			return stageHits.HitFlag;
		}
	}
	return false;
}

bool Player::HitLineZGimmick(float movePower)
{
	for (auto& gimmick : gimmickList_)
	{
		auto gType = gimmick->GetType();
		switch (gType)
		{
			//�����蔻����s��
		case GIMMICK_TYPE::FRAME:
		case GIMMICK_TYPE::WALL:
		case GIMMICK_TYPE::DOOR:
		case GIMMICK_TYPE::DRAWER:
		case GIMMICK_TYPE::DIAL:
		case GIMMICK_TYPE::DL_DOOR:
			break;
		default:
			continue;
			break;
		}
		//�ǂ̎�
		if (gType == GIMMICK_TYPE::WALL)
		{
			//���b�N��Ԃ���������Ă�����
			bool flg = gimmick->GetIsLockFlg();
			if (!flg)
			{
				//�����蔻����s��Ȃ�
				continue;
			}
		}
		//���̎�
		if (gType == GIMMICK_TYPE::DOOR ||
			gType == GIMMICK_TYPE::DL_DOOR ||
			gType == GIMMICK_TYPE::DRAWER)
		{
			auto aState_ = gimmick->GetAnimState_();
			//�J���Ă����Ԃ�������
			if (aState_ == GIMMICK_ANIM::OPEN)
			{
				//�����蔻����s��Ȃ�
				continue;
			}
		}
		Vector3 from = Vector3(pos_.x, pos_.y + 500, pos_.z);
		Vector3 player2TargetPosVector = (Vector3(pos_.x , pos_.y + 500,
			pos_.z + movePower) - from).Normalized();
		Vector3 to = from + player2TargetPosVector * 250;
		auto stageHits = MV1CollCheck_Line(lpModelMng.GetID(gimmick->GetCollModelStr())[gimmick->GetModelNum()], -1, from.toVECTOR(), to.toVECTOR());
		if (stageHits.HitFlag)
		{
			return stageHits.HitFlag;
		}
	}
	return false;
}

void Player::HitCapsuleAndGoal()
{
	isHitGoal_ = false;
	auto hits = MV1CollCheck_Capsule(
		lpModelMng.GetID("resource/model/Goal.mv1")[0],
		-1,
		capsule_.topPos,
		capsule_.downPos,
		capsule_.radius);

	if (hits.HitNum > 0)
	{
		for (auto& inventory : inventoryList_)
		{
			if (inventory.itemID_ == ItemID::Entrance_Key)
			{
				isGameClear_ = true;
			}
		}
		isHitGoal_ = true;
	}
	//�Փ˔���p�̓��I�\���̂̃������[���
	MV1CollResultPolyDimTerminate(hits);
}

void Player::AddInventory(Vector3 targetPos)
{
	pickUpFlag_ = false;
	catchFlg_ = false;
	for (auto& item : itemList_)
	{
		//�A�C�e�����擾�܂��̓��b�N���������Ă��鎞��
		if (item->GetPickUpFlg() || item->GetLockFlg())
		{
			//����
			continue;
		}
		Vector3 from = Vector3(pos_.x, pos_.y + CAMERA_HEIGHT, pos_.z);
		Vector3 player2TargetPosVector = (targetPos - from).Normalized();
		Vector3 to = from + player2TargetPosVector * INTERACT_LINE_LENGTH;
		auto itemcoll = MV1CollCheck_Line(lpModelMng.GetID(item->GetModelString())[item->GetModelNumber()], -1, from.toVECTOR(), to.toVECTOR());
		if (itemcoll.HitFlag == 1)
		{
			switch (item->GetItemID())
			{
			case ItemID::MusicRoom_Key:
			case ItemID::Entrance_Key:
			case ItemID::Kitchen_Key:
			case ItemID::FoodBank_Key:
			case ItemID::ChildRoom_Key:
				//�J�������\�����Ă��Ȃ�������
				if (!minCamera_->GetIsOnCamera())
				{
					//����
					continue;
				}
				break;
			default:
				break;
			}
			pickUpFlag_ = true;
			catchFlg_ = true;
			item->SetHitFlg(true);
		}

		if (pickUpFlag_)
		{
			if (controller_->CheckInputKey(KeyID::Interact))
			{
				//���߂ăA�C�e�����������Ƃ����ATutorialflg��true
				lpTutorialMng.SetTutorialFlg(TutorialProgress::Action, true);
				bool flag = false;
				for (auto& inventory : inventoryList_)
				{
					if (inventory.itemID_ == item->GetItemID())
					{
						flag = true;
					}
				}
				if (!flag)
				{
					item->SetPickUpFlg(true);
					lpSoundMng.PlayingSound("./resource/sound/PickUpSE.mp3");

					VECTOR Scale = MV1GetScale(
						lpModelMng.GetID(item->GetModelString())
						[item->GetModelNumber()]
					);
					inventoryList_.emplace_back(
						item->GetModelString(),
						item->GetModelNumber(),
						item->GetItemID(),
						Vector3{ 0,0,0 },
						Vector3{ Scale.x,Scale.y,Scale.z },
						"",
						ItemID::Non
					);
					itemID_ = item->GetItemID();

					break;
				}
			}
		}
		else
		{
			item->SetHitFlg(false);
		}
	}

	for (auto& item : itemList_)
	{
		if (item->GetItemID() == lpSocketServer.GetItemID())
		{
			bool flag = false;
			for (auto& inventory : inventoryList_)
			{
				if (inventory.itemID_ == item->GetItemID())
				{
					flag = true;
				}
			}
			if (!flag)
			{
				item->SetPickUpFlg(true);
				VECTOR dd = MV1GetScale(lpModelMng.GetID(item->GetModelString())[item->GetModelNumber()]);
				inventoryList_.emplace_back(item->GetModelString(),
					item->GetModelNumber(),
					item->GetItemID(),
					Vector3{ 0,0,0 },
					Vector3{ dd.x,dd.y,dd.z },
					""
				);
				break;
			}
		}
	}
	lpMouseIcon.SetFlg(pickUpFlag_);
}

void Player::InteractGimmick(Vector3 targetPos)
{
	int modelNum = 0;
	for (auto& gimmick : gimmickList_)
	{
		std::string name = gimmick->GetName();
		modelNum = gimmick->GetModelNum();
		//����̃I�u�W�F�N�g�̎����̓����蔻��𖳎�
		auto gType = gimmick->GetType();
		switch (gType)
		{
			//�����蔻����s��
		case GIMMICK_TYPE::DOOR:
		case GIMMICK_TYPE::DRAWER:
		case GIMMICK_TYPE::DIAL:
		case GIMMICK_TYPE::DL_DOOR:
		case GIMMICK_TYPE::JUKE_BOX:
			break;
		default:
			continue;
		}
		Vector3 from = Vector3(pos_.x, pos_.y + CAMERA_HEIGHT, pos_.z);
		Vector3 player2TargetPosVector = (targetPos - from).Normalized();
		Vector3 to = from + player2TargetPosVector * INTERACT_LINE_LENGTH;
		auto coll = MV1CollCheck_Line(lpModelMng.GetID(gimmick->GetCollModelStr())[modelNum], -1, from.toVECTOR(), to.toVECTOR());
		if (coll.HitFlag == 1)
		{
			//�����̋����Ɠ������Ă���
			gimmick->SetHitFlg(true);
			catchFlg_ = true;
		}
		//���������|���S���̖@����ݒ�
		gimmick->SetHitNormal(coll.Normal);
		if (CheckInventory(gimmick))
		{
			if (gimmick->GetIsLockFlg() == false)
			{
				lpSoundMng.StopSound("./resource/sound/pianoBGM2.mp3");
			}
		}
		if (!pickUpFlag_)
		{
			if (catchFlg_)
			{
				//�C���^���N�g����
				if (controller_->CheckInputKey(KeyID::Interact))
				{
					//���߂ăM�~�b�N�������Ƃ����ATutorialflg��true
					lpTutorialMng.SetTutorialFlg(TutorialProgress::Action, true);

					//�C���^���N�g�t���O��true
					gimmick->SetInteractFlg(true);

					if (gType == GIMMICK_TYPE::DOOR)
					{
						//�C���x���g�����ɓ���̃A�C�e������������
						if (CheckInventory(gimmick))
						{
							//���b�N������
							gimmick->SetIsLockFlg(false);
						}
					}
					break;
				}
			}
			else
			{
				gimmick->SetHitFlg(false);
			}
		}
	}
}

void Player::GimmickEvent(Vector3 targetPos)
{
	bool catchFlg = false;
	char numchar = '0';
	int modelNum = 0;
	bool cameraFlg = minCamera_->GetIsOnCamera();;
	bool fFlg = minCamera_->GetFlashFlag();
	for (auto& gimmick : gimmickList_)
	{
		modelNum = gimmick->GetModelNum();
		//����̃I�u�W�F�N�g�̎����̓����蔻��𖳎�
		auto gType = gimmick->GetType();
		switch (gType)
		{
			//�����蔻����s��
		case GIMMICK_TYPE::DIAL:
		case GIMMICK_TYPE::DL_DOOR:
		case GIMMICK_TYPE::TEXT:
		case GIMMICK_TYPE::WALL:
			break;
		default:
			continue;
			break;
		}
		if (gType == GIMMICK_TYPE::TEXT)
		{
			if (cameraFlg)
			{
				//���b�N������
				gimmick->SetIsLockFlg(false);
			}
			else
			{
				//���b�N������
				gimmick->SetIsLockFlg(true);
			}
		}
		//�_�C�����J�E���g�X�V����
		DialCountUpdate(gimmick, modelNum, numchar);

		//�_�C�����ԍ��������ԍ��ƈ�v���Ă�����
		if (dialNumStr_ == gimmick->GetUnlockNumber())
		{
			if (gType == GIMMICK_TYPE::DL_DOOR)
			{
				//���b�N������
				gimmick->SetIsLockFlg(false);
			}
		}

		Vector3 from = Vector3(pos_.x, pos_.y + CAMERA_HEIGHT, pos_.z);
		Vector3 player2TargetPosVector = (targetPos - from).Normalized();
		Vector3 to = from + player2TargetPosVector * WALL_INTERACT_LINE_LENGTH;
		auto coll = MV1CollCheck_Line(lpModelMng.GetID(gimmick->GetCollModelStr())[modelNum], -1, from.toVECTOR(), to.toVECTOR());
		if (coll.HitFlag == 1)
		{
			//�����̋����Ɠ������Ă���
			catchFlg = true;
		}

		if (catchFlg)
		{
			//�t���b�V����������
			if (fFlg)
			{
				//�C���x���g�����ɓ���̃A�C�e������������
				if (CheckInventory(gimmick))
				{
					if (gType == GIMMICK_TYPE::WALL)
					{
						//���b�N������
						gimmick->SetIsLockFlg(false);
						break;
					}
				}
			}
		}
	}
}

void Player::ConnectingDrawerToItem(void)
{
	for (auto& gimmick : gimmickList_)
	{
		for (auto& item : itemList_)
		{
			if ((gimmick->GetName() == "Celler_Drawer" && item->GetItemID()==ItemID::Celler_Key) ||
				(gimmick->GetName() == "StoryBookC_Drawer" && item->GetItemID() == ItemID::Story_Book_C)||
				(gimmick->GetName() == "EnemyMemo_Drawer" && item->GetItemID() == ItemID::Enemy_Memo))
			{
				if (gimmick->GetInteractFlg())
				{
					item->SetMoveFlg(true);
				}
			}
		}
	}
}

void Player::DialCountUpdate(const std::shared_ptr<Gimmick>& gimmick, int modelNum, char numchar)
{
	//���f�����������ꍇ
	if (modelNum >= MAX_DIAL_NUM)
	{
		//�z��̗v�f���𒴂��Ȃ��悤�ɗ]����o��
		modelNum %= MAX_DIAL_NUM;
	}
	//�J�E���g�A�b�v
	dial_[modelNum] = gimmick->GetDialNumber();

	//int->char�ϊ�(ASCII����int�ɕϊ����邽��0��ǉ�)
	numchar = static_cast<char>(dial_[modelNum] + '0');

	//�X�V���鐔�����폜
	dialNumStr_.erase(dialNumStr_.begin() + modelNum);

	//�폜�����ꏊ�ɍX�V����������}��
	dialNumStr_.insert(dialNumStr_.begin() + modelNum, numchar);
}

bool Player::CheckInventory(const std::shared_ptr<Gimmick>& gimmick)
{
	for (auto& inventory : inventoryList_)
	{
		//�C���x���g�����ɓ���̃A�C�e������������
		if (inventory.itemID_ == gimmick->GetItemID())
		{
			inventory.usedItemID_ = inventory.itemID_;
			return true;
		}
	}
	return false;
}

void Player::Sound(void)
{
	if (!dashFlag_)
	{
		if (oldpos_ == pos_)
		{
			lpSoundMng.StopSound("./resource/sound/playerWalkSE.mp3");
			lpSoundMng.StopSound("./resource/sound/playerRunSE.mp3");
			lpSoundMng.StopSound("./resource/sound/playerRunVoice.mp3");
		}
		else
		{
			lpSoundMng.PlayingSound("./resource/sound/playerWalkSE.mp3");
			lpSoundMng.StopSound("./resource/sound/playerRunSE.mp3");
			lpSoundMng.StopSound("./resource/sound/playerRunVoice.mp3");
		}
	}
	else
	{
		lpSoundMng.StopSound("./resource/sound/playerWalkSE.mp3");
		lpSoundMng.PlayingSound("./resource/sound/playerRunSE.mp3");
		lpSoundMng.PlayingSound("./resource/sound/playerRunVoice.mp3");
	}

	if (minCamera_->GetFlashFlag())
	{
		lpSoundMng.PlayingSound("./resource/sound/ShotSE.mp3");
	}
}

bool Player::GetLowSensFlg(void)
{
	return lowSensFlg_;
}

void Player::SlideLinePosition(void)
{
	VECTOR distance{};

	auto fallHit = MV1CollCheck_Line(collisionHandle_, -1, VGet(pos_.x, pos_.y, pos_.z), VGet(pos_.x, pos_.y + COLLISION_LINE_LENGTH, pos_.z));
	if (fallHit.HitFlag)
	{
		//�߂肱�񂾕��␳
		distance = VSub(fallHit.HitPosition, pos_.toVECTOR());
		pos_.y += distance.y;
	}
	else
	{
		//�������Ă��Ȃ���Η�������
		if (state_ != State::Fall)
		{
			state_ = State::Fall;
			fallGravity_ = 0.0f;
		}
	}

	if (state_ == State::Fall)
	{
		pos_.y -= (FALL_SPEED + fallGravity_);
		fallGravity_ += FALL_ACCEL;
		fallHit = MV1CollCheck_Line(collisionHandle_, -1, VGet(pos_.x, pos_.y, pos_.z), VGet(pos_.x, pos_.y + COLLISION_LINE_LENGTH, pos_.z));
		//�߂肱�񂾕��␳
		if (fallHit.HitFlag)
		{
			distance = VSub(fallHit.HitPosition, pos_.toVECTOR());
			pos_.y += distance.y;
			state_ = State::Walk;
		}
	}
	//�J�v�Z���ʒu�X�V
	capsule_.topPos = VGet(pos_.x, pos_.y + CAPSULE_RELATIVE_TOP_Y, pos_.z);
	capsule_.downPos = VGet(pos_.x, pos_.y + CAPSULE_RELATIVE_DOWN_Y, pos_.z);
}
