#include "DxLib.h"
#include "../Utility/InputControl.h"
#include "ResultScene.h"
#include "../Object/RankingData.h"
#include "../Utility/Guide.h"
#include "../Utility/GameData.h"
#include "../Utility/SpreadsheetClient.h"

ResultScene::ResultScene() :background_image(0), enter_se(0), score(GameData::GetScore()), is_score_in_top10(false), data_loaded(false)
{
}

ResultScene::~ResultScene()
{
}


//初期化処理
void ResultScene::Initialize()
{
	//画像の読み込み
	background_image = LoadGraph("Resource/images/Scene/Result/background.png");

	//エラーチェック
	if (background_image == -1)
	{
		throw("Resource/images/Scene/Result/background.pngがありません\n");
	}

	font_handle_h1 = CreateFontToHandle("Segoe UI", 180, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, 5);
	score_font_handle = CreateFontToHandle("Segoe UI", 160, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, 3, 1);
	font_handle_h3 = CreateFontToHandle("Segoe UI", 60, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, 3, 1);
	buttonGuidFont = CreateFontToHandle("メイリオ", 23, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

	//BGMの読み込み
	background_sound = LoadSoundMem("Resource/sounds/bgm/title.mp3");

	enter_se = LoadSoundMem("Resource/sounds/se/enter.mp3");

	if (background_sound == -1)
	{
		throw("Resource/sounds/bgm/title.mp3がありません\n");
	}

	if (enter_se == -1)
	{
		throw("Resource/sounds/se/enter.mp3がありません\n");
	}

	//BGMの再生
	PlaySoundMem(background_sound, DX_PLAYTYPE_LOOP, FALSE);

	// スコアの取得
	this->score = GameData::GetScore();

	// スクリプトIDを設定
	SpreadsheetClient client(L"AKfycbwTncJE7fNBzWd4aZqG7Q7_ysP9XTNcV91VlStNzjDP3Xw95oEKgYwiNVQgU73AXxpv");

	client.IsScoreInTop10(this->score).then([this](pplx::task<bool> task) {
		try {
			// 例外が発生しなかった場合には正常にロードされたとみなす
			this->is_score_in_top10 = task.get();
			this->data_loaded = true;
		}
		catch (...) {
			// エラー処理: 例外が発生した場合
			this->is_score_in_top10 = false;
			// ロードされていなくても続行させる
			this->data_loaded = true;
		}
	});
}


//更新処理
eSceneType ResultScene::Update()
{
	//Bボタンでランキングに遷移する
	if (InputControl::GetButtonDown(XINPUT_BUTTON_A))
	{

		// データロードの試行の結果が届いていない場合は、現在のシーンを返す
		if (!data_loaded)
		{
			return GetNowScene();
		}
		// SE再生
		PlaySoundMem(enter_se, DX_PLAYTYPE_BACK, TRUE);
		//SEが鳴り終わってから画面推移する。
		while (CheckSoundMem(enter_se)) {}
		return eSceneType::E_RANKING_INPUT;
	}

	return GetNowScene();
}


//描画処理
void ResultScene::Draw()const
{
	//背景画像を描画
	DrawGraph(0, 0, background_image, TRUE);

	DrawStringToHandle(GetDrawCenterX("スコア", font_handle_h1), 30, "スコア", 0x4532ED, font_handle_h1, 0xffffff);
	//const char score_char = score;
	DrawFormatString2ToHandle(GetDrawCenterX(std::to_string(score).c_str(), score_font_handle), 400, 0xE6106E, 0xffffff, score_font_handle, "%d", score);

	if (data_loaded == true) {
		if (is_score_in_top10)
		{
			DrawStringToHandle(GetDrawCenterX("おめでとうございます！", font_handle_h3), 230,
				"おめでとうございます！", 0xEF9309, font_handle_h3, 0xffffff);
			DrawStringToHandle(GetDrawCenterX("ランキング10位以内に入りました！", font_handle_h3), 310,
				"ランキング10位以内に入りました！", 0xEF9309, font_handle_h3, 0xffffff);
		}
		else
		{
			DrawStringToHandle(GetDrawCenterX("ランキング10位以内に入りませんでした。", font_handle_h3), 230,
				"ランキング10位以内に入りませんでした。", 0xED4132, font_handle_h3, 0xffffff);
			DrawStringToHandle(GetDrawCenterX("が、ランキングにスコアを残しましょう！", font_handle_h3), 310,
				"が、ランキングにスコアを残しましょう！", 0xED4132, font_handle_h3, 0xffffff);
		}
	}

	const std::vector<guideElement> gamepad_guides = {
guideElement({"A"}, "ランキング入力画面へ", GUIDE_SHAPE_TYPE::FIXED_CIRCLE,
		 buttonGuidFont, 0xFFFFFF, A_BUTTON_COLOR,
		 0xEB3229, 0xFFFFFF),
	};

	// ボタンガイドの描画
	DrawGuides(gamepad_guides, 505.0f, 660.0f, 5.0f, 60.0f);
}


//終了時処理
void ResultScene::Finalize()
{
	//シーンの切り替えが行われたらBGMを止める
	StopSoundMem(background_sound);
	DeleteSoundMem(background_sound);

	DeleteSoundMem(enter_se);

	//読み込んだ画像を削除
	DeleteGraph(background_image);

	// フォントの削除
	DeleteFontToHandle(font_handle_h1);
	DeleteFontToHandle(score_font_handle);
	DeleteFontToHandle(font_handle_h3);
	DeleteFontToHandle(buttonGuidFont);
}


//現在のシーン情報を取得
eSceneType ResultScene::GetNowScene()const
{
	return eSceneType::E_RESULT;
}