#include<DxLib.h>
#include "../common/manager/ModelMng.h"
#include "../common/manager/ImageMng.h"
#include "../common/OriginalShader.h"
#include "Stage.h"
#include "gimmick/Gimmick.h"
#include "gimmick/Door.h"
#include "gimmick/LockDoor.h"
#include "gimmick/DrawerFrame.h"
#include "gimmick/Drawer.h"
#include "gimmick/InnerLockDoor.h"
#include "gimmick/Dial.h"
#include "gimmick/DialFrame.h"
#include "gimmick/DialLockDoor.h"
#include "gimmick/FakeWall.h"
#include "gimmick/HiddenText.h"
#include "gimmick/Piano.h"
#include "gimmick/JukeBox.h"
#include "gimmick/Water.h"
#include "item/Item.h"
#include "item/Phot.h"
#include "item/Key.h"
#include "Item/Memo.h"
#include "Item/Book.h"

constexpr float NORMAL = 1.0f;
constexpr float SIZE_SCALE_X = 32.5f;
constexpr float SIZE_SCALE_Y = 85.0f;
constexpr float SIZE_SCALE_Z = 10.0f;
constexpr int VERSION = 6;

Stage::Stage()
{
	lpModelMng.GetID("resource/model/stage/Stage1.mv1");
	lpModelMng.GetID("resource/model/stage/Window Large.mv1");
	lpModelMng.GetID("resource/model/stage/Window Small.mv1");
	//stageHandle_ = MV1LoadModel("resource/model/stage/map.mv1");
	Init();
}

Stage::~Stage()
{
	Release();
}

void Stage::Init(void)
{
	//�S��.�O������G(�J�n)---------------------------------------------------------------------------
	//�o�C�i���f�[�^��ǂݍ��ݏ����܂Ƃ�
	LoadObjData();

	//�g�p����f�[�^�����M�~�b�N���X�g�֒ǉ�
	AddGimmick();

	//�A�C�e�����X�g�֒ǉ�
	AddItem();
	//�S��.�O������G(�I��)---------------------------------------------------------------------------

	//�X�e�[�W�z�u
	MV1SetPosition(lpModelMng.GetID("resource/model/stage/Stage1.mv1")[0], VGet(0, 0, 0));
	MV1SetPosition(lpModelMng.GetID("resource/model/stage/Window Large.mv1")[0], VGet(-6000, 0, -18000));
	MV1SetPosition(lpModelMng.GetID("resource/model/stage/Window Small.mv1")[0], VGet(-200, 0, -15005));

	MV1SetScale(lpModelMng.GetID("resource/model/stage/Window Large.mv1")[0], VGet(SIZE_SCALE_X, SIZE_SCALE_Y, SIZE_SCALE_Z));
	MV1SetScale(lpModelMng.GetID("resource/model/stage/Window Small.mv1")[0], VGet(SIZE_SCALE_X-2.5, SIZE_SCALE_Y, SIZE_SCALE_Z));
	MV1SetRotationXYZ(lpModelMng.GetID("resource/model/stage/Window Small.mv1")[0], VGet( 0, Deg2RadF(90), 0));

	//���C�e�B���O
	int VertexShader = LoadVertexShader("resource/shader/vs/NormalModel_vs.vso");
	int PixelShader = LoadPixelShader ("resource/shader/ps/GameModel_ps.pso");
	origShader_ = std::make_unique<OriginalShader>(VertexShader, PixelShader);
	//�f�v�X
	int VertexShader2 = LoadVertexShader("resource/shader/vs/NormalModel_vs.vso");
	int PixelShader2 = LoadPixelShader("resource/shader/ps/DrawShadowMap_ps.pso");
	depthShader_ = std::make_unique<OriginalShader>(VertexShader2, PixelShader2);

	MV1SetPosition(lpModelMng.GetID("resource/model/player/player_robot_new.mv1")[0],VGet(20600, 3950,-10450));
	MV1SetRotationXYZ(lpModelMng.GetID("resource/model/player/player_robot_new.mv1")[0], VGet(0, Deg2RadF(110), Deg2RadF(-70)));
}

void Stage::Release(void)
{
	//MV1DeleteModel(stageHandle_);

	//�S��.�O������G(�J�n)---------------------------------------------------------------------------
	//�M�~�b�N���
	for (auto& gimmick : gimmickList_)
	{
		gimmick->Release();
	}

	//�A�C�e�����
	for (auto& item : itemList_)
	{
		item->Release();
	}
	//�S��.�O������G(�I��)---------------------------------------------------------------------------
}

void Stage::Update(void)
{
	//�M�~�b�N�X�V
	for (auto& gimmick : gimmickList_)
	{
		gimmick->Update();
	}

	//�A�C�e���X�V
	for (auto& item : itemList_)
	{
		item->Update();
	}
}

void Stage::Draw(void)
{

}

void Stage::DrawDepth(bool minCameraFlg)
{
	depthShader_->Draw(lpModelMng.GetID("resource/model/stage/Stage1.mv1")[0]);
}

void Stage::Draw(bool minCameraFlg, int depthRT)
{
	//�X�e�[�W�`��
	origShader_->Draw(lpModelMng.GetID("resource/model/stage/Stage1.mv1")[0], depthRT);
	origShader_->Draw(lpModelMng.GetID("resource/model/stage/Window Large.mv1")[0]);
	origShader_->Draw(lpModelMng.GetID("resource/model/stage/Window Small.mv1")[0]);

	//�S��.�O������G(�J�n)---------------------------------------------------------------------------
	//�M�~�b�N�`��
	for (auto& gimmick : gimmickList_)
	{
		gimmick->Draw();
	}

	//�A�C�e���`��
	for (auto& item : itemList_)
	{
		item->Draw(minCameraFlg);
	}
	//�S��.�O������G(�I��)---------------------------------------------------------------------------

	origShader_->Draw(lpModelMng.GetID("resource/model/player/player_robot_new.mv1")[0], depthRT);
}

std::vector<std::shared_ptr<Gimmick>> Stage::GetGimmickList(void)
{
	return gimmickList_;
}

std::vector<std::shared_ptr<Item>> Stage::GetItemList(void)
{
	return itemList_;
}

void Stage::LoadObjData(void)
{
	//�o�C�i���f�[�^��ǂݍ���
	//�M�~�b�N
	LoadBinData<GimmickObj>(gimmickObjList_, "./resource/data/gimmick/Door.dat");
	LoadBinData<GimmickObj>(gimmickObjList_, "./resource/data/gimmick/Dial.dat");
	LoadBinData<GimmickObj>(gimmickObjList_, "./resource/data/gimmick/Wall.dat");
	LoadBinData<GimmickObj>(gimmickObjList_, "./resource/data/gimmick/TextObj.dat");
	LoadBinData<GimmickObj>(gimmickObjList_, "./resource/data/gimmick/Water.dat");
	LoadBinData<GimmickObj>(drawerObjList_, "./resource/data/gimmick/Drawer.dat", "DrawerObj");
	LoadBinData<GimmickObj>(drawerFrameObjList_, "./resource/data/gimmick/DrawerFrame.dat", "DrawerFrameObj");
	LoadBinData<GimmickObj>(jukeBoxObjList_, "./resource/data/gimmick/JukeBox.dat","JukeBoxObj");
	//�A�C�e��
	LoadBinData(keyObjList_, "./resource/data/gimmick/Key.dat", "./resource/data/gimmick/KeyMove.dat", "KeyObj");
	LoadBinData(photObjList_, "./resource/data/gimmick/Phot.dat", "./resource/data/gimmick/Phot.dat", "PhotObj");
	LoadBinData(memoObjList_, "./resource/data/gimmick/Memo.dat", "./resource/data/gimmick/MemoMove.dat", "MemoObj");
	LoadBinData(bookObjList_, "./resource/data/gimmick/Book.dat", "./resource/data/gimmick/BookMove.dat", "BookObj");
	
}

void Stage::AddGimmick(void)
{
	//�g�p����f�[�^�����M�~�b�N���X�g�֒ǉ�
	//��
	AddGimmickList<FakeWall>(gimmickObjList_, L"Gimmick_Wall",ItemID::Phot);

	//�_�C�A����
	AddGimmickList<DialLockDoor>(gimmickObjList_, L"Red_B","0315");

	//��
	AddGimmickList<Water>(gimmickObjList_, L"water");

	//�W���[�N�{�b�N�X
	AddGimmickList<JukeBox>(jukeBoxObjList_);

	//�B��Ă��镶��
	AddGimmickList<HiddenText>(gimmickObjList_,L"firstHint");
	AddGimmickList<HiddenText>(gimmickObjList_,L"secondHint");

	//�_�C�A��
	AddGimmickList<Dial>(gimmickObjList_, L"Dial");
	AddGimmickList<DialFrame>(gimmickObjList_, L"Dial_Frame");

	//�����o��
	AddGimmickList<DrawerFrame>(drawerFrameObjList_);
	AddGimmickList<Drawer>(drawerObjList_);

	//��
	AddGimmickList<Door>(gimmickObjList_, L"Dark_A");
	AddGimmickList<Door>(gimmickObjList_, L"Dark_A_Double");

	//����̌������炵���J�����Ȃ���
	AddGimmickList<InnerLockDoor>(gimmickObjList_, L"White_B_Double",-NORMAL);
	AddGimmickList<InnerLockDoor>(gimmickObjList_, L"White_B",NORMAL);
	AddGimmickList<InnerLockDoor>(gimmickObjList_, L"White_C",-NORMAL);

	//���t����
	AddGimmickList<LockDoor>(gimmickObjList_, L"White_A_Double",ItemID::Kitchen_Key);
	AddGimmickList<LockDoor>(gimmickObjList_, L"Red_A",ItemID::FoodBank_Key);
	AddGimmickList<LockDoor>(gimmickObjList_, L"Dark_B_Double",ItemID::MusicRoom_Key);
	AddGimmickList<LockDoor>(gimmickObjList_, L"Dark_C",ItemID::ChildRoom_Key);
	AddGimmickList<LockDoor>(gimmickObjList_, L"Red_C",ItemID::Celler_Key);
	AddGimmickList<LockDoor>(gimmickObjList_, L"White_C_Double",ItemID::Max);


	gimmickList_.emplace_back(std::make_unique<Piano>());
	//gimmickList_.emplace_back(std::make_unique<JukeBox>());

}

void Stage::AddItem(void)
{
	//��
	AddItemList<Key>(keyObjList_);
	//�ʐ^
	AddItemList<Phot>(photObjList_);
	//�{
	AddItemList<Book>(bookObjList_);
	//����
	AddItemList<Memo>(memoObjList_);
}

template<typename T>
void Stage::LoadBinData(std::vector<T>& gList, const char* bData)
{
	int file = FileRead_open(bData);
	int version;
	FileRead_read(&version, sizeof(version), file);
	if (version == VERSION)
	{
		while (!FileRead_eof(file))
		{
			std::string name;	//���O
			Vector3 pos;	//���W
			Vector3 rot;	//��]
			Vector3 scale;	//��]
			char nameSize;
			//�n���h���A���O�A���W�A��]���ǂݎ��
			FileRead_read(&nameSize, sizeof(nameSize), file);
			name.resize(static_cast<int>(nameSize));
			FileRead_read(&name[0], nameSize, file);
			FileRead_read(&pos, sizeof(pos), file);
			FileRead_read(&rot, sizeof(rot), file);
			FileRead_read(&scale, sizeof(scale), file);
			//���O�A���W�A��]�����i�[
			gList.emplace_back(name, pos, rot, scale);
		}
	}
	//�t�@�C�������
	FileRead_close(file);
}

template<typename T>
void Stage::LoadBinData(std::vector<T>& gList, const char* bData, std::string pName)
{
	int file = FileRead_open(bData);
	int version;
	FileRead_read(&version, sizeof(version), file);
	if (version == VERSION)
	{
		while (!FileRead_eof(file))
		{
			std::string name;	//���O
			Vector3 pos;		//���W
			Vector3 rot;		//��]
			Vector3 scale;		//�g��l
			char nameSize;
			//�n���h���A���O�A���W�A��]���ǂݎ��
			FileRead_read(&nameSize, sizeof(nameSize), file);
			name.resize(static_cast<int>(nameSize));
			FileRead_read(&name[0], nameSize, file);
			FileRead_read(&pos, sizeof(pos), file);
			FileRead_read(&rot, sizeof(rot), file);
			FileRead_read(&scale, sizeof(scale), file);
			//�e�I�u�W�F�N�g�̏��͕K�v�Ȃ��̂Ŏ���
			if (pName == name)
			{
				continue;
			}
			//���O�A���W�A��]�A�g��l�����i�[
			gList.emplace_back(name, pos, rot, scale);
		}
	}
	//�t�@�C�������
	FileRead_close(file);
}

void Stage::LoadBinData(std::vector<ItemObj>& iList, const char* bData1, const char* bData2, std::string pName)
{
	int file1 = FileRead_open(bData1);
	int file2 = FileRead_open(bData2);
	int version;
	FileRead_read(&version, sizeof(version), file1);
	FileRead_read(&version, sizeof(version), file2);
	if (version == VERSION)
	{
		while (!FileRead_eof(file1) && !FileRead_eof(file2))
		{
			std::string name;	//���O
			Vector3 bPos;	//���W
			Vector3 rot;	//��]
			Vector3 scale;	//��]
			char nameSize;

			std::string name2;	//���O
			char nameSize2;
			Vector3 aPos;	//���W
			Vector3 rot2;	//��]
			Vector3 scale2;	//��]
			//�n���h���A���O�A���W�A��]���ǂݎ��
			FileRead_read(&nameSize, sizeof(nameSize), file1);
			name.resize(static_cast<int>(nameSize));
			FileRead_read(&name[0], nameSize, file1);
			FileRead_read(&bPos, sizeof(bPos), file1);
			FileRead_read(&rot, sizeof(rot), file1);
			FileRead_read(&scale, sizeof(scale), file1);

			FileRead_read(&nameSize2, sizeof(nameSize2), file2);
			name2.resize(static_cast<int>(nameSize2));
			FileRead_read(&name2[0], nameSize2, file2);
			FileRead_read(&aPos, sizeof(aPos), file2);
			FileRead_read(&rot2, sizeof(rot2), file2);
			FileRead_read(&scale2, sizeof(scale2), file2);
			//�e�I�u�W�F�N�g�̏��͕K�v�Ȃ��̂Ŏ���
			if (pName == name)
			{
				continue;
			}
			//���O�A���W�A��]�����i�[
			iList.emplace_back(name, bPos, aPos, rot, scale);
		}
	}
	//�t�@�C�������
	FileRead_close(file1);
	FileRead_close(file2);
}

std::wstring Stage::ConvertToWstring(std::string& str)
{
	auto cnt = MultiByteToWideChar(
		CP_UTF8,
		0,
		str.c_str(),
		str.length(),
		nullptr,
		0
	);

	std::wstring wstr;
	wstr.resize(cnt);

	MultiByteToWideChar(
		CP_UTF8,
		0,
		str.c_str(),
		str.length(),
		&wstr[0],
		wstr.length()
	);

	return wstr;
}


template<typename T>
void Stage::AddGimmickList(std::vector<GimmickObj>& gList)
{
	int i = 0;
	for (auto& gimmick : gList)
	{
		//�M�~�b�N���X�g�ɒǉ�&����(�I�u�W�F�N�g���A���W�A�p�x�A�g��l�A������)
		gimmickList_.emplace_back(std::make_unique<T>(gimmick.objName_, gimmick.pos_, gimmick.angle_, gimmick.scale_, i));
		i++;
	}
}

//�M�~�b�N���X�g�֒ǉ�
template<typename T>
void Stage::AddGimmickList(std::vector<GimmickObj>& gList, std::wstring objName)
{
	std::wstring wstr;
	int i = 0;
	for (auto& gimmick : gList)
	{
		wstr = ConvertToWstring(gimmick.objName_);
		//�o�C�i���f�[�^�̃I�u�W�F�N�g���Ǝw�肵�����O����v���Ă���Ȃ�
		if (wstr == objName)
		{
			//�M�~�b�N���X�g�ɒǉ�&����(�I�u�W�F�N�g���A���W�A�p�x�A�g��l�A������)
			gimmickList_.emplace_back(std::make_unique<T>(gimmick.objName_,gimmick.pos_,gimmick.angle_,gimmick.scale_,i));
			i++;
		}
		else
		{
			//������玟��
			continue;
		}
	}
}

template<typename T>
void Stage::AddGimmickList(std::vector<GimmickObj>& gList, std::wstring objName,ItemID itemID)
{
	std::wstring wstr;
	int i = 0;
	for (auto& gimmick : gList)
	{
		wstr = ConvertToWstring(gimmick.objName_);
		//�o�C�i���f�[�^�̃I�u�W�F�N�g���Ǝw�肵�����O����v���Ă���Ȃ�
		if (wstr == objName)
		{
			//�M�~�b�N���X�g�ɒǉ�
			gimmickList_.emplace_back(std::make_unique<T>(gimmick.objName_, gimmick.pos_, gimmick.angle_, gimmick.scale_, i,itemID));
			i++;
		}
		else
		{
			//������玟��
			continue;
		}
	}
}
template<typename T>
void Stage::AddGimmickList(std::vector<GimmickObj>& gList, std::wstring objName, float normal)
{
	std::wstring wstr;
	int i = 0;
	for (auto& gimmick : gList)
	{
		wstr = ConvertToWstring(gimmick.objName_);
		//�o�C�i���f�[�^�̃I�u�W�F�N�g���Ǝw�肵�����O����v���Ă���Ȃ�
		if (wstr == objName)
		{
			//�M�~�b�N���X�g�ɒǉ�
			gimmickList_.emplace_back(std::make_unique<T>(gimmick.objName_, gimmick.pos_, gimmick.angle_, gimmick.scale_, i, normal));
			i++;
		}
		else
		{
			//������玟��
			continue;
		}
	}
}
template<typename T>
void Stage::AddGimmickList(std::vector<GimmickObj>& gList, std::wstring objName, std::string unlockNumber)
{
	std::wstring wstr;
	int i = 0;
	for (auto& gimmick : gList)
	{
		wstr = ConvertToWstring(gimmick.objName_);
		//�o�C�i���f�[�^�̃I�u�W�F�N�g���Ǝw�肵�����O����v���Ă���Ȃ�
		if (wstr == objName)
		{
			//�M�~�b�N���X�g�ɒǉ�
			gimmickList_.emplace_back(std::make_unique<T>(gimmick.objName_, gimmick.pos_, gimmick.angle_, gimmick.scale_, i,unlockNumber));
			i++;
		}
		else
		{
			//������玟��
			continue;
		}
	}
}

template<typename T>
void Stage::AddItemList(std::vector<ItemObj>& iList)
{
	int id=0;
	int num = 0;
	std::wstring wstr;
	for (auto& item : iList)
	{
		wstr = ConvertToWstring(item.objName_);
		//�o�C�i���f�[�^�̃I�u�W�F�N�g���Ǝw�肵�����O����v���Ă���Ȃ�
		if (wstr == L"Entrance_Key")
		{
			id = static_cast<int>(ItemID::Entrance_Key);
		}
		if (wstr == L"storyMemo_B")
		{
			id = static_cast<int>(ItemID::Story_Memo_B);
		}
		if (wstr == L"storyBook_A")
		{
			id = static_cast<int>(ItemID::Story_Book_A);
		}
		if (wstr == L"map")
		{
			id = static_cast<int>(ItemID::Mapitem);
			num = 0;
		}
		itemList_.emplace_back(std::make_unique<T>(item.objName_,item.homePos_,item.movedPos_, item.angle_, item.scale_,static_cast<ItemID>(id),num));
		id++;
		num++;
	}
}

void Stage::AddMarkerList(std::vector<MarkerObj>& gList, std::wstring objName)
{
	int i = 0;
	std::wstring wstr;
	for (auto& marker : markerObjList_)
	{
		wstr = ConvertToWstring(marker.objName_);
		//�o�C�i���f�[�^�̃I�u�W�F�N�g���Ǝw�肵�����O����v���Ă���Ȃ�
		if (wstr == objName)
		{
			//����
			continue;
		}
		else
		{
			//�A�C�e�����X�g�ɒǉ�
			markerList_.emplace_back(std::make_unique<ARmarker>(marker.pos_,marker.angle_,static_cast<MarkerID>(i)));
			i++;
		}
	}
}
