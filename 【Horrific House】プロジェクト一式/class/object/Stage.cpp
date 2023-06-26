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
	//担当.三ヶ島滉季(開始)---------------------------------------------------------------------------
	//バイナリデータを読み込み処理まとめ
	LoadObjData();

	//使用するデータだけギミックリストへ追加
	AddGimmick();

	//アイテムリストへ追加
	AddItem();
	//担当.三ヶ島滉季(終了)---------------------------------------------------------------------------

	//ステージ配置
	MV1SetPosition(lpModelMng.GetID("resource/model/stage/Stage1.mv1")[0], VGet(0, 0, 0));
	MV1SetPosition(lpModelMng.GetID("resource/model/stage/Window Large.mv1")[0], VGet(-6000, 0, -18000));
	MV1SetPosition(lpModelMng.GetID("resource/model/stage/Window Small.mv1")[0], VGet(-200, 0, -15005));

	MV1SetScale(lpModelMng.GetID("resource/model/stage/Window Large.mv1")[0], VGet(SIZE_SCALE_X, SIZE_SCALE_Y, SIZE_SCALE_Z));
	MV1SetScale(lpModelMng.GetID("resource/model/stage/Window Small.mv1")[0], VGet(SIZE_SCALE_X-2.5, SIZE_SCALE_Y, SIZE_SCALE_Z));
	MV1SetRotationXYZ(lpModelMng.GetID("resource/model/stage/Window Small.mv1")[0], VGet( 0, Deg2RadF(90), 0));

	//ライティング
	int VertexShader = LoadVertexShader("resource/shader/vs/NormalModel_vs.vso");
	int PixelShader = LoadPixelShader ("resource/shader/ps/GameModel_ps.pso");
	origShader_ = std::make_unique<OriginalShader>(VertexShader, PixelShader);
	//デプス
	int VertexShader2 = LoadVertexShader("resource/shader/vs/NormalModel_vs.vso");
	int PixelShader2 = LoadPixelShader("resource/shader/ps/DrawShadowMap_ps.pso");
	depthShader_ = std::make_unique<OriginalShader>(VertexShader2, PixelShader2);

	MV1SetPosition(lpModelMng.GetID("resource/model/player/player_robot_new.mv1")[0],VGet(20600, 3950,-10450));
	MV1SetRotationXYZ(lpModelMng.GetID("resource/model/player/player_robot_new.mv1")[0], VGet(0, Deg2RadF(110), Deg2RadF(-70)));
}

void Stage::Release(void)
{
	//MV1DeleteModel(stageHandle_);

	//担当.三ヶ島滉季(開始)---------------------------------------------------------------------------
	//ギミック解放
	for (auto& gimmick : gimmickList_)
	{
		gimmick->Release();
	}

	//アイテム解放
	for (auto& item : itemList_)
	{
		item->Release();
	}
	//担当.三ヶ島滉季(終了)---------------------------------------------------------------------------
}

void Stage::Update(void)
{
	//ギミック更新
	for (auto& gimmick : gimmickList_)
	{
		gimmick->Update();
	}

	//アイテム更新
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
	//ステージ描画
	origShader_->Draw(lpModelMng.GetID("resource/model/stage/Stage1.mv1")[0], depthRT);
	origShader_->Draw(lpModelMng.GetID("resource/model/stage/Window Large.mv1")[0]);
	origShader_->Draw(lpModelMng.GetID("resource/model/stage/Window Small.mv1")[0]);

	//担当.三ヶ島滉季(開始)---------------------------------------------------------------------------
	//ギミック描画
	for (auto& gimmick : gimmickList_)
	{
		gimmick->Draw();
	}

	//アイテム描画
	for (auto& item : itemList_)
	{
		item->Draw(minCameraFlg);
	}
	//担当.三ヶ島滉季(終了)---------------------------------------------------------------------------

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
	//バイナリデータを読み込み
	//ギミック
	LoadBinData<GimmickObj>(gimmickObjList_, "./resource/data/gimmick/Door.dat");
	LoadBinData<GimmickObj>(gimmickObjList_, "./resource/data/gimmick/Dial.dat");
	LoadBinData<GimmickObj>(gimmickObjList_, "./resource/data/gimmick/Wall.dat");
	LoadBinData<GimmickObj>(gimmickObjList_, "./resource/data/gimmick/TextObj.dat");
	LoadBinData<GimmickObj>(gimmickObjList_, "./resource/data/gimmick/Water.dat");
	LoadBinData<GimmickObj>(drawerObjList_, "./resource/data/gimmick/Drawer.dat", "DrawerObj");
	LoadBinData<GimmickObj>(drawerFrameObjList_, "./resource/data/gimmick/DrawerFrame.dat", "DrawerFrameObj");
	LoadBinData<GimmickObj>(jukeBoxObjList_, "./resource/data/gimmick/JukeBox.dat","JukeBoxObj");
	//アイテム
	LoadBinData(keyObjList_, "./resource/data/gimmick/Key.dat", "./resource/data/gimmick/KeyMove.dat", "KeyObj");
	LoadBinData(photObjList_, "./resource/data/gimmick/Phot.dat", "./resource/data/gimmick/Phot.dat", "PhotObj");
	LoadBinData(memoObjList_, "./resource/data/gimmick/Memo.dat", "./resource/data/gimmick/MemoMove.dat", "MemoObj");
	LoadBinData(bookObjList_, "./resource/data/gimmick/Book.dat", "./resource/data/gimmick/BookMove.dat", "BookObj");
	
}

void Stage::AddGimmick(void)
{
	//使用するデータだけギミックリストへ追加
	//壁
	AddGimmickList<FakeWall>(gimmickObjList_, L"Gimmick_Wall",ItemID::Phot);

	//ダイアル扉
	AddGimmickList<DialLockDoor>(gimmickObjList_, L"Red_B","0315");

	//水
	AddGimmickList<Water>(gimmickObjList_, L"water");

	//ジュークボックス
	AddGimmickList<JukeBox>(jukeBoxObjList_);

	//隠れている文字
	AddGimmickList<HiddenText>(gimmickObjList_,L"firstHint");
	AddGimmickList<HiddenText>(gimmickObjList_,L"secondHint");

	//ダイアル
	AddGimmickList<Dial>(gimmickObjList_, L"Dial");
	AddGimmickList<DialFrame>(gimmickObjList_, L"Dial_Frame");

	//引き出し
	AddGimmickList<DrawerFrame>(drawerFrameObjList_);
	AddGimmickList<Drawer>(drawerObjList_);

	//扉
	AddGimmickList<Door>(gimmickObjList_, L"Dark_A");
	AddGimmickList<Door>(gimmickObjList_, L"Dark_A_Double");

	//特定の向きからしか開けられない扉
	AddGimmickList<InnerLockDoor>(gimmickObjList_, L"White_B_Double",-NORMAL);
	AddGimmickList<InnerLockDoor>(gimmickObjList_, L"White_B",NORMAL);
	AddGimmickList<InnerLockDoor>(gimmickObjList_, L"White_C",-NORMAL);

	//鍵付き扉
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
	//鍵
	AddItemList<Key>(keyObjList_);
	//写真
	AddItemList<Phot>(photObjList_);
	//本
	AddItemList<Book>(bookObjList_);
	//メモ
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
			std::string name;	//名前
			Vector3 pos;	//座標
			Vector3 rot;	//回転
			Vector3 scale;	//回転
			char nameSize;
			//ハンドル、名前、座標、回転情報読み取り
			FileRead_read(&nameSize, sizeof(nameSize), file);
			name.resize(static_cast<int>(nameSize));
			FileRead_read(&name[0], nameSize, file);
			FileRead_read(&pos, sizeof(pos), file);
			FileRead_read(&rot, sizeof(rot), file);
			FileRead_read(&scale, sizeof(scale), file);
			//名前、座標、回転情報を格納
			gList.emplace_back(name, pos, rot, scale);
		}
	}
	//ファイルを閉じる
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
			std::string name;	//名前
			Vector3 pos;		//座標
			Vector3 rot;		//回転
			Vector3 scale;		//拡大値
			char nameSize;
			//ハンドル、名前、座標、回転情報読み取り
			FileRead_read(&nameSize, sizeof(nameSize), file);
			name.resize(static_cast<int>(nameSize));
			FileRead_read(&name[0], nameSize, file);
			FileRead_read(&pos, sizeof(pos), file);
			FileRead_read(&rot, sizeof(rot), file);
			FileRead_read(&scale, sizeof(scale), file);
			//親オブジェクトの情報は必要ないので次へ
			if (pName == name)
			{
				continue;
			}
			//名前、座標、回転、拡大値情報を格納
			gList.emplace_back(name, pos, rot, scale);
		}
	}
	//ファイルを閉じる
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
			std::string name;	//名前
			Vector3 bPos;	//座標
			Vector3 rot;	//回転
			Vector3 scale;	//回転
			char nameSize;

			std::string name2;	//名前
			char nameSize2;
			Vector3 aPos;	//座標
			Vector3 rot2;	//回転
			Vector3 scale2;	//回転
			//ハンドル、名前、座標、回転情報読み取り
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
			//親オブジェクトの情報は必要ないので次へ
			if (pName == name)
			{
				continue;
			}
			//名前、座標、回転情報を格納
			iList.emplace_back(name, bPos, aPos, rot, scale);
		}
	}
	//ファイルを閉じる
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
		//ギミックリストに追加&生成(オブジェクト名、座標、角度、拡大値、複製数)
		gimmickList_.emplace_back(std::make_unique<T>(gimmick.objName_, gimmick.pos_, gimmick.angle_, gimmick.scale_, i));
		i++;
	}
}

//ギミックリストへ追加
template<typename T>
void Stage::AddGimmickList(std::vector<GimmickObj>& gList, std::wstring objName)
{
	std::wstring wstr;
	int i = 0;
	for (auto& gimmick : gList)
	{
		wstr = ConvertToWstring(gimmick.objName_);
		//バイナリデータのオブジェクト名と指定した名前が一致しているなら
		if (wstr == objName)
		{
			//ギミックリストに追加&生成(オブジェクト名、座標、角度、拡大値、複製数)
			gimmickList_.emplace_back(std::make_unique<T>(gimmick.objName_,gimmick.pos_,gimmick.angle_,gimmick.scale_,i));
			i++;
		}
		else
		{
			//違ったら次へ
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
		//バイナリデータのオブジェクト名と指定した名前が一致しているなら
		if (wstr == objName)
		{
			//ギミックリストに追加
			gimmickList_.emplace_back(std::make_unique<T>(gimmick.objName_, gimmick.pos_, gimmick.angle_, gimmick.scale_, i,itemID));
			i++;
		}
		else
		{
			//違ったら次へ
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
		//バイナリデータのオブジェクト名と指定した名前が一致しているなら
		if (wstr == objName)
		{
			//ギミックリストに追加
			gimmickList_.emplace_back(std::make_unique<T>(gimmick.objName_, gimmick.pos_, gimmick.angle_, gimmick.scale_, i, normal));
			i++;
		}
		else
		{
			//違ったら次へ
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
		//バイナリデータのオブジェクト名と指定した名前が一致しているなら
		if (wstr == objName)
		{
			//ギミックリストに追加
			gimmickList_.emplace_back(std::make_unique<T>(gimmick.objName_, gimmick.pos_, gimmick.angle_, gimmick.scale_, i,unlockNumber));
			i++;
		}
		else
		{
			//違ったら次へ
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
		//バイナリデータのオブジェクト名と指定した名前が一致しているなら
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
		//バイナリデータのオブジェクト名と指定した名前が一致しているなら
		if (wstr == objName)
		{
			//次へ
			continue;
		}
		else
		{
			//アイテムリストに追加
			markerList_.emplace_back(std::make_unique<ARmarker>(marker.pos_,marker.angle_,static_cast<MarkerID>(i)));
			i++;
		}
	}
}
