#include "TitleScene.h"
#include "../Utility/InputControl.h"
#include "DxLib.h"

TitleScene::TitleScene() :background_image(NULL), menu_image(NULL),
cursor_image(NULL), menu_cursor(0), client(L"AKfycbyoJ4KKmOTRqUji0Tycg6710ZE8SlRKMCuXO9YtUzQ0ZhPx2-WO5yCKKM8xMA8fbthB")
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
	menu_image = LoadGraph("Resource/images/menu.bmp");
	cursor_image = LoadGraph("Resource/images/cone.bmp");

	//エラーチェック
	if (background_image == -1)
	{
		throw("Resource/images/Scene/Title/background.pngがありません\n");
	}
	if (menu_image == -1)
	{
		throw("Resource/images/menu.bmpがありません\n");
	}
	if (cursor_image == -1)
	{
		throw("Resource/images/cursor.bmpがありません\n");
	}

	// スプレッドシートのデータを取得
	client.GetSpreadsheetData();

	// スプレッドシートにデータを書き込み
	client.PostSpreadsheetData(L"player123", 1000);

	const auto task = client.GetPlayCount();
	// 非同期タスクの結果を同期的に待つ
	play_count = task.get();

	// 使用する文字コードを utf8 に設定
	//SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);

	//printfDx("PlayCount: %s\n", play_count.c_str());
}


//更新処理
eSceneType TitleScene::Update()
{
	//カーソル下移動
	if (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_DOWN) || InputControl::GetLStickDirection() == StickDirection::Down)
	{
		menu_cursor++;
		//１番下に到達したら、一番上にする
		if (menu_cursor > 3)
		{
			menu_cursor = 0;
		}
	}


	//カーソル上移動
	if (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_UP) || InputControl::GetLStickDirection() == StickDirection::Up)
	{
		menu_cursor--;
		//１番上に到達したら、一番下にする
		if (menu_cursor < 0)
		{
			menu_cursor = 3;
		}
	}

	//カーソル決定（決定した画面に遷移する）
	if (InputControl::GetButtonDown(XINPUT_BUTTON_B))
	{
		switch (menu_cursor)
		{
			case 0:
				// プレイ回数をインクリメント
				client.IncrementPlayCount();
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
	DrawGraph(0, 0, background_image, FALSE);

	//メニュー画像の描画
	DrawGraph(120, 200, menu_image, TRUE);

	//カーソル画像の描画
	DrawRotaGraph(90, 220 + menu_cursor * 40, 0.7, DX_PI / 2.0, cursor_image, TRUE);

	//std::string shifted_play_count = WStringToShiftJIS(play_count);
	//DrawFormatString(0, 0, 0xffffff, "プレイ回数: %s", shifted_play_count.c_str());
	DrawFormatString(0, 0, 0xffffff, "プレイ回数: %d", play_count);
}


//終了時処理
void TitleScene::Finalize()
{
	//読み込んだ画像の削除
	DeleteGraph(background_image);
	DeleteGraph(menu_image);
	DeleteGraph(cursor_image);
}


//現在のシーン情報を取得
eSceneType TitleScene::GetNowScene()const
{
	return eSceneType::E_TITLE;
}