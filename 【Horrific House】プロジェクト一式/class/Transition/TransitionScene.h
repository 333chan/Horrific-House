#pragma once
#include "../../class/Scene/BaseScene.h"

// 明示的にキャストする必要がある
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
    // 解放処理
    void Release(void) override final {};
protected:
    // 子供のクラスは触れるが外からは触れない
    SceneID GetSceneID(void) override final{ return SceneID::Transition; };

    UniqueScene beforScene_;
    UniqueScene afterScene_;

    double count;
};

