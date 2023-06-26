#pragma once
#include "BaseScene.h"

class ExitUI;
class ExitScene :
    public BaseScene
{
public:
    ExitScene();
    ExitScene(SceneID sceneID);
    ExitScene(SceneID sceneID, UniqueScene ownScene, SceneID beforeID);

    ~ExitScene();

    // ����������
    void Init() override;

    // �X�V�X�e�b�v
    UniqueScene Update(UniqueScene scene) override;

    // �`�揈��
    void DrawScreen(void) override;

    // �������
    void Release(void) override;

    //�V�[����ID��Ԃ�
    SceneID GetSceneID(void) override { return SceneID::Exit; };

private:
    
    //�V�[���؂�ւ��֐�
    UniqueScene UpdateScene(UniqueScene& scene);
    SceneID sceneID_;           //��O�̃V�[��
    UniqueScene beforeScene_;    //�I�v�V�����V�[���̑O�̃V�[��
    SceneID beforeSceneID_;      //�I�v�V�����V�[���̑O�̃V�[��ID
    //UI
    std::unique_ptr<ExitUI>exitUI_;
};

