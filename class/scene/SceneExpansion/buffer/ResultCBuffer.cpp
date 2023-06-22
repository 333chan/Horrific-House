#include "ResultCBuffer.h"
#include "../../../common/manager/CBufferMng.h"

ResultCBuffer::ResultCBuffer()
{
	Init();
}

ResultCBuffer::~ResultCBuffer()
{
}

void ResultCBuffer::Init(void)
{
	//バッファの作成
	lpCBufferMng.Remove();
	lpCBufferMng.Create();

	//生成
	lpCBufferMng.CreatePixelCbuffer(4);
	lpCBufferMng.CreatePixelCbuffer(4);
	//カウント用
	lpCBufferMng.SetPixelCbuffer(8, 0,0.0f);
	//重力加速度
	lpCBufferMng.SetPixelCbuffer(8, 1, 9.8f);
	//π
	lpCBufferMng.SetPixelCbuffer(8, 2, DX_PI_F);
	//カメラ位置
	lpCBufferMng.SetPixelCbuffer(9, 0, FLOAT4(0, 0, 0, 0));

	lpCBufferMng.CreateVertexCbuffer(5, 4);
	lpCBufferMng.CreateVertexCbuffer(6, 4);

	//カウント用
	lpCBufferMng.SetVertexCbuffer(5, 0,0.0f);
	//重力加速度
	lpCBufferMng.SetVertexCbuffer(5, 1, 9.8f);
	//π
	lpCBufferMng.SetVertexCbuffer(5, 2, DX_PI_F);
	//セルの数
	lpCBufferMng.SetVertexCbuffer(5, 3, 16.0f);

	//カメラ位置
	lpCBufferMng.SetVertexCbuffer(6, 0, FLOAT4(0, 0, 0, 0));
}

void ResultCBuffer::Update(float count,Vector3 cameraPos)
{
	lpCBufferMng.Update();

	lpCBufferMng.SetPixelCbuffer(8, 0, count);
	lpCBufferMng.SetPixelCbuffer(9, 0, FLOAT4(cameraPos.x, cameraPos.y, cameraPos.z, 0.0f));
	lpCBufferMng.SetVertexCbuffer(5, 0, count);
	lpCBufferMng.SetVertexCbuffer(6, 0, FLOAT4(cameraPos.x, cameraPos.y, cameraPos.z, 0.0f));
}
