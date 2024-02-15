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
	DrawString(600, 290, "スコア", 0xEB3229);
	DrawFormatString(180, 290, 0xEB3229, "         =%6d", score);
}


//終了時処理
void ResultScene::Finalize()
{
	//読み込んだ画像を削除
	DeleteGraph(background_image);
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

	//避けた数と得点を取得
	for (int i = 0; i < 3; i++)
	{
		fscanf_s(fp, "%d,\n", &enemy_count[i]);
	}

	//ファイルクローズ
	fclose(fp);
}
