#pragma once
#include <string>
#include <map>
#include <vector>
#include "../common/Geometry.h"
//再生方法
enum class PLAY_TYPE
{
	NORMAL,		//通常
	REVERSE		//逆再生
};

//アニメーションの再生状態
enum class ANIM_STATE {
	PLAYING,		//再生中
	STOPPING		//停止中
};

class AnimationController
{
public:
	//アニメーション情報
	struct Animation
	{
		int model = 0;					//モデルハンドル
		int attachNo = 0;				//アニメーション番号
		float speed = 0.0f;				//再生速度
		float totalTime = 0.0f;			//総再生時間
		float frame = 0.0f;				//フレームカウンター
	};

	//コンストラクタ（モデルハンドル）
	AnimationController(int modelId);
	~AnimationController();

	//アニメーション追加
	//引数:種類,ファイルパス,再生速度
	void Add(int type, std::string path, float speed);

	//引数:種類,モデルハンドル,再生速度
	void Add(int type, int handle, float speed);
	
	//アニメーション再生
	//アニメーション種類、ループ再生の有無
	void Play(int type, bool isLoop);

	//アニメーションの種類、アニメーション番号、ループ再生の有無
	void Play(int type, int num, bool isLoop);

	//逆再生
	//アニメーション種類、ループ再生の有無
	void PlayBack(int type, bool isLoop);

	//更新処理
	void Update(double delta);

	//開放
	void Release(void);

	//アニメーション状態を返す
	ANIM_STATE GetPlayState(void);

private:
	//モデルID
	int model_;
	//アニメーションコンテナ
	std::map<int, Animation> animations_;
	//アニメーションの種類
	int animType_;
	//アニメーション情報
	Animation playAnim_;

	//再生方法
	PLAY_TYPE playType;
	// アニメーションをループするかしないか
	bool mIsLoop;

	// アニメーションを止めたままにする
	bool mIsStop;

	//再生状態
	ANIM_STATE animState_;
};

