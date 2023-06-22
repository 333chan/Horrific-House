#include "Item.h"

using namespace std;

Item::Item()
{
    modelString_="";
    name_ = "";
    modelNum_=0;
    homePos_={0.0f,0.0f,0.0f,};
    movedPos_ = { 0.0f,0.0f,0.0f, };
    angle_ = { 0.0f,0.0f,0.0f, };
    scale_ = { 0.0f,0.0f,0.0f, };
    pickupFlg_=false;
    moveFlg_=false;
    itemId_=ItemID::Non;
}

Item::~Item()
{
}




