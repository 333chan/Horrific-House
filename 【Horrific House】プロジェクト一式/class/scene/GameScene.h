#pragma once
#include<vector>
#include<chrono>
#include<string>
#include "BaseScene.h"
#include "../common/Geometry.h"
#include "../common/Light.h"

class Camera;
class Controller;
class Stage;
class Player;
class Enemy;

class CBufferMng;
class SocketServer;
class Goal;
class OriginalShader;
class GameCBuffer;
class GameUI;
enum class CntType;
class EffekseerEffectHandle;
class Effects;

class GameScene :
    public BaseScene
{
public:
    GameScene();
	GameScene(CntType type);
	~GameScene();

    // ����������
    void Init(void) override;

    // �X�V�X�e�b�v
    UniqueScene Update(UniqueScene scene) override;

    // �`�揈��
    void DrawScreen(void) override;

    // �������
    void Release(void) override;

    //�V�[����ID��Ԃ�
    SceneID GetSceneID(void) override { return SceneID::Game; };

private:
    //�R���g���[�����
    std::unique_ptr<Controller> controller_;

    //�X�e�[�W���
    std::unique_ptr<Stage> stage_;

    //�J�������
    std::unique_ptr<Camera> camera_;

    //�v���C���[���
    std::unique_ptr<Player> player_;

    //�G���
    std::unique_ptr<Enemy> enemy_;

    //�S�[�����
    std::unique_ptr<Goal> goal_;
    //�萔�o�b�t�@
    std::unique_ptr<GameCBuffer> cBuffer_;
    //�`��p�V�F�[�_�[
    std::unique_ptr<OriginalShader> origShader_;
    //�[�x�V�F�[�_�[
	std::unique_ptr<OriginalShader> depthShader_;
    //UI
    std::unique_ptr<GameUI> gameUI_;
    //�X�N���[��
    int screenCameraID_;
    int GaussScreenCameraID_;
	int DepthMapTextureID_;
   
    //�C���x���g���̔w�i�n���h��
    int inventoryBackGroundHandle_;

    //�|�X�g�G�t�F�N�g�p
    std::unique_ptr<OriginalShader> postEffect_;
    int psPostEffect_;
    float postEffectShade_;
    CntType cntType_;
    //�f�t�H���g���W
    Vector3 defPos_;
    //�f�t�H���g�^�[�Q�b�g���W
    Vector3 defTargetPos_;

    //--Effekseer--
    std::vector<EffekseerEffectHandle> effHandle_;//�G�t�F�N�g���\�[�X
    std::vector<Effects> effects_;                //�G�t�F�N�g�Ǘ��p

    //void AddEffect(std::string effecName);
    //----------

    //�G�t�F�N�g
    Effect dust_;
};
