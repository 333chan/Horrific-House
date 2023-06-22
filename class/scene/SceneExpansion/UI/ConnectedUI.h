#pragma once
#include "UI.h"

enum class ConnectedColUI
{
	Non,
	Title,
	Text0,
	Text1,
	Text2,
	Image0,
	Image1,
	Image2,
	And,
	Cancel,
	Tab,
	Max
};
class Controller;
enum class ControllerType;
class ConnectedUI :
    public UI
{
public:
	ConnectedUI();
	~ConnectedUI();
	void Init()override;
	void Update()override;
	void Draw()override;
	//��������UI
	ConnectedColUI GetHitUICol();
	//�^�C�v��
	CntType GetCntType();
private:
	//UIMap�̏�����
	void InitUIMap();
	//�N���b�N���ē���������
	void PutClickHit();
	//�R���g���[�����
	std::unique_ptr<Controller> controller_;
	//UIMap
	std::map<ConnectedColUI, UIObj<ConnectedColUI>> connetctedUIMap_;
	//���蓖������UI
	ConnectedColUI decideHitCol_;
	//�I�������R���g���[���[�^�C�v
	CntType cntType_;
	//IP�A�h���X�\���p�̃t�H���g
	int font_;
};
