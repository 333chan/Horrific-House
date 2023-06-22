#pragma once
#include "Obj.h"
//#include "gimmick/Gimmick.h"
#include "marker/ARmarker.h"
#include "../common/Geometry.h"

class Gimmick;
class DrawerFrame;
class Drawer;
class Door;
class LockDoor;
class InnerLockDoor;
class Item;
class Dial;
class DialFrame;
class DialLockDoor;
class HiddenText;
class FakeWall;
class Water;
class Book;

class OriginalShader;

struct ItemObj;
struct GimmickObj;
struct MarkerObj;

enum class ItemID;

class Stage :
    public Obj
{
public:
	Stage();
	~Stage();

	//������
	void Init(void) override;
	//�j��
	void Release(void) override;
	//�X�V
	void Update(void) override;
	//�`��
	void Draw(void) override;
	void DrawDepth(bool minCameraFlg);
	void Draw(bool minCameraFlg, int depthRT);

	/// <summary>
	/// �M�~�b�N���X�g��Ԃ�
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	std::vector<std::shared_ptr<Gimmick>> GetGimmickList(void);

	/// <summary>
	/// �A�C�e�����X�g��Ԃ�
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	std::vector<std::shared_ptr<Item>> GetItemList(void);


private:
	//�S��.�O������G(�J�n)---------------------------------------------------------------------------
	//�f�[�^�Ǎ������܂Ƃ�
	void LoadObjData(void);

	//�M�~�b�N���X�g�ǉ������܂Ƃ�
	void AddGimmick(void);
	//�A�C�e�����X�g�ǉ������܂Ƃ�
	void AddItem(void);
	/// <summary>
	/// string->wstring�^�ɕϊ�
	/// </summary>
	/// <param name="str"></param>
	/// <returns></returns>
	std::wstring ConvertToWstring(std::string& str);


	/// <summary>
	/// �o�C�i���f�[�^�̂��ׂĂ�ǂݍ���
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="�i�[���������X�g"></param>
	/// <param name="�o�C�i���f�[�^��"></param>
	template<typename T>
	void LoadBinData(std::vector<T>& gList, const char* bData);

	/// <summary>
	/// �o�C�i���f�[�^�̐e�I�u�W�F�N�g�ȊO��ǂݍ���
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="�i�[���������X�g"></param>
	/// <param name="�o�C�i���f�[�^��"></param>
	/// <param name="�e�I�u�W�F�N�g��"></param>
	template<typename T>
	void LoadBinData(std::vector<T>& gList, const char* bData,std::string pName);


	/// <summary>
	/// �ړ��A�C�e���p�f�[�^�Ǎ�
	/// </summary>
	/// <param name="�A�C�e�����X�g"></param>
	/// <param name="�z�u�f�[�^"></param>
	/// <param name="�ړ���̔z�u�f�[�^"></param>
	/// <param name="�e�I�u�W�F�N�g��"></param>
	void LoadBinData(std::vector<ItemObj>& iList, const char* bData1, const char* bData2, std::string pName);

	/// <summary>
	/// ���ׂẴf�[�^���M�~�b�N���X�g�ǉ�
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="�M�~�b�N���X�g"></param>
	template<typename T>
	void AddGimmickList(std::vector<GimmickObj>& gList);

	/// <summary>
	/// �w�肵���I�u�W�F�N�g���̃f�[�^�̂݃M�~�b�N���X�g�ǉ�
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="�M�~�b�N���X�g"></param>
	/// <param name="�I�u�W�F�N�g��"></param>
	template<typename T>
	void AddGimmickList(std::vector<GimmickObj>& gList, std::wstring objName);

	/// <summary>
	/// �M�~�b�N���X�g�ǉ�
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="�M�~�b�N���X�g"></param>
	/// <param name="�I�u�W�F�N�g"></param>
	/// <param name="�Ή����������A�C�e��ID"></param>
	template<typename T>
	void AddGimmickList(std::vector<GimmickObj>& gList, std::wstring objName, ItemID itemID);

	/// <summary>
	/// �M�~�b�N���X�g�ǉ�
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="�M�~�b�N���X�g"></param>
	/// <param name="�I�u�W�F�N�g"></param>
	/// <param name="�w�肵�����@���̒l"></param>
	template<typename T>
	void AddGimmickList(std::vector<GimmickObj>& gList, std::wstring objName, float normal);

	/// <summary>
	/// �M�~�b�N���X�g�ǉ�
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="�M�~�b�N���X�g"></param>
	/// <param name="�I�u�W�F�N�g"></param>
	/// <param name="�_�C�A���̉����ԍ�"></param>
	template<typename T>
	void AddGimmickList(std::vector<GimmickObj>& gList, std::wstring objName, std::string unlockNumber);

	/// <summary>
	/// �}�[�J�[���X�g�ɒǉ�
	/// </summary>
	/// <param name="�}�[�J�[���X�g"></param>
	/// <param name="�I�u�W�F�N�g��"></param>
	void AddMarkerList(std::vector<MarkerObj>& gList, std::wstring objName);
	
	/// <summary>
	/// �A�C�e�����X�g�̒ǉ�
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="�A�C�e�����X�g"></param>
	template<typename T>
	void AddItemList(std::vector<ItemObj>& iList);

	//�M�~�b�N
	//���ׂẴf�[�^�i�[�p���X�g
	std::vector<GimmickObj> gimmickObjList_;

	//�����o�����X�g
	std::vector<GimmickObj> drawerObjList_;

	//�����o���g���X�g
	std::vector<GimmickObj> drawerFrameObjList_;

	//�W���[�N�{�b�N�X���X�g
	std::vector<GimmickObj> jukeBoxObjList_;

	//�g�p����f�[�^�݂̂��i�[���郊�X�g
	std::vector<std::shared_ptr<Gimmick>> gimmickList_;

	//�}�[�J�[
	//���ׂẴf�[�^�i�[�p���X�g
	std::vector<MarkerObj> markerObjList_;
	//�g�p����f�[�^�݂̂��i�[���郊�X�g
	std::vector<std::shared_ptr<ARmarker>> markerList_;

	//�A�C�e�����X�g
	//��
	std::vector<ItemObj> keyObjList_;

	//�ʐ^
	std::vector<ItemObj> photObjList_;

	//����
	std::vector<ItemObj> memoObjList_;

	//�{
	std::vector<ItemObj> bookObjList_;

	//�g�p����f�[�^�݂̂��i�[���郊�X�g
	std::vector<std::shared_ptr<Item>> itemList_;
	//�S��.�O������G(�I��)---------------------------------------------------------------------------

	//�I���W�i���V�F�[�_�[
	std::unique_ptr<OriginalShader> origShader_;
	std::unique_ptr<OriginalShader> depthShader_;

	//���f���n���h��(stage)
	int stageHandle_;


};

