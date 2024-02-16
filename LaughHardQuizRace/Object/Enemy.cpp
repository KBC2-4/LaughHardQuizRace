#include "Enemy.h"
#include "DxLib.h"

Enemy::Enemy(int type, int handle, bool appreciation) :type(type), image(handle), speed_x(0.0f),
location(0.0f), box_size(0.0f), is_stop(false), scale(0.7f)
{
	if (appreciation) {
		
		//scale = 1.5f; // スケールを1.5fに設定
		location = Vector2D(1380.0f, 400.0f);
		return;
	}
	// typeが9よりも大きい場合
	if (this->type > 9) {
		scale = 1.0f; // スケールを1.0fに設定

		//当たり判定の設定
		box_size = Vector2D(42.0f, 60.0f);

		// スピードにtypeが9を超える比率を加える
		speed_x += (this->type - 9) * (2.0f / (this->type - 9));
	}
}
	

Enemy::~Enemy()
{

}

void Enemy::Initialize()
{
	//出現させるX座標パターンを取得
	//const float random_y = static_cast<float>(GetRand(4) * 105 + 40);
	//生成位置の設定
	//location = Vector2D(1380.0f, random_y);
	location = Vector2D(1380.0f, 400.0f);
	//当たり判定の設定
	box_size = Vector2D(31.0f, 60.0f);
	//速さの設定
	speed_x = 2;

}


void Enemy::Updata(float speed)
{

	if (is_stop) { return; }
	//位置情報に移動量を加算する
	location += Vector2D(this->speed_x + speed - 6, 0.0f);
}

void Enemy::Update()
{
	location += Vector2D(this->speed_x * GetRand(1), this->speed_y);

	//// ウィンドウサイズを取得
	//int width, height;
	//width = 1280;
	//height = 720;

	//// 画面の左端を超えた場合
	//if (location.x < 0) {
	//	location.x = static_cast<float>(width); // 画面の右端から再出現
	//}
	//// 画面の右端を超えた場合
	//else if (location.x > static_cast<float>(width)) {
	//	location.x = 0; // 画面の左端から再出現
	//}

	//// 画面の上端を超えた場合
	//if (location.y < 0) {
	//	location.y = static_cast<float>(height); // 画面の下端から再出現
	//}
	//// 画面の下端を超えた場合
	//else if (location.y > static_cast<float>(height)) {
	//	location.y = 0; // 画面の上端から再出現
	//}

}


void Enemy::Draw()const
{
	//敵画像の描画
	DrawRotaGraphF(location.x, location.y, scale, 0.0, image, TRUE);

	// 当たり判定を描画
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);;
	//DrawBoxAA(location.x - box_size.x / 2, location.y - box_size.y / 2, location.x + box_size.x / 2, location.y + box_size.y / 2, GetColor(255, 0, 0), TRUE, -1);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


void Enemy::Finalize()
{

}


//敵のタイプを取得
int Enemy::GetType()const
{
	return type;
}


//位置情報を取得
Vector2D Enemy::GetLocation()const
{
	return location;
}


//当たり判定の大きさを取得
Vector2D Enemy::GetBoxSize()const
{
	return box_size;
}

void Enemy::IsStop(bool result)
{
	this->is_stop = result;
}

void Enemy::SetPosition(const float x, const float y)
{
	this->location.x = x;
	this->location.y = y;
}

void Enemy::SetMovement(float speed_x, float speed_y) {
	this->speed_x = speed_x;
	this->speed_y = speed_y;
}
