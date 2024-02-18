#pragma once

#include "SceneBase.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"
#include "../Object/QuestionItems.h"
#include "../Utility/Animation/AnimatedRectangle.h"
#include "../Utility/Animation/MoveAnimation.h"

class GameMainScene :public SceneBase
{
private:
	int font_handle_h2;     //フォントハンドル
	int font_handle_h3;     //フォントハンドル
	int font_handle_h4;     //フォントハンドル
	int buttonGuidFont;     //ガイド表示用フォントハンドル
	int answer_font_handle;     //選択肢表示用フォントハンドル
	int score;            // スコア
	int add_score;       // 追加するスコア
	int background_image;     //背景画像
	int idle_bgm;     //アイドル時のBGM
	int question_bgm;     //問題画面時のBGM
	int wrong_se;        //不正解 音声ハンドル
	int correct_se;      //正解 音声ハンドル
	int cursor_move_se;       //カーソル移動 音声ハンドル
	int enter_se;        //決定 音声ハンドル

	int board_image;     //ボード画像
	int scroll;         //走行距離
	int enemy_image[15];  //敵画像
	int time_limit;      //制限時間
	int start_count;     //ゲーム開始時のカウント
	std::vector<guideElement> gamepad_guides; // ボタンガイド表示用

	short current_question_num;		//出題している問題番号
	std::vector<short> question_num;//出題した問題番号
	short question_count;			//何問題かのカウント
	bool answer_correct;			//生徒の解答の正誤
	short clear_count;				//正解した問題数


	// アニメーション用
	int answer_anim;				// 正誤のアニメーション用
	int size_anim_count;			// スケールアニメーション用

	enum class Answer {
		unanswered, //未回答
		wrong,		//不正解
		correct,	//正解
	}answer;

	// ゲームの状態
	enum class State {
		idle,		//待機
		question,	//問題
		answer,		//解答
		result,		//リザルト画面へ
	};

	State currentState = State::idle; // 現在のステート
	State previousState = State::idle; // 前のステート

	bool clear_flg;   //ゲームクリア時のフラグ
	Player* player;      //プレイヤー
	Enemy** enemy;       //敵
	QuestionItems* question;  //問題

	int selectMenu; // 問題への解答をする選択中のメニュー
	AnimatedRectangle animatedRect;
	MoveAnimation board_effect;
	MoveAnimation board;

	bool hitEnemies[10] = { false }; // 10体の敵に対する当たり判定結果を一時的に保持する配列

public:
	GameMainScene();
	virtual ~GameMainScene();

	virtual void Initialize() override;
	virtual eSceneType Update() override;
	virtual void Draw() const override;
	virtual void Finalize() override;

	virtual eSceneType GetNowScene() const override;

private:
	// スコアを加算する
	void AddScore();
	// 正誤を描画する
	void DrawAnswer() const;
	// エネミーを	生成
	void CreateEnemy() const;
	// 問題を生成
	void CreateQuestion();
	//当たり判定
	bool IsHitCheck(Player* p, Enemy* e) const;
	void BubbleSort(Enemy enemies[], int n) const;
	// ステートを更新する関数
	void SetState(State newState);

	// ステートが切り替わったかどうかをチェックする関数
	bool IsStateChanged() const;
};

