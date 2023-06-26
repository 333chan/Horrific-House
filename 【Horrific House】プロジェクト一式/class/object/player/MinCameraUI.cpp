#include "MinCameraUI.h"
#include "../../input/Controller.h"


MinCameraUI::MinCameraUI()
{
	type_ = ControllerType::Key;
	Init();
}
MinCameraUI::MinCameraUI(ControllerType type)
{
	type_ = type;
	Init();
}

MinCameraUI::~MinCameraUI()
{
}

void MinCameraUI::Init()
{
	//tmx
	tmxObj_.LoadTmx("resource/tmx/cameraUI.tmx", false);
	if (type_ == ControllerType::Pad)
	{
		//�p�b�h��������
		cameraUIMap_ =
		{
			{"snap","photographPad"},
			{"flash","flashPad"}
		};
	}
	else
	{
		//�p�b�h�ȊO
		cameraUIMap_ =
		{
			{"snap","photograph"},
			{"flash","flash"}
		};
	}
}

void MinCameraUI::Update()
{
}

void MinCameraUI::Draw()
{
	for (auto& coll : tmxObj_.GetCameraCollList())
	{
		CollDrawCenter(coll.second, cameraUIMap_[coll.first]);

		/*DrawBox(static_cast<int>(coll.second.first.x),
			static_cast<int>(coll.second.first.y),
			static_cast<int>(coll.second.first.x) + static_cast<int>(coll.second.second.x),
			static_cast<int>(coll.second.first.y) + static_cast<int>(coll.second.second.y),
			0x222222, false);*/
	}
}
