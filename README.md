# Horrific-House
〇作成期間：3年生後期/4ヶ月

〇チーム制作:5人

〇制作目的:学内コンテストに向けて、チームメンバーのやりたい事に力を入れたゲームを制作することを目標に制作しました。

〇動作確認したOS、バージョン

  ・Windows 10 Education 24H2 64bit

〇開発に使用したライブラリ、ミドルウェアとそのバージョン
  
  ・DXライブラリ 3.23
  
〇開発に使用したエンジンのバージョン(ステージのモデル制作、ARアプリの制作に使用)
  
  ・Unity 2020.3.30f1
  
〇開発に使用した開発環境のバージョン
  
  ・Visual Studio Professional 2022 (64bit) - LTSC 17.0

〇担当箇所

・プログラム

・Player：アイテムの取得、インベントリに追加
	class/object/player/Player.cpp
	
・Itme：モデル、アイテムリストの制作
	class/object/Itme/..
	
その他

・企画

・Stage：モデル制作、描画

・Enamy：モデルの収集、アニメーションの制作

・InventoryScene：アイテム説明の制作・描画

・素材の収集

〇ARモードについて

対応機種はAndroidのみです。

ARモードで遊ぶためには、専用アプリをスマートフォンにDLしてください。

1:ゲームを起動するPCとスマートフォンを同じネットワークに接続。

2:ゲームを起動してセレクトデバイスからスマートフォンを選択、画面の指示に従う

※アプリがQRコードからインストールできない場合、

  「ARapp」フォルダにある「ARhorror.apk」をスマートフォンに
  
   インストールしてください。
  
※ゲームパッドを接続状態でゲームを起動するとゲームパッドの操作が優先され

  マウスでの操作ができません
  
接続できない場合

スマホとPCの接続されているネットワークを同じにしてください。

wifiの設定がプライベートになっている可能性があります、パブリックにしておいてください。

IPv4が複数あり、今回使用するIPが取れてない可能性があります。

下記の作業をしてみてください。

①コマンドプロンプトを起動

②ipconfigと入力

③IPv4 アドレス を探して

④IPv4 アドレス・・・・:の数字をスマホで入力
