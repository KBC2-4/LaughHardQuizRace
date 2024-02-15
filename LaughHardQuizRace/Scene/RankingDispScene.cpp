#include "RankingDispScene.h"
#include "../Utility/InputControl.h"
#include "DxLib.h"
#include "../Utility/json.hpp"
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

	//エラーチェック
	if (background_image == -1)
	{
		throw("Resource/images/Scene/Ranking/background.pngがありません\n");
	}

	//ランキング情報を取得
	ranking = new RankingData;
	ranking->Initialize();
}

//更新処理
eSceneType RankingDispScene::Update()
{
	//Bボタンが押されたら、タイトルに戻る
	if (InputControl::GetButtonDown(XINPUT_BUTTON_B))
	{
		return eSceneType::E_TITLE;
	}

	return GetNowScene();
}

// std::wstring を Shift_JIS に変換する関数
std::string WStringToShiftJIS(const std::wstring& wstr) {
	int size_needed = WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string result(size_needed, 0);
	WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), &result[0], size_needed, NULL, NULL);
	return result;
}

//描画処理
void RankingDispScene::Draw()const
{
	//背景画像の描画
	DrawGraph(0, 0, background_image, FALSE);

	//取得したランキングデータを描画する
	for (int i = 0; i < 5; i++)
	{
		DrawFormatString(50, 170 + i * 25, 0xffffff, "%2d %-15s %6d",
			ranking->GetRank(i), ranking->GetName(i), ranking->GetScore(i));
	}

	// JSONデータのパース
	//try {
	//	json rankingData = json::parse(WStringToShiftJIS(ranking->GetData()).c_str());


	//	// ランキングデータの描画
	//	for (int i = 0; i < rankingData.size() && i < 5; ++i) {
	//		// JSONオブジェクトからデータを取得
	//		/*std::string playerName = rankingData[i]["PlayerID"];
	//		int score = rankingData[i]["Score"];*/
	//		std::string playerName;
	//		int score = 0;

	//		if (rankingData[i].contains("PlayerID") && rankingData[i]["PlayerID"].is_string()) {
	//			std::string playerName = rankingData[i].at("PlayerID").get<std::string>();
	//		}

	//		if (rankingData[i].contains("Score") && rankingData[i]["Score"].is_number_integer()) {
	//			int score = rankingData[i].at("Score").get<int>();
	//		}


	//		// DXライブラリで文字列を描画（ここではShift_JISに変換しています）
	//		std::wstring playerNameW(playerName.begin(), playerName.end());
	//		DrawFormatString(50, 170 + i * 25, GetColor(255, 255, 255), "%2d %-15s %6d",
	//			i + 1, playerNameW.c_str(), score);
	//	}
	//}
	//catch (json::parse_error& e) {
	//	// パースエラー
	//}
	//catch (json::type_error& e) {
	//	// 型エラー
	//}
	//catch (...) {
	//	// その他の例外
	//}

	//try {
	//	json rankingData = json::parse(WStringToShiftJIS(ranking->GetData()).c_str());

	//	for (int i = 0; i < rankingData.size() && i < 5; ++i) {
	//		// JSONオブジェクトからデータを取得（空の場合はデフォルト値を使用）
	//		std::string playerName = rankingData[i].value("PlayerID", "");
	//		int score = rankingData[i].value("Score", 0); // スコアが空の場合は0とする

	//		// 空の行を無視する
	//		if (playerName.empty() && score == 0) continue;

	//		// DXライブラリで文字列を描画
	//		std::wstring playerNameW(playerName.begin(), playerName.end());
	//		DrawFormatString(50, 170 + i * 25, GetColor(255, 255, 255), "%2d %-15s %6d",
	//			i + 1, playerNameW.c_str(), score);
	//	}
	//}
	//catch (const json::exception& e) {
	//	// JSONの例外処理
	//}

	// JSON形式のランキングデータ
	//std::string jsonData = R"([
 //       {"PlayerID": "Player1", "Score": 100},
 //       {"PlayerID": "Player2", "Score": 95},
 //   ])";

	// データのパースとソート
	try {
		auto rankingEntries = ranking->ParseAndSortRankingData(WStringToShiftJIS(ranking->GetData()).c_str());

		// ランキングデータの描画
		for (size_t i = 0; i < rankingEntries.size() && i < 5; ++i) {
			std::wstring playerNameW(rankingEntries[i].playerName.begin(), rankingEntries[i].playerName.end());
			DrawFormatString(50, 170 + i * 25, GetColor(255, 255, 255), "%d %-15s %d",
				i + 1, playerNameW.c_str(), rankingEntries[i].score_);
		}
	}
	catch (const json::exception& e) {
	}
	// 取得したランキングデータを表示（ここでは単純に全データを文字列として表示）
	//DrawString(0, 0, WStringToShiftJIS(ranking->GetData()).c_str(), GetColor(255, 255, 255));
}


//終了時処理
void RankingDispScene::Finalize()
{
	//読み込んだ画像の削除
	DeleteGraph(background_image);

	//動的メモリの解放
	ranking->Finalize();
	delete ranking;
}


//現在のシーン情報を取得
eSceneType RankingDispScene::GetNowScene()const
{
	return eSceneType::E_RANKING_DISP;
}