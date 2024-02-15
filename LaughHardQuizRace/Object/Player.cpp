#include "Player.h"
#include "../Utility/InputControl.h"
#include "DxLib.h"
#include "../Utility/MathUtils.h"


Player::Player() :is_active(false), image(NULL), location(0.0f), box_size(0.0f), is_stop(false),
angle(0.0f),speed(0.0f), anim(2000.0f, 2.0f)
{
	// 現在の経過時間を取得
	previous_time = GetNowCount();

}


Player::~Player()
{

}


//初期化処理
void Player::Initialize()
{
	is_active = true;
	location = Vector2D(320.0f, 380.0f);
	box_size = Vector2D(140.0f, 200.0f);
	angle = 0.0f;
	speed = 1.0f;
	//画像の読み込み
	image = LoadGraph("Resource/images/Scene/GameMain/player.png");

	//エラーチェック
	if (image == -1)
	{
		throw("Resource/images/Scene/GameMain/player.pngがありません\n");
	}
}



//更新処理
void Player::Update()
{
	//操作不可状態であれば、自身を回転させる
	if (!is_active)
	{
		angle += DX_PI_F / 24.0f;
		speed = 0.2f;
		if (angle >= DX_PI_F * 4.0f)
		{
			is_active = true;
		}
		return;
	}


	if (!is_stop) {

		//移動処理
		Movement();
	}

	// アイドルアニメーション

	 // 現在の時間を取得
	const int currentTime = GetNowCount();

	// アニメーションの更新
	const float animationOffset = anim.GetAnimationOffset(currentTime);

	// アニメーションによる位置の更新
	location.y += animationOffset;

}


//描画処理
void Player::Draw()
{
	//プレイヤー画像の描画
	DrawRotaGraphF(location.x, location.y, 1.0f, angle, image, TRUE);

	// 当たり判定を描画
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);;
	//DrawBoxAA(location.x - box_size.x / 2, location.y - box_size.y / 2, location.x + box_size.x / 2, location.y + box_size.y / 2, GetColor(255, 0, 0), TRUE, -1);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


//終了時処理
void Player::Finalize()
{
	//読み込んだ画像を削除
	DeleteGraph(image);

}


//状態設定処理
void Player::SetActive(bool flg)
{
	this->is_active = flg;
}


//位置情報取得処理
Vector2D Player::GetLocation()const
{
	return this->location;
}


//当たり判定の大きさ取得処理
Vector2D Player::GetBoxSize()const
{
	return this->box_size;
}


//速さ取得処理
float Player::GetSpeed()const
{
	return this->speed;
}


//移動処理
void Player::Movement()
{
	Vector2D move = Vector2D(0.0f);
	angle = 0.0f;

	//十字移動処理
	if (InputControl::GetButton(XINPUT_BUTTON_DPAD_LEFT) || InputControl::GetLStickDirection() == StickDirection::Left)
	{
		move += Vector2D(-1.0f, 0.0f);
		angle = -DX_PI_F / 18;
	}
	if (InputControl::GetButton(XINPUT_BUTTON_DPAD_RIGHT) || InputControl::GetLStickDirection() == StickDirection::Right)
	{
		move += Vector2D(1.0f, 0.0f);
		angle = DX_PI_F / 18;
	}
	//if (InputControl::GetButton(XINPUT_BUTTON_DPAD_UP) || InputControl::GetLStickDirection() == StickDirection::Up)
	//{
	//	move += Vector2D(0.0f, -1.0f);
	//}
	//if (InputControl::GetButton(XINPUT_BUTTON_DPAD_DOWN) || InputControl::GetLStickDirection() == StickDirection::Down)
	//{
	//	move += Vector2D(0.0f, 1.0f);
	//}

	location += move;

	//画面外に行かないように制限する
	if ((location.x < box_size.x) || (location.x >= 1280.f - 180.0f) ||
		(location.y < box_size.y) || (location.y >= 720.0f - box_size.y))
	{
		location -= move;
	}
}

void Player::IsStop(bool result)
{
	this->is_stop = result;
}