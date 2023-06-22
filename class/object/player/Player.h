#pragma once
#include "../Obj.h"
#include"../../common/Geometry.h"
#include"../../common/Transform.h"
#include"../../common/Capsule.h"

class Stamina;
class Controller;
class MinCamera;
struct GimmickObj;
class Gimmick;
struct ItemObj;
struct InventoryObj;
class Item;
enum class ItemID;
enum class ControllerType;
class OriginalShader;

enum class State
{
    Walk,   //����
    Fall,   //����
    Max
};

class Player :
    public Obj
{
public:
    Player(ControllerType type, const std::vector<std::shared_ptr<Gimmick>>& gList, const std::vector<std::shared_ptr<Item>>& iList);
    ~Player();

    //������
    void Init(void) override;
    //�j��
    void Release(void) override;
    //�X�V
    void Update(void) override;
    void Update(Vector3 cameraAngle,Vector3 targetPos);
    //�`��
    void Draw(void) override;
    void Draw(int screenID);
    //�ʒu���擾
    Vector3 GetPosition(void);
    bool GetminCameraFlag();
    bool GetIsGameClear();

    //�_�b�V�������Ă��邩�Ԃ�
    bool GetDashFlag(void);

    //�J�����̃����_�����O�^�[�Q�b�g�ݒ�
    void SetMinCameraRenderTarget(int screenID);

    //�t���b�V���̓����蔻��
    bool CheckHitFlash(Vector3 targetPos, Vector3 cameraAngle);

    //�J�����̎ʐ^����Ԃ�
    std::vector<int>& GetMinCameraPhotoVector(void);

    //�����蔻��p�̕ϐ���Ԃ�
    std::vector<Collider>& GetColliders(void);
    std::vector<InventoryObj>& GetInventoryList(void);
    Capsule GetCapsule(void);
    ItemID GetItemID(void);

    bool GetLowSensFlg(void);

private:
    //�ړ�����
    void MovePosition(Vector3 cameraAngle);
    //�΂߈ړ��̋�����������
    bool CheckDiagonalMove(void);

    bool HitLineX(float movePower, int& collision);
    bool HitLineZ(float movePower, int& collision);

    bool HitLineXGimmick(float movePower);
    bool HitLineZGimmick(float movePower);

    void SlideLinePosition(void);

    void HitCapsuleAndGoal(void);

    //�C���x���g���ւ̒ǉ�����
    void AddInventory(Vector3 targetPos);

    //�S��.�O������G(�J�n)---------------------------------------------------------------------------
    //�C���^���N�g����(�M�~�b�N)
    void InteractGimmick(Vector3 targetPos);
    
    //�M�~�b�N�̃C�x���g����
    void GimmickEvent(Vector3 targetPos);

    //�����o���ƃA�C�e�����̓������Ȃ���
    void ConnectingDrawerToItem(void);

    //�_�C�����J�E���g�X�V����
    void DialCountUpdate(const std::shared_ptr<Gimmick> &gimmick,int modelNum,char numchar);

    //�C���x���g���̒��g���m�F
    bool CheckInventory(const std::shared_ptr<Gimmick>& gimmick);
    //�S��.�O������G(�I��)---------------------------------------------------------------------------

    //�T�E���h����
    void Sound(void);

    //�ʒu
    Vector3 pos_;       //����
    Vector3 oldpos_;    //1�t���[���O

    //��]
    Quaternion quaRot_;

    VECTOR PlayerRot_;
    //���
    std::unique_ptr<Controller> controller_; //�R���g���[��   
    std::unique_ptr<Stamina> stamina_;       //�X�^�~�i

    bool pickUpFlag_;
    bool catchFlg_;
    bool lowSensFlg_;
    //�����蔻��p
    std::vector<Collider> colliders_;   //�}�b�v�֌W
    Capsule capsule_;  //�v���C���[�̃J�v�Z��
    std::unique_ptr<MinCamera> minCamera_;  //�v���C���[�̃J�v�Z��

    //���
    State state_;
    //�d��
    float fallGravity_;

    //�����蔻��p�n���h��
    int collisionHandle_;

    //���X�g
    std::vector<std::shared_ptr<Gimmick>> gimmickList_;
    std::vector<std::shared_ptr<Item>> itemList_;

    std::vector<InventoryObj> inventoryList_;
    //�_�C�����ԍ�
    int dial_[4];
    //�_�C�����ԍ�(������)
    std::string dialNumStr_="0000";
    bool dashFlag_;

    bool isGameClear_;
    bool isHitGoal_;

    //�v���C���[���擾���������
    ItemID itemID_;
};
