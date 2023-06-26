#include "Gimmick.h"



Gimmick::Gimmick()
{
    //モデルのファイルパス
    modelStr_ = "NONE";

    //モデル複製数
    modelNum_ = 0;

    //座標
    pos_ = { 0.0f,0.0f,0.0f };

    //拡大値
    scale_ = { 0.0f,0.0f,0.0f };

    //回転
    angle_ = { 0.0f,0.0f,0.0f };

    //ギミックの種類
    type_ = GIMMICK_TYPE::NONE;

    //アニメーション状態
    animState_ = GIMMICK_ANIM::NONE;

    //ロック状態
    isLock_ = true;

    //インタラクトフラグ
    interactFlg_ = false;

    //当たり判定フラグ
    hitFlg_ = false;

    //オブジェクト名
    name_ = "";

    //ロックされている扉を開けるために必要な鍵のID
    itemID_ = ItemID(0);

    //あたった時の法線
    hitNormal_ = { 0.0f ,0.0f,0.0f };

    //ダイアルの数字
    dialNumber_ = 0;

    //ロック解除の番号
    unlockNum_ = "0000";
}

Gimmick::~Gimmick()
{
}

//void Gimmick::LoadBinData(std::vector<GimmickObj>& gList, const char* bData)
//{
//	int file = FileRead_open(bData);
//	int version;
//	FileRead_read(&version, sizeof(version), file);
//	if (version == 6)
//	{
//		while (!FileRead_eof(file))
//		{
//			std::string name;	//名前
//			Vector3 pos;	//座標
//			Vector3 rot;	//回転
//			char nameSize;
//			//ハンドル、名前、座標、回転情報読み取り
//			FileRead_read(&nameSize, sizeof(nameSize), file);
//			name.resize(static_cast<int>(nameSize));
//			FileRead_read(&name[0], nameSize, file);
//			FileRead_read(&pos, sizeof(pos), file);
//			FileRead_read(&rot, sizeof(rot), file);
//			//ハンドル、名前、座標、回転情報を格納
//			gList.emplace_back(name, pos, rot);
//		}
//	}
//	//ファイルを閉じる
//	FileRead_close(file);
//}
//
//void Gimmick::LoadBinData(std::vector<GimmickObj>& gList, const char* bData, std::string objName)
//{
//	int file = FileRead_open(bData);
//	int version;
//	FileRead_read(&version, sizeof(version), file);
//	if (version == 6)
//	{
//		while (!FileRead_eof(file))
//		{
//			std::string name;	//名前
//			Vector3 pos;	//座標
//			Vector3 rot;	//回転
//			char nameSize;
//			//ハンドル、名前、座標、回転情報読み取り
//			FileRead_read(&nameSize, sizeof(nameSize), file);
//			name.resize(static_cast<int>(nameSize));
//			FileRead_read(&name[0], nameSize, file);
//			FileRead_read(&pos, sizeof(pos), file);
//			FileRead_read(&rot, sizeof(rot), file);
//			if (objName != name)
//			{
//				continue;
//			}
//			//名前、座標、回転情報を格納
//			gList.emplace_back(name, pos, rot);
//		}
//	}
//	//ファイルを閉じる
//	FileRead_close(file);
//}
//
//std::wstring Gimmick::ConvertToWstring(std::string& str)
//{
//	auto cnt = MultiByteToWideChar(
//		CP_UTF8,
//		0,
//		str.c_str(),
//		str.length(),
//		nullptr,
//		0
//	);
//
//	wstring wstr;
//	wstr.resize(cnt);
//
//	MultiByteToWideChar(
//		CP_UTF8,
//		0,
//		str.c_str(),
//		str.length(),
//		&wstr[0],
//		wstr.length()
//	);
//
//	return wstr;
//}

//void Gimmick::LoadModel(std::vector<GimmickObj>& gList,std::wstring objName, const char* mPath)
//{
//	wstring wstr;
//	for (auto& gimmick : gList)
//	{
//		//バイナリデータのオブジェクト名をwstringに変換して代入
//		wstr = ConvertToWstring(gimmick.objName_);
//		//指定したオブジェクト名と同じなら		
//		if (wstr == objName)
//		{
//			//指定したモデルをロードする
//			gimmick.path_ = MV1LoadModel(mPath);
//		}
//	}
//}

//bool Gimmick::CheckObjName(std::vector<GimmickObj>& gList, std::wstring objName)
//{
//	wstring wstr;
//	for (auto& gimmick : gList)
//	{
//		//バイナリデータのオブジェクト名をwstringに変換して代入
//		wstr = ConvertToWstring(gimmick.objName_);
//		//指定したオブジェクト名と同じなら		
//		if (wstr == objName)
//		{
//			//指定したモデルをロードする
//		}
//	}
//}
