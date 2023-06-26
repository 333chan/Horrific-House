#pragma once
#include<DxLib.h>
#include <vector>
#include <string>
#include "../Geometry.h"

#define lpCBufferMng CBufferMng::GetInstance()

class CBufferMng
{
public:

	static CBufferMng& GetInstance()
	{
		static CBufferMng s_Instance;
		return s_Instance;
	}
	
	// Cbufferのアップデート
	void Update();

	//描画デバッグ
	void Draw()const;
	/// <summary>
	/// ピクセルバッファのbuffernumの番地の中身の表示
	/// </summary>
	/// <param name="str">デバッグ表示の時わかりやすくするために先頭に表示したい文字</param>
	/// <param name="buffernum">バッファの番地</param>
	/// <param name="pixelbuffersize">numの番地の大きさ(そのバッファを作った時の大きさ)</param>
	void DebugBufferDraw(std::string str, int x,int buffernum , int pixelbuffersize) const;

	void Create();

	void Remove();

	/// <summary>
	/// 頂点バッファの作成
	/// </summary>
	/// <param name="vertexbuffersize">頂点バッファのサイズ</param>
	void CreateVertexCbuffer(const int vertexbuffersize);

	/// <summary>
	/// ピクセルバッファの作成
	/// </summary>
	/// <param name="pixelbuffersize">ピクセルバッファのサイズ</param>
	void CreatePixelCbuffer( const int pixelbuffersize);

	/// <summary>
	/// 頂点バッファのnumの番地に作成
	/// </summary>
	/// <param name="num"></param>
	/// <param name="vertexbuffersize"></param>
	void CreateVertexCbuffer(int num, const int vertexbuffersize);

	/// <summary>
	/// ピクセルバッファのnumの番地に作成
	/// </summary>
	/// <param name="num"></param>
	/// <param name="pixelbuffersize"></param>
	void CreatePixelCbuffer(int num, const int pixelbuffersize);

	//バッファの値セット
	/// <summary>
	/// 頂点シェーダーのcbuffer値セット
	/// </summary>
	/// <param name="buffernum">バッファーの番地</param>
	/// <param name="num">cbufferの中の値をいれる番地</param>
	/// <param name="value">入れる値</param>
	void SetVertexCbuffer(const int buffernum, const int num, const float value);

	/// <summary>
	/// 頂点シェーダーのcbufferの値をfloat4でセット
	/// </summary>
	/// <param name="buffernum">バッファーの番地</param>
	/// <param name="num">cbufferの中の値をいれる番地</param>
	/// <param name="value4">入れる値(float4)</param>
	void SetVertexCbuffer(const int buffernum, const int num, const FLOAT4 value4);

	void SetVertexCbuffer(int buffernum, int cbufferFloatNum, MATRIX value);



	/// <summary>
	/// ピクセルシェーダーのcbufferの値セット
	/// </summary>
	/// <param name="buffernum">バッファーの番地</param>
	/// <param name="num">cbufferの中の値をいれる番地</param>
	/// <param name="value">入れる値</param>
	void SetPixelCbuffer(const int buffernum, const int num, const float value);

	/// <summary>
	/// ピクセルシェーダーのcbufferの値をfloat4でセット
	/// </summary>
	/// <param name="buffernum">バッファーの番地</param>
	/// <param name="num">cbufferの中の値をいれる番地</param>
	/// <param name="value4">入れる値(float4)</param>
	void SetPixelCbuffer(const int buffernum, const int num, const FLOAT4 value4);

	//void SetPixelCbuffer(const int buffernum, const int num, const Matrix value4x4);

	Vector3 GetALLambientLight();
	void SetALLambientLight(Vector3 value);
private:
	// コンストラクタ
	CBufferMng();
	//デストラクタ
	~CBufferMng();

	//頂点バッファの配列
	std::vector<int> vertexCbuffer_;
	//ピクセルバッファの配列
	std::vector<int> pixelCbuffer_;

	int VertexCbufferNum_;
	int PixelCbufferNum_;
	//アンビエントライト強さ
	Vector3 ALLambientLight_;
};

