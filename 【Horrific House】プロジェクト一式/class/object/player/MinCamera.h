#pragma once
#include <memory>
#include<vector>
#include<array>
#include "../../common/Geometry.h"

class Controller;
class OriginalShader;
class MinCameraUI;
enum class ControllerType;

constexpr int MEMO_VERTS_NUM = 4;

enum class MoveState
{
    NONE,   //�B���Ă��Ȃ�
    STOP,   //�d����
    SHRINK, //�����ɏk��
    DOWN,   //���Ɉړ�
    MAX
};

class MinCamera
{
public:
    MinCamera(ControllerType type);
    ~MinCamera();

    //������
    void Init(void);
    void InitMemoPosition(void);
    //�X�V
    void Update(bool IsCamera);
    //�`��
    void Draw(void);

    bool GetIsOnCamera()const;
    void SetIsOnCamera(bool IsCamera);

    void MoveSub(float& pos ,float max , float value);
    void MoveAdd(float& pos, float max, float value);

    //�t���b�V�������������ǂ�������
    bool GetFlashFlag(void);

    //�����_�[�^�[�Q�b�g�Z�b�g
    void SetRenderTarget(int screenID);

    //�ʐ^�n���h���擾
    std::vector<int>& GetPhotoVector(void);
private:

    //�J�����̈ʒu�A��]�X�V
    void MoveCamera(void);

    //�t���b�V������
    void FlashCamera(void);

    //��������
    void MemoProcess(void);

    //�R���g���[�����
    std::unique_ptr<Controller> controller_;
    std::unique_ptr<MinCameraUI> minCameraUI_;  //�v���C���[�̃J�v�Z��

    //�V�F�[�_
    std::unique_ptr<OriginalShader> origShader_;

    //�J�������\���Ă��邩���ۂ̃t���O
    bool onCamera_;

    //�ʒu
    Vector3 pos_;
    //�p�x
    Vector3 angle_;

    bool flashOneFrameFlag_;//�������u�Ԃ̂݃t���O
    double stepWhiteTime_;  //��ʂ���������

    //�J������ʏ��
    std::unique_ptr<OriginalShader> cameraScreen_;   
    int psCameraHandle_;      //�s�N�Z���V�F�[�_ 
    int renderTarget_;        //�����_�����O�^�[�Q�b�g
    float whiteRange_;        //�J�����t���b�V���̔��������͈̔�
    float time_;              //����

    //�����ʐ^���
    std::unique_ptr<OriginalShader> memoScreen_;
    std::array<Vector3, MEMO_VERTS_NUM> memoPos_;//�e���_�̈ʒu
    std::vector<int> photoVector_;  //�ʐ^�̏��ۑ���
    int psMemoHandle_;              //�s�N�Z���V�F�[�_ 
    int photoCount_;                //�ʐ^���B������
    double stepMemoTime_;           //�B�e���̎ʐ^�d������
    double stepMemoLerpTime_;       //��Ԏ���
    double downAccel_;              //���������x
    MoveState moveState_;           //�ʐ^�̏��
    MoveState oldMoveState_;        //��O�̎ʐ^�̏��
};
