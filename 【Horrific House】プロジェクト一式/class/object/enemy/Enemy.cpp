#include<malloc.h>
#include "Enemy.h"
#include "../../common/manager/SoundMng.h"
#include "../../common/manager/ModelMng.h"
#include"Dijkstra.h"
#include "../gimmick/Gimmick.h"
#include "../../common/AnimationController.h"
#include "../../Scene/SceneManager.h"
#include"../../common/Capsule.h"
#include"../Item/Item.h"
#include"../../common/OriginalShader.h"

//敵の大きさ
constexpr float SIZE_SCALE = 3.7f;

//プレイヤー探索用定数
constexpr float SEARCH_ANGLE	= 20.0f;	//索敵角度
constexpr float SEARCH_RANGE	= 8000.0f;	//索敵範囲
constexpr float SEARCH_OFFSET_Y = 300.0f;	//視野のオフセットｙ軸

//プレイヤー追跡用定数
constexpr double HUNT_LIMIT_TIME = 3.0;	//範囲外にいるときの追跡する時間

//回転時間
double ROTATION_TIME = 1.0f;
double ROTATION_TIME_gmov = 0.1f;//ゲームオーバー時にプレイヤーのほうを見る（仮）

//プレイヤーのオフセット
constexpr float PLAYER_OFFSET_Y = 76.0f;		//高さ
constexpr float PLAYER_CAPSULE_OFFSET_RADIUS = 500.0f;	//カプセルのオフセット半径

//スタン関係
constexpr double STAN_MAX_LIMIT_TIME  = 7.6;	//最大時間
constexpr double STAN_MIN_LIMIT_TIME  = 0.5;	//最小時間
constexpr double STAN_RESUSTANCE_TIME = 1.52;	//スタンするたびに引かれる時間
constexpr double STAN_HEAL_IME		  = 0.5;	//増やす時間

Enemy::Enemy(const std::vector<std::shared_ptr<Gimmick>>& gList)
{
	gimmickList_ = gList;
	Init();
}

Enemy::Enemy(Vector3 pos)
{
	Init(pos);
	transform_.pos_ = pos;
}

Enemy::~Enemy()
{
	int a = 0;
}

void Enemy::Init(void)
{
	Vector3 vectorZero = { 0.0f,0.0f,0.0f };
	//トランスフォーム
	transform_.Init("./resource/model/enemy/ghost3.mv1", vectorZero, {SIZE_SCALE, SIZE_SCALE, SIZE_SCALE }, vectorZero, vectorZero);
	transform_.quaRotLocal_ = Quaternion(VGet(0.0f, Deg2RadF(180.0f), 0.0f));
	transform_.quaRot_ = Quaternion(VGet(0.0f, Deg2RadF(180.0f), 0.0f));
	transform_.Update();

	//アニメーション関係
	animController_ = std::make_unique<AnimationController>(transform_.modelHandle_);
	animController_->Add((int)EnemyAnimation::Idle, "./resource/model/enemy/anim/Idle.mv1", 20.0f);
	animController_->Add((int)EnemyAnimation::Run, "./resource/model/enemy/anim/Run.mv1", 60.0f);
	animController_->Add((int)EnemyAnimation::FemaleDynamicPose, "./resource/model/enemy/anim/Female Dynamic Pose.mv1", 20.0f);
	animController_->Add((int)EnemyAnimation::Damage, "./resource/model/enemy/anim/Damage.mv1", 20.0f);
	animController_->Add((int)EnemyAnimation::Walk, "./resource/model/enemy/anim/Walk.mv1", 45.0f);
	animController_->Play((int)EnemyAnimation::Walk, true);

	//当たり判定
	MV1SetupCollInfo(transform_.modelHandle_, -1, 1, 1, 1);
	

	huntFlag_ = false;		//追跡フラグ
	wallAlphaFlag_ = false;	//壁透過フラグ
	stepHuntTime_ = 0.0;	//追跡時間カウント

	targetRot_ = Quaternion();
	stepRotTime_ = 0.0f;

	dijkstra_ = std::make_unique<Dijkstra>();
	playerCapsule_ = Capsule();
	hitPlayerFlag_ = false;
	stanFlag_ = false;
	nowStanTime_ = STAN_MAX_LIMIT_TIME;
	stepStanTime_ = 0.0;
	searchDir_ = vectorZero;
}

void Enemy::Init(Vector3 pos)
{
	Vector3 vectorZero = { 0.0f,0.0f,0.0f };
	//トランスフォーム
	transform_.Init("./resource/model/enemy/ghost3.mv1", pos, { SIZE_SCALE, SIZE_SCALE, SIZE_SCALE }, vectorZero, vectorZero);
	animController_ = std::make_unique<AnimationController>(transform_.modelHandle_);
	animController_->Add((int)EnemyAnimation::Idle, "./resource/model/enemy/anim/Idle.mv1", 20.0f);
	animController_->Add((int)EnemyAnimation::Run, "./resource/model/enemy/anim/Run.mv1", 60.0f);
	animController_->Add((int)EnemyAnimation::FemaleDynamicPose, "./resource/model/enemy/anim/Female Dynamic Pose.mv1", 20.0f);
	animController_->Add((int)EnemyAnimation::Damage, "./resource/model/enemy/anim/Damage.mv1", 20.0f);
	animController_->Add((int)EnemyAnimation::Walk, "./resource/model/enemy/anim/Walk.mv1", 45.0f);
	animController_->Play((int)EnemyAnimation::Walk, true);
	MV1SetupCollInfo(transform_.modelHandle_, -1, 1, 1, 1);

	transform_.quaRotLocal_ = Quaternion(VGet(0.0f, Deg2RadF(180.0f), 0.0f));
	transform_.quaRot_ = Quaternion(VGet(0.0f, Deg2RadF(180.0f), 0.0f));
	transform_.Update();
	MV1SetPosition(transform_.modelHandle_, transform_.pos_.toVECTOR());

	huntFlag_ = false;
	stepHuntTime_ = 0.0;

	targetRot_ = Quaternion();
	stepRotTime_ = 0.0f;

	dijkstra_ = std::make_unique<Dijkstra>();
	playerCapsule_ = Capsule();
	stanFlag_ = false;
	nowStanTime_ = STAN_MAX_LIMIT_TIME;
	stepStanTime_ = 0.0;
	searchDir_ = vectorZero;
}

void Enemy::Release(void)
{
}

void Enemy::Update(void)
{
	animController_->Update(lpSceneMng.GetDeltaTime());
	//音の座標設定
	Set3DPositionSoundMem(transform_.pos_.toVECTOR(), lpSoundMng.GetID("./resource/sound/enemyWalkSE.mp3")[0]);
	Set3DPositionSoundMem(transform_.pos_.toVECTOR(), lpSoundMng.GetID("./resource/sound/WalkVoice.mp3")[0]);
	Set3DPositionSoundMem(transform_.pos_.toVECTOR(), lpSoundMng.GetID("./resource/sound/RunVoice.mp3")[0]);
	Set3DPositionSoundMem(transform_.pos_.toVECTOR(), lpSoundMng.GetID("./resource/sound/HeatBeat.mp3")[0]);
	Set3DPositionSoundMem(transform_.pos_.toVECTOR(), lpSoundMng.GetID("./resource/sound/chaseBGM.mp3")[0]);

	if (!huntFlag_)
	{
		//音の停止
		lpSoundMng.StopSound("./resource/sound/HeatBeat.mp3");
		lpSoundMng.StopSound("./resource/sound/chaseBGM.mp3");
		lpSoundMng.StopSound("./resource/sound/RunVoice.mp3");
		if (!stanFlag_)
		{
			//音の再生
			lpSoundMng.Play3DSound("./resource/sound/enemyWalkSE.mp3", transform_.pos_, 7000.0f);
			lpSoundMng.Play3DSound("./resource/sound/WalkVoice.mp3", transform_.pos_, 7000.0f);
		}

	}

	stepStanTime_ += lpSceneMng.GetDeltaTime();
}

void Enemy::Update(Capsule capsule, const Vector3 playerPos,bool hitFlash, ItemID itemID,const std::vector<std::shared_ptr<Gimmick>>& gimmick)
{
	playerCapsule_ = capsule;
	//更新
	Update();

	//スタン時は移動や追跡等をしない
	if (stanFlag_)
	{
		RecoverFlash();
		return;
	}
	else
	{
		DamageFlash(hitFlash);
	}

	//追跡中かどうか
	if (!huntFlag_)
	{
		//プレイヤーを探索
		SearchPlayer(playerPos);
	}
	else
	{
		//プレイヤーを追跡
		goalPos_ = playerPos;
		HuntPlayer();
	}

	//スタン時間を長くする
	RecoverStanTime();

	bool wallAlphaFlag = false;
	for (const auto& g : gimmick)
	{
		if (g->GetType() == GIMMICK_TYPE::WALL)
		{
			wallAlphaFlag = g->GetIsLockFlg();
			break;
		}
	}

	//壁を透過した瞬間に追跡モードにする
	if (!wallAlphaFlag && !wallAlphaFlag_)
	{
		wallAlphaFlag_ = true;

		huntFlag_ = true;
		animController_->Play(EnemyAnimation::Run, true);
		//音の再生
		lpSoundMng.Play3DSound("./resource/sound/RunVoice.mp3", transform_.pos_, 15000.0f);
		lpSoundMng.Play3DSound("./resource/sound/HeatBeat.mp3", transform_.pos_, 15000.0f);
		lpSoundMng.Play3DSound("./resource/sound/chaseBGM.mp3", transform_.pos_, 10000.0f);
	}

	//ダイクストラ処理
	DijkstraProcess(playerPos,itemID,wallAlphaFlag);

	transform_.Update();
	hitPlayerFlag_ = CheckHitPlayer();
}
//ゲームオーバー時の演出用更新処理
void Enemy::PKMotion(Vector3 TargetPos)
{
	Update();
	animController_->Play(EnemyAnimation::Idle, true);
	Lock_On_Target(TargetPos);
	transform_.Update();
}

void Enemy::Draw(void)
{
	transform_.Draw();

#ifdef _DEBUG

	VECTOR ePos = transform_.pos_.toVECTOR();
	ePos.y += SEARCH_OFFSET_Y;

	auto angle = transform_.rot_;

	float searchRad = Deg2RadF(SEARCH_ANGLE);

	float sinX1 = sinf(angle.y);
	float sinZ1 = cosf(angle.y);

	float sinX2 = sinf(angle.y - searchRad);
	float sinZ2 = cosf(angle.y - searchRad);

	float sinX3 = sinf(angle.y + searchRad);
	float sinZ3 = cosf(angle.y + searchRad);

	VECTOR pos1 = transform_.pos_.toVECTOR();
	VECTOR pos2 = transform_.pos_.toVECTOR();
	VECTOR pos3 = transform_.pos_.toVECTOR();

	pos1.x += sinX1 * SEARCH_RANGE;
	pos1.y += SEARCH_OFFSET_Y;
	pos1.z += sinZ1 * SEARCH_RANGE;

	pos2.x += sinX2 * SEARCH_RANGE;
	pos2.y += SEARCH_OFFSET_Y;
	pos2.z += sinZ2 * SEARCH_RANGE;

	pos3.x += sinX3 * SEARCH_RANGE;
	pos3.y += SEARCH_OFFSET_Y;
	pos3.z += sinZ3 * SEARCH_RANGE;

	MV1_COLL_RESULT_POLY lineHit{};

	//目の前の線
	lineHit = MV1CollCheck_Line(lpModelMng.GetID("resource/model/stage/Coll4.mv1")[0], -1, ePos, pos1);
	if (lineHit.HitFlag)
	{
		pos1 = lineHit.HitPosition;
	}
	targetPos_ = { pos1.x ,pos1.y ,pos1.z };
	//左の線
	lineHit = MV1CollCheck_Line(lpModelMng.GetID("resource/model/stage/Coll4.mv1")[0], -1, ePos, pos2);
	if (lineHit.HitFlag)
	{
		pos2 = lineHit.HitPosition;
	}

	//目の前の線
	lineHit = MV1CollCheck_Line(lpModelMng.GetID("resource/model/stage/Coll4.mv1")[0], -1, ePos, pos3);
	if (lineHit.HitFlag)
	{
		pos3 = lineHit.HitPosition;
	}

	int modelNum = 0;
	for (auto& gimmick : gimmickList_)
	{
		auto gType = gimmick->GetType();
		switch (gType)
		{
		case GIMMICK_TYPE::NONE:
		case GIMMICK_TYPE::FRAME:
		case GIMMICK_TYPE::DRAWER:
		case GIMMICK_TYPE::DIAL:
		case GIMMICK_TYPE::TEXT:
		case GIMMICK_TYPE::WALL:
		case GIMMICK_TYPE::WATER:
			continue;
		case GIMMICK_TYPE::DL_DOOR:
		case GIMMICK_TYPE::DOOR:
			break;
		default:
			break;
		}
		modelNum = gimmick->GetModelNum();
		lineHit = MV1CollCheck_Line(lpModelMng.GetID(gimmick->GetCollModelStr())[modelNum], -1, ePos, pos1);
		if (lineHit.HitFlag)
		{
			pos1 = lineHit.HitPosition;
		}

		//左の線
		lineHit = MV1CollCheck_Line(lpModelMng.GetID(gimmick->GetCollModelStr())[modelNum], -1, ePos, pos2);
		if (lineHit.HitFlag)
		{
			pos2 = lineHit.HitPosition;
		}

		//目の前の線
		lineHit = MV1CollCheck_Line(lpModelMng.GetID(gimmick->GetCollModelStr())[modelNum], -1, ePos, pos3);
		if (lineHit.HitFlag)
		{
			pos3 = lineHit.HitPosition;
		}
	}
	DrawLine3D(ePos, pos1, 0xffff00);
	DrawLine3D(ePos, pos2, 0xffff00);
	DrawLine3D(ePos, pos3, 0xffff00);

	DrawTriangle3D(ePos, pos2, pos1, 0x888800,true);	//左
	DrawTriangle3D(ePos, pos1, pos3, 0x888800, true);	//右

	DrawFormatString(0, 300, 0xffffff, "敵の座標 : (%.1f, %.1f, %.1f)", transform_.pos_.x, transform_.pos_.y, transform_.pos_.z);

	dijkstra_->Draw();
#endif // _DEBUG
}

Vector3 Enemy::GetPosition(void)
{
	return transform_.pos_;
}

Vector3 Enemy::GetDirectionVec(void)
{
	auto vec = (searchDir_ - transform_.pos_);
	vec.Normalize();
	return vec;
}

Vector3 Enemy::GetTargetLine(void)
{
	return targetPos_;
}

bool Enemy::GetHitPlayerFlag(void)
{
	return hitPlayerFlag_;
}

bool Enemy::GetHuntFlag(void)
{
	return huntFlag_;
}

void Enemy::SearchPlayer(const Vector3 playerPos)
{
	if (Search(playerPos))
	{
		//プレイヤー発見時
		huntFlag_ = true;
		//プレイヤーの位置にノードを追加
		dijkstra_->AddHuntNode({ playerPos.x,playerPos.y - PLAYER_OFFSET_Y,playerPos.z }, transform_.pos_);
		animController_->Play(EnemyAnimation::Run, true);

		//音の再生
		lpSoundMng.Play3DSound("./resource/sound/RunVoice.mp3",transform_.pos_, 15000.0f);
		lpSoundMng.Play3DSound("./resource/sound/HeatBeat.mp3", transform_.pos_, 15000.0f);
		lpSoundMng.Play3DSound("./resource/sound/chaseBGM.mp3", transform_.pos_, 10000.0f);
	}
}

bool Enemy::Search(const Vector3 playerPos)
{
	bool hitFlag = false;
	VECTOR ePos = transform_.pos_.toVECTOR();
	ePos.y += SEARCH_OFFSET_Y;

	auto angle = transform_.rot_;

	float searchRad = Deg2RadF(SEARCH_ANGLE);

	VECTOR pos1 = transform_.pos_.toVECTOR();
	VECTOR pos2 = transform_.pos_.toVECTOR();
	VECTOR pos3 = transform_.pos_.toVECTOR();

	pos1.x += sinf(angle.y) * SEARCH_RANGE;
	pos1.y += SEARCH_OFFSET_Y;
	pos1.z += cosf(angle.y) * SEARCH_RANGE;

	pos2.x += sinf(angle.y - searchRad) * SEARCH_RANGE;
	pos2.y += SEARCH_OFFSET_Y;
	pos2.z += cosf(angle.y - searchRad) * SEARCH_RANGE;

	pos3.x += sinf(angle.y + searchRad) * SEARCH_RANGE;
	pos3.y += SEARCH_OFFSET_Y;
	pos3.z += cosf(angle.y + searchRad) * SEARCH_RANGE;

	MV1_COLL_RESULT_POLY lineHit{};

	searchDir_ = ConvertVECTORtoVector3(pos1);

	//目の前の線
	lineHit = MV1CollCheck_Line(lpModelMng.GetID("resource/model/stage/Coll4.mv1")[0], -1, ePos, pos1);
	if (lineHit.HitFlag)
	{
		pos1 = lineHit.HitPosition;
	}

	//左の線
	lineHit = MV1CollCheck_Line(lpModelMng.GetID("resource/model/stage/Coll4.mv1")[0], -1, ePos, pos2);
	if (lineHit.HitFlag)
	{
		pos2 = lineHit.HitPosition;
	}

	//右の線
	lineHit = MV1CollCheck_Line(lpModelMng.GetID("resource/model/stage/Coll4.mv1")[0], -1, ePos, pos3);
	if (lineHit.HitFlag)
	{
		pos3 = lineHit.HitPosition;
	}
	
	int modelNum = 0;
	for (auto& gimmick : gimmickList_)
	{
		auto gType = gimmick->GetType();
		switch (gType)
		{
		case GIMMICK_TYPE::NONE:
		case GIMMICK_TYPE::FRAME:
		case GIMMICK_TYPE::DRAWER:
		case GIMMICK_TYPE::DIAL:
		case GIMMICK_TYPE::TEXT:
			continue;
		case GIMMICK_TYPE::WALL:
		case GIMMICK_TYPE::DL_DOOR:
		case GIMMICK_TYPE::DOOR:
			break;
		default:
			break;
		}
		modelNum = gimmick->GetModelNum();
		lineHit = MV1CollCheck_Line(lpModelMng.GetID(gimmick->GetCollModelStr())[modelNum], -1, ePos, pos1);
		if (lineHit.HitFlag)
		{
			pos1 = lineHit.HitPosition;
		}

		//左の線
		lineHit = MV1CollCheck_Line(lpModelMng.GetID(gimmick->GetCollModelStr())[modelNum], -1, ePos, pos2);
		if (lineHit.HitFlag)
		{
			pos2 = lineHit.HitPosition;
		}

		//目の前の線
		lineHit = MV1CollCheck_Line(lpModelMng.GetID(gimmick->GetCollModelStr())[modelNum], -1, ePos, pos3);
		if (lineHit.HitFlag)
		{
			pos3 = lineHit.HitPosition;
		}
	}

	int hit = 0;

	//敵の視界に衝突しているかどうか判定
	hit = HitCheck_Capsule_Triangle(
		playerCapsule_.topPos,
		playerCapsule_.downPos,
		playerCapsule_.radius,
		ePos,
		pos2,
		pos1);

	if (hit)
	{
		return true;
	}

	hit = HitCheck_Capsule_Triangle(
		playerCapsule_.topPos,
		playerCapsule_.downPos,
		playerCapsule_.radius,
		ePos,
		pos1,
		pos3);

	if (hit)
	{
		return true;
	}

	return false;
}

void Enemy::HuntPlayer(void)
{
	//見つかって三秒以内ならtrue
	if (stepHuntTime_ < HUNT_LIMIT_TIME)
	{
		if (!Search(goalPos_))
		{
			//視界外ならカウント
			stepHuntTime_ += lpSceneMng.GetDeltaTime();
		}
		else
		{
			//視界内の場合は時間を0.0にする
			stepHuntTime_ = 0.0;
		}
	}
	else
	{
		//3秒以上経っている場合は追跡フラグを解除する
		if (!Search(goalPos_))
		{
			huntFlag_ = false;
			lpSoundMng.StopSound("./resource/sound/HeatBeat.mp3");
			lpSoundMng.StopSound("./resource/sound/chaseBGM.mp3");
			animController_->Play(EnemyAnimation::Walk, true);
			dijkstra_->ConnectNearNode(transform_.pos_);
		}
		stepHuntTime_ = 0.0;
	}
}

void Enemy::DijkstraProcess(Vector3 playerPos,ItemID itemID,bool alphaFlag)
{
	//移動処理
	Vector3 pos = dijkstra_->Update(huntFlag_, { playerPos.x,playerPos.y - PLAYER_OFFSET_Y,playerPos.z }, itemID,alphaFlag);
	Vector3 moveVec = (pos - transform_.pos_).Normalized();
	
	//Y軸回転処理
	Quaternion rotY = AngleAxis(atan2(moveVec.x, moveVec.z), { 0.0f, 1.0f, 0.0f });

	//角度差を求める
	double angleDeg = AngleDifference(rotY, targetRot_);

	//閾値
	if (angleDeg >= 1.0)
	{
		stepRotTime_ = ROTATION_TIME;
		targetRot_ = rotY;
	}

	stepRotTime_ -= lpSceneMng.GetDeltaTime();

	//回転補間
	transform_.quaRot_ = transform_.quaRot_.Slerp(targetRot_, (ROTATION_TIME - stepRotTime_) / ROTATION_TIME);
	transform_.pos_ = pos;
}

bool Enemy::CheckHitPlayer(void)
{
	//敵とプレイヤーが衝突した場合、ゲームオーバーへ
	auto hits = MV1CollCheck_Capsule(transform_.modelHandle_, -1, playerCapsule_.topPos, playerCapsule_.downPos, playerCapsule_.radius + PLAYER_CAPSULE_OFFSET_RADIUS);
	bool hitFlag = false;

	if (hits.HitNum > 0)
	{
		hitFlag = true;
	}
	//後始末
	MV1CollResultPolyDimTerminate(hits);
	return hitFlag;
}

void Enemy::DamageFlash(bool hitFlash)
{
	if (hitFlash)
	{
		stepStanTime_ = 0.0;
		stanFlag_ = true;
		animController_->Play(EnemyAnimation::Damage, false);
		lpSoundMng.StopSound("./resource/sound/enemyWalkSE.mp3");
		lpSoundMng.Play3DSound("./resource/sound/WalkVoice.mp3", transform_.pos_, 7000.0f);
	}
}

void Enemy::RecoverFlash(void)
{
	if (stepStanTime_ > nowStanTime_)
	{
		huntFlag_ = false;
		stanFlag_ = false;
		animController_->Play(EnemyAnimation::Walk, true);
		//スタン時間短縮
		ReduceStanTime();
	}
}

void Enemy::RecoverStanTime(void)
{

	nowStanTime_ += lpSceneMng.GetDeltaTime() / 6.0;
	if (nowStanTime_ > STAN_MAX_LIMIT_TIME)
	{
		nowStanTime_ = STAN_MAX_LIMIT_TIME;
	}
}

void Enemy::ReduceStanTime(void)
{
	nowStanTime_ -= STAN_RESUSTANCE_TIME;

	//最低スタン時間は0.5秒
	if (nowStanTime_ < STAN_MIN_LIMIT_TIME)
	{
		nowStanTime_ = STAN_MIN_LIMIT_TIME;
	}
}

void Enemy::Lock_On_Target(Vector3 targetPos)
{
	Vector3 moveVec = (targetPos - transform_.pos_).Normalized();

	//Y軸回転処理
	Quaternion rotY = AngleAxis(atan2(moveVec.x, moveVec.z), { 0.0f, 1.0f, 0.0f });

	//角度差を求める
	double angleDeg = AngleDifference(rotY, targetRot_);

	//閾値条件
	if (angleDeg >= 1.0)
	{
		stepRotTime_ = ROTATION_TIME_gmov;
		targetRot_ = rotY;
	}

	stepRotTime_ -= lpSceneMng.GetDeltaTime();

	transform_.quaRot_ = transform_.quaRot_.Slerp(targetRot_, (ROTATION_TIME_gmov - stepRotTime_) / ROTATION_TIME_gmov);
}
