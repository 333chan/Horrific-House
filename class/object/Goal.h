#pragma once
#include "Obj.h"
#include "../Common/Geometry.h"
class Goal :
    public Obj
{
public:
    //コンストラクタ
    Goal(std::string handle, Vector3 pos);
    //デストラクタ
    ~Goal();

    //初期化
    void Init(void)override;
    void Release(void)override;
    //更新
    void Update(void)override;
    void Draw(void)override;

private:
    //位置
    Vector3 pos_;
    std::string path_;
};


