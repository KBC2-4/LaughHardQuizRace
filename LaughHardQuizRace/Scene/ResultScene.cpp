#include "DxLib.h"
#include "../Utility/InputControl.h"
#include "ResultScene.h"
#include "../Object/RankingData.h"

ResultScene::ResultScene() :background_image(0), score(0)
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

	font_handle_h2 = CreateFontToHandle("Segoe UI", 50, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	font_handle_h3 = CreateFontToHandle("Segoe UI", 20, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);

	//ゲーム結果の読み込み
	ReadResultData();
}


//更新処理
eSceneType ResultScene::Update()
{
	//Bボタンでランキングに遷移する
	if (InputControl::GetButtonDown(XINPUT_BUTTON_B))
	{
		return eSceneType::E_TITLE;
		//return eSceneType::E_RANKING_INPUT;
	}

	return GetNowScene();
}


//描画処理
void ResultScene::Draw()const
{
	//背景画像を描画
	DrawGraph(0, 0, background_image, TRUE);

	//for (int i = 0; i < 3; i++)
	//{
	//	DrawRotaGraph(230, 230 + (i * 20), 0.3f, DX_PI_F / 2, enemy_image[i], TRUE);

	//	DrawFormatString(260, 222 + (i * 21), GetColor(255, 255, 255), "%6d x%4d=%6d",
	//		enemy_count[i], (i + 1) * 50, (i + 1) * 50 * enemy_count[i]);
	//}
	DrawStringToHandle(600, 290, "スコア", 0xEB3229, font_handle_h2);
	DrawFormatStringToHandle(600, 400, 0xEB3229, font_handle_h2, "%6d", score);
}


//終了時処理
void ResultScene::Finalize()
{
	//読み込んだ画像を削除
	DeleteGraph(background_image);

	// フォントの削除
	DeleteFontToHandle(font_handle_h2);
	DeleteFontToHandle(font_handle_h3);
}


//現在のシーン情報を取得
eSceneType ResultScene::GetNowScene()const
{
	return eSceneType::E_RESULT;
}

//リザルトデータの読み込み
void ResultScene::ReadResultData()
{
	//ファイルオープン
	FILE* fp = nullptr;
	errno_t result = fopen_s(&fp, "Resource/dat/result_data.csv", "r");

	//エラーチェック
	if (result != 0)
	{
		throw("Resource/dat/result_data.csvが読み込めません\n");
	}

	//結果を読み込む
	fscanf_s(fp, "%d,\n", &score);

	//ファイルクローズ
	fclose(fp);
}
