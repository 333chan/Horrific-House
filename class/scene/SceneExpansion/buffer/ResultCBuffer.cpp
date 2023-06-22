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
	//�o�b�t�@�̍쐬
	lpCBufferMng.Remove();
	lpCBufferMng.Create();

	//����
	lpCBufferMng.CreatePixelCbuffer(4);
	lpCBufferMng.CreatePixelCbuffer(4);
	//�J�E���g�p
	lpCBufferMng.SetPixelCbuffer(8, 0,0.0f);
	//�d�͉����x
	lpCBufferMng.SetPixelCbuffer(8, 1, 9.8f);
	//��
	lpCBufferMng.SetPixelCbuffer(8, 2, DX_PI_F);
	//�J�����ʒu
	lpCBufferMng.SetPixelCbuffer(9, 0, FLOAT4(0, 0, 0, 0));

	lpCBufferMng.CreateVertexCbuffer(5, 4);
	lpCBufferMng.CreateVertexCbuffer(6, 4);

	//�J�E���g�p
	lpCBufferMng.SetVertexCbuffer(5, 0,0.0f);
	//�d�͉����x
	lpCBufferMng.SetVertexCbuffer(5, 1, 9.8f);
	//��
	lpCBufferMng.SetVertexCbuffer(5, 2, DX_PI_F);
	//�Z���̐�
	lpCBufferMng.SetVertexCbuffer(5, 3, 16.0f);

	//�J�����ʒu
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
