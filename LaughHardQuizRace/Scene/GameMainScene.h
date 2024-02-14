#pragma once

#include "SceneBase.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"
#include "../Object/Question.h"
#include "../Utility/Animation/AnimatedRectangle.h"

class GameMainScene:public SceneBase
{
private:
	int font_handle_h2;     //フォントハンドル
	int font_handle_h3;     //フォントハンドル
	int font_handle_h4;     //フォントハンドル
	int high_score;      //ハイスコア
	int back_ground;     //背景画像
	int mileage;         //走行距離
	int enemy_image[3];  //敵画像
	int enemy_count[3];  //通り過ぎた敵カウント
	Player* player;      //プレイヤー
	Enemy** enemy;       //敵
	//Question question;  //問題

	AnimatedRectangle animatedRect;

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
	//ハイスコア読み込み処理
	void ReadHighScore();
	//当たり判定
	bool IsHitCheck(Player* p, Enemy* e) const;
	void BubbleSort(Enemy enemies[], int n) const;
};

