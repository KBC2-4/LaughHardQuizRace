#pragma once

#include "../Utility/Vector2D.h"

class Enemy
{private:
	int type;             //タイプ
	int image;            //画像
	float speed_x;          //速さX
	float speed_y;          //速さY
	bool Quizhide;
	Vector2D location;    //位置情報
	Vector2D box_size;    //当たり判定の大きさ
	bool is_stop = false;       //一時停止フラグ
	float scale;		// 画像の拡大率

public:
	Enemy(int type, int handle);
	~Enemy();

	void Initialize();            //初期化処理
	void Updata(float speed);     //更新処理
	void Update();				 // 更新処理（泳ぐ）
	void Draw()const;             //描画処理
	void Finalize();              //終了時処理
	void QuizHide(bool flg);      //クイズ表記処理

	int GetType()const;           //タイプ取得
	Vector2D GetLocation()const;  //位置情報の取得
	Vector2D GetBoxSize()const;   //当たり判定の大きさを取得
	void IsStop(bool result);        // 一時停止処理
	void SetPosition(float x, float y);	// 位置情報の設定
	void SetMovement(float speed_x, float speed_y);		// 速さの設定
};

