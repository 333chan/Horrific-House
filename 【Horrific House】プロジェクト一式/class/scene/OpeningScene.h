#pragma once
#include "BaseScene.h"

enum class CntType;
class OpeningUI;

class OpeningScene : 
	public BaseScene
{
public:
    OpeningScene();
    OpeningScene(CntType type);
    ~OpeningScene();

    // ����������
    void Init() override;

    // �X�V�X�e�b�v
    UniqueScene Update(UniqueScene scene) override;

    // �`�揈��
    void DrawScreen(void) override;

    // �������
    void Release(void) override;

    //�V�[����ID��Ԃ�
    SceneID GetSceneID(void) override { return SceneID::Opening; };

   
private:
    //SE�X�V
    void UpdateSE();
    //UI
    std::unique_ptr<OpeningUI> openingUI_;
    //�I�������R���g���[���[�^�C�v
    CntType cntType_;
};

