#include "TitleScene.h"
#include "../Utility/InputControl.h"
#include "DxLib.h"

TitleScene::TitleScene() :background_image(NULL), menu_image(NULL), scroll(0), title_image(NULL), background_sound(NULL),
cursor_image(NULL), menu_cursor(0)/*, client(L"AKfycbyoJ4KKmOTRqUji0Tycg6710ZE8SlRKMCuXO9YtUzQ0ZhPx2-WO5yCKKM8xMA8fbthB")*/
{

}

TitleScene::~TitleScene()
{

}


//初期化処理
void TitleScene::Initialize()
{
	//画像の読み込み
	background_image = LoadGraph("Resource/images/Scene/Title/background.png");
	menu_image = LoadGraph("Resource/images/menu.png");
	cursor_image = LoadGraph("Resource/images/cone.png");
	title_image = LoadGraph("Resource/images/title2.png");

	//BGMの読み込み
	background_sound = LoadSoundMem("Resource/sounds/bgm/Electric_Shine.mp3");

	//エラーチェック
	if (background_image == -1)
	{
		throw("Resource/images/Scene/Title/background.pngがありません\n");
	}
	if (menu_image == -1)
	{
		throw("Resource/images/menu.pngがありません\n");
	}
	if (cursor_image == -1)
	{
		throw("Resource/images/cursor.pngがありません\n");
	}
	if (title_image == -1)
	{
		throw("Resource/images/title2.pngがありません\n");
	}

	if (background_sound == -1)
	{
		throw("Resource/sounds/bgm/Electric_Shine.mp3がありません\n");
	}
	// スプレッドシートのデータを取得
	//client.GetSpreadsheetData();

	//// スプレッドシートにデータを書き込み
	//client.PostSpreadsheetData(L"player123", 1000);

	//const auto task = client.GetPlayCount();
	//// 非同期タスクの結果を同期的に待つ
	//play_count = task.get();

	// 使用する文字コードを utf8 に設定
	//SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);

	//printfDx("PlayCount: %s\n", play_count.c_str());

	//BGMの再生
	PlaySoundMem(background_sound, DX_PLAYTYPE_BACK, FALSE);
}


//更新処理
eSceneType TitleScene::Update()
{

	//カーソル下移動
	if (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_DOWN) || InputControl::GetLStickDirection() == StickDirection::Down)
	// 背景画像を無限スクロール
	if(scroll <= 1280)
	{
		scroll++;
	}else
	{
		scroll = 0;
	}

	// 左スティックの倒された方向を取得
	const StickDirection direction = InputControl::GetLStickDirection(20000, 0);

	//カーソル下移動
	if (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_DOWN) 
		|| direction == StickDirection::Down)
	{
		menu_cursor = (menu_cursor + 1) % 4;
	}


	//カーソル上移動
	if (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_UP) 
		|| direction == StickDirection::Up)
	{
		menu_cursor = (menu_cursor + 3) % 4;
	}

	//カーソル決定（決定した画面に遷移する）
	if (InputControl::GetButtonDown(XINPUT_BUTTON_B))
	{
		switch (menu_cursor)
		{
			case 0:
				// プレイ回数をインクリメント
				//client.IncrementPlayCount();
				return eSceneType::E_MAIN;
				
			case 1:
				return eSceneType::E_RANKING_DISP;

			case 2:
				return eSceneType::E_HELP;

			case 3:
				return eSceneType::E_END;
		}
	}

	//現在のシーンタイプを返す
	return GetNowScene();
}


//描画処理
void TitleScene::Draw()const
{
	//タイトル画像の描画
	DrawGraph(scroll % 1280 - 1280, 0, background_image, TRUE);
	DrawGraph(scroll % 1280, 0, background_image, TRUE);
	//DrawGraph(0, 0, background_image, FALSE);
	DrawGraph(300, 70, title_image, TRUE);

	//メニュー画像の描画
	DrawGraph(420, 300, menu_image, TRUE);

	//カーソル画像の描画
	DrawRotaGraph(430, 350 + menu_cursor * 75, 0.7, DX_PI / 0.1, cursor_image, TRUE);
	DrawRotaGraph(800, 350 + menu_cursor * 75, 0.7, DX_PI / 0.1, cursor_image, TRUE);
	//std::string shifted_play_count = WStringToShiftJIS(play_count);
	//DrawFormatString(0, 0, 0xffffff, "プレイ回数: %s", shifted_play_count.c_str());
	DrawFormatString(0, 0, 0xffffff, "プレイ回数: %d", play_count);

	
}


//終了時処理
void TitleScene::Finalize()
{
	//シーンの切り替えが行われたらBGMを止める
	StopSoundMem(background_sound);
	DeleteSoundMem(background_sound);

	//読み込んだ画像の削除
	DeleteGraph(background_image);
	DeleteGraph(title_image);
	DeleteGraph(menu_image);
	DeleteGraph(cursor_image);
}


//現在のシーン情報を取得
eSceneType TitleScene::GetNowScene()const
{
	return eSceneType::E_TITLE;
}