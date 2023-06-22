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
	//UIMap‚Ì‰Šú‰»
	void InitUIMap();
	//ƒNƒŠƒbƒN‚µ‚Ä“–‚½‚Á‚½‚Ì
	void PutClickHit();
	//UIMap
	std::map<TitleColUI, UIObj<TitleColUI>> titleUIMap_;
	//Œˆ’è“–‚½‚Á‚½UI
	TitleColUI decideHitCol_;
	
};

