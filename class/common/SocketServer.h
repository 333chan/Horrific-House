#pragma once
#include <sstream>


enum class ItemID;
using CameraValue = std::pair<int, int>;
#define lpSocketServer SocketServer::GetInstance()
class SocketServer
{
public:
	static SocketServer& GetInstance()
	{
		static SocketServer s_Instance;
		return s_Instance;
	}
	//�ʐM�ɐڑ�
	void InternetConnection();
	//�ʐM���肩��̒l���o��
	bool Receive();
	//�ʐM��؂�
	void CommunicationCutoff();

	//�擾�֐�
	CameraValue GetCameraValue()const;
	//�}�E�X�̒ʐM�����Ă邩�擾
	bool GetMouseflag()const;
	//�L�[�ړ����͎擾
	int GetKeyValue()const;
	//�ړ��ȊO�̃L�[���͎擾
	int GetSupplementValue()const;
	//�g�т̃A�C�e����ID�擾
	ItemID GetItemID()const;
	//�C���x���g���̑I���擾
	std::string GetinventorySelect()const;
	//�f�[�^�����ł�������
	bool GetIsConnection()const;
private:
	SocketServer();
	~SocketServer();
	/// <summary>
	/// ��������ꕶ�������o����int��������
	/// </summary>
	/// <param name="value">�������ϐ�</param>
	/// <param name="dataStream">������</param>
	void SplitStorage(int& value, std::stringstream& dataStream);
	//�ʐM�����������J�����̈ړ��l
	CameraValue cameraValue_;		
	//�}�E�X�̒ʐM�����Ă邩
	bool mouseflag_;
	//�L�[�ړ�����
	int keyValue_;
	//�ړ��ȊO�̃L�[����
	int supplementValue_;
	//�g�т̃A�C�e����ID
	ItemID itemId_;
	//�C���x���g���̑I��
	std::string inventorySelect_;
	//�f�[�^�����ł�������
	bool IsConnection;
};

