#pragma once
#include "../Obj.h"
#include "../../common/Geometry.h"
#include "../../common/Transform.h"

class OriginalShader;

enum class MarkerID{//ARマーカーの識別用ID
    test,
    red,
    blue,
    green,
    yellow,
    cyan,
    max
};
//アイテムオブジェクト情報
struct MarkerObj
{
    std::string objName_;   //オブジェクト名
    Vector3 pos_;           //座標
    Vector3 angle_;	//回転
    Vector3 scale_;
    MarkerObj(std::string objName, const Vector3& pos, const Vector3& angle, const Vector3& scale) :
        objName_(objName),
        pos_(pos),
        angle_(angle),
        scale_(scale) {};
};

class ARmarker :public Obj
{
public:
	ARmarker();
	ARmarker(Vector3 pos,Vector3 angle,MarkerID id);
	~ARmarker();

    //初期化
    void Init(void)override;
    void Init(Vector3 pos, Vector3 angle, MarkerID id);

    //更新
    void Update(void)override;

    //描画
    void Draw(void)override;

    //開放
    void Release(void)override;

private:
    Transform transform_;//モデルの情報
    //bool pickupFlg_ = false;//拾ったかどうか
    std::unique_ptr<OriginalShader> origShader_;
};

