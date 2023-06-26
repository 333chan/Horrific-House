#pragma once
#include<memory>
#include "BaseScene.h"

class ConnectedUI;

class ConnectedScene :
    public BaseScene
{
public:
    ConnectedScene();
    ~ConnectedScene();

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
    std::unique_ptr<ConnectedUI> connectedUI_;

};
