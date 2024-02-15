#include "GameMainScene.h"
#include "../Object/RankingData.h"
#include "DxLib.h"
#include <math.h>
#include "../Utility/InputControl.h"

#define QUESTION_NUM 40

GameMainScene::GameMainScene() :high_score(0), back_ground(NULL), mileage(0), player(nullptr), answer_anim(0),
/*question("Resource/dat/question.csv"), */ time_limit(0), start_count(GetNowCount() + 1000 * 100), clear_flg(false),
size_anim_count(0), currentState(State::idle), difficulty(1)
{
	font_handle_h2 = CreateFontToHandle("Segoe UI", 50, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	font_handle_h3 = CreateFontToHandle("Segoe UI", 20, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);
	font_handle_h4 = CreateFontToHandle("Segoe UI", 10, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);
	answer_font_handle = CreateFontToHandle("Segoe UI", 40, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);

	for (int i = 0; i < 3; i++)
	{
		enemy_image[i] = NULL;
		enemy_count[i] = NULL;
	}
}

GameMainScene::~GameMainScene()
{
	StopSoundMem(background_sound);
	DeleteSoundMem(background_sound);
	DeleteFontToHandle(font_handle_h2);
	DeleteFontToHandle(font_handle_h3);
	DeleteFontToHandle(font_handle_h4);
	DeleteFontToHandle(answer_font_handle);
}

//初期化処理
void GameMainScene::Initialize()
{
	//高得点値を読み込む
	ReadHighScore();

	//画像の読み込み
	back_ground = LoadGraph("Resource/images/Scene/GameMain/background.png");
	//barrier_image = LoadGraph(      "Resource/images/barrier.png");
	const int result = LoadDivGraph("Resource/images/fish.png", 3, 1, 3, 120, 63,
		enemy_image);

	board_image = LoadGraph("Resource/images/Scene/GameMain/board.png");

	//BGMの読み込み
	background_sound = LoadSoundMem("Resource/sounds/bgm/Electric_Shine.mp3");

	//エラーチェック
	if (back_ground == -1)
	{
		throw("Resource/images/Scene/GameMain/background.pngがありません\n");
	}

	if (board_image == -1)
	{
		throw("Resource/images/Scene/GameMain/board.pngがありません\n");
	}

	if (result == -1)
	{
		throw("Resource/images/fish.pngがありません\n");
	}

	if (background_sound == -1)
	{
		throw("Resource/sounds/bgm/Electric_Shine.mp3がありません\n");
	}

	//オブジェクトの生成
	player = new Player;
	enemy = new Enemy * [2];
	question = new QuestionItems;

	animatedRect = AnimatedRectangle(board_image, 3000, 600, 800, 340);
	board = MoveAnimation(board_image, 2000, 400, 800, 400, 0.1f, 1.0f, 1000);

	//オブジェクトの初期化
	player->Initialize();

	for (int i = 0; i < 2; i++)
	{
		enemy[i] = nullptr;
	}

	CreateEnemy();

	//BGMの再生
	PlaySoundMem(background_sound, DX_PLAYTYPE_LOOP, FALSE);
}

//更新処理
eSceneType GameMainScene::Update()
{


	//制限時間の経過
	time_limit = GetNowCount() - start_count;

	//制限時間が0以上になった場合、リザルト画面へ
	if (time_limit > 0) { clear_flg = true; }

	if (currentState == State::question) {

		// 左スティックの倒された方向を取得
		const StickDirection direction = InputControl::GetLStickDirection(20000, 5);

		if ((InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_UP) || direction == StickDirection::Up)
			|| (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_DOWN) || direction == StickDirection::Down))
		{
			selectMenu = (selectMenu + 1) % 2;
		}

		if (InputControl::GetButtonDown(XINPUT_BUTTON_A)) {

			// ステートを解答状態に変更
			SetState(State::answer);

			if (answer_correct == selectMenu)
			{
				answer = Answer::correct;
			}
			else
			{
				answer = Answer::wrong;
			}
		}

		/*if (InputControl::GetButtonDown(XINPUT_BUTTON_A)) {
			if (answer_correct == 0) { answer = GameMainScene::Answer::wrong; }
			else { answer = Answer::correct; }
		}
		else if (InputControl::GetButtonDown(XINPUT_BUTTON_B)) {
			if (answer_correct == 1) { answer = GameMainScene::Answer::wrong; }
			else { answer = Answer::correct; }
		}*/


		//不正解だった場合、制限時間を減算させる
		if (answer == Answer::wrong) {
			time_limit -= 1000 * 5;
			//PlaySoundMem(wrong_se, DX_PLAYTYPE_BACK, TRUE);
		}
		//正解だった場合、clear_countを加算し、スコアを加算させる
		else if (answer == Answer::correct) {
			time_limit += 1000 * 1;
			clear_count++;
			score += 50;
			//PlaySoundMem(correct_se, DX_PLAYTYPE_BACK, TRUE);
		}

		//BボタンまたはAボタンを押した時
		if ((InputControl::GetButtonDown(XINPUT_BUTTON_A))
			|| InputControl::GetButtonDown(XINPUT_BUTTON_B)) {
			next_question = true;

			selectMenu = 0;

			// 問題生成
			CreateQuestion();

			// ステートを待機状態に変更
			SetState(State::idle);
			//正誤のアニメーションを開始
			//answer_anim_count = 0;
			//タイム加減算のアニメーションを開始
			//addtime_anim_count = 0;
		}

	}

	if (size_anim_count < 60) {
		size_anim_count++;

	}

	if (IsStateChanged()) {

		// 待機から問題への処理
		if (previousState == State::idle && currentState == State::question) {

		}

		// 問題から解答への処理
		if (previousState == State::question && currentState == State::answer) {

		}

		// 解答から待機への処理
		if (previousState == State::answer && currentState == State::idle) {

			player->IsStop(false);
			player->SetActive(true);
			board.Close();

			//敵生成
			CreateEnemy();
		}

	}

	//プレイヤーの更新
	player->Update();

	//移動距離の更新
	mileage += static_cast<int>(player->GetSpeed()) - 5;


	//敵の更新と当たり判定チェック
	for (int i = 0; i < 2; i++)
	{
		// 当たり判定結果をリセット
		hitEnemies[i] = false;

		if (enemy[i] != nullptr)
		{
			enemy[i]->Updata(player->GetSpeed());

			//画面外に行ったら、敵を削除してスコア加算
			if (enemy[i]->GetLocation().x <= 0.0f)
			{
				enemy_count[enemy[i]->GetType()]++;
				enemy[i]->Finalize();
				delete enemy[i];
				enemy[i] = nullptr;
			}

			//当たり判定の確認
			// 当たり判定の結果を配列に保持
			hitEnemies[i] = IsHitCheck(player, enemy[i]);
			if (hitEnemies[i]) {

				SetState(State::question);
				player->IsStop(true);
				enemy[i]->IsStop(true);
				player->SetActive(false);

				enemy[i]->Finalize();
				delete enemy[i];
				enemy[i] = nullptr;
			}
		}
	}

	if (currentState == State::question) {
		board.Update(16);
	}

	//if (currentState == State::idle) {

	//	if (time_limit < answer_anim - 3000) {
	//		//解答状況を未回答にリセット
	//		answer = Answer::unanswered;

	//		printfDx("%d\n", time_limit);
	//	}
	//}


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
	// 背景画像の描画
	DrawGraph(mileage % 1280 + 1280, 0, back_ground, TRUE);
	DrawGraph(mileage % 1280, 0, back_ground, TRUE);

	DrawFormatString2ToHandle(770, 50, 0x4E75ED, 0xFFFFFF, font_handle_h2, "残り時間：%5d.%.3d", -time_limit / 1000, -time_limit % 1000);
	DrawFormatString2ToHandle(50, 50, 0xFF0000, 0xFFFFFF, font_handle_h2, "スコア：%2d", score);
	// 敵の描画
	for (int i = 0; i < 2; i++)
	{
		if (enemy[i] != nullptr)
		{
			enemy[i]->Draw();
		}
	}

	//プレイヤーの描画
	player->Draw();

	//for (int i = 0; i < 2; i++) {
	//	// Updateでの当たり判定結果がtrueの場合
	//	if (hitEnemies[i]) {

	//	}
	//}

	if (currentState == State::question)
	{
		board.Draw();
	}

	{//キャンバス

		// 画面サイズ
		constexpr int screenWidth = 1280;
		constexpr int screenHeight = 720;

		// キャンバスサイズ
		constexpr int canvasWidth = 800;
		constexpr int canvasHeight = 400;

		int canvas_x1;
		int canvas_x2;
		int canvas_y1;
		int canvas_y2;
		short speed = 5;

		canvas_x1 = 490;
		canvas_y1 = 150;
		//キャンバス 描画
		//DrawBoxAA(canvas_x1, canvas_y1, canvas_x2, canvas_y2, 0xFFFFFF, TRUE);
		//DrawExtendGraph(canvas_x1, canvas_y1, canvas_x2, canvas_y2, paper_image, TRUE);


		DrawExtendFormatString2ToHandle(canvas_x1 + 30, canvas_y1, size_anim_count * 0.01 + 0.4, size_anim_count * 0.01 + 0.4,
			0xFF0000, 0xFFFFFF, font_handle_h2, "%2d問目", question_count + 1);

		//問題 描画
		DrawExtendFormatString2ToHandle(GetDrawCenterX(question->GetQuestion(next_question_num).c_str(), font_handle_h2), canvas_y1 + 100,
			/*size_anim_count * 0.01 + 0.2*/1.0f, size_anim_count * 0.01 + 0.2, 0xF5A000, 0xEFBD00, font_handle_h2, "%s",
			question->GetQuestion(next_question_num).c_str());

		//生徒の解答 描画
		//DrawExtendStringToHandle(canvas_x1 - 200, 370, size_anim_count * 0.01 + 0.4, size_anim_count * 0.01 + 0.4, "選択肢",
		//	0xFFFFFF, font_handle_h3, 0x000000);
		//int char_num = GetDrawFormatStringWidthToHandle(font_handle_h4, question->GetAnswer(next_question_num, answer_correct).c_str());
		//for (int i = 0; i < char_num / 20; i++) {
		//	DrawExtendStringToHandle(canvas_x1 + 100 + i * 20, 580, size_anim_count * 0.01 + 0.4, size_anim_count * 0.01 + 0.4, "_", 0xFFFFFF, font_handle_h2, 0x000000);
		//	//DrawLineAA(canvas_x1 + 300, 640, canvas_x1 + 300 + i * 20, 580, 0x000000, size_anim_count * 0.01 + 0.8);
		//}

		//const bool question_num = GetRand(1);

		// 選択肢
		const std::string a1 = question->GetAnswer(next_question_num, answer_correct);
		const std::string a2 = question->GetAnswer(next_question_num, !answer_correct);

		// 各選択肢のフォント
		int a_font = font_handle_h2;

		// 選択肢の文字数が10文字以上の場合
		if (a1.length() > 10) {
			a_font = answer_font_handle;
		}

		DrawExtendFormatString2ToHandle(canvas_x1 + 30, 400, (size_anim_count * 0.01 + 0.4), (size_anim_count * 0.01 + 0.4),
			0x00bfff, selectMenu == 0 ? 0x00FFE1 : 0x0000cd, a_font, "%6s",
			a1.c_str());
		DrawExtendFormatString2ToHandle(canvas_x1 + 30, 470, (size_anim_count * 0.01 + 0.4), (size_anim_count * 0.01 + 0.4),
			0x00bfff, selectMenu == 1 ? 0x00FFE1 : 0x0000cd, a_font, "%6s",
			a2.c_str());
	}

	//正誤表示の座標
	int x = 660;
	int y = 400;
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(answer_anim_count));
	//正誤表示

	switch (answer)
	{

		/*case GameMain::Answer::unanswered:
			break;*/
	case Answer::wrong:

	{//キャンバス
		float canvas_x1 = x;
		float canvas_x2;
		float canvas_y1 = y;
		float canvas_y2;
		DrawCircleAA(canvas_x1 - 200, canvas_y1 - 200, 25, 30, 0x4171bb, 50.0F);
		DrawCircleAA(canvas_x1 + 200, canvas_y1 + 200, 25, 30, 0x4171bb, 50.0F);
		DrawCircleAA(canvas_x1 + 200, canvas_y1 - 200, 25, 30, 0x4171bb, 50.0F);
		DrawCircleAA(canvas_x1 - 200, canvas_y1 + 200, 25, 30, 0x4171bb, 50.0F);
		DrawLineAA(canvas_x1 - 200, canvas_y1 - 200, canvas_x1 + 200, canvas_y1 + 200, 0x4171bb, 50.0F);
		DrawLineAA(canvas_x1 + 200, canvas_y1 - 200, canvas_x1 - 200, canvas_y1 + 200, 0x4171bb, 50.0F);

		DrawStringToHandle(1000, 90, "- 5.000", 0xFF0000, font_handle_h3);
	}
	break;
	case Answer::correct:
		DrawCircleAA(x, y, 200, 30, 0xd2672f, FALSE, 50.0F);
		DrawStringToHandle(1000, 90, "+ 1.000", 0xFFFF00, font_handle_h3);
		break;
	default:
		break;
	}

	//! デバッグ 問題の表示
	//for (const auto& item : question.GetQuizItems()) {
	//	printfDx("Question: %s\n", item.question.c_str());
	//	printfDx("Difficulty: %d\n", item.difficultyLevel);
	//	printfDx("Answer: %s\n", item.answer.c_str());

	//	if (!item.wrongs.empty()) {
	//		for (const auto& wrong : item.wrongs) {
	//			printfDx("Wrong: %s\n", wrong.c_str());
	//		}
	//	}
	//	else {
	//		// wrongsが空の場合
	//		printfDx("Wrongs: None\n");
	//	}

	//	printfDx("-----------------------\n");
	//}




	//! DEBUG エネミー
	//for (int i = 0; i < 2; i++)
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

	for (int i = 0; i < 2; i++)
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


int GameMainScene::GetDrawCenterX(const char* string, int font_handle, int margin)const {

	//画面幅
	const int screenX = 1280;

	if (margin >= screenX || margin <= -screenX) {
		margin = 0;
	}

	if (font_handle == 0) {
		font_handle = DX_DEFAULT_FONT_HANDLE;
	}


	const int w = screenX / 2 + margin - (GetDrawFormatStringWidthToHandle(font_handle, string) / 2);
	return w;
}

void GameMainScene::CreateEnemy()
{
	for (int i = 0; i < 1; i++)
	{
		if (enemy[i] == nullptr)
		{
			const int type = GetRand(3) % 3;

			// 難易度で新しい種類のエネミーを生成する
			//const int type = (GetRand(3) % 3) + difficulty;
			enemy[i] = new Enemy(type, enemy_image[type]);
			enemy[i]->Initialize();
			printfDx("CreateEnemy");
			break;
		}
	}
}

void GameMainScene::CreateQuestion()
{
	next_question = false;
	//問題のカウントを加算
	++question_count;
	//生徒の正誤を乱数でランダムで取得
	answer_correct = GetRand(1);
	//まだ出されていない問題を次に出す
	next_question_num = GetRand(QUESTION_NUM - 1);

	while (std::find(question_num.begin(), question_num.end(), next_question_num) != question_num.end()) {

		next_question_num = GetRand(QUESTION_NUM - 1);

		//printfDx("next_question_num:%d", next_question_num);
		////デバッグ
		//for (int num : question_num) {
		//	printfDx("[%d]", num);

					//解答をリセット

	}

	// 解答のアニメーション用に現在の経過時間を格納
	answer_anim = time_limit;

	//解答状況を未回答にリセット
	answer = Answer::unanswered;

	//次の問題番号をプッシュ
	question_num.push_back(next_question_num);
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
bool GameMainScene::IsHitCheck(Player* p, Enemy* e) const
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
	const Vector2D diff_location = p->GetLocation() - e->GetLocation();

	//当たり判定サイズの大きさを取得
	const Vector2D box_ex = p->GetBoxSize() + e->GetBoxSize();

	//コリジョンデータより位置情報の差分が小さいなら、ヒット判定とする
	return ((fabsf(diff_location.x) < box_ex.x) && (fabsf(diff_location.y) < box_ex.y));
}

void GameMainScene::SetState(State newState)
{
	previousState = currentState; // 現在のステートを前のステートに設定
	currentState = newState; // 新しいステートを現在のステートに設定
}

bool GameMainScene::IsStateChanged() const
{
	return currentState != previousState; // 現在のステートと前のステートが異なるかどうかを返す
}
