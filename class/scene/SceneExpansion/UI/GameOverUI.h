#pragma once
#include "UI.h"
#include "../../../common/Geometry.h"
#include "../../../tmx/TmxObj.h"

enum class GameOverColUI
{
	Non,
	GameOver,
	ToTitle,
	Restart,
	Max
};

class Controller;
enum class ControllerType;

class GameOverUI :
    public UI
{
public:
	GameOverUI();
	~GameOverUI();
	void Init()override;
	void Update()override;
	void Draw()override;
	//��������UI
	GameOverColUI GetHitUICol();

private:
	//UIMap�̏�����
	void InitUIMap();
	//�N���b�N���ē���������
	void PutClickHit();

	//�R���g���[�����
	std::unique_ptr<Controller> controller_;
	//UIMap
	std::map<GameOverColUI, UIObj<GameOverColUI>> gameoverUIMap_;
	//���蓖������UI
	GameOverColUI decideHitCol_;

};

