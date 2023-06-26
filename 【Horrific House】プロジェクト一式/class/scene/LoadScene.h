#pragma once
#include "BaseScene.h"
enum class CntType;
class LoadUI;

class LoadScene :
    public BaseScene
{

public:
    LoadScene();
    LoadScene(CntType type);
    ~LoadScene();
    
    // ����������
    void Init(void) override;

    // �X�V�X�e�b�v
    UniqueScene Update(UniqueScene scene) override;

    // �`�揈��
    void DrawScreen(void) override;

    // �������
    void Release(void) override;

    //�V�[����ID��Ԃ�
    SceneID GetSceneID(void) override { return SceneID::Load; };

private:
    //UI
    std::unique_ptr<LoadUI> loadUI_;
    //�I�������R���g���[���[�^�C�v
    CntType cntType_;
};

