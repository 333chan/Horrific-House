#pragma once
#include<memory>
#include"BaseScene.h"

//class Stage;
class GameScene;
class SelectUI;

class SelectScene :
    public BaseScene
{
public:
    SelectScene();
    ~SelectScene();

    // ����������
    void Init() override;

    // �X�V�X�e�b�v
    UniqueScene Update(UniqueScene scene) override;

    // �`�揈��
    void DrawScreen(void) override;

    // �������
    void Release(void) override;

    //�V�[����ID��Ԃ�
    SceneID GetSceneID(void) override { return SceneID::Select; };

private:
    //�V�[���؂�ւ��֐�
    UniqueScene UpdateScene(UniqueScene& scene);
    //UI
    std::unique_ptr<SelectUI> selectUI_;

   

};
