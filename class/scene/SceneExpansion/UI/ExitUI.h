#pragma once
#include "UI.h"

enum class SceneID;
enum class ExitColUI
{
	Non,
	Title,
	Yes,
	YesOption,
	No,
	NoOption,
	Max
};

class Controller;
enum class ControllerType;

class ExitUI :
    public UI
{
public:	
	ExitUI();
	ExitUI(SceneID sceneID);
	~ExitUI();
	void Init()override;
	void Update()override;
	void Draw()override;
	//��������UI�擾
	ExitColUI GetHitUICol();

private:
	//UIMap�̏�����
	void InitUIMap();
	//�N���b�N���ē���������
	void PutClickHit();
	//�R���g���[�����
	std::unique_ptr<Controller> controller_;
	//UIMap
	std::map<ExitColUI, UIObj<ExitColUI>> exitUIMap_;
	//���蓖������UI
	ExitColUI decideHitCol_;
	//��O�̃V�[��
	SceneID sceneID_;
};

