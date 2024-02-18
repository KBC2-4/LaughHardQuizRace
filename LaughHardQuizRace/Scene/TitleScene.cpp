#include "TitleScene.h"
#include "../Utility/InputControl.h"
#include "DxLib.h"
#include "../Utility/Guide.h"

TitleScene::TitleScene() :background_image(NULL), menu_image(NULL), scroll(0), title_image(NULL), background_sound(NULL),
cursor_image(NULL), menu_cursor(0),cursor_move_se(0),enter_se(0), client(L"AKfycbyCbERdoRMfYxiJLrMaQMS1grPSBHRDSPtoPvH0yrY-7QTZjRLeBYgBqA8zTUY56GtL")
{
	buttonGuidFont = CreateFontToHandle("メイリオ", 23, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

	//enemyManager = EnemyManager();
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
	background_sound = LoadSoundMem("Resource/sounds/bgm/title.mp3");
	enter_se = LoadSoundMem("Resource/sounds/se/enter.mp3");
	cursor_move_se = LoadSoundMem("Resource/sounds/se/cursor_move.mp3");

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
		throw("Resource/sounds/bgm/title.mp3がありません\n");
	}

	if (enter_se == -1)
	{
		throw("Resource/sounds/se/enter.mp3がありません\n");
	}

	if (cursor_move_se == -1)
	{
		throw("Resource/sounds/se/cursor_move.mp3がありません\n");
	}

	// スプレッドシートのデータを取得
	//client.GetSpreadsheetData();

	const auto task = client.GetPlayCount();
	// 非同期タスクの結果を同期的に待つ
	play_count = task.get();

	//BGMの再生
	PlaySoundMem(background_sound, DX_PLAYTYPE_LOOP, FALSE);

	//enemyManager.LoadImages();

	//ランキング情報を取得
	ranking = new RankingData;
	ranking->Initialize();
}


//更新処理
eSceneType TitleScene::Update()
{

	// 背景画像を無限スクロール
	if(scroll <= 1280)
	{
		scroll++;
	}else
	{
		scroll = 0;
	}

	// 左スティックの倒された方向を取得
	const StickDirection direction = InputControl::GetLStickDirection(20000, 10);

	//カーソル下移動
	if (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_DOWN) 
		|| direction == StickDirection::Down)
	{
		// SE再生
		PlaySoundMem(cursor_move_se, DX_PLAYTYPE_BACK, TRUE);
		menu_cursor = (menu_cursor + 1) % 4;
	}


	//カーソル上移動
	if (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_UP) 
		|| direction == StickDirection::Up)
	{
		// SE再生
		PlaySoundMem(cursor_move_se, DX_PLAYTYPE_BACK, TRUE);
		menu_cursor = (menu_cursor + 3) % 4;
	}

	//カーソル決定（決定した画面に遷移する）
	if (InputControl::GetButtonDown(XINPUT_BUTTON_A))
	{
		// SE再生
		PlaySoundMem(enter_se, DX_PLAYTYPE_BACK, TRUE);
		//SEが鳴り終わってから画面推移する。
		/*while (CheckSoundMem(enter_se)) {}*/

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

	//enemyManager.Update();

	//現在のシーンタイプを返す
	return GetNowScene();
}


//描画処理
void TitleScene::Draw()const
{

	//タイトル画像の描画
	DrawGraph(scroll % 1280 - 1280, 0, background_image, TRUE);
	DrawGraph(scroll % 1280, 0, background_image, TRUE);

	//enemyManager.Draw();

	//DrawGraph(0, 0, background_image, FALSE);
	DrawGraph(300, 70, title_image, TRUE);

	//メニュー画像の描画
	DrawGraph(420, 300, menu_image, TRUE);

	//カーソル画像の描画
	DrawRotaGraph(430, 350 + menu_cursor * 75, 0.7, DX_PI / 0.1, cursor_image, TRUE);
	DrawRotaGraph(800, 350 + menu_cursor * 75, 0.7, DX_PI / 0.1, cursor_image, TRUE);
	//std::string shifted_play_count = WStringToShiftJIS(play_count);
	//DrawFormatString(0, 0, 0xffffff, "プレイ回数: %s", shifted_play_count.c_str());

	/*使用する文字コードを utf8 に設定*/
	//SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);

	DrawFormatStringToHandle(50, 100, 0xffffff, buttonGuidFont, "プレイ回数: %d", play_count);
	/*使用する文字コードを shift-jis に設定*/
	//SetUseCharCodeFormat(DX_CHARCODEFORMAT_SHIFTJIS);

	const std::vector<guideElement> gamepad_guides = {
					guideElement({"L"}, "移動", GUIDE_SHAPE_TYPE::JOYSTICK, buttonGuidFont, 0x000000,
						 0xFFFFFF, 0xFFFFFF),
guideElement({"A"}, "決定", GUIDE_SHAPE_TYPE::FIXED_CIRCLE,
		 buttonGuidFont, 0xFFFFFF, A_BUTTON_COLOR,
		 0xEB3229, 0xFFFFFF),
	};

	// ボタンガイドの描画
	DrawGuides(gamepad_guides, 505.0f, 660.0f, 5.0f, 60.0f);

	//取得したランキングデータを描画する
	//for (int i = 0; i < 1; i++)
	//{
	//	/*DrawFormatStringToHandle(50, 170 + i * 25, 0xffffff, buttonGuidFont, "%2d %-15s %6d",
	//		ranking->GetRank(i), ranking->GetName(i), ranking->GetScore(i));*/
	//		DrawFormatStringToHandle(50, 170 + i * 25, 0xffffff, buttonGuidFont, "前回のスコア：%6d",
	//			ranking->GetScore(i));
	//}
}


//終了時処理
void TitleScene::Finalize()
{
	//シーンの切り替えが行われたらBGMを止める
	StopSoundMem(background_sound);
	DeleteSoundMem(background_sound);

	DeleteSoundMem(enter_se);
	DeleteSoundMem(cursor_move_se);

	DeleteFontToHandle(buttonGuidFont);

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