#include "CBufferMng.h"
#include "../../../_debug/_DebugConOut.h"
#include "../Light.h"

/// <summary>
/// Dxlibで使っていない最小のバッファ番地
/// </summary>
constexpr int CBUFFER_MIN = 4;
constexpr int VERTEX_CBUFFER = 4;
constexpr int PIXEL_RESERVE = 20;
constexpr float AMBIENT = 0.35f;

Vector3 CBufferMng::GetALLambientLight()
{
	return ALLambientLight_;
}

void CBufferMng::SetALLambientLight(Vector3 value)
{
	ALLambientLight_ = value;
}

CBufferMng::CBufferMng()
{
	ALLambientLight_ = { AMBIENT,AMBIENT,AMBIENT };
}

CBufferMng::~CBufferMng()
{
	//cbufferの削除
	for (int i = 0; i < vertexCbuffer_.size(); i++)
	{
		DeleteShaderConstantBuffer(vertexCbuffer_[i]);
	}
	for (int i = 0; i < pixelCbuffer_.size(); i++)
	{
		DeleteShaderConstantBuffer(pixelCbuffer_[i]);
	}
}

void CBufferMng::Update()
{
	//頂点バッファ数回す
	for (int i = 0; i < vertexCbuffer_.size(); i++)
	{
		//バッファの更新
		UpdateShaderConstantBuffer(vertexCbuffer_[i]);
		//バッファの設定
		SetShaderConstantBuffer(vertexCbuffer_[i], DX_SHADERTYPE_VERTEX,i);
	}
	//ピクセルバッファ数回す
	for (int i = 0; i < pixelCbuffer_.size(); i++)
	{
		//バッファの更新
		UpdateShaderConstantBuffer(pixelCbuffer_[i]);
		//バッファの設定
		SetShaderConstantBuffer(pixelCbuffer_[i], DX_SHADERTYPE_PIXEL,i);
	}
}

void CBufferMng::Draw()const
{
#ifdef _DEBUG
	DrawFormatString(200, 800, 0x999999, "vertexbuffersize : %d\n", vertexCbuffer_.size());
	DrawFormatString(200, 820, 0x999999, "pixelbuffersize : %d\n", pixelCbuffer_.size());
	DrawFormatString(200, 840, 0x999999, "(dxlbi + vertex) buffer size : %d\n", CBUFFER_MIN + vertexCbuffer_.size());
#endif	//_DEBUG
}

void CBufferMng::DebugBufferDraw(std::string str,int x, int buffernum, int pixelbuffersize) const
{

#ifdef _DEBUG
	//なぜか終了するときに例外エラーで怒られる　誰か原因わかったら直して
	float* Cbufferf = static_cast<float*>(GetBufferShaderConstantBuffer(vertexCbuffer_[buffernum]));
	int j = 0;
	int dd = 15;
	int ma = 70;
	DrawFormatString(x-300, 0, 0xff9999, "%s", str.c_str());

	if (pixelbuffersize < ma)
	{
		for (int i = 0; i < pixelbuffersize; i++)
		{
			//TRACE("%s　buffer%d : %f\n", str.c_str(), i, Cbufferf[i]);
			DrawFormatString(x, j, 0x998899, "buffer%d: %f\n",i, Cbufferf[i]);
			j += dd;
		}
	}
	else
	{

		for (int i = 0; i < ma; i++)
		{
			//TRACE("%s　buffer%d : %f\n", str.c_str(), i, Cbufferf[i]);
			DrawFormatString(x, j, 0x998899, "buffer%d: %f\n",i, Cbufferf[i]);
			j += dd;
		}
		j = 0;
		for (int i = ma; i < pixelbuffersize; i++)
		{
			//TRACE("%s　buffer%d : %f\n", str.c_str(), i, Cbufferf[i]);
			DrawFormatString(x+200, j, 0x998899, "buffer%d: %f\n", i, Cbufferf[i]);
			j += dd;
		}
	}
#endif	//_DEBUG
}

void CBufferMng::Create()
{
	vertexCbuffer_.resize(CBUFFER_MIN + VERTEX_CBUFFER);
	pixelCbuffer_.reserve(PIXEL_RESERVE);
	pixelCbuffer_.resize(CBUFFER_MIN + VERTEX_CBUFFER);
}

void CBufferMng::Remove()
{
	vertexCbuffer_.clear();
	pixelCbuffer_.clear();
	vertexCbuffer_.shrink_to_fit();
	pixelCbuffer_.shrink_to_fit();

	//cbufferの削除
	//for (int i = 0; i < vertexCbuffer_.size(); i++)
	//{
	//	DeleteShaderConstantBuffer(vertexCbuffer_[i]);
	//}
	//for (int i = 0; i < pixelCbuffer_.size(); i++)
	//{
	//	DeleteShaderConstantBuffer(pixelCbuffer_[i]);
	//}
}

void CBufferMng::CreateVertexCbuffer(const int vertexbuffersize)
{
	//バッファの作成
	vertexCbuffer_.emplace_back(CreateShaderConstantBuffer(sizeof(float) * (vertexbuffersize)));
	TRACE("vertexbuffersize : %d\n", vertexCbuffer_.size());
}

void CBufferMng::CreatePixelCbuffer(const int pixelbuffersize)
{
	//バッファの作成
	pixelCbuffer_.emplace_back(CreateShaderConstantBuffer(sizeof(float) * (pixelbuffersize)));
	TRACE("pixelbuffersize : %d\n", pixelCbuffer_.size());
}

void CBufferMng::CreateVertexCbuffer(int num ,const int vertexbuffersize)
{
	//バッファの作成
	vertexCbuffer_[num] = CreateShaderConstantBuffer(sizeof(float) * (vertexbuffersize));
	TRACE("vertexbuffersize : %d\n", num);
}

void CBufferMng::CreatePixelCbuffer(int num,const int pixelbuffersize)
{
	//バッファの作成
	pixelCbuffer_[num] = CreateShaderConstantBuffer(sizeof(float) * (pixelbuffersize));
	TRACE("pixelbuffersize : %d\n", num);
}

void CBufferMng::SetVertexCbuffer(int buffernum, int cbufferFloatNum, float value)
{
	//頂点バッファの取り出し
	float* Cbufferf = static_cast<float*>(GetBufferShaderConstantBuffer(vertexCbuffer_[buffernum]));

	//中身の変更
	Cbufferf[cbufferFloatNum] = value;
}

void CBufferMng::SetVertexCbuffer(int buffernum, int cbufferFloat4Num, FLOAT4 value4)
{
	//頂点バッファの取り出し
	FLOAT4* Cbufferf4 = static_cast<FLOAT4*>(GetBufferShaderConstantBuffer(vertexCbuffer_[buffernum]));
	//中身の変更
	Cbufferf4[cbufferFloat4Num].x = value4.x;
	Cbufferf4[cbufferFloat4Num].y = value4.y;
	Cbufferf4[cbufferFloat4Num].z = value4.z;
	Cbufferf4[cbufferFloat4Num].w = value4.w;
}

void CBufferMng::SetVertexCbuffer(int buffernum, int cbufferFloatNum, MATRIX value)
{
	//頂点バッファの取り出し
	MATRIX* CbufferM = static_cast<MATRIX*>(GetBufferShaderConstantBuffer(vertexCbuffer_[buffernum]));

	//中身の変更
	CbufferM[cbufferFloatNum] = value;
}
void CBufferMng::SetPixelCbuffer(int buffernum, int cbufferFloatNum, float value)
{
	//頂点バッファの取り出し
	float* Cbufferf = static_cast<float*>(GetBufferShaderConstantBuffer(pixelCbuffer_[buffernum]));

	//中身の変更
	Cbufferf[cbufferFloatNum] = value;
}

void CBufferMng::SetPixelCbuffer(int buffernum, int cbufferFloat4Num, FLOAT4 value4)
{
	//頂点バッファの取り出し
	FLOAT4* Cbufferf4 = static_cast<FLOAT4*>(GetBufferShaderConstantBuffer(pixelCbuffer_[buffernum]));

	//中身の変更
	Cbufferf4[cbufferFloat4Num].x = value4.x;
	Cbufferf4[cbufferFloat4Num].y = value4.y;
	Cbufferf4[cbufferFloat4Num].z = value4.z;
	Cbufferf4[cbufferFloat4Num].w = value4.w;
}

//void CBufferMng::SetPixelCbuffer(const int buffernum, const int num, const Matrix value4x4)
//{
//	//Matrix* Cbufferf4 = static_cast<Matrix*>(GetBufferShaderConstantBuffer(pixelCbuffer_[buffernum]));
//
//}
