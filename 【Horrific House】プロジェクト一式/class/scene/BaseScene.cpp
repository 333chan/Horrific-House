#include<DxLib.h>
#include "BaseScene.h"
#include "../common/manager/SoundMng.h"
#include "../common/manager/imageMng.h"
#include "../common/manager/ModelMng.h"
BaseScene::BaseScene()
{
	SetDrawScreen(DX_SCREEN_BACK);
	GetDrawScreenSize(&screenSizeX_, &screenSizeY_);
	screenID_ = MakeScreen(screenSizeX_, screenSizeY_, true);
	//albedRT_ = MakeScreen(screenSizeX_, screenSizeY_, true);
	//normalSpecRT_ = MakeScreen(screenSizeX_, screenSizeY_, true);


	///*SetDrawValidFloatTypeGraphCreateFlag(true);
	//SetCreateDrawValidGraphChannelNum(1);
	//SetCreateGraphChannelBitDepth(32);*/
	//depthRT_ = MakeScreen(4096, 4096, true);
	/*SetDrawValidFloatTypeGraphCreateFlag(FALSE);
	SetCreateDrawValidGraphChannelNum(0);
	SetCreateGraphChannelBitDepth(0);*/

	//浮動小数点RGBA 32ビット
	/*SetDrawValidFloatTypeGraphCreateFlag(true);
	SetCreateDrawValidGraphChannelNum(4);
	SetCreateGraphChannelBitDepth(32);
	worldPosRT_ = MakeScreen(screenSizeX_, screenSizeY_, true);
	SetDrawValidFloatTypeGraphCreateFlag(FALSE);
	SetCreateDrawValidGraphChannelNum(0);
	SetCreateGraphChannelBitDepth(0);*/
	InitVertext();
}

BaseScene::~BaseScene()
{
}

void BaseScene::InitVertext(void)
{
	//頂点初期化

	//共通
	for (auto& v : verts_)
	{
		v.dif = GetColorU8(255, 255, 255, 255);
		v.spc = GetColorU8(0, 0, 0, 0);
		v.pos.z = 0.0f;
		v.rhw = 1.0f;
		v.su = 0.0f;
		v.sv = 0.0f;
	}
	//左上
	verts_[0].pos.x = 0.0f;
	verts_[0].pos.y = 0.0f;
	//右上
	verts_[1].pos.x = static_cast<float>(screenSizeX_);
	verts_[1].pos.y = 0.0f;
	//左下
	verts_[2].pos.x = 0.0f;
	verts_[2].pos.y = static_cast<float>(screenSizeY_);
	//右下
	verts_[3].pos.x = static_cast<float>(screenSizeX_);
	verts_[3].pos.y = static_cast<float>(screenSizeY_);
	//UV座標
	verts_[0].u = 0.0f;
	verts_[0].v = 0.0f;

	verts_[1].u = 1.0f;
	verts_[1].v = 0.0f;

	verts_[2].u = 0.0f;
	verts_[2].v = 1.0f;

	verts_[3].u = 1.0f;
	verts_[3].v = 1.0f;
}

void BaseScene::Draw()
{
	DrawGraph(0, 0, screenID_, true);
}

void BaseScene::DrawPostEffect(int ps)
{
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	SetUsePixelShader(ps);
	SetUseTextureToShader(0, screenID_);
	SetUseTextureToShader(1, -1);
	DrawPrimitive2DToShader(verts_.data(), verts_.size(), DX_PRIMTYPE_TRIANGLESTRIP);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void BaseScene::DrawPostEffect(int ps, int texture1, int texture2)
{
	SetUsePixelShader(ps);
	SetUseTextureToShader(0, texture1);
	SetUseTextureToShader(1, texture2);
	DrawPrimitive2DToShader(verts_.data(), verts_.size(), DX_PRIMTYPE_TRIANGLESTRIP);
}

void BaseScene::PreLoading()
{
	
	////ロードがこっち方が速い
	//SetUseASyncLoadFlag(true);
	//
	//

	//SetUseASyncLoadFlag(false);

	
}

void BaseScene::Loading()
{
#ifdef _DEBUG
	//ロードがこっち方が速い
	//開発中だけこっちでする
	lpModelMng.GetID("resource/model/stage/Stage1.mv1");
#endif
	SetUseASyncLoadFlag(true);
	/*lpModelMng.GetID("resource/model/stage/titlemap.mv1");*/
	/*lpImageMng.GetID("resource/image/minCamera.png");*/
	//ビギニング画像
	lpImageMng.GetID("resource/image/TeamLogo.png", "teamLogo");
	lpImageMng.GetID("resource/image/warning_red01.png", "warningImage");

	lpImageMng.GetID("resource/image/tips.png", "tipsBar");
//タイトル画像
	lpImageMng.GetID("resource/image/Font/file.png", "title");
	lpImageMng.GetID("resource/image/Font/play.png", "play");
	lpImageMng.GetID("resource/image/Font/option.png", "option");
	lpImageMng.GetID("resource/image/Font/exit.png", "exit");

	lpImageMng.GetID("resource/image/Font/gamePlay.png", "gamePlay");
	lpImageMng.GetID("resource/image/Font/optionOpen.png", "optionOpen");
	lpImageMng.GetID("resource/image/Font/returnToDesktop.png", "returnToDesktop");
	lpImageMng.GetID("resource/image/Font/click.png", "mouse");
	lpImageMng.GetID("resource/image/Font/anyButton.png", "anyButton");
	//オプション画像
	lpImageMng.GetID("resource/image/Font/optionTag.png", "optionTag");
	lpImageMng.GetID("resource/image/Font/controlTag.png", "controlTag");
	lpImageMng.GetID("resource/image/Font/audioTag.png", "audioTag");
	lpImageMng.GetID("resource/image/Font/graphicTag.png", "graphicTag");
	lpImageMng.GetID("resource/image/Font/titleTag.png", "titleReturnTag");
	lpImageMng.GetID("resource/image/Font/returnTag.png", "returnTag");
	lpImageMng.GetID("resource/image/Font/volume.png", "volume");
	lpImageMng.GetID("resource/image/Font/brightness.png", "brightness");
	lpImageMng.GetID("resource/image/Font/key.png", "key");
	lpImageMng.GetID("resource/image/Font/smp.png", "smp");
	lpImageMng.GetID("resource/image/Font/pad.png", "pad");

	lpImageMng.GetID("resource/image/option/select10.png", "selection");
	lpImageMng.GetID("resource/image/option/select2.png", "selection1");
	lpImageMng.GetID("resource/image/option/select2.png", "selection2");
	lpImageMng.GetID("resource/image/option/keyboard.png", "keyboard");
	lpImageMng.GetID("resource/image/option/SumahoImage.png", "SumahoImage");
	lpImageMng.GetID("resource/image/option/gamePad.png", "padImage");

	lpImageMng.GetID("resource/image/Font/spControl.png", "supportControl");
	lpImageMng.GetID("resource/image/Font/spAudio.png", "supportAudio");
	lpImageMng.GetID("resource/image/Font/spGraphic.png", "supportGraphic");
	lpImageMng.GetID("resource/image/Font/spTitleReturn.png", "supportTitleReturn");
	lpImageMng.GetID("resource/image/Font/spReturn.png", "supportReturn");

	lpImageMng.GetID("resource/image/option/line3.png", "line2");
	lpImageMng.GetID("resource/image/option/box.png", "select5");
	lpImageMng.GetID("resource/image/option/box.png", "operation");
	lpImageMng.GetID("resource/image/option/Bbox.png", "Bbox");
	lpImageMng.GetID("resource/image/Font/masterValume.png", "masterValume");
	lpImageMng.GetID("resource/image/Font/screenBrightness.png", "screenBrightness");
	lpImageMng.GetID("resource/image/option/bg1.png","bg");
//セレクト画像
	lpImageMng.GetID("resource/image/Font/Select/select_title.png", "select_title");
	lpImageMng.GetID("resource/image/Font/Select/select_key.png", "select_key");
	lpImageMng.GetID("resource/image/Font/Select/select_ar.png", "select_ar");
	lpImageMng.GetID("resource/image/Font/Select/select_pad.png", "select_pad");
	lpImageMng.GetID("resource/image/Font/Select/select_decide.png", "select_decide");
	lpImageMng.GetID("resource/image/Font/Select/select_return.png", "select_return");

	lpImageMng.GetID("resource/image/Font/Select/select_title_tip.png", "select_title_tip");
	lpImageMng.GetID("resource/image/Font/Select/select_key_tip.png", "select_key_tip");
	lpImageMng.GetID("resource/image/Font/Select/select_ar_tip.png", "select_ar_tip");
	lpImageMng.GetID("resource/image/Font/Select/select_pad_tip.png", "select_pad_tip");
	lpImageMng.GetID("resource/image/Font/Select/select_decide_tip.png", "select_decide_tip");
	lpImageMng.GetID("resource/image/Font/Select/select_return_tip.png", "select_return_tip");

	lpImageMng.GetID("resource/image/select/desktop_pc.png", "desktopPc_Image");
	lpImageMng.GetID("resource/image/select/gamePad.png", "gamePad_Image");
	lpImageMng.GetID("resource/image/select/smhmini.png", "smh_Image");
//接続
	lpImageMng.GetID("resource/image/Font/Connected/ar_title.png", "ar_title");
	lpImageMng.GetID("resource/image/Font/Connected/ar_text0.png", "ar_text0");
	lpImageMng.GetID("resource/image/Font/Connected/ar_text1.png", "ar_text1");
	lpImageMng.GetID("resource/image/Font/Connected/ar_text2.png", "ar_text2");
	lpImageMng.GetID("resource/image/Font/Connected/ar_image0.png", "ar_image0");
	lpImageMng.GetID("resource/image/Font/Connected/ar_image1.png", "ar_image1");
	lpImageMng.GetID("resource/image/Font/Connected/ar_image2.png", "ar_image2");
	lpImageMng.GetID("resource/image/Font/Connected/ar_and.png", "ar_and");
	lpImageMng.GetID("resource/image/Font/Connected/ar_cancel.png", "ar_cancel");

//オープニング画像
	lpImageMng.GetID("resource/image/Font/Opening/opening_first.png", "opening_first");
	lpImageMng.GetID("resource/image/Font/Opening/opening_second.png", "opening_second");
	lpImageMng.GetID("resource/image/Font/Opening/opening_third.png", "opening_third");
	lpImageMng.GetID("resource/image/Font/Opening/opening_forth.png", "opening_forth");
	lpImageMng.GetID("resource/image/Font/Opening/opening_five.png", "opening_five");
	lpImageMng.GetID("resource/image/Font/Opening/opening_six.png", "opening_six");
	lpImageMng.GetID("resource/image/Font/Opening/opening_seven.png", "opening_seven");

//エンディング画像
	lpImageMng.GetID("resource/image/Font/Ending/ending_non.png", "ending_non");
	lpImageMng.GetID("resource/image/Font/Ending/ending_first.png", "ending_first");
	lpImageMng.GetID("resource/image/Font/Ending/ending_second.png", "ending_second");
	lpImageMng.GetID("resource/image/Font/Ending/ending_third.png", "ending_third");
	lpImageMng.GetID("resource/image/Font/Ending/ending_forth.png", "ending_forth");
	lpImageMng.GetID("resource/image/Font/Ending/ending_five.png", "ending_five");
	lpImageMng.GetID("resource/image/Font/Ending/ending_six.png", "ending_six");
//exit
	lpImageMng.GetID("resource/image/Font/Exit/ExitTitle.png", "ExitTitle");
	lpImageMng.GetID("resource/image/Font/Exit/ExitToTitle.png", "ExitToTitle");

	lpImageMng.GetID("resource/image/Font/Exit/Yes.png", "Yes");
	lpImageMng.GetID("resource/image/Font/Exit/No.png", "No");
//ゲーム画像
	//---Tutorial---
	//Keyboard
	lpImageMng.GetID("resource/image/Font/Tutorial/key_Tutorial00.png", "key_Tutorial0");
	lpImageMng.GetID("resource/image/Font/Tutorial/key_Tutorial1.png", "key_Tutorial1");
	lpImageMng.GetID("resource/image/Font/Tutorial/key_Tutorial2.png", "key_Tutorial2");
	lpImageMng.GetID("resource/image/Font/Tutorial/key_Tutorial3.png", "key_Tutorial3");
	lpImageMng.GetID("resource/image/Font/Tutorial/key_Tutorial4.png", "key_Tutorial4");
	lpImageMng.GetID("resource/image/Font/Tutorial/key_Tutorial5.png", "key_Tutorial5");
	lpImageMng.GetID("resource/image/Font/Tutorial/key_Tutorial6.png", "key_Tutorial6");
	lpImageMng.GetID("resource/image/Font/Tutorial/key_Tutorial7.png", "key_Tutorial7");
	lpImageMng.GetID("resource/image/Font/Tutorial/key_Tutorial8.png", "key_Tutorial8");
	//Pad
	lpImageMng.GetID("resource/image/Font/Tutorial/pad_Tutorial0.png", "pad_Tutorial0");
	lpImageMng.GetID("resource/image/Font/Tutorial/pad_Tutorial1.png", "pad_Tutorial1");
	lpImageMng.GetID("resource/image/Font/Tutorial/pad_Tutorial2.png", "pad_Tutorial2");
	lpImageMng.GetID("resource/image/Font/Tutorial/pad_Tutorial3.png", "pad_Tutorial3");
	lpImageMng.GetID("resource/image/Font/Tutorial/pad_Tutorial4.png", "pad_Tutorial4");
	lpImageMng.GetID("resource/image/Font/Tutorial/pad_Tutorial5.png", "pad_Tutorial5");
	lpImageMng.GetID("resource/image/Font/Tutorial/pad_Tutorial6.png", "pad_Tutorial6");
	lpImageMng.GetID("resource/image/Font/Tutorial/pad_Tutorial7.png", "pad_Tutorial7");
	lpImageMng.GetID("resource/image/Font/Tutorial/pad_Tutorial8.png", "pad_Tutorial8");
	//スマホ
	lpImageMng.GetID("resource/image/Font/Tutorial/smh_Tutorial0.png", "smh_Tutorial0");
	lpImageMng.GetID("resource/image/Font/Tutorial/smh_Tutorial1.png", "smh_Tutorial1");
	lpImageMng.GetID("resource/image/Font/Tutorial/smh_Tutorial2.png", "smh_Tutorial2");
	lpImageMng.GetID("resource/image/Font/Tutorial/smh_Tutorial3.png", "smh_Tutorial3");
	lpImageMng.GetID("resource/image/Font/Tutorial/smh_Tutorial4.png", "smh_Tutorial4");
	lpImageMng.GetID("resource/image/Font/Tutorial/smh_Tutorial5.png", "smh_Tutorial5");
	lpImageMng.GetID("resource/image/Font/Tutorial/smh_Tutorial6.png", "smh_Tutorial6");
	lpImageMng.GetID("resource/image/Font/Tutorial/smh_Tutorial7.png", "smh_Tutorial7");
	lpImageMng.GetID("resource/image/Font/Tutorial/smh_Tutorial8.png", "smh_Tutorial8");
	//--------------
	
	lpImageMng.GetID("resource/image/Font/photograph.png", "photograph");
	lpImageMng.GetID("resource/image/Font/photographPad.png", "photographPad");

	lpImageMng.GetID("resource/image/Font/flash.png", "flash");
	lpImageMng.GetID("resource/image/Font/flashPad.png", "flashPad");

	lpImageMng.GetID("resource/image/mouse/interactText.png", "interactText");

	//インベントリ画像
	lpImageMng.GetID("resource/image/inventory/kitchenkeydescribe.png", "kitchenkeydescribe");		//キッチンの鍵の説明画像
	lpImageMng.GetID("resource/image/inventory/Entrancekeydescribe.png", "Entrancekeydescribe");	//玄関の鍵の説明画像
	lpImageMng.GetID("resource/image/inventory/Pantrykeydescribe.png", "Pantrykeydescribe");		//食糧庫の鍵の説明画像
	lpImageMng.GetID("resource/image/inventory/musicroomkeydescribe.png", "musicroomkeydescribe");	//音楽室の鍵の説明画像
	lpImageMng.GetID("resource/image/inventory/childRoomkeydescribe.png", "childRoomkeydescribe");	//子供部屋の説明画像
	lpImageMng.GetID("resource/image/inventory/basementkeydescribe.png", "basementkeydescribe");	//地下室の鍵の説明画像
	lpImageMng.GetID("resource/image/inventory/Photdescide.png", "Photdescide");					//写真の説明画像
	lpImageMng.GetID("resource/image/inventory/enemymemo.png", "enemymemo");						//敵の弱点の説明画像
	lpImageMng.GetID("resource/image/inventory/storymemo_a.png", "storymemo_a");					//ストーリーメモaの説明画像
	lpImageMng.GetID("resource/image/inventory/storymemo_b.png", "storymemo_b");					//ストーリーメモbの説明画像
	lpImageMng.GetID("resource/image/inventory/storymemo_c.png", "storymemo_c");					//ストーリーメモc兼壁の説明画像1
	lpImageMng.GetID("resource/image/inventory/hint_d.png", "hint_d");								//壁の説明画像2
	lpImageMng.GetID("resource/image/inventory/hint_f.png", "hint_f");								//調理器具の説明画像
	lpImageMng.GetID("resource/image/inventory/hint_g.png", "hint_g");								//楽譜の説明画像
	lpImageMng.GetID("resource/image/inventory/hint_h.png", "hint_h");								//ゲームソフトの説明画像
	lpImageMng.GetID("resource/image/inventory/hint_q.png", "hint_q");								//応接間の絵画の説明画像
	lpImageMng.GetID("resource/image/inventory/hint_j.png", "hint_j");								//猫の時計と絵画の説明画像
	lpImageMng.GetID("resource/image/inventory/hint_t.png", "hint_t");								//警告版の説明画像
	lpImageMng.GetID("resource/image/inventory/mapItemdescide.png", "mapItemdescide");				//マップアイテムの説明画像

	lpImageMng.GetID("resource/image/Load/gageImage4.png","fram");	//マスク画像

//ギミックUI画像
	lpImageMng.GetID("resource/image/gimmick/KitchenKeyUI.png", "KitchenUI");
	lpImageMng.GetID("resource/image/gimmick/MusicRoomKeyUI.png", "MusicRoomUI");
	lpImageMng.GetID("resource/image/gimmick/FoodBankKeyUI.png", "FoodBankUI");
	lpImageMng.GetID("resource/image/gimmick/ChildRoomKeyUI.png", "ChildRoomUI");
	lpImageMng.GetID("resource/image/gimmick/RayLockUI.png", "RayLockUI");
	lpImageMng.GetID("resource/image/gimmick/CellerKeyUI.png", "CellerUI");
	lpImageMng.GetID("resource/image/gimmick/EntranceKeyUI.png", "EntranceUI");
	lpImageMng.GetID("resource/image/gimmick/UseKitchenKeyUI.png", "UseKitchenUI");
	lpImageMng.GetID("resource/image/gimmick/UseMusicRoomKeyUI.png", "UseMusicRoomUI");
	lpImageMng.GetID("resource/image/gimmick/UseFoodBankKeyUI.png", "UseFoodBankUI");
	lpImageMng.GetID("resource/image/gimmick/UseChildRoomKeyUI.png", "UseChildRoomUI");
	lpImageMng.GetID("resource/image/gimmick/RayUnlockUI.png", "RayUnlockUI");
	lpImageMng.GetID("resource/image/gimmick/UseCellerKeyUI.png", "UseCellerUI");
	lpImageMng.GetID("resource/image/gimmick/DialLockUI.png", "DialLockUI");
	//リザルト画像
	lpImageMng.GetID("resource/image/Font/congratulation.png", "resultCongratulation");
	lpImageMng.GetID("resource/image/Font/clear/name.png", "resultLogo");
	lpImageMng.GetID("resource/image/Font/clear/return.png", "resultTitle");
	lpImageMng.GetID("resource/image/Font/totitle.png", "Totitle");

//ゲームオーバー画像
	lpImageMng.GetID("resource/image/Font/gameover.png", "gameover");
	lpImageMng.GetID("resource/image/Font/restart.png", "reStartTag");
	lpImageMng.GetID("resource/image/Font/Totitle.png", "toTitleTag");
	lpImageMng.GetID("resource/image/Font/spTitleReturn.png", "spTitleReturn");
	lpImageMng.GetID("resource/image/Font/spReturn.png", "spReturn");
	lpImageMng.GetID("resource/image/gameover/Blood5.png", "selectB");

//モデル画像
	//ステージ
	lpModelMng.GetID("resource/model/stage/titlemap.mv1");
	lpModelMng.GetID("resource/model/UI/Play.mv1");
	lpModelMng.GetID("resource/model/UI/Option.mv1");
	lpModelMng.GetID("resource/model/UI/Exit.mv1");

	lpModelMng.GetID("resource/model/stage/Stage1.mv1");
	
	lpModelMng.GetID("resource/model/stage/Coll3.mv1");

	lpModelMng.GetID("./resource/model/enemy/ghost3.mv1", 2);

	//スクラップ君
	lpModelMng.GetID("resource/model/player/player_robot_new.mv1");
	//2Dサウンド
	lpSoundMng.GetID("resource/sound/playerWalkSE.mp3");		//プレイヤー歩行音
	lpSoundMng.GetID("resource/sound/playerRunSE.mp3");			//プレイヤー走行音
	lpSoundMng.GetID("resource/sound/playerRunVoice.mp3");		//プレイヤー走行時息音
	lpSoundMng.GetID("resource/sound/PlayerRunOutVoice.mp3");	//走行後息を整える
	lpSoundMng.GetID("resource/sound/cursorSE.mp3");			//マウスカーソルを合わせた時の音
	lpSoundMng.GetID("resource/sound/PopUpSE.mp3");				//チュートリアル等の出現音
	lpSoundMng.GetID("resource/sound/gamestartSE.mp3");			//ゲーム開始の選択音
	lpSoundMng.GetID("resource/sound/selectSE2.mp3");			//メニューの選択音
	lpSoundMng.GetID("resource/sound/InventoryOpenSE.mp3");		//インベントリの開音
	lpSoundMng.GetID("resource/sound/InventoryCloseSE.mp3");	//インベントリの閉音
	lpSoundMng.GetID("resource/sound/ShotSE.mp3");				//写真撮影音
	lpSoundMng.GetID("resource/sound/NumberKeyOpenSE.mp3");		//数字鍵の開錠音
	lpSoundMng.GetID("resource/sound/PickUpSE.mp3");			//取得音
	lpSoundMng.GetID("resource/sound/LockDoorSE.mp3");			//開かないドアを調べた音
	lpSoundMng.GetID("resource/sound/DoorOpenSE.mp3");			//ドア開閉音
	lpSoundMng.GetID("resource/sound/DoorSE.mp3");				//ドア開閉音
	lpSoundMng.GetID("resource/sound/DoorCloseSE.mp3");			//ドア開閉音
	lpSoundMng.GetID("resource/sound/DrawerOpenSE.mp3");		//引き出し開閉音
	lpSoundMng.GetID("resource/sound/DrawerCloseSE.mp3");		//引き出し開閉音
	lpSoundMng.GetID("resource/sound/chaseBGM.mp3");			//追跡BGM
	lpSoundMng.GetID("resource/sound/gameBGM.mp3");				//通常BGM
	lpSoundMng.GetID("resource/sound/opbgm.mp3");				//OPBGM
	lpSoundMng.GetID("resource/sound/seaBGM.mp3");				//海のBGM
	lpSoundMng.GetID("resource/sound/TitleBGM.mp3");			//タイトルBGM
	lpSoundMng.GetID("resource/sound/horrornoise.mp3");			//死んだときのノイズに
	lpSoundMng.GetID("resource/sound/gameoverBGM.mp3");			//ゲームオーバー時のBGM
	lpSoundMng.GetID("resource/sound/DialSE.mp3");				//ゲームオーバー時のBGM
	lpSoundMng.GetID("resource/sound/run_Grass.mp3");			//草の上を走る音
	lpSoundMng.GetID("resource/sound/guuuSE.mp3");				//おなかのなる音
	lpSoundMng.GetID("resource/sound/takeApicture SE.mp3");		//撮影音

	//音量を設定する
	lpSoundMng.ChangeVolume(125, "./resource/sound/playerWalkSE.mp3");
	lpSoundMng.ChangeVolume(175, "./resource/sound/playerRunSE.mp3");
	lpSoundMng.ChangeVolume(175, "./resource/sound/playerRunVoice.mp3");
	lpSoundMng.ChangeVolume(200, "./resource/sound/gameBGM.mp3");
	lpSoundMng.ChangeVolume(255, "./resource/sound/TitleBGM.mp3");
	lpSoundMng.ChangeVolume(185, "./resource/sound/gameoverBGM.mp3");
	lpSoundMng.ChangeVolume(255, "./resource/sound/horrornoise.mp3");
	lpSoundMng.ChangeVolume(100, "resource/sound/GameOverSE.mp3"); 
	lpSoundMng.ChangeVolume(180, "resource/sound/opbgm.mp3");
	lpSoundMng.ChangeVolume(160, "./resource/sound/chaseBGM.mp3");
	lpSoundMng.ChangeVolume(255, "./resource/sound/HeatBeat.mp3");
	lpSoundMng.ChangeVolume(175, "resource/sound/run_Grass.mp3");
	lpSoundMng.ChangeVolume(155, "resource/sound/seaBGM.mp3");
	lpSoundMng.ChangeVolume(200, "resource/sound/guuuSE.mp3");
	lpSoundMng.ChangeVolume(200, "resource/sound/gamestartSE.mp3");



	SetUseASyncLoadFlag(false);

	// 3Dサウンドの読み込み
	SetCreate3DSoundFlag(true);
	lpSoundMng.GetID("./resource/sound/enemyWalkSE.mp3");	//敵歩行足音
	lpSoundMng.GetID("./resource/sound/WalkVoice.mp3");		//敵歩行ボイス
	lpSoundMng.GetID("./resource/sound/RunVoice.mp3");		//敵走行ボイス
	lpSoundMng.GetID("./resource/sound/GameOverSE.mp3");	//ゲームオーバー
	lpSoundMng.GetID("./resource/sound/HeatBeat.mp3");		//心臓音、追跡中に
	lpSoundMng.GetID("./resource/sound/jukeBoxBGM.mp3");	//ジュークボックストラップBGM
	lpSoundMng.GetID("./resource/sound/pianoBGM2.mp3");		//ピアノ、3種類
	lpSoundMng.GetID("./resource/sound/test.wav");			//テスト用
	SetCreate3DSoundFlag(false);

	//セットしてある音量をデフォルトに設定
	lpSoundMng.SetDefaultVolume();
	//lpSoundMng.ChangeVolume(255, "./resource/sound/RunVoice.mp3");
	lpSoundMng.ChangeVolume(200,"./resource/sound/jukeBoxBGM.mp3");	//ジュークボックストラップBGM

	GraphFilter(lpImageMng.GetID("bg")[0], DX_GRAPH_FILTER_GAUSS, 16, 500);
	GraphFilter(lpImageMng.GetID("bg")[0], DX_GRAPH_FILTER_HSB, 0, 0, 0, -150);
	GraphFilter(lpImageMng.GetID("selection")[0], DX_GRAPH_FILTER_HSB, 0, 0, 0, 205);
	GraphFilter(lpImageMng.GetID("selection1")[0], DX_GRAPH_FILTER_HSB, 0, 0, 0, 100);
	GraphFilter(lpImageMng.GetID("selection2")[0], DX_GRAPH_FILTER_HSB, 0, 0, 0, 256);
}

void BaseScene::AfterLoading()
{
	//非同期が走っていたら非同期追加は出来ない
	SetUseASyncLoadFlag(true);
	lpModelMng.GetID("resource/model/gimmick/Door_White1_Double.mv1");
	lpModelMng.GetID("resource/model/gimmick/Door_White2.mv1");
	lpModelMng.GetID("resource/model/gimmick/Door_White2_Double.mv1");
	lpModelMng.GetID("resource/model/gimmick/Door_White3.mv1");
	lpModelMng.GetID("resource/model/gimmick/Door_White3_Double.mv1");
	//赤
	lpModelMng.GetID("resource/model/gimmick/Door_Red1.mv1");
	lpModelMng.GetID("resource/model/gimmick/Door_Red2.mv1");
	lpModelMng.GetID("resource/model/gimmick/Door_Red3.mv1");
	//黒
	lpModelMng.GetID("resource/model/gimmick/Door_Dark1.mv1", 4);
	lpModelMng.GetID("resource/model/gimmick/Door_Dark1_Double.mv1", 5);
	lpModelMng.GetID("resource/model/gimmick/Door_Dark2.mv1");
	lpModelMng.GetID("resource/model/gimmick/Door_Dark2_Double.mv1");
	lpModelMng.GetID("resource/model/gimmick/Door_Dark3.mv1");
	//引き出し
	lpModelMng.GetID("resource/model/gimmick/drawer_frame_L.mv1", 8);
	lpModelMng.GetID("resource/model/gimmick/drawer.mv1", 32);
	//ダイアル
	lpModelMng.GetID("resource/model/gimmick/Dial.mv1", 4);
	//枠
	lpModelMng.GetID("resource/model/gimmick/Dial_Frame.mv1");
	lpModelMng.GetID("resource/model/gimmick/firstHint.mv1");
	lpModelMng.GetID("resource/model/gimmick/secondHint.mv1");
	lpModelMng.GetID("resource/model/gimmick/water.mv1");
	lpModelMng.GetID("resource/model/gimmick/JukeBox.mv1");
	//当たり判定用モデル
	//lpModelMng.GetID(gDirectory + "coll/CloseDoorColl.mv1", 40);

//アイテム
	lpModelMng.GetID("resource/model/Item/old_key.mv1", 6);
	lpModelMng.GetID("resource/model/Item/phot.mv1");
	lpModelMng.GetID("resource/model/Item/memo1.mv1", 9);
	lpModelMng.GetID("resource/model/Item/map.mv1");
	lpModelMng.GetID("resource/model/Item/book.mv1", 4);

	//ARマーカー
		/*lpModelMng.GetID("resource/model/marker/redMarker.mv1");
		lpModelMng.GetID("resource/model/marker/blueMarker.mv1");
		lpModelMng.GetID("resource/model/marker/greenMarker.mv1");
		lpModelMng.GetID("resource/model/marker/orangeMarker.mv1");
		lpModelMng.GetID("resource/model/marker/purpleMarker.mv1");*/

		//リザルトシーンのモデル
	lpModelMng.GetID("./resource/model/stage/cliff.mv1");
	lpModelMng.GetID("./resource/model/stage/dome/Skydome.mv1");
	lpModelMng.GetID("./resource/model/stage/water100.mv1");
	SetUseASyncLoadFlag(false);

}

