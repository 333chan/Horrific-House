#pragma once
#ifdef _DEBUG
#include <windows.h>
#include <memory>
#include <string>
#include <tuple>
#include <chrono>		// 精度は1msec程度
#include <DxLib.h>

// デバッグ機能を使うための初期化関数
#define _dbgSetup(A,B,C)					_DebugDispOut::GetInstance().Setup(A,B,C)

// デバッグ表示の透明度の変更
#define _dbgSetAlpha(A)						_DebugDispOut::GetInstance().SetAlpha(A)

// ゲームループの先頭で呼び出す初期化関数
#define _dbgStartDraw()						_DebugDispOut::GetInstance().StartDrawDebug()

// デバッグ表示をバックバッファに描画する際に呼び出す
#define _dbgDraw()							_DebugDispOut::GetInstance().DrawDebug()

// デバッグ描画版のDxLib命令(引数はオリジナルと同じなので公式リファレンスを参照の事
#define _dbgDrawGraph(fmt, ...)				_DebugDispOut::GetInstance().DrawGraph(fmt, __VA_ARGS__) 
#define _dbgDrawBox(fmt, ...)				_DebugDispOut::GetInstance().DrawBox(fmt, __VA_ARGS__) 
#define _dbgDrawLine(fmt, ...)				_DebugDispOut::GetInstance().DrawLine(fmt, __VA_ARGS__) 
#define _dbgDrawLine3D(fmt, ...)			_DebugDispOut::GetInstance().DrawLine3D(fmt, __VA_ARGS__) 
#define _dbgDrawCircle(fmt, ...)			_DebugDispOut::GetInstance().DrawCircle(fmt, __VA_ARGS__) 
#define _dbgDrawPixel(fmt, ...)				_DebugDispOut::GetInstance().DrawPixel(fmt, __VA_ARGS__) 
#define _dbgDrawString(fmt, ...)			_DebugDispOut::GetInstance().DrawString(fmt, __VA_ARGS__)
#define _dbgDrawFormatString(fmt, ...) 		_DebugDispOut::GetInstance().SetScreen(); \
											DrawFormatString(fmt, __VA_ARGS__);\
											_DebugDispOut::GetInstance().RevScreen()

// 内部変数を画面上描画する際の開始位置設定(デフォルトは0,0)
#define _dbgSetMesPos(X,Y)					_DebugDispOut::GetInstance().SetMesPos(X,Y)
// 内部変数を画面上描画(引数は、文字色,文字,数値用の変数(intもしくは浮動小数点))、複数回呼び出すと自動的に下にずれて表示されます
#define _TRACE_S(COLOR,STR,NUM)				_DebugDispOut::GetInstance().TRACE_S(COLOR,STR,NUM);

// 動作FPSを表示する場合
#define _dbgDrawFPS()						_DebugDispOut::GetInstance().DrawFPS()

using ChronoSysClock = std::chrono::system_clock::time_point;

class _DebugDispOut
{
public:
	static _DebugDispOut &GetInstance()
	{
		return (*s_Instance);
	}
	int DrawGraph(int x, int y, int GrHandle, int TransFlag);
	int DrawBox(int x1, int y1, int x2, int y2, unsigned int Color, int FillFlag);
	int DrawString(int x, int y, char* String, unsigned int Color);
	void TRACE_S(int Color, std::string str, int num);
	void TRACE_S(int Color, std::string str, double num);
	//	int DrawFormatString(int x, int y, unsigned int Color, std::string FormatString, ...);
	int DrawLine(int x1, int y1, int x2, int y2, unsigned int Color);
	int DrawLine3D(VECTOR pos1,VECTOR pos2, unsigned int Color);
	int DrawCircle(int x, int y, int r, unsigned int Color, int FillFlag);
	int DrawPixel(int x, int y, unsigned int Color);
	void DrawFPS(void);
	bool StartDrawDebug(void);
	bool DrawDebug(void);
	bool SetAlpha(int alpha);
	bool Setup(int screenSizeX,int screenSizeY, int alpha);
	bool SetWait(double timeCnt);
	void SetScreen(void);
	void RevScreen(void);
	void WaitMode(void);
	void SetMesPos(int x, int y);
private:
	struct _DebugDispOutDeleter
	{
		void operator()(_DebugDispOut* _debugContOut) const
		{
			delete _debugContOut;
		}
	};

	_DebugDispOut();
	~_DebugDispOut();
	static std::unique_ptr<_DebugDispOut, _DebugDispOutDeleter> s_Instance;
	int _alpha;
	ChronoSysClock fpsStartTime_;
	ChronoSysClock fpsEndTime_;
	ChronoSysClock  startTime_;
	ChronoSysClock  endTime_;
	double waitTime_;
	bool dispFlag_;
	int ghBefor_;
	bool clsFlag_;
	int endKey_[2];
	int pouseKey_[2];
	int homeKey_[2];
	int f1Key_[2];
	int backSp_[2];
	int DbgScreen_;
	int fpsView_;
	int fpsCount_;
	int screenSizeX_;
	int screenSizeY_;
	int mesPosX_;
	int mesPosY_;
	int drawLineY_;
};

#else

#define _dbgSetup(A,B,C)
#define _dbgSetAlpha(A)
#define _dbgStartDraw()
#define _dbgDraw()
#define _dbgDrawGraph(fmt, ...)
#define _dbgDrawBox(fmt, ...)
#define _dbgDrawLine(fmt, ...)
#define _dbgDrawCircle(fmt, ...)
#define _dbgDrawPixel(fmt, ...)
#define _dbgDrawString(fmt, ...)
#define _dbgDrawFormatString(fmt, ...)
#define _dbgDrawFPS()
#define _TRACE_S(COLOR,STR,NUM)	

#endif	// _DEBUG