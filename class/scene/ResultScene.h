#pragma once
#include "BaseScene.h"
#include "../tmx/TmxObj.h"

class Controller;
class ResultUI;
class Camera;
class ResultCBuffer;
class OriginalShader;
enum class ControllerType;

class ResultScene :
    public BaseScene
{
public:
    ResultScene(ControllerType type);
    ~ResultScene();

    // ������
    void Init(void) override;

    // �X�V
    UniqueScene Update(UniqueScene scene) override;

    // �`��
    void DrawScreen(void) override;

    //�j��
    void Release(void) override;

    //�V�[����ID��Ԃ�
    SceneID GetSceneID(void) override { return SceneID::Result; };
private:

    //�L���[�u�}�b�v�̐���
    void CreateCubeMap(void);
    //�V�[���؂�ւ��֐�
    UniqueScene UpdateScene(UniqueScene& scene);

    //�R���g���[�����
    std::unique_ptr<Controller> controller_;
    //�R���g���[���^�C�v
    ControllerType contorollerType_;

    //�J�������
    std::unique_ptr<Camera> camera_;

    //�V�F�[�_
    std::unique_ptr<OriginalShader> origShader_;

    //UI
    std::unique_ptr<ResultUI> resultUI_;

    //�萔�o�b�t�@
    std::unique_ptr<ResultCBuffer> cBuffer_;

    //�萔�o�b�t�@�Ŏg�p����J�E���g
    float stepCount_;

    //�L���[�u�}�b�v�n���h��
    int cubeTexture_;

    //�X�J�C�h�[���̉�]
    float skyRotate_;
};
