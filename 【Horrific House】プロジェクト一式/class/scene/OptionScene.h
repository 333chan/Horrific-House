#pragma once
#include<memory>
#include "BaseScene.h"
#include "../tmx/TmxObj.h"

class OptionUI;

class Camera;
class GameCBuffer;
class OptionScene :
    public BaseScene
{
public:
    OptionScene(UniqueScene ownScene,SceneID beforeID);
    ~OptionScene();

    // ����������
    void Init(void) override;

    // �X�V�X�e�b�v
    UniqueScene Update(UniqueScene scene) override;

    // �`�揈��
    void DrawScreen(void) override;

    // �������
    void Release(void) override;

    //�V�[����ID��Ԃ�
    SceneID GetSceneID(void) override { return SceneID::Option; };
private:
    //�V�[���؂�ւ��֐�
    UniqueScene UpdateScene(UniqueScene& scene);

    UniqueScene beforeScene_;    //�O�̃V�[��(��ɃQ�[���V�[��)
   
    SceneID beforeSceneID_;      //�O�̃V�[��ID
    //UI
    std::unique_ptr<OptionUI> optionUI_;
    //�o�b�t�@
    std::unique_ptr<GameCBuffer> cBuffer_;
    //�J�������
    std::unique_ptr<Camera> camera_;
};