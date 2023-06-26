#include"Controller.h"

Controller::Controller()
{
}

Controller::~Controller()
{
}

bool Controller::CheckInputKey(KeyID id)
{
	if (controllerData_[id][static_cast<int>(ControllerAge::Now)] && !controllerData_[id][static_cast<int>(ControllerAge::Old)])
	{
		return true;
	}
	return false;
}

bool Controller::CheckLongInputKey(KeyID id)
{
	if (controllerData_[id][static_cast<int>(ControllerAge::Now)])
	{
		return true;
	}
	return false;
}
