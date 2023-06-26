#include "../../common/manager/SoundMng.h"
#include "../../common/manager/TutorialMng.h"
#include "Piano.h"

Piano::Piano()
{
    Init();
}

Piano::~Piano()
{
    //lpSoundMng.StopSound("./resource/sound/test.wav");
    //StopSoundMem(lpSoundMng.GetID("./resource/sound/test.wav")[0]);
}

void Piano::Init(void)
{
    //インタラクトフラグ
    interactFlg_ = false;
    //ダイアルナンバー
    dialNumber_ = 0;
    //ロックフラグ
    isLock_ = false;
    //当たり判定フラグ
    hitFlg_ = false;
    //解除番号
    unlockNum_ = "0000";
    //対応させたいアイテムID
    itemID_ = ItemID(0);
    //当たり判定法線
    hitNormal_ = { 0.0f,0.0f,0.0f };
    //アニメーション状態
    animState_ = GIMMICK_ANIM::NONE;
    //ギミックの種類
    type_ = GIMMICK_TYPE::NONE;

    pos_ = {-12300,5100,-9000};
   
}

void Piano::Update(void)
{
    if (lpTutorialMng.GetTutorialFlg(TutorialProgress::Inventry))
    {
        lpSoundMng.Play3DSound("./resource/sound/pianoBGM2.mp3", pos_, 15000.0f);
    }
    Set3DPositionSoundMem(pos_.toVECTOR(), lpSoundMng.GetID("./resource/sound/pianoBGM2.mp3")[0]);
}

void Piano::Draw(void)
{
}

void Piano::Release(void)
{
    //StopSoundMem(lpSoundMng.GetID("./resource/sound/pianoBGM2.mp3")[0]);
}

int Piano::GetModelNum(void)
{
    return 0;
}

std::string Piano::GetCollModelStr(void)
{
    return std::string();
}
