#include "../../../common/manager/CBufferMng.h"
#include "GameOverCBuffer.h"


constexpr float CNTUP_VAL = 0.005f;   // �|�C���g���C�g�̐�
//�J����
constexpr int CBUFFER_CAMERA = 8;
constexpr int CBUFFER_CAMERA_SIZE = 8;

GameOverCBuffer::GameOverCBuffer()
{
	Init();
}

GameOverCBuffer::~GameOverCBuffer()
{
}

void GameOverCBuffer::Init(void)
{
	//�o�b�t�@�̏�����
	lpCBufferMng.Remove();
	lpCBufferMng.Create();

	//�J�����o�b�t�@�̍쐬
	lpCBufferMng.CreatePixelCbuffer(CBUFFER_CAMERA_SIZE);
	//�o�b�t�@�֑��
	int i = 0;
	lpCBufferMng.SetPixelCbuffer(CBUFFER_CAMERA, i, 0);
	i++;
	lpCBufferMng.SetPixelCbuffer(CBUFFER_CAMERA, i, 0);

	srand(10);
	cnt_ = 0.0f;
	lpCBufferMng.Update();

}

void GameOverCBuffer::Update(void)
{
	lpCBufferMng.Update();
	cnt_ += CNTUP_VAL;
	lpCBufferMng.SetPixelCbuffer(CBUFFER_CAMERA, 0, cnt_);
}

void GameOverCBuffer::Update(double time)
{
	lpCBufferMng.Update();
	
	lpCBufferMng.SetPixelCbuffer(CBUFFER_CAMERA, 1, (float)time);
}
