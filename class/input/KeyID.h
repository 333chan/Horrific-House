#pragma once
#include<type_traits>

//キーの種類
enum class KeyID
{
	Up,			//奥に移動
	Down,		//手前に移動
	Left,		//左に移動
	Right,		//右に移動
	Decide,		//決定
	Cancel,		//キャンセル
	Dash,		//ダッシュ
	Inventory,	//インベントリ
	Pause,		//ポーズボタン
	Interact,	//インタラクト(アイテムを調べたり拾ったりする)
	Shoot,		//撮影
	Attack,		//スマホフラッシュ
	ShootForAndroid,		//スマホメモ
	DashForAndroid,		//スマホダッシュ
	Max
};

static KeyID begin(KeyID) { return KeyID::Up; };
static KeyID end(KeyID) { return KeyID::Max; };
static KeyID operator++(KeyID& state) { return state = static_cast<KeyID>(std::underlying_type<KeyID>::type(state) + 1); };
static KeyID operator*(const KeyID& state) { return state; };

