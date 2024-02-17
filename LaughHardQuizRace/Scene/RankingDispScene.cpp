#include "RankingDispScene.h"

#include <iostream>

#include "../Utility/InputControl.h"
#include "DxLib.h"
#include "../Utility/json.hpp"
#include "../Utility/Guide.h"
using json = nlohmann::json;

RankingDispScene::RankingDispScene() :background_image(NULL), ranking(nullptr)
{

}

RankingDispScene::~RankingDispScene()
{

}


//初期化処理
void RankingDispScene::Initialize()
{
	//画像の読み込み
	background_image = LoadGraph("Resource/images/Scene/Ranking/background.png");

	font_handle_h2 = CreateFontToHandle("Segoe UI", 50, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	font_handle_h3 = CreateFontToHandle("Segoe UI", 20, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);
	font_handle_h1 = CreateFontToHandle("Segoe UI", 100, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);
	buttonGuidFont = CreateFontToHandle("メイリオ", 23, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

	const int result = LoadDivGraph("Resource/images/Scene/Ranking/crowns.png", 3, 3, 1, 66, 48,
		crown_image);

	//エラーチェック
	if (background_image == -1)
	{
		throw("Resource/images/Scene/Ranking/background.pngがありません\n");
	}

	if (result == -1)
	{
		throw("Resource/images/Scene/Ranking/crowns.pngがありません\n");
	}

	//ランキング情報を取得
	ranking = new RankingData;
	ranking->Initialize();
}

//更新処理
eSceneType RankingDispScene::Update()
{
	//Bボタンが押されたら、タイトルに戻る
	if (InputControl::GetButtonDown(XINPUT_BUTTON_A))
	{
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

	//std::wstring wstrData = ranking->GetData();
	//std::string strData = WStringToShiftJIS(wstrData);
	//clsDx();
	//printfDx("Ranking Data: %s\n", strData.c_str());

	// データ取得後
	std::wstring rawData = ranking->GetData();
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
		printfDx("JSON Parse Error: %s\n", e.what());
	}
	catch (...) {
		// その他のエラー
		printfDx("エラーが発生しました。\n");
	}



	const std::vector<guideElement> gamepad_guides = {
guideElement({"A"}, "タイトルへ", GUIDE_SHAPE_TYPE::FIXED_CIRCLE,
		 buttonGuidFont, 0xFFFFFF, 0xEBE146,
		 0xEB3229, 0xFFFFFF),
	};

	// ボタンガイドの描画
	DrawGuides(gamepad_guides, 505.0f, 660.0f, 5.0f, 60.0f);
}


//終了時処理
void RankingDispScene::Finalize()
{
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