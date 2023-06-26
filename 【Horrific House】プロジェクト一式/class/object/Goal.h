#pragma once
#include "Obj.h"
#include "../Common/Geometry.h"
class Goal :
    public Obj
{
public:
    //�R���X�g���N�^
    Goal(std::string handle, Vector3 pos);
    //�f�X�g���N�^
    ~Goal();

    //������
    void Init(void)override;
    void Release(void)override;
    //�X�V
    void Update(void)override;
    void Draw(void)override;

private:
    //�ʒu
    Vector3 pos_;
    std::string path_;
};


