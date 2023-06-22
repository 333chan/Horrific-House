	〇お願い〇

①.hlslはこのフォルダに作成。
②.psoや.vsoは別フォルダ。以下パス(全てコピペで可)。
プロパティ→HLSLコンパイラ→出力ファイル→オブジェクトファイル名:

				ピクセルシェーダ: $(ProjectDir)/resource/shader/ps/%(Filename).pso
				頂点シェーダ	: $(ProjectDir)/resource/shader/vs/%(Filename).vso

③ *_vs.hlsl, *_ps.hlsl, のように最後に_psか_vsをつけること。