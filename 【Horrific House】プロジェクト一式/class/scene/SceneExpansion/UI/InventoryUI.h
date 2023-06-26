#pragma once
#include "UI.h"

enum class InventoryColUI
{
    Non,
    tab,
    Max
};

class InventoryUI :
    public UI
{
public:
    InventoryUI();
    ~InventoryUI();
    void Init()override;
    void Update()override;
    void Draw()override;
private:
};

