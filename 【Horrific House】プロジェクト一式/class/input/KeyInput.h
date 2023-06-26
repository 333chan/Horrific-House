#pragma once
#include"Controller.h"

class KeyInput :
	public Controller
{
public:
	KeyInput();
	~KeyInput();

	virtual void Update(void) override;
	virtual ControllerType GetControllerType(void) override { return ControllerType::Key; };
private:
	std::array<char, 256U> keyData_;
	std::map<KeyID, unsigned int> inputTable_;

	/// <summary>
	/// �g�т̈ړ��{�^���̓�������
	/// </summary>
	/// <param name="id">id</param>
	/// <param name="mouseflag">�ʐM�f�[�^���}�E�X�ǂ���</param>
	void SimultaneousTelephoneButton(KeyID id ,bool PcMouseflag ,bool communicationMouseflag);
	/// <summary>
	/// �g�т̈ړ��{�^���ȊO�̓�������
	/// </summary>
	/// <param name="id">��ڂ�id</param>
	void MoveOnlyTelephoneButton(KeyID id);
};

