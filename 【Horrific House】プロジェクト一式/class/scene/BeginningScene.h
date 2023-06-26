#pragma once
#include <map>
#include "BaseScene.h"
enum class beginningType
{
    Non,
    Logo,
    Warning,
};


class BeginningUI;


class BeginningScene :
    public BaseScene
{
public:
    BeginningScene();
    BeginningScene(beginningType type);
    ~BeginningScene();

    // ����������
    void Init() override;

    // �X�V�X�e�b�v
    UniqueScene Update(UniqueScene scene) override;

    // �`�揈��
    void DrawScreen(void) override;

    // �������
    void Release(void) override;

    //�V�[����ID��Ԃ�
    SceneID GetSceneID(void) override { return SceneID::Beginning; };

private:
    //�`��֐��|�C���^�ɑ������֐�
    void UpdateLogo();
    void UpdateWarning();
    //�`��֐��|�C���^�ɑ������֐�
    void DrawLogo();         
    void DrawWarning();           
    //���̃V�[��
    beginningType afterScene_;

    //�X�V�֐��|�C���^
    void (BeginningScene::* updateFuncPtr_)();
    //�`��֐��|�C���^
    void (BeginningScene::* drawFuncPtr_)();
    //���̃^�C�v�ƍX�V�A�`��֐��|�C���^�̃}�b�v
    std::map<beginningType, std::pair<void (BeginningScene::*)(), void (BeginningScene::*)()>> beginningTypeMap_;
    //�o�ߎ���
    double elapsedTime_;
};
