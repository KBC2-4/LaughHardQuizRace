#include "GameMainScene.h"
#include "../Object/RankingData.h"
#include "DxLib.h"
#include <math.h>

GameMainScene::GameMainScene() :high_score(0), back_ground(NULL), mileage(0), player(nullptr),enemy(nullptr)
{
	font_handle_h2 = CreateFontToHandle("Segoe UI", 50, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	font_handle_h3 = CreateFontToHandle("Segoe UI", 20, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);
	font_handle_h4 = CreateFontToHandle("Segoe UI", 10, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);

	for (int i = 0; i < 3; i++)
	{
		enemy_image[i] = NULL;
		enemy_count[i] = NULL;
	}
}

GameMainScene::~GameMainScene()
{
	DeleteFontToHandle(font_handle_h2);
	DeleteFontToHandle(font_handle_h3);
	DeleteFontToHandle(font_handle_h4);
}

//初期化処理
void GameMainScene::Initialize()
{
	//高得点値を読み込む
	ReadHighScore();

	//画像の読み込み
	back_ground = LoadGraph(        "Resource/images/Scene/GameMain/background.png");
	//barrier_image = LoadGraph(      "Resource/images/barrier.png");
	const int result = LoadDivGraph("Resource/images/fish.png", 3, 1, 3, 120, 63,
		enemy_image);

	//エラーチェック
	if (back_ground == -1)
	{
		throw("Resource/images/Scene/GameMain/background.pngがありません\n");
	}

	if (result == -1)
	{
		throw("Resource/images/fish.pngがありません\n");
	}

	//オブジェクトの生成
	player = new Player;
	enemy = new Enemy * [10];

	//オブジェクトの初期化
	player->Initialize();

	for (int i = 0; i < 10; i++)
	{
		enemy[i] = nullptr;
	}
}

//更新処理
eSceneType GameMainScene::Update()
{
	//プレイヤーの更新
	player->Update();

	//移動距離の更新
	mileage += static_cast<int>(player->GetSpeed()) - 5;

	//敵生成処理
	//if (mileage / 20 % 100 == 0)
	// 100マイルごとに敵を生成
	if (mileage % 100 == 0)
	{
		//! デバッグ
		//printfDx("%d\n", mileage);
		for (int i = 0; i < 10; i++)
		{
			if (enemy[i] == nullptr)
			{
				const int type = GetRand(3) % 3;
				enemy[i] = new Enemy(type, enemy_image[type]);
				enemy[i]->Initialize();
				break;
			}
		}
	}

	//敵の更新と当たり判定チェック
	for (int i = 0; i < 10; i++)
	{
		if (enemy[i] != nullptr)
		{
			enemy[i]->Updata(player->GetSpeed());

			//画面外に行ったら、敵を削除してスコア加算
			if (enemy[i]->GetLocation().y >= 640.0f)
			{
				enemy_count[enemy[i]->GetType()]++;
				enemy[i]->Finalize();
				delete enemy[i];
				enemy[i] = nullptr;
			}

			//当たり判定の確認
			if (IsHitCheck(player, enemy[i]))
			{
				player->SetActive(false);
				player->Draw_Qizu(false);
				player->DecreaseHp(-50.0f);
				enemy[i]->QuizHide(false);
				enemy[i]->Finalize();
				delete enemy[i];
				enemy[i] = nullptr;
			}
		}
	}

	//プレイヤーの燃料化体力が０未満なら、リザルトに遷移する
	if (/*player->GetFuel() < 0.0f || */player->GetHp() < 0.0f)
	{
		return eSceneType::E_RESULT;
	}
	return GetNowScene();
}

//描画処理
void GameMainScene::Draw()const
{
	//背景画像の描画
	DrawGraph(mileage % 1280 + 1280, 0, back_ground, TRUE);
	DrawGraph(mileage % 1280, 0, back_ground, TRUE);

	//敵の描画
	for (int i = 0; i < 10; i++)
	{
		if (enemy[i] != nullptr)
		{
			enemy[i]->Draw();
		}
	}

	//プレイヤーの描画
	player->Draw();

	//UIの描画
	//DrawBox(500, 0, 640, 480, GetColor(0, 153, 0), TRUE);
	//SetFontSize(16);
	//DrawFormatString2ToHandle(510, 20, 0xffffff, 0x000000, font_handle_h3, "ハイスコア");
	//DrawFormatString2ToHandle(505, 50, 0xffffff, 0xff8c00, font_handle_h3, "%08dM", high_score);
	//DrawFormatString2ToHandle(510, 80, 0xffffff, 0x000000, font_handle_h3, "避けた数");
	//for (int i = 0; i < 3; i++)
	//{
	//	DrawRotaGraph(523 + (i * 50), 130, 0.3, 0, enemy_image[i], TRUE, FALSE);
	//	DrawFormatString(510 + (i * 50), 150, GetColor(255, 255, 255), "%03d", enemy_count[i]);
	//}
	//DrawFormatString2ToHandle(510, 180, 0xffffff, 0x000000, font_handle_h3, "走行距離");
	//DrawFormatString2ToHandle(505, 220, 0xffffff, 0x00fa9a, font_handle_h3,  "%08dM", mileage / 10);
	//DrawFormatString2ToHandle(510, 260, 0xffffff, 0x000000, font_handle_h3, "スピード");
	////DrawFormatString2ToHandle(555, 280, 0xffffff, 0x0000ff, font_handle_h2, "%0.f", player->GetSpeed());
	//DrawFormatString2ToHandle(500, 280, 0xffffff, 0x0000ff, font_handle_h2, "%3.f km/h", player->GetSpeed() * 15.0f);
	//DrawFormatString2ToHandle(470, 280, 0xffffff, 0x0000ff, font_handle_h2, "%d", player->GetGear());

	////バリア枚数の描画
	//for (int i = 0; i < player->GetBarrierCount(); i++)
	//{
	//	DrawRotaGraph(520 + i * 50, 380, 0.32f, 0, barrier_image, TRUE, FALSE);
	//}

	//燃料ゲージの描画
	//float fx = 510.0f;
	//float fy = 390.0f;
	//DrawFormatString(fx, fy, GetColor(0, 0, 0), "FUEL METER");
	//DrawBoxAA(fx, fy + 20.0f, fx + (player->GetFuel() * 100 / 20000), fy + 40.0f,
	//    GetColor(0, 102, 204), TRUE);
	//DrawBoxAA(fx, fy + 20.0f, fx + 100.0f, fy + 40.0f, GetColor(0, 0, 0), FALSE);

	//体力ゲージの描画
	//float fx = 510.0f;
	//float fy = 430.0f;
	//DrawFormatString(fx, fy, GetColor(0, 0, 0), "PLAYER HP");
	//DrawBoxAA(fx, fy + 20.0f, fx + (player->GetHp() * 100 / 1000), fy + 40.0f,
	//	GetColor(255, 0, 0), TRUE);
	//DrawBoxAA(fx, fy + 20.0f, fx + 100.0f, fy + 40.0f, GetColor(0, 0, 0),
	//	FALSE);




	//! DEBUG
	//for (int i = 0; i < 10; i++)
	//{
	//	if (enemy[i] != nullptr) {
	//		const int lane = static_cast<int>((enemy[i]->GetLocation().x - 40) / 105) + 1;
	//		DrawFormatString(100, 20 + i * 20, 0xffffff, "Enemy: レーン：%d x %f , y %f", lane, enemy[i]->GetLocation().x, enemy[i]->GetLocation().y);
	//	}
	//}
}


//終了時処理
void GameMainScene::Finalize()
{
	//スコアを計算する
	int score = (mileage / 10 * 10);
	for (int i = 0; i < 3; i++)
	{
		score += (i + 1) * 50 * enemy_count[i];
	}

	//リザルトデータの書き込み
	FILE* fp = nullptr;
	//ファイルオープン
	errno_t result = fopen_s(&fp, "Resource/dat/result_data.csv", "w");

	//エラーチェック
	if (result != 0)
	{
		throw("Resource/dat/result_data.csvが開けません\n");
	}

	//スコアを保存
	fprintf(fp, "%d,\n", score);

	//避けた数と得点を保存
	for (int i = 0; i < 3; i++)
	{
		fprintf(fp, "%d,\n", enemy_count[i]);
	}

	//ファイルクローズ
	fclose(fp);

	//動的確保したオブジェクトを削除する
	player->Finalize();
	delete player;

	for (int i = 0; i < 10; i++)
	{
		if (enemy[i] != nullptr)
		{
			enemy[i]->Finalize();
			delete enemy[i];
			enemy[i] = nullptr;
		}
	}

	delete[] enemy;
}

//現在のシーン情報を取得
eSceneType GameMainScene::GetNowScene()const
{
	return eSceneType::E_MAIN;
}


//ハイスコア読み込み
void GameMainScene::ReadHighScore()
{
	RankingData data;
	data.Initialize();

	high_score = data.GetScore(0);

	data.Finalize();
}


//当たり判定処理（プレイヤーと敵）
bool GameMainScene::IsHitCheck(Player* p, Enemy* e)
{
	//プレイヤーがバリアを張っていたら、当たり判定を無視する
	//if (p->IsBarrier())
	//{
	//	return false;
	//}

	//敵情報がなければ、当たり判定を無視する
	if (e == nullptr)
	{
		return false;
	}

	//位置情報の差分を取得
	Vector2D diff_location = p->GetLocation() - e->GetLocation();

	//当たり判定サイズの大きさを取得
	Vector2D box_ex = p->GetBoxSize() + e->GetBoxSize();

	//コリジョンデータより位置情報の差分が小さいなら、ヒット判定とする
	return ((fabsf(diff_location.x) < box_ex.x) && (fabsf(diff_location.y) < box_ex.y));
}