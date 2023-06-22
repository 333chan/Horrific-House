#pragma once
#include "TitleUI.h"
class PreTitleUI :
    public TitleUI
{
public:
	PreTitleUI();
	~PreTitleUI();
	void Init()override;
	void Update()override;
	void Draw()override;

private:
	//UIMap�̏�����
	void InitUIMap();
	//�N���b�N���ē���������
	void PutClickHit();
	//UIMap
	std::map<TitleColUI, UIObj<TitleColUI>> titleUIMap_;
	//���蓖������UI
	TitleColUI decideHitCol_;
	
};

