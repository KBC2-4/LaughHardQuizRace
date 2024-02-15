#pragma once

#include "SceneBase.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"
#include "../Object/Question.h"
#include "../Utility/Animation/AnimatedRectangle.h"
#include "../Utility/Animation/MoveAnimation.h"

class GameMainScene :public SceneBase
{
private:
	int font_handle_h2;     //フォントハンドル
	int font_handle_h3;     //フォントハンドル
	int font_handle_h4;     //フォントハンドル
	int score;            //スコア
	int high_score;      //ハイスコア
	int back_ground;     //背景画像
	int board_image;     //ボード画像
	int mileage;         //走行距離
	int enemy_image[3];  //敵画像
	int enemy_count[3];  //通り過ぎた敵カウント
	int time_limit;      //制限時間
	int start_count;     //ゲーム開始時のカウント


	int difficulty = 1; //出題する問題の難易度
	short next_question_num;		//次に出題する問題番号
	std::vector<short> question_num;//出題した問題番号
	bool next_question;				//次の問題へ移行するフラグ
	short question_count;			//問題のカウント
	bool answer_correct;			//生徒の解答の正誤
	short clear_count;				//正解した問題数

	int size_anim_count;

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

	/// <summary>
	/// 文字の描画するX座標が中心になるX座標を取得する。
	/// </summary>
	/// <param name="string">描画する文字列</param>
	/// <param name="font_handle">描画に使用するフォントハンドル(未入力又は0の場合はデフォルトフォントハンドルを使用</param>
	/// <param name="margin">中央のX座標に間隔を空ける値</param>
	int GetDrawCenterX(const char* string, int font_handle = 0, int margin = 0)const;

private:
	// エネミーを	生成
	void CreateEnemy();
	// 問題を生成
	void CreateQuestion();
	//ハイスコア読み込み処理
	void ReadHighScore();
	//当たり判定
	bool IsHitCheck(Player* p, Enemy* e) const;
	void BubbleSort(Enemy enemies[], int n) const;
	// ステートを更新する関数
	void SetState(State newState);

	// ステートが切り替わったかどうかをチェックする関数
	bool IsStateChanged() const;
};

