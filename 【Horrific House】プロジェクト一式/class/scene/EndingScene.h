#pragma once
#include "BaseScene.h"
#include "../tmx/TmxObj.h"

enum class CntType;
class EndingUI;

class EndingScene :
    public BaseScene
{
public:
    EndingScene();
    EndingScene(CntType type);
    ~EndingScene();

    // ����������
    void Init() override;

    // �X�V�X�e�b�v
    UniqueScene Update(UniqueScene scene) override;

    // �`�揈��
    void DrawScreen(void) override;

    // �������
    void Release(void) override;

    //�V�[����ID��Ԃ�
    SceneID GetSceneID(void) override { return SceneID::Ending; };


private:
    void UpdateSE();
    //�V�[���؂�ւ��֐�
    UniqueScene UpdateScene(UniqueScene& scene);

    ////�J�������
    //std::unique_ptr<Camera> camera_;

    Vector3 pos_;
    std::string modleStr_;

    std::unique_ptr<EndingUI> endingUI_;
    CntType cntType_;
    std::unique_ptr<Controller> controller_;
};

