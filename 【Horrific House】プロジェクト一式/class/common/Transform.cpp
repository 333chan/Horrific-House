#include <DxLib.h>
#include "Transform.h"
#include "manager/ModelMng.h"

void Transform::Init(std::string modelName, Vector3 pos, Vector3 scale, Vector3 quaRot, Vector3 quaRotLocal)
{
	modelHandle_ = lpModelMng.GetID(modelName.c_str())[0];
	pos_ = pos;
	scale_ = scale;
	quaRot_ = quaRot.toVECTOR();
	quaRotLocal_ = quaRotLocal.toVECTOR();
	Update();
}

void Transform::Update(void)
{
	// 大きさ
	matScale_ = MGetScale(scale_.toVECTOR());

	// 回転
	rot_ = ConvertVECTORtoVector3(quaRot_.ToEuler());
	matRot_ = quaRot_.ToMatrix();

	// 位置
	matPos_ = MGetTranslate(pos_.toVECTOR());

	// 行列の合成
	MATRIX mat = MGetIdent();
	mat = MMult(mat, matScale_);
	Quaternion q = quaRot_.Mult(quaRotLocal_);
	mat = MMult(mat, q.ToMatrix());
	mat = MMult(mat, matPos_);

	// 行列をモデルに判定
	if (modelHandle_ != -1)
	{
		MV1SetMatrix(modelHandle_, mat);
	}
	MV1RefreshCollInfo(modelHandle_);
}

void Transform::Draw(void)
{
	MV1DrawModel(modelHandle_);
}

void Transform::Release(void)
{
}

VECTOR Transform::GetForward(void)
{
	return GetDir({ 0.0f, 0.0f, 1.0f });
}

VECTOR Transform::GetBack(void)
{
	return GetDir({ 0.0f, 0.0f, -1.0f });
}

VECTOR Transform::GetRight(void)
{
	return GetDir({ 1.0f, 0.0f, 0.0f });
}

VECTOR Transform::GetLeft(void)
{
	return GetDir({ -1.0f, 0.0f, 0.0f });
}

VECTOR Transform::GetUp(void)
{
	return GetDir({ 0.0f, 1.0f, 0.0f });
}

VECTOR Transform::GetDown(void)
{
	return GetDir({ 0.0f, -1.0f, 0.0f });
}

VECTOR Transform::GetDir(VECTOR vec)
{
	return quaRot_.PosAxis(vec);
}
