#pragma once
#include<array>
#include<map>
#include"KeyID.h"

enum class ControllerType
{
	Key,
	Pad,
	Max
};

enum class ControllerAge
{
	Now,
	Old,
	Max
};

using AgeBool = std::array<bool, static_cast<size_t>(ControllerAge::Max)>;
using ControllerData = std::map<KeyID, AgeBool>;

class Controller
{
public:
	Controller();
	virtual ~Controller();
	virtual void Update(void) = 0;

	//コントローラの種類を取得
	virtual ControllerType GetControllerType(void) = 0;

	//コントローラのデータを取得
	const ControllerData& GetControllerData(void)
	{
		return controllerData_;
	}

	//指定したキーIDが押されたかどうかを返す。長押しは不可(true:押された/false:押されていない)
	bool CheckInputKey(KeyID id);
	bool CheckLongInputKey(KeyID id);

private:

protected:
	ControllerData controllerData_;
};