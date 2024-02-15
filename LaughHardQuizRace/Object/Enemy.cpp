#include "Enemy.h"
#include "DxLib.h"

Enemy::Enemy(int type, int handle) :type(type), image(handle), speed(0.0f),
location(0.0f), box_size(0.0f), is_stop(false)
{

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
	speed = static_cast<float>(this->type * 2);
}


void Enemy::Updata(float speed)
{

	if (is_stop) { return; }
	//位置情報に移動量を加算する
	location += Vector2D(this->speed + speed - 6, 0.0f);
}


void Enemy::Draw()const
{
	//敵画像の描画
	DrawRotaGraphF(location.x, location.y, 1.0f, 0.0, image, TRUE);

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
