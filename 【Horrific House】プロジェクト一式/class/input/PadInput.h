#pragma once
#include"Controller.h"

class PadInput :
	public Controller
{
public:
	PadInput();
	~PadInput();

	virtual void Update(void) override;
	virtual ControllerType GetControllerType(void) override { return ControllerType::Pad; };
private:
	std::map<KeyID, unsigned int> inputTable_;
};

