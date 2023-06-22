#pragma once
#include "BaseScene.h"
#include <vector>
#include "../tmx/TmxObj.h"
#include "../common/Geometry.h"
#include "../common/SocketServer.h"

class Controller;
class MouseIcon;
class Item;
class OriginalShader;
struct InventoryObj;
enum class ItemID;
enum class ControllerType;

constexpr int WEIGHTS_NUM = 8;
constexpr double  PHOTO_LERP_TIME = 0.15;	 //�ړ���Ԏ���

//�u���[�\����
struct BlurData
{
    int renderTarget = 1;   //�����_�����O�^�[�Q�b�g
    int vsHandle = -1;      //���_�V�F�[�_�n���h��
    Vector2 imageSize;      //�摜�T�C�Y
    std::unique_ptr<OriginalShader> origShader; //�`��p�V�F�[�_�N���X

    //������(make_unique��cpp���ŏ�����)
    void Init(Vector2Int screenSize,std::string vsPath)
    {
        float width, height;

        renderTarget = MakeScreen(screenSize.x, screenSize.y);
        vsHandle = LoadVertexShader(vsPath.c_str());
        GetGraphSizeF(renderTarget, &width, &height);
        imageSize = { width ,height };
    }
};

struct Photo
{
    int handle = -1;        //�摜�n���h��
    bool addFlag = false;   //�ǉ��t���O
    bool expansionFlag = false;
    Vector2 addPos;         //�J�[�\���������Ă���ꍇ�̒ǉ��ʒu
    double stepLerpTime = PHOTO_LERP_TIME;  //��Ԏ���

};

class InventoryScene :
    public BaseScene
{
public:
    InventoryScene(UniqueScene ownScene,int bgHandle,ControllerType type, const std::vector<InventoryObj>& inventoryList, const std::vector<int>& photo);
    ~InventoryScene();

    // ����������
    void Init(void) override;

    // �X�V�X�e�b�v
    UniqueScene Update(UniqueScene scene) override;
    // �`�揈��
    void DrawScreen(void) override;

    // �������
    void Release(void) override;

    //�V�[����ID��Ԃ�
    SceneID GetSceneID(void) override { return SceneID::Inventory; };

private:
    //�u���[�֌W������
    void InitBlur(void);

    //���_������
    void InitVerts(void);

    //�u���[�̏d�݌v�Z
    void CalcBlurWeights(void);

    //�ʐ^�ƃ}�E�X�̈ʒu�̓����蔻��
    void CollMousePosAndPhoto(void);

    //�ʐ^�`��
    void DrawPhoto(std::string name,Vector2 pos,Vector2 imageSize);

    //�C���x���g��������
    void InitInventory(void);
    //�C���x���g���X�V
    void UpdateInventory(void);
    //�C���x���g���`��
    void DrawInventory(void);
    //�e�[�u���̏�����
    void InitTable(void);

    //�R���g���[�����
    std::unique_ptr<Controller> controller_;

    //�t���[���o�b�t�@��w�i�`��p
    std::array<VERTEX2D, 4> totalBlurVerts_;
    
    //�e�[�u��
    std::map<std::string , std::string> inventoryTable_;    //�C���x���g���̊O�g�̃e�[�u��
    std::map<std::string, std::string> optionTable_;        //����̃e�[�u��
    std::map<std::string, Vector3> itemTable_;              //�A�C�e���̘g�e�[�u��
    std::map<ItemID, std::string> explanationTable_;        //�����̃e�[�u��

    // �A�C�e�����X�g
    std::vector<InventoryObj> inventoryList_;

    //�ʐ^���
    std::map<std::string,Photo> photoTbl_;         //�x�N�^�[
    std::unique_ptr<OriginalShader> photoOrigShader_; //�`��p�V�F�[�_�N���X
    std::unique_ptr<OriginalShader> origShader_;

    int psPhotoHandle_;                               //�s�N�Z���V�F�[�_

    //�u���[���
    BlurData blurX_;               //��
    BlurData blurY_;               //�c
    int psBlurXandYHandle_;        //�s�N�Z���V�F�[�_
    int mainRenderTarget_;         //���C�������_�����O�^�[�Q�b�g
    float weights_[WEIGHTS_NUM];   //�d��

    //�O�̃V�[��(��ɃQ�[���V�[��)
    UniqueScene beforeScene_;

    //�w�i�摜(�O�̃V�[���̉��)
    int backGroundHandle_;

    int font_;
    //tmx
    TmxObj tmxObj_;
    //��������ID
    ItemID hitItemID_;
    //�q���g
    bool isDrawTips_;

    int test_;
};
