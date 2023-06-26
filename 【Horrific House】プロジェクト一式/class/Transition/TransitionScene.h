#pragma once
#include "../../class/Scene/BaseScene.h"

// �����I�ɃL���X�g����K�v������
enum class Transition
{
    CrossFade,
    FadeInOut,
    In,
    Out,
    Max
};


class TransitionScene :
    public BaseScene
{
public:
    TransitionScene(UniqueScene beforScene, UniqueScene afterScene);
    TransitionScene(UniqueScene afterScene);
    ~TransitionScene();
private:
    void Init(void) override;
    UniqueScene Update(UniqueScene ownScene)override;
    virtual bool UpdateTransition() = 0;
    // �������
    void Release(void) override final {};
protected:
    // �q���̃N���X�͐G��邪�O����͐G��Ȃ�
    SceneID GetSceneID(void) override final{ return SceneID::Transition; };

    UniqueScene beforScene_;
    UniqueScene afterScene_;

    double count;
};

