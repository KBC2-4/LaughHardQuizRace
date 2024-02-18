#include "GameMainScene.h"
#include "../Object/RankingData.h"
#include "DxLib.h"
#include <math.h>
#include "../Utility/InputControl.h"
#include "../Utility/Guide.h"
#include "../Utility/GameData.h"

#define QUESTION_NUM 40

GameMainScene::GameMainScene() :background_image(NULL), scroll(0), player(nullptr), answer_anim(0), idle_bgm(0), board_image(0), score(0), add_score(0),
/*question("Resource/dat/question.csv"), */ time_limit(0), start_count(GetNowCount() + 1000 * 100), clear_flg(false), selectMenu(0), clear_count(0), question_count(0),
currentState(State::idle), correct_se(0), cursor_move_se(0), wrong_se(0), enter_se(0), current_question_num(GetRand(QUESTION_NUM - 1))
{
	font_handle_h2 = CreateFontToHandle("Segoe UI", 50, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	font_handle_h3 = CreateFontToHandle("Segoe UI", 20, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);
	font_handle_h4 = CreateFontToHandle("Segoe UI", 10, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);
	question_font_handle = CreateFontToHandle("Segoe UI", 50, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, 1);
	answer_font_handle = CreateFontToHandle("Segoe UI", 40, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);
	buttonGuidFont = CreateFontToHandle("メイリオ", 23, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

	for (int i = 0; i < 15; i++)
	{
		enemy_image[i] = NULL;
	}
}

GameMainScene::~GameMainScene()
{
	// BGMが再生されている場合、停止し削除
	if(CheckSoundMem(idle_bgm) == 1)
	{
		StopSoundMem(idle_bgm);
	}
	DeleteSoundMem(idle_bgm);

	// BGMが再生されている場合、停止し削除
	if (CheckSoundMem(question_bgm) == 1)
	{
		StopSoundMem(question_bgm);
	}
	DeleteSoundMem(question_bgm);

	DeleteSoundMem(enter_se);
	DeleteSoundMem(cursor_move_se);
	DeleteSoundMem(wrong_se);
	DeleteSoundMem(correct_se);

	// フォントの削除
	DeleteFontToHandle(font_handle_h2);
	DeleteFontToHandle(font_handle_h3);
	DeleteFontToHandle(font_handle_h4);
	DeleteFontToHandle(question_font_handle);
	DeleteFontToHandle(answer_font_handle);
	DeleteFontToHandle(buttonGuidFont);

	// 画像の削除
	DeleteGraph(background_image);
	DeleteGraph(board_image);

	for (int i = 0; i < 15; i++)
	{
		DeleteGraph(enemy_image[i]);
	}
}

//初期化処理
void GameMainScene::Initialize()
{

	// 画像の読み込み
	background_image = LoadGraph("Resource/images/Scene/GameMain/background.png");
	const int result = LoadDivGraph("Resource/images/Scene/GameMain/fish.png", 15, 3, 5, 200, 138,
		enemy_image);


	board_image = LoadGraph("Resource/images/Scene/GameMain/board.png");

	// BGMの読み込み
	idle_bgm = LoadSoundMem("Resource/sounds/bgm/idle_time.mp3");
	question_bgm = LoadSoundMem("Resource/sounds/bgm/question_time.mp3");

	// SEの読み込み
	enter_se = LoadSoundMem("Resource/sounds/se/enter.mp3");
	cursor_move_se = LoadSoundMem("Resource/sounds/se/cursor_move.mp3");
	wrong_se = LoadSoundMem("Resource/sounds/se/wrong.mp3");
	correct_se = LoadSoundMem("Resource/sounds/se/correct.mp3");

	//エラーチェック
	if (background_image == -1)
	{
		throw("Resource/images/Scene/GameMain/background.pngがありません\n");
	}

	if (board_image == -1)
	{
		throw("Resource/images/Scene/GameMain/board.pngがありません\n");
	}

	if (result == -1)
	{
		throw("Resource/images/Scene/GameMain/fish.pngがありません\n");
	}

	if (idle_bgm == -1)
	{
		throw("Resource/sounds/bgm/idle_time.mp3がありません\n");
	}

	if (enter_se == -1)
	{
		throw("Resource/sounds/se/enter.mp3がありません\n");
	}

	if (cursor_move_se == -1)
	{
		throw("Resource/sounds/se/cursor_move.mp3がありません\n");
	}

	if (wrong_se == -1)
	{
		throw("Resource/sounds/se/wrong.mp3がありません\n");
	}

	if (correct_se == -1)
	{
		throw("Resource/sounds/se/correct.mp3がありません\n");
	}

	//オブジェクトの生成
	player = new Player;
	enemy = new Enemy * [2];
	question = new QuestionItems;

	animatedRect = AnimatedRectangle(board_image, 3000, 600, 800, 340);
	board_effect = MoveAnimation(board_image, 2000, 400, 800, 400, 0.1f * 5.0f, 1.0f * 5.0f, 1000, 1.0);
	board = MoveAnimation(board_image, 640, 400, 640, 400, 0.1f, 1.0f, 2000);

	//オブジェクトの初期化
	player->Initialize();

	for (int i = 0; i < 2; i++)
	{
		enemy[i] = nullptr;
	}

	CreateEnemy();

	//BGMの再生
	PlaySoundMem(idle_bgm, DX_PLAYTYPE_LOOP, FALSE);

	// ガイド表示を設定
	gamepad_guides = {
		guideElement({"L"}, "移動", GUIDE_SHAPE_TYPE::JOYSTICK, buttonGuidFont, 0x000000,0xFFFFFF, 0xFFFFFF),
	};
}

//更新処理
eSceneType GameMainScene::Update()
{

	//制限時間の経過
	time_limit = GetNowCount() - start_count;

	//制限時間が0以下になった場合、または問題が無くなった場合リザルト画面へ
	if (time_limit >= 0 || question_count == MONDAI)
	{
		// 残り時間を0にする。
		time_limit = 0;
		// GameDataにスコアを保存
		GameData::SetScore(score);

		clear_flg = true;
		return eSceneType::E_RESULT;
	}

	if (currentState == State::question) {

		// 左スティックの倒された方向を取得
		const StickDirection direction = InputControl::GetLStickDirection(20000, 10);

		if ((InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_UP) || direction == StickDirection::Up)
			|| (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_DOWN) || direction == StickDirection::Down))
		{
			// SE再生
			PlaySoundMem(cursor_move_se, DX_PLAYTYPE_BACK, TRUE);
			selectMenu = (selectMenu + 1) % 2;
		}

		// Aボタンを押した時
		if (InputControl::GetButtonDown(XINPUT_BUTTON_A)) {

			// SE再生
			PlaySoundMem(enter_se, DX_PLAYTYPE_BACK, TRUE);

			//解答状況を未回答にリセット
			answer = Answer::unanswered;

			// ステートを解答状態に変更
			SetState(State::answer);

			if (answer_correct == selectMenu)
			{	// 正解
				answer = Answer::correct;
			}
			else
			{	// 不正解

				answer = Answer::wrong;
				player->SetActive(false);
			}



			//不正解だった場合、制限時間を減算させる
			if (answer == Answer::wrong) {
				start_count -= 1000 * 5;
				// SE再生
				PlaySoundMem(wrong_se, DX_PLAYTYPE_BACK, TRUE);
			}
			//正解だった場合、clear_countを加算し、スコアを加算させる
			else if (answer == Answer::correct) {

				start_count += 1000 * 1;
				clear_count++;

				// スコアを加算
				AddScore();

				// SE再生
				PlaySoundMem(correct_se, DX_PLAYTYPE_BACK, TRUE);

			}

			selectMenu = 0;

			// 問題生成
			CreateQuestion();

			// question_bgmが再生中の場合、停止
			if (CheckSoundMem(question_bgm) == 1)
			{
				StopSoundMem(question_bgm);
			}
			//BGMの再生
			PlaySoundMem(idle_bgm, DX_PLAYTYPE_LOOP, FALSE);

			// ガイド表示を更新
			gamepad_guides = {
		guideElement({"L"}, "移動", GUIDE_SHAPE_TYPE::JOYSTICK, buttonGuidFont, 0x000000,0xFFFFFF, 0xFFFFFF),
			};

			// ステートを待機状態に変更
			SetState(State::idle);
		}

	}

	if (IsStateChanged()) {

		// 待機から問題への処理
		if (previousState == State::idle && currentState == State::question) {

			// 当たったエネミーを削除
			for (int i = 0; i < 2; i++)
			{
				if (hitEnemies[i]) {

					enemy[i]->Finalize();
					delete enemy[i];
					enemy[i] = nullptr;
				}
			}

		}

		// 問題から解答への処理
		if (previousState == State::question && currentState == State::answer) {

		}

		// 解答から待機への処理
		if (previousState == State::answer && currentState == State::idle) {


			player->IsStop(false);
			player->SetActive(true);
			board_effect.Close();
			//board.Close();

			//敵生成
			CreateEnemy();
		}

	}

	//プレイヤーの更新
	player->Update();

	//移動距離の更新
	scroll += static_cast<int>(player->GetSpeed()) - 5;


	//敵の更新と当たり判定チェック
	for (int i = 0; i < 2; i++)
	{
		// 当たり判定結果をリセット
		hitEnemies[i] = false;

		if (enemy[i] != nullptr)
		{
			enemy[i]->Updata(player->GetSpeed());

			//画面外に行ったら、敵を削除
			if (enemy[i]->GetLocation().x <= 0.0f)
			{
				enemy[i]->Finalize();
				delete enemy[i];
				enemy[i] = nullptr;
			}

			//当たり判定の確認
			// 当たり判定の結果を配列に保持
			hitEnemies[i] = IsHitCheck(player, enemy[i]);
			if (hitEnemies[i]) {

				// idle_bgmが再生中の場合、停止
				if (CheckSoundMem(idle_bgm) == 1)
				{
					StopSoundMem(idle_bgm);
				}
				//BGMの再生
				PlaySoundMem(question_bgm, DX_PLAYTYPE_LOOP, TRUE);
				

				// ガイド表示を更新
				gamepad_guides = {
					guideElement({"L"}, "選択", GUIDE_SHAPE_TYPE::JOYSTICK, buttonGuidFont, 0x000000,0xFFFFFF, 0xFFFFFF),
					guideElement({"A"}, "決定", GUIDE_SHAPE_TYPE::FIXED_CIRCLE,buttonGuidFont, 0xFFFFFF, A_BUTTON_COLOR,0xEB3229, 0xFFFFFF),
				};

				// エネミーに当たった場合、ステートを問題に変更
				SetState(State::question);

				// プレイヤーの動きを無効化
				player->IsStop(true);
				enemy[i]->IsStop(true);
			}
		}
	}

	if (currentState == State::question) {
		board_effect.Update(60);
		//board.Update(60);
	}

	return GetNowScene();
}

//描画処理
void GameMainScene::Draw()const
{
	// 背景画像の描画
	DrawGraph(scroll % 1280 + 1280, 0, background_image, TRUE);
	DrawGraph(scroll % 1280, 0, background_image, TRUE);


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
		board_effect.Draw();
		//board.Draw();


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


			DrawFormatString2ToHandle(canvas_x1 + 30, canvas_y1, 0xFF0000, 0xFFFFFF, question_font_handle, "%2d問目", question_count + 1);

			//問題 描画
			DrawFormatString2ToHandle(GetDrawCenterX(question->GetQuestion(current_question_num).c_str(), font_handle_h2), canvas_y1 + 100,0xF5A000, 0xffffff, question_font_handle, "%s",
				question->GetQuestion(current_question_num).c_str());

			// 選択肢 描画
			const std::string a1 = question->GetAnswer(current_question_num, answer_correct);
			const std::string a2 = question->GetAnswer(current_question_num, !answer_correct);

			// 各選択肢のフォント
			int a_font = font_handle_h2;

			// 選択肢の文字数が10文字以上の場合
			if (a1.length() > 10) {
				a_font = answer_font_handle;
			}

			DrawFormatString2ToHandle(GetDrawCenterX(a1.c_str(), a_font), 420, 0x00bfff, selectMenu == 0 ? 0x00FFE1 : 0x0000cd, a_font, "%s", a1.c_str());
			DrawFormatString2ToHandle(GetDrawCenterX(a1.c_str(), a_font), 490, 0x00bfff, selectMenu == 1 ? 0x00FFE1 : 0x0000cd, a_font, "%s",
				a2.c_str());
		}
	}


	// HUDの描画
	DrawFormatString2ToHandle(770, 50, 0x4E75ED, 0xFFFFFF, font_handle_h2, "残り時間：%5d.%.3d", -time_limit / 1000, -time_limit % 1000);
	DrawFormatString2ToHandle(50, 50, 0xFF0000, 0xFFFFFF, font_handle_h2, "スコア：%2d", score);

	// ボタンガイドの描画
	DrawGuides(gamepad_guides, 505.0f, 660.0f, 5.0f, 60.0f);

	// 正誤アニメーションを描画
	DrawAnswer();

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

}


//終了時処理
void GameMainScene::Finalize()
{



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



void GameMainScene::AddScore()
{
	// 難易度に応じてscoreを増加させる
	switch (question->GetDifficulty(current_question_num)) {
	case 1:
		add_score = 50; // 難易度1の場合
		break;
	case 2:
		add_score = 100; // 難易度2の場合
		break;
	case 3:
		add_score = 300; // 難易度3の場合
		break;
	default:
		break;
	}

	// スコアに加算
	score += add_score;
}

void GameMainScene::DrawAnswer() const
{
	// アニメーションの経過時間を計算
	const int elapsed_time = GetNowCount() - answer_anim;
	constexpr int fadein_time = 800; // 800ミリ秒
	constexpr int fadeout_time = 1000; // 1秒
	constexpr int anim_time = fadein_time + fadeout_time;

	// アルファ値の計算
	int alpha = 0;
	if (elapsed_time <= fadein_time) { // フェードイン
		alpha = (elapsed_time * 255) / fadein_time;
	}
	else if (elapsed_time <= anim_time) { // フェードアウト
		alpha = 255 - ((elapsed_time - fadein_time) * 255) / fadeout_time;
	}
	else {
		// アニメーション終了
		alpha = 0;
	}

	//正誤表示の座標
	const int x = 660;
	const int y = 400;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha));
	//正誤表示

	switch (answer)
	{

		//case Answer::unanswered:
		//	break;
	case Answer::wrong:		// 不正解

	{//キャンバス

		// バツを描画
		const float canvas_x1 = x;
		const float canvas_y1 = y;
		DrawCircleAA(canvas_x1 - 200, canvas_y1 - 200, 25, 30, 0x4171bb, 50.0F);
		DrawCircleAA(canvas_x1 + 200, canvas_y1 + 200, 25, 30, 0x4171bb, 50.0F);
		DrawCircleAA(canvas_x1 + 200, canvas_y1 - 200, 25, 30, 0x4171bb, 50.0F);
		DrawCircleAA(canvas_x1 - 200, canvas_y1 + 200, 25, 30, 0x4171bb, 50.0F);
		DrawLineAA(canvas_x1 - 200, canvas_y1 - 200, canvas_x1 + 200, canvas_y1 + 200, 0x4171bb, 50.0F);
		DrawLineAA(canvas_x1 + 200, canvas_y1 - 200, canvas_x1 - 200, canvas_y1 + 200, 0x4171bb, 50.0F);

		// 残り時間の加減算の可視化 
		DrawStringToHandle(1000, 120, "- 5.000", 0xFF2200, font_handle_h2);
	}
	break;
	case Answer::correct:		// 正解
		// 丸を描画
		DrawCircleAA(x, y, 200, 30, 0xd2672f, FALSE, 50.0f);
		// 残り時間の加減算の可視化 
		DrawStringToHandle(1000, 120, "+ 1.000", 0xFFFF00, font_handle_h2);

		// 加算したスコアの可視化 
		DrawFormatStringToHandle(200, 120, 0xFFF600, font_handle_h2, "+ %d", add_score);
		break;
	default:
		break;
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameMainScene::CreateEnemy() const
{
	for (int i = 0; i < 1; i++)
	{
		if (enemy[i] == nullptr)
		{
			// 難易度
			const int difficulty = question->GetDifficulty(current_question_num);
			// 生成するエネミーの種類
			int type;

			// 難易度に基づいてエネミーの種類を決定する
			switch (difficulty) {
			case 1:
				// 難易度1の場合
				type = GetRand(9); // 0から8までの乱数
				break;
			case 2:
				// 難易度2の場合
				type = 9 + GetRand(3); // 9から11までの乱数
				break;
			case 3:
				// 難易度3の場合
				type = 12 + GetRand(1); // 12または13の乱数
				break;
			default:
				// それ以外の難易度の場合、typeは0とする
				type = 0;
				break;
			}


			//const int type = GetRand(14) % 14;

			// 難易度で新しい種類のエネミーを生成する
			enemy[i] = new Enemy(type, enemy_image[type]);
			enemy[i]->Initialize();
			break;
		}
	}
}

void GameMainScene::CreateQuestion()
{
	//問題のカウントを加算
	++question_count;
	//生徒の正誤を乱数でランダムで取得
	answer_correct = GetRand(1);
	//まだ出されていない問題を次に出す
	current_question_num = GetRand(QUESTION_NUM - 1);

	while (std::find(question_num.begin(), question_num.end(), current_question_num) != question_num.end()) {

		current_question_num = GetRand(QUESTION_NUM - 1);

	}

	// 解答のアニメーション用に現在の経過時間を格納
	answer_anim = GetNowCount();

	//次の問題番号をプッシュ
	question_num.push_back(current_question_num);
}


//当たり判定処理（プレイヤーと敵）
bool GameMainScene::IsHitCheck(Player* p, Enemy* e) const
{

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
