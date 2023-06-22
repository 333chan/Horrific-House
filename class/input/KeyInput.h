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
	/// 携帯の移動ボタンの同時押し
	/// </summary>
	/// <param name="id">id</param>
	/// <param name="mouseflag">通信データがマウスどうか</param>
	void SimultaneousTelephoneButton(KeyID id ,bool PcMouseflag ,bool communicationMouseflag);
	/// <summary>
	/// 携帯の移動ボタン以外の同時押し
	/// </summary>
	/// <param name="id">一個目のid</param>
	void MoveOnlyTelephoneButton(KeyID id);
};

