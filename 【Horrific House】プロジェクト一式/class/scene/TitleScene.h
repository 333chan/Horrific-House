#pragma once
#include <string>
#include "BaseScene.h"
#include "../common/Geometry.h"

class Camera;
class OriginalShader;
class TitleCBuffer;
class TitleUI;

class TitleScene :
    public BaseScene
{
public:
    TitleScene();
    ~TitleScene();

    // ����������
    void Init() override;

    // �X�V�X�e�b�v
    UniqueScene Update(UniqueScene scene) override;

    // �`�揈��
    void DrawScreen(void) override;

    // �������
    void Release(void) override;

    //�V�[����ID��Ԃ�
    SceneID GetSceneID(void) override { return SceneID::Title; };

private:
    //�V�[���؂�ւ��֐�
    UniqueScene UpdateScene(UniqueScene& scene);

    //�J�������
    std::unique_ptr<Camera> camera_;
    //���f���̍��W
    Vector3 pos_;
    //���f���̕�����
    std::string modleStr_;
    //�V�F�[�_�[�`��p
    std::unique_ptr<OriginalShader> origShader_;
    //�^�C�g���̒萔�o�b�t�@
    std::unique_ptr<TitleCBuffer> cBuffer_;
    //�^�C�g����UI
    std::unique_ptr<TitleUI> titleUI_;

};
