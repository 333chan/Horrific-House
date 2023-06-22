#pragma once
#include "UI.h"

enum class SelectColUI
{
	Non,
	Title,
	Key,
	Pad,
	Ar,
	Decide,
	Return,
	Mouse,
	Operation,
	Max
};

class Controller;
enum class ControllerType;
class SelectUI :
    public UI
{
public:
    SelectUI();
    ~SelectUI();
    void Init()override;
    void Update()override;
    void Draw()override;
	//��������UI
	SelectColUI GetHitUICol();
	//�I�������R���g���[���[�^�C�v�擾
	CntType GetCntType();
private:
	//UIMap�̏�����
	void InitUIMap();
	//�N���b�N���ē���������
	void PutClickHit();
	//�R���g���[�����
	std::unique_ptr<Controller> controller_;
	//UIMap
	std::map<SelectColUI, UISelectObj<SelectColUI>> selectUIMap_;
	//���蓖������UI
	SelectColUI decideHitCol_;
	//�I�������R���g���[���[�^�C�v
	CntType cntType_;
};

