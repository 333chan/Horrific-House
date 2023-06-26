#include "Gimmick.h"



Gimmick::Gimmick()
{
    //���f���̃t�@�C���p�X
    modelStr_ = "NONE";

    //���f��������
    modelNum_ = 0;

    //���W
    pos_ = { 0.0f,0.0f,0.0f };

    //�g��l
    scale_ = { 0.0f,0.0f,0.0f };

    //��]
    angle_ = { 0.0f,0.0f,0.0f };

    //�M�~�b�N�̎��
    type_ = GIMMICK_TYPE::NONE;

    //�A�j���[�V�������
    animState_ = GIMMICK_ANIM::NONE;

    //���b�N���
    isLock_ = true;

    //�C���^���N�g�t���O
    interactFlg_ = false;

    //�����蔻��t���O
    hitFlg_ = false;

    //�I�u�W�F�N�g��
    name_ = "";

    //���b�N����Ă�������J���邽�߂ɕK�v�Ȍ���ID
    itemID_ = ItemID(0);

    //�����������̖@��
    hitNormal_ = { 0.0f ,0.0f,0.0f };

    //�_�C�A���̐���
    dialNumber_ = 0;

    //���b�N�����̔ԍ�
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
//			std::string name;	//���O
//			Vector3 pos;	//���W
//			Vector3 rot;	//��]
//			char nameSize;
//			//�n���h���A���O�A���W�A��]���ǂݎ��
//			FileRead_read(&nameSize, sizeof(nameSize), file);
//			name.resize(static_cast<int>(nameSize));
//			FileRead_read(&name[0], nameSize, file);
//			FileRead_read(&pos, sizeof(pos), file);
//			FileRead_read(&rot, sizeof(rot), file);
//			//�n���h���A���O�A���W�A��]�����i�[
//			gList.emplace_back(name, pos, rot);
//		}
//	}
//	//�t�@�C�������
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
//			std::string name;	//���O
//			Vector3 pos;	//���W
//			Vector3 rot;	//��]
//			char nameSize;
//			//�n���h���A���O�A���W�A��]���ǂݎ��
//			FileRead_read(&nameSize, sizeof(nameSize), file);
//			name.resize(static_cast<int>(nameSize));
//			FileRead_read(&name[0], nameSize, file);
//			FileRead_read(&pos, sizeof(pos), file);
//			FileRead_read(&rot, sizeof(rot), file);
//			if (objName != name)
//			{
//				continue;
//			}
//			//���O�A���W�A��]�����i�[
//			gList.emplace_back(name, pos, rot);
//		}
//	}
//	//�t�@�C�������
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
//		//�o�C�i���f�[�^�̃I�u�W�F�N�g����wstring�ɕϊ����đ��
//		wstr = ConvertToWstring(gimmick.objName_);
//		//�w�肵���I�u�W�F�N�g���Ɠ����Ȃ�		
//		if (wstr == objName)
//		{
//			//�w�肵�����f�������[�h����
//			gimmick.path_ = MV1LoadModel(mPath);
//		}
//	}
//}

//bool Gimmick::CheckObjName(std::vector<GimmickObj>& gList, std::wstring objName)
//{
//	wstring wstr;
//	for (auto& gimmick : gList)
//	{
//		//�o�C�i���f�[�^�̃I�u�W�F�N�g����wstring�ɕϊ����đ��
//		wstr = ConvertToWstring(gimmick.objName_);
//		//�w�肵���I�u�W�F�N�g���Ɠ����Ȃ�		
//		if (wstr == objName)
//		{
//			//�w�肵�����f�������[�h����
//		}
//	}
//}
