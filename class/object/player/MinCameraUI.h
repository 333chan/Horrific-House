#pragma once
#include <map>
#include "../../scene/SceneExpansion/UI/UI.h"
enum class ControllerType;
class MinCameraUI:
	UI
{
public:
	MinCameraUI();
	MinCameraUI(ControllerType type);
	~MinCameraUI();
	void Init()override;
	void Update()override;
	void Draw()override;
private:
	std::map<std::string,std::string> cameraUIMap_;
	ControllerType type_;
};

