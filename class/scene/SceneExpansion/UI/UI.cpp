#include "UI.h"
#include "../../../common/manager/SoundMng.h"

UI::UI()
{
}

UI::~UI()
{
}

std::string UI::PutMouseHit(CollListName& colllist)
{
    //ƒ}ƒEƒXŽæ“¾
    GetMousePoint(&mousePos_.x, &mousePos_.y);

    for (auto& coll : colllist)
    {
        if (CheckCollMouse()(mousePos_, coll.second))
        {
            
            lpSoundMng.PlaySoundOneTime("resource/sound/cursorSE.mp3");
            return coll.first;
        }
    }
    lpSoundMng.ResetCnt("resource/sound/cursorSE.mp3");
    return "";
}

void UI::CollDrawCenterX(Collision coll, std::string image)
{
    Vector2Int GraphSize;

    Vector2 cen = { coll.first.x + coll.second.x / 2 ,coll.first.y + coll.second.y / 2 };

    GetGraphSize(lpImageMng.GetID(image)[0], &GraphSize.x, &GraphSize.y);
    GraphSize.x /= 2;
    DrawGraph(static_cast<int>(cen.x) - GraphSize.x, static_cast<int>(coll.first.y), lpImageMng.GetID(image)[0], true);

}

void UI::CollDrawCenter(Collision coll, std::string image)
{
    Vector2Int GraphSize;

    Vector2 cen = { coll.first.x + coll.second.x / 2 ,coll.first.y + coll.second.y / 2 };

    GetGraphSize(lpImageMng.GetID(image)[0], &GraphSize.x, &GraphSize.y);
    GraphSize.x /= 2;
    GraphSize.y /= 2;
    DrawGraph(static_cast<int>(cen.x) - GraphSize.x, static_cast<int>(coll.first.y), lpImageMng.GetID(image)[0], true);

}
