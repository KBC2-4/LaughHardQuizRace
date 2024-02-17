#include "DxLib.h"
#include "../Utility/InputControl.h"
#include "ResultScene.h"
#include "../Object/RankingData.h"
#include "../Utility/Guide.h"
#include "../Utility/GameData.h"

ResultScene::ResultScene() :background_image(0), score(GameData::GetScore())
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

	font_handle_h1 = CreateFontToHandle("Segoe UI", 180, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	font_handle_h2 = CreateFontToHandle("Segoe UI", 160, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);
	buttonGuidFont = CreateFontToHandle("メイリオ", 23, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

	//BGMの読み込み
	background_sound = LoadSoundMem("Resource/sounds/bgm/title.mp3");

	if (background_sound == -1)
	{
		throw("Resource/sounds/bgm/title.mp3がありません\n");
	}

	//BGMの再生
	PlaySoundMem(background_sound, DX_PLAYTYPE_LOOP, FALSE);

	// スコアの取得
	this->score = GameData::GetScore();;
}


//更新処理
eSceneType ResultScene::Update()
{
	//Bボタンでランキングに遷移する
	if (InputControl::GetButtonDown(XINPUT_BUTTON_A))
	{
		//return eSceneType::E_TITLE;
		//return eSceneType::E_RANKING_INPUT;
	}

	return GetNowScene();
}


//描画処理
void ResultScene::Draw()const
{
	//背景画像を描画
	DrawGraph(0, 0, background_image, TRUE);

	DrawStringToHandle(440, 80, "スコア", 0x10B3E6, font_handle_h1, 0xE67B10);
	DrawFormatString2ToHandle(370, 430, 0xE6106E, 0x10E617, font_handle_h2, "%6d", score);

	const std::vector<guideElement> gamepad_guides = {
guideElement({"A"}, "タイトルへ", GUIDE_SHAPE_TYPE::FIXED_CIRCLE,
		 buttonGuidFont, 0xFFFFFF, 0xEBE146,
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

	//読み込んだ画像を削除
	DeleteGraph(background_image);

	// フォントの削除
	DeleteFontToHandle(font_handle_h1);
	DeleteFontToHandle(font_handle_h2);
	DeleteFontToHandle(buttonGuidFont);
}


//現在のシーン情報を取得
eSceneType ResultScene::GetNowScene()const
{
	return eSceneType::E_RESULT;
}