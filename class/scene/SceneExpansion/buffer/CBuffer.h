#pragma once
#include "../SceneExpansion.h"
#include "../../../common/Geometry.h"
#include "../../../common/Light.h"
struct DOF
{
	Vector3 Focus;
	float pad0;
	Vector3 Area;
	float pad1;
};

class CBuffer
{
public:
	CBuffer();
	~CBuffer();
	void Draw();
private:

protected:

	DOF Dof_;

	Vector2Int mousePos_;
	Vector2Int oldMousePos_;

	/*Vector3 cameraPos_;
	Vector3 cameraTargetPos_;*/
};