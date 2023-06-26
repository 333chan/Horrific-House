#pragma once
#include "../Obj.h"
#include "../../common/Geometry.h"
#include "../../common/Transform.h"

class OriginalShader;

enum class MarkerID{//AR�}�[�J�[�̎��ʗpID
    test,
    red,
    blue,
    green,
    yellow,
    cyan,
    max
};
//�A�C�e���I�u�W�F�N�g���
struct MarkerObj
{
    std::string objName_;   //�I�u�W�F�N�g��
    Vector3 pos_;           //���W
    Vector3 angle_;	//��]
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

    //������
    void Init(void)override;
    void Init(Vector3 pos, Vector3 angle, MarkerID id);

    //�X�V
    void Update(void)override;

    //�`��
    void Draw(void)override;

    //�J��
    void Release(void)override;

private:
    Transform transform_;//���f���̏��
    //bool pickupFlg_ = false;//�E�������ǂ���
    std::unique_ptr<OriginalShader> origShader_;
};

