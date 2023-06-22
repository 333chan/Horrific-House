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

	//�R���g���[���̎�ނ��擾
	virtual ControllerType GetControllerType(void) = 0;

	//�R���g���[���̃f�[�^���擾
	const ControllerData& GetControllerData(void)
	{
		return controllerData_;
	}

	//�w�肵���L�[ID�������ꂽ���ǂ�����Ԃ��B�������͕s��(true:�����ꂽ/false:������Ă��Ȃ�)
	bool CheckInputKey(KeyID id);
	bool CheckLongInputKey(KeyID id);

private:

protected:
	ControllerData controllerData_;
};