#pragma once
#include<memory>
#include "BaseScene.h"
#include "../tmx/TmxObj.h"


class Controller;
class Camera;
class Enemy;
class OriginalShader;
enum class ControllerType;
class GameOverCBuffer;
class GameOverUI;
class AnimationController;
enum class CntType;
class GameOverScene :
    public BaseScene
{
public:
    GameOverScene(ControllerType type,Vector3 playerPos,Vector3 enemyPos,Vector3 cameraAngle, CntType cntType);
    ~GameOverScene();

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

    //UniqueScene beforeScene_;    //�O�̃V�[��(��ɃQ�[���V�[��)
    UniqueScene returningScene_; //�V�[����؂�ւ��鎞�p
   // SceneID beforeSceneID_;      //�O�̃V�[��ID

    //�R���g���[�����
    std::unique_ptr<Controller> controller_;
    //�G���
    std::unique_ptr<Enemy> enemy_;
    //�A�j���[�V�����R���g���[��
    std::unique_ptr<AnimationController> animController_;

    //�t�@�[�X�g��tmx�̕�����Z�J���h�͏����̃|�C���^�[
    std::map<std::string, void (GameOverScene::*)()> selectMap_;//�����܂Ƃ߃��X�g
    //std::map<std::string, std::string>* listToDraw_;        //�`�悷�郊�X�g�̃|�C���^
    // 
    //�t�@�[�X�g��tmx�̕�����Z�J���h�͉摜�̕�����
    std::map<std::string, std::string> displayMap_;        //�`�悾�����X�g
    std::map<std::string, std::string> toTitleList_;    //�^�C�g���ɍs�����X�g
    std::map<std::string, std::string> reStartList_;    //���X�^�[�g���X�g
    std::map<std::string, std::string> supportMap_;

    TmxObj tmxObj_;     //�Փ˔���ptmx
    std::string selection_; //���I�����Ă�����̂��i�[����ꏊ

    std::pair<std::string, std::string> OldhitMouse_;       //���}�E�X���������Ă�����̂��i�[����ꏊ
    std::pair<std::string, std::string> hitMouse_;       //���}�E�X���������Ă�����̂��i�[����ꏊ

    double Cnt_;       //���o�̃J�E���g
    int Volume_;    //����
    int brightness_;//���邳

    //�[�[�[�[�Q�[���I�[�o�[���̉��o�֘A�[�[�[�[
    std::unique_ptr<Camera> camera_;
    bool directionEndflg_;//���o�I���t���O
    Vector3 playerPos_;//�v���C���[�̍��W
    Vector3 enemyPos_;//�G�{�̂̍��W
    Vector3 ghostPos_;//���o�p�̓G�̍��W
    Vector3 dirCameraPos_;//���o�p�J�����̍��W
    Vector3 dirCameraTargetPos_;//���o�p�J�����̒����_���W
    Vector3 dirCameraPosOffset_;//�I�t�Z�b�g
    bool flag_;

    std::unique_ptr<GameOverCBuffer> cBuffer_;
    std::unique_ptr<GameOverUI> gameOverUI_;
    std::unique_ptr<OriginalShader> origShader_;//�X�e�[�W�`��p
    std::unique_ptr<OriginalShader> origShader2_;//�|�X�g�G�t�F�N�g�`��p

    int renderTarget_;        //�����_�����O�^�[�Q�b�g
    int renderTarget2_;        //�����_�����O�^�[�Q�b�g
    int effectRenderTarget_;
   
    void SetRenderTarget(int screenID);
    //�V�[���؂�ւ��֐�
    UniqueScene UpdateScene(UniqueScene& scene);
    //���o�̍X�V
    void UpdateDirection();
    //���o�̕`��
    void DrawDirection();
    //�w�肵������(�b)�҂�
    bool WaitCnt(double waitCnt);
    //�|�X�g�G�t�F�N�g�p
    std::unique_ptr<OriginalShader> postEffect_;
    int psPostEffect_;
    float postEffectShade_;

    CntType cntType_;
    
};

