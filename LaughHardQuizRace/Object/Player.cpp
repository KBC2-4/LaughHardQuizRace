#include "Player.h"
#include "../Utility/InputControl.h"
#include "DxLib.h"
#include "../Utility/MathUtils.h"


Player::Player() :is_active(false), image(NULL), location(0.0f), box_size(0.0f),
angle(0.0f),speed(0.0f), gear(0), hp(0.0f), fuel(0.0f)
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
	box_size = Vector2D(31.0f, 60.0f);
	angle = 0.0f;
	speed = 1.0f;
	gear = 0;
	hp = 1000;
	fuel = 6000;

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
		speed = 1.0f;
		if (angle >= DX_PI_F * 4.0f)
		{
			is_active = true;
		}
		return;
	}

	//燃料の消費
	fuel -= speed;

	//移動処理
	Movement();

	//加減速処理
	Acceleration();

	//バリア処理
	//if (InputControl::GetButtonDown(XINPUT_BUTTON_B) && barrier_count > 0)
	//{
	//	if (barrier == nullptr)
	//	{
	//		barrier_count--;
	//		barrier = new Barrier;
	//	}
	//}

	////バリアが生成されていたら、更新を行う
	//if (barrier != nullptr)
	//{
	//	//バリア時間が経過したか？していたら、削除する
	//	if (barrier->IsFinished(this->speed))
	//	{
	//		delete barrier;
	//		barrier = nullptr;
	//	}
	//}
}


//描画処理
void Player::Draw()
{
	//プレイヤー画像の描画
	DrawRotaGraphF(location.x, location.y, 1.0f, angle, image, TRUE);

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


//体力減少処理
void Player::DecreaseHp(float value)
{
	this->hp += value;
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

float Player::GetGear() const
{
	return this->gear;
}


//燃料取得処理
float Player::GetFuel()const
{
	return this->fuel;
}


//体力取得処理
float Player::GetHp()const
{
	return this->hp;
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
	if (InputControl::GetButton(XINPUT_BUTTON_DPAD_UP) || InputControl::GetLStickDirection() == StickDirection::Up)
	{
		move += Vector2D(0.0f, -1.0f);
	}
	if (InputControl::GetButton(XINPUT_BUTTON_DPAD_DOWN) || InputControl::GetLStickDirection() == StickDirection::Down)
	{
		move += Vector2D(0.0f, 1.0f);
	}

	location += move;

	//画面外に行かないように制限する
	if ((location.x < box_size.x) || (location.x >= 1280.f - 180.0f) ||
		(location.y < box_size.y) || (location.y >= 720.0f - box_size.y))
	{
		location -= move;
	}
}

// ギアチェンジ処理
void Player::UpdateGear()
{
	if (InputControl::GetButton(XINPUT_BUTTON_A) && gear < 5) {
		gear++; // ギアアップ
	}
	if (InputControl::GetButton(XINPUT_BUTTON_B) && gear > 1) {
		gear--; // ギアダウン
	}
}



// //加減速処理 (長押し対応版)
//void Player::Acceleration()
//{
//	// 経過時間(秒)
//	// 現在の時刻を取得
//	const unsigned int current_time = GetNowCount();
//
//	// 経過時間(秒)を計算
//	float deltaTime = static_cast<float>(current_time - previous_time) * 0.001f; // ミリ秒から秒への変換
//
//	// 次のフレームのために現在の時刻を保存
//	previous_time = current_time;
//
//	const float acceleration_amount = 10.0f * deltaTime; // 秒間10スピード加速
//	const float deceleration_amount = 10.0f * deltaTime; // 秒間10スピード減速
//
//	//LBボタンが押されたら、減速する
//	if (InputControl::GetButton(XINPUT_BUTTON_LEFT_SHOULDER) && speed > 1.0f)
//	{
//		speed -= deceleration_amount;
//		if (speed < 1.0f) speed = 1.0f; // 最小速度を保証
//	}
//
//	//RBボタンが押されたら、加速する
//	if (InputControl::GetButton(XINPUT_BUTTON_RIGHT_SHOULDER) && speed < 100.0f)
//	{
//		speed += acceleration_amount;
//		if (speed > 100.0f) speed = 100.0f; // 最大速度を保証
//	}
//}


//加減速処理 (疑似スピードメーター)
void Player::Acceleration()
{
	// 経過時間(秒)
	// 現在の時刻を取得
	const unsigned int current_time = GetNowCount();

	// 経過時間(秒)を計算
	float deltaTime = static_cast<float>(current_time - previous_time) * 0.001f; // ミリ秒から秒への変換

	// 次のフレームのために現在の時刻を保存
	previous_time = current_time;

	// 加速度と減速度を調整
	const float acceleration_rate = 0.5f; // 加速率（秒間の加速度）
	const float deceleration_rate = 0.5f; // 減速率（秒間の減速度）

	// 加速
	if (InputControl::GetButton(XINPUT_BUTTON_RIGHT_SHOULDER) && speed < 100.0f) {
		// 加速量をdeltaTimeに応じて調整
		float acceleration_amount = acceleration_rate * deltaTime;
		speed += acceleration_amount;
		if (speed > 100.0f) speed = 100.0f; // 最大速度を保証
	}

	// 減速
	if (InputControl::GetButton(XINPUT_BUTTON_LEFT_SHOULDER) && speed > 1.0f) {
		// 減速量をdeltaTimeに応じて調整
		float deceleration_amount = deceleration_rate * deltaTime;
		speed -= deceleration_amount;
		if (speed < 1.0f) speed = 1.0f; // 最小速度を保証
	}

}