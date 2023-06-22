#include "ARmarker.h"
#include "../../common/manager/ImageMng.h"
#include "../../common/manager/SoundMng.h"
#include "../../common/manager/ModelMng.h"
#include "../../common/OriginalShader.h"

constexpr float SCALE = 3.0f;

ARmarker::ARmarker()
{
}

ARmarker::ARmarker(Vector3 pos,Vector3 angle,MarkerID id)
{
	Init(pos,angle,id);
}

ARmarker::~ARmarker()
{
	
}

void ARmarker::Init(void)
{
}

void ARmarker::Init(Vector3 pos, Vector3 angle, MarkerID id)
{
	Vector3 vectorZero = { 0.0f,0.0f,0.0f };
	switch (id)
	{
	case MarkerID::test:
		//transform_.Init("./resource/model/marker/redMarker.mv1", pos, { SCALE,SCALE,SCALE }, vectorZero, vectorZero);
		transform_.Init("resource/model/marker/redMarker.mv1", pos, { SCALE,SCALE,SCALE }, angle, vectorZero);
		break;
	case MarkerID::red:
		//transform_.Init("./resource/model/marker/redMarker.mv1", pos, { SCALE,SCALE,SCALE }, vectorZero, vectorZero);
		transform_.Init("resource/model/marker/obakemarkerWall_red.mv1", pos, { SCALE,SCALE,SCALE }, angle, vectorZero);
		break;
	case MarkerID::blue:
		//transform_.Init("./resource/model/marker/blueMarker.mv1", pos, { SCALE,SCALE,SCALE }, vectorZero, vectorZero);
		transform_.Init("resource/model/marker/obakemarkerWall_blue.mv1", pos, { SCALE,SCALE,SCALE }, angle, vectorZero);
		break;
	case MarkerID::green:
		//transform_.Init("./resource/model/marker/greenMarker.mv1", pos, { SCALE,SCALE,SCALE }, vectorZero, vectorZero);
		transform_.Init("resource/model/marker/obakemarkerWall_green.mv1", pos, { SCALE,SCALE,SCALE }, angle, vectorZero);
		break;
	case MarkerID::yellow:
		//transform_.Init("./resource/model/marker/orangeMarker.mv1", pos, { SCALE,SCALE,SCALE }, vectorZero, vectorZero);
		transform_.Init("resource/model/marker/obakemarkerWall_yellow.mv1", pos, { SCALE,SCALE,SCALE }, angle, vectorZero);
		break;
	case MarkerID::cyan:
		//transform_.Init("./resource/model/marker/purpleMarker.mv1", pos, { SCALE,SCALE,SCALE }, vectorZero, vectorZero);
		transform_.Init("resource/model/marker/obakemarkerWall_cyan.mv1", pos, { SCALE,SCALE,SCALE }, angle, vectorZero);
		break;
	case MarkerID::max:
		break;
	default:
		break;
	}

	//transform_.quaRotLocal_ = Quaternion(VGet(angle.x, Utility::Deg2RadF(angle.y), angle.z));
	transform_.quaRot_ = Quaternion(VGet(angle.x, Deg2RadF(angle.y), angle.z));
	transform_.Update();
	lpSoundMng.Play3DSound("./resource/sound/WalkVoice.mp3", transform_.pos_, 3000.0f);
	int VS = LoadVertexShader("resource/shader/vs/NormalModel_vs.vso");
	int PS = LoadPixelShader("resource/shader/ps/GameModel_ps.pso");
	origShader_ = std::make_unique<OriginalShader>(VS, PS);

}

void ARmarker::Update(void)
{
	
	//‰¹‚ÌÀ•WÝ’è
	Set3DPositionSoundMem(transform_.pos_.toVECTOR(), lpSoundMng.GetID("./resource/sound/WalkVoice.mp3")[0]);
}

void ARmarker::Draw(void)
{
	origShader_->Draw(transform_.modelHandle_);
	//MV1DrawModel(transform_.modelHandle_);
	//DrawGraph(0, 100, lpImageMng.GetID("./resource/image/testmarker.jpg")[0], false);
	//DrawGraph(100, 600, lpImageMng.GetID("./resource/image/testmarker2.jpg")[0], false);
}

void ARmarker::Release(void)
{
}
