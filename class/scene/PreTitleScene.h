#pragma once
#include "BaseScene.h"


class PreTitleUI;

class PreTitleScene :
    public BaseScene
{
public:
    PreTitleScene();
    ~PreTitleScene();

    // ����������
    void Init() override;

    // �X�V�X�e�b�v
    UniqueScene Update(UniqueScene scene) override;

    // �`�揈��
    void DrawScreen(void) override;

    // �������
    void Release(void) override;

    //�V�[����ID��Ԃ�
    SceneID GetSceneID(void) override { return SceneID::PreTitle; };

private:
    //UI
    std::unique_ptr<PreTitleUI> preTitleUI_;
};

