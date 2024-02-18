#include "RankingDispScene.h"

#include <iostream>

#include "../Utility/InputControl.h"
#include "DxLib.h"
#include "../Utility/json.hpp"
#include "../Utility/Guide.h"
#include "../Utility/GameData.h"
using json = nlohmann::json;

RankingDispScene::RankingDispScene() :background_image(NULL), ranking(nullptr),
font_handle_h2(0), font_handle_h3(0), font_handle_h1(0), buttonGuidFont(0), rawData(L"")
{
	for (int i = 0; i < 3; i++)
	{
		crown_image[i] = NULL;
	}
}


RankingDispScene::~RankingDispScene()
{

}


//初期化処理
void RankingDispScene::Initialize()
{
	// 画像の読み込み
	background_image = LoadGraph("Resource/images/Scene/Ranking/background.png");

	font_handle_h2 = CreateFontToHandle("Segoe UI", 50, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	font_handle_h3 = CreateFontToHandle("Segoe UI", 20, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);
	font_handle_h1 = CreateFontToHandle("Segoe UI", 100, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);
	buttonGuidFont = CreateFontToHandle("メイリオ", 23, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

	const int result = LoadDivGraph("Resource/images/Scene/Ranking/crowns.png", 3, 3, 1, 66, 48,
		crown_image);

	//BGMの読み込み
// 前回の再生途中のBGMそれを再生
	if (GameData::GetPrevBGM() == -1)
	{
		background_sound = LoadSoundMem("Resource/sounds/bgm/ranking.mp3");
		if (background_sound == -1)
		{
			throw("Resource/sounds/bgm/ranking.mp3がありません\n");
		}
	}
	else
	{
		background_sound = GameData::GetPrevBGM();
	}
	// SEの読み込み
	enter_se = LoadSoundMem("Resource/sounds/se/enter.mp3");

	// エラーチェック
	if (background_image == -1)
	{
		throw("Resource/images/Scene/Ranking/background.pngがありません\n");
	}

	if (result == -1)
	{
		throw("Resource/images/Scene/Ranking/crowns.pngがありません\n");
	}

	if (enter_se == -1)
	{
		throw("Resource/sounds/se/enter.mp3がありません\n");
	}

	//ランキング情報を取得
	ranking = new RankingData;
	ranking->Initialize();

	//BGMの再生
	PlaySoundMem(background_sound, DX_PLAYTYPE_LOOP, FALSE);
}

//更新処理
eSceneType RankingDispScene::Update()
{

	//std::wstring wstrData = ranking->GetData();
	//std::string strData = WStringToShiftJIS(wstrData);
	//clsDx();
	//printfDx("Ranking Data: %s\n", strData.c_str());

	// データ取得後
	this->rawData = ranking->GetData();
	//{	//! デバッグ用
	//printfDx("Raw Data Length: %d\n", rawData.length());

	//// 変換後
	//std::string shiftJISData = WStringToShiftJIS(rawData);
	//printfDx("Converted Data Length: %d\n", shiftJISData.length());
	//printfDx("Converted Data: %s\n", shiftJISData.c_str());

	//// パース前
	//if (!shiftJISData.empty() && shiftJISData.back() != '}' && shiftJISData.back() != ']') {
	//	printfDx("Warning: JSON data might be incomplete.\n");
	//}
	//}

	//Bボタンが押されたら、タイトルに戻る
	if (InputControl::GetButtonDown(XINPUT_BUTTON_A))
	{
		// SE再生
		PlaySoundMem(enter_se, DX_PLAYTYPE_BACK, TRUE);
		//SEが鳴り終わってから画面推移する。
		while (CheckSoundMem(enter_se)) {}

		// ランキングデータ取得中の場合は、推移しない
		if (rawData.empty())
		{
			return GetNowScene();
		}
		return eSceneType::E_TITLE;
	}



	return GetNowScene();
}

// std::wstring を Shift_JIS に変換する関数
std::string WStringToShiftJIS(const std::wstring& wstr) {
	int size_needed = WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL) + 1; // +1 for null terminator
	std::string result(size_needed, '\0');
	WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), &result[0], size_needed, NULL, NULL);
	return result;
}

//描画処理
void RankingDispScene::Draw()const
{
	//背景画像の描画
	DrawGraph(0, 0, background_image, FALSE);

	//DrawStringToHandle(400, 30, "ランキング", 0x0C74FF, font_handle_h1, 0xffffff);


	if (rawData.empty())
	{
		DrawStringToHandle(370, 200, "読み込み中...", 0x0C74FF, font_handle_h1, 0xffffff);
	}

	constexpr int base_x = 240;
	constexpr int base_y = 60;

	try {
		auto json = nlohmann::json::parse(rawData);

		// JSONデータが配列またはオブジェクトの場合
		if (json.is_array() || json.is_object()) {
			int i = 1; // ランキング順位
			int y = base_y; // Y座標の初期値
			for (auto& element : json) {
				// PlayerIDとScoreを取得
				std::string playerID = element["PlayerID"].get<std::string>();
				int score = element["Score"].get<int>();
				std::string addDate = element["Date"].get<std::string>();


				// UTF-8からShift-JISへ変換
				// 使用する文字コードを utf8 に設定
				SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
				if (i <= 3)
				{
					DrawGraph(base_x - 60, y + 5, crown_image[i - 1], TRUE);
				}
				DrawFormatString2ToHandle(base_x, y, 0x0C74FF, 0xffffff, font_handle_h2, u8"%2d位", i);
				DrawStringToHandle(base_x + 140, y, playerID.c_str(), 0x0C74FF, font_handle_h2, 0xffffff);
				DrawFormatString2ToHandle(base_x + 440, y, 0x0C74FF, 0xffffff, font_handle_h2, u8"%-d", score);
				DrawStringToHandle(base_x + 600, y, addDate.c_str(), 0x0C74FF, font_handle_h3, 0xffffff);
				y += 60; // 次の項目のためにY座標を下げる
				i++;

				// 使用する文字コードを shift-jis に設定
				SetUseCharCodeFormat(DX_CHARCODEFORMAT_SHIFTJIS);
			}
		}
	}
	catch (nlohmann::json::parse_error& e) {
		// パースエラー
		std::cerr << "JSON Parse Error: " << e.what() << std::endl;
	}
	catch (...) {
		// その他のエラー
		std::cerr << "エラーが発生しました。\n";
	}



	const std::vector<guideElement> gamepad_guides = {
guideElement({"A"}, "タイトルへ", GUIDE_SHAPE_TYPE::FIXED_CIRCLE,
		 buttonGuidFont, 0xFFFFFF, A_BUTTON_COLOR,
		 0xEB3229, 0xFFFFFF),
	};

	// ボタンガイドの描画
	DrawGuides(gamepad_guides, 505.0f, 660.0f, 5.0f, 60.0f);
}


//終了時処理
void RankingDispScene::Finalize()
{
	//シーンの切り替えが行われたらBGMを止める
	StopSoundMem(background_sound);
	// 格納されたBGMと異なる場合は削除
	if (GameData::GetPrevBGM() != background_sound)
	{
		DeleteSoundMem(background_sound);
	}

	// 次のシーンに同じBGMは使用していないため、初期化
	GameData::SetPrevBGM(-1);

	DeleteSoundMem(enter_se);

	//読み込んだ画像の削除
	DeleteGraph(background_image);

	for (int i = 0; i < 3; i++)
	{
		DeleteGraph(crown_image[i]);
	}

	// フォントの削除
	DeleteFontToHandle(font_handle_h2);
	DeleteFontToHandle(font_handle_h3);
	DeleteFontToHandle(font_handle_h1);
	DeleteFontToHandle(buttonGuidFont);


	//動的メモリの解放
	ranking->Finalize();
	delete ranking;
}


//現在のシーン情報を取得
eSceneType RankingDispScene::GetNowScene()const
{
	return eSceneType::E_RANKING_DISP;
}