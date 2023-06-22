#include "SocketServer.h"
#include <stdio.h>
#include <string>
#include <sstream>
//#define _WINSOCK2API_
#include <WinSock2.h>// WinSocket���g�p���邽�߂�Include

#include <WS2tcpip.h>// WinSocket���g�p���邽�߂�Include
#include <DxLib.h>
#include "../object/Item/Item.h"
#include"../../_debug/_DebugConOut.h"

// WinSocket���g�p���邽�߂�lib�t�@�C��
#pragma comment(lib, "ws2_32.lib")

SOCKET sock;	//�w�b�_�[�ɏ�������WinSock2�œ{��ꂽ�̂ł����ɏ����Ă���


SocketServer::SocketServer()
{
}

SocketServer::~SocketServer()
{	
	CommunicationCutoff();
}

void SocketServer::InternetConnection()
{
	//������IP���擾
	IPDATA IPAddress[16];
	GetMyIPAddress(IPAddress);
	TRACE("IP�A�h���X�@�@%d.%d.%d.%d\n", IPAddress->d1, IPAddress->d2, IPAddress->d3, IPAddress->d4);
	TRACE("�L�[:W       ����:017\n");
	TRACE("�L�[:A       ����:030\n");
	TRACE("�L�[:S       ����:031\n");
	TRACE("�L�[:D       ����:032\n");
	TRACE("�L�[:LSHIFT  ����:042\n");
	TRACE("�L�[:TAB     ����:015\n");
	TRACE("�L�[:ESC     ����:001\n");
	TRACE("�L�[:Space   ����:057\n");
	TRACE("�L�[:E       ����:018\n");
	TRACE("���N���b�N   ����:101\n");
	TRACE("�E�N���b�N   ����:102\n");
	TRACE("�Q���������͐����𑫂�������\n");
	
	WSADATA wsaData;
	struct sockaddr_in sockAddr;

	// Winsock�̏�����
	WSAStartup(MAKEWORD(2, 0), &wsaData);
	// �\�P�b�g���쐬
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	// AF_INET��n�����Ƃɂ��IPv4�ŒʐM���鎖���w��
	sockAddr.sin_family = AF_INET;
	// �|�[�g�ԍ����w��
	sockAddr.sin_port = htons(12345);
	// INADDR_ANY���w�肵�Ăǂ�IP����ł��ʐM���󂯎���Ԃɂ���B
	sockAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	// �\�P�b�g�ɃA�h���X�����蓖�Ă�
	bind(sock, (struct sockaddr*)&sockAddr, sizeof(sockAddr));
	// �ڑ��\�P�b�g��񓯊����[�h�ɂ���
	u_long val = 1;
	ioctlsocket(sock, FIONBIO, &val);
}

bool SocketServer::Receive()
{

	char buf[256];
	int data;
	//�z���0���Z�b�g
	memset(buf, 0, sizeof(buf));
	//�f�[�^�̎󂯎��
	data = recv(sock, buf, sizeof(buf), 0);
	//�󂯎�����f�[�^�̊m�F
	if (data < 1)
	{
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			// �f�[�^�����ĂȂ��B
			cameraValue_.first = 0;
			cameraValue_.second = 0;
			mouseflag_ = false;
			keyValue_ = 0;
			supplementValue_ = false;
			itemId_ = ItemID::Non;
		}
	}
	else
	{
		//�f�[�^�������B
		//�f�[�^�������t���O��true
		IsConnection = true;

		std::stringstream dataStream;
		dataStream << buf;

		//�f�[�^�̊i�[
		TRACE(" x : ");
		SplitStorage(cameraValue_.first, dataStream);
		TRACE(" y : ");
		SplitStorage(cameraValue_.second, dataStream);
		TRACE(" falg : ");
		int mouseflagint;
		SplitStorage(mouseflagint, dataStream);
		TRACE(" key : ");
		int keyValue = 0;
		SplitStorage(keyValue, dataStream);
		TRACE("�_�b�V��");
		int dashValue;
		SplitStorage(dashValue, dataStream);
		TRACE("�A�C�e��ID");
		int itemId;
		SplitStorage(itemId, dataStream);
		TRACE("�C���x���g���I��");
		int inventorySelect;
		SplitStorage(inventorySelect, dataStream);
		TRACE("\n\n");

		keyValue_ = keyValue;
		mouseflag_ = mouseflagint;
		supplementValue_ = dashValue;
		itemId_ = static_cast<ItemID>(itemId);
		inventorySelect_ = std::to_string(inventorySelect);
	}

	return true;

}

void SocketServer::CommunicationCutoff()
{
	// �\�P�b�g�ʐM���I��
	closesocket(sock);
	// Winsock���I��
	WSACleanup();
}

int SocketServer::GetKeyValue()const
{
	return keyValue_;
}

int SocketServer::GetSupplementValue()const
{
	return supplementValue_;
}

ItemID SocketServer::GetItemID()const
{
	return itemId_;
}

std::string SocketServer::GetinventorySelect() const
{
	return inventorySelect_;
}

bool SocketServer::GetIsConnection() const
{
	return IsConnection;
}

bool SocketServer::GetMouseflag()const
{
	return mouseflag_;
}

CameraValue SocketServer::GetCameraValue()const
{
	return cameraValue_;
}

void SocketServer::SplitStorage(int& value, std::stringstream& dataStream)
{
	std::string numstr;
	std::getline(dataStream, numstr, ',');
	value = atoi(numstr.c_str());
	TRACE(" data %d ", value);
}

