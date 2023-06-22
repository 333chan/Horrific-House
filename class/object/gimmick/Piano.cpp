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
    //�C���^���N�g�t���O
    interactFlg_ = false;
    //�_�C�A���i���o�[
    dialNumber_ = 0;
    //���b�N�t���O
    isLock_ = false;
    //�����蔻��t���O
    hitFlg_ = false;
    //�����ԍ�
    unlockNum_ = "0000";
    //�Ή����������A�C�e��ID
    itemID_ = ItemID(0);
    //�����蔻��@��
    hitNormal_ = { 0.0f,0.0f,0.0f };
    //�A�j���[�V�������
    animState_ = GIMMICK_ANIM::NONE;
    //�M�~�b�N�̎��
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
