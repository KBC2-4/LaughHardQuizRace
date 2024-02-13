#include "InputControl.h"
#include "DxLib.h"
#include <iostream>

//静的メンバ変数定義
bool InputControl::now_button[16] = {};
bool InputControl::old_button[16] = {};
float InputControl::trigger[2] = {};
Vector2D InputControl::stick[2] = {};
int InputControl::frame_counter = 0;

//入力機能：更新処理
void InputControl::Update()
{
	//XIputコントローラーの入力値を取得する
	XINPUT_STATE input_state = {};
	GetJoypadXInputState(DX_INPUT_PAD1, &input_state);

	//ボタン入力値の更新
	for (int i = 0; i < 16; i++)
	{
		old_button[i] = now_button[i];
		now_button[i] = static_cast<bool>(input_state.Buttons[i]);
	}

	//トリガー入力の更新(0.0f～1.0fに範囲を制限する)
	trigger[0] = static_cast<float>(input_state.LeftTrigger) / static_cast<float>(UCHAR_MAX);
	trigger[1] = static_cast<float>(input_state.RightTrigger) / static_cast<float>(UCHAR_MAX);

	//左スティック入力値の更新(-1.0f～1.0fに範囲を制限する)
	if (input_state.ThumbLX > 0.0f)
	{
		stick[0].x = static_cast<float>(input_state.ThumbLX) / static_cast<float>(SHRT_MAX);
	}
	else
	{
		stick[0].x = -(static_cast<float>(input_state.ThumbLX) / static_cast<float>(SHRT_MIN));
	}
	if (input_state.ThumbLY > 0.0f)
	{
		stick[0].y = static_cast<float>(input_state.ThumbLY) / static_cast<float>(SHRT_MAX);
	}
	else
	{
		stick[0].y = static_cast<float>(input_state.ThumbLY) / static_cast<float>(SHRT_MIN);
	}

	//右スティック入力値の更新(-1.0f～1.0fに範囲を制限する)
	if (input_state.ThumbRX > 0.0f)
	{
		stick[1].x = static_cast<float>(input_state.ThumbRX) / static_cast<float>(SHRT_MAX);
	}
	else
	{
		stick[1].x = -(static_cast<float>(input_state.ThumbRX) / static_cast<float>(SHRT_MIN));
	}
	if (input_state.ThumbRY > 0.0f)
	{
		stick[1].y = static_cast<float>(input_state.ThumbRY) / static_cast<float>(SHRT_MAX);
	}
	else
	{
		stick[1].y = static_cast<float>(input_state.ThumbRY) / static_cast<float>(SHRT_MIN);
	}
}

//ボタン取得：押してる間
bool InputControl::GetButton(int button)
{
	return CheckButtonRange(button) && (now_button[button] &&
		old_button[button]);
}

//ボタン取得：押した瞬間
bool InputControl::GetButtonDown(int button)
{
	return CheckButtonRange(button) && (now_button[button] &&
		!old_button[button]);
}

//ボタン取得：離した瞬間
bool InputControl::GetButtonUp(int button)
{
	return CheckButtonRange(button) && (!now_button[button] &&
		old_button[button]);
}

//左トリガー取得
float InputControl::GetLeftTrigger()
{
	return trigger[0];
}

//右トリガー取得
float InputControl::GetRightTrigger()
{
	return trigger[1];
}

//左スティック取得
Vector2D InputControl::GetLeftStick()
{
	return stick[0];
}

//右スティック取得
Vector2D InputControl::GetRightStick()
{
	return stick[1];
}

//ボタン配列範囲チェック
bool InputControl::CheckButtonRange(int button)
{
	return(0 <= button && button < 16);
}

/**
* @brief 左スティックが閾値を超えているかどうかを返す
* @param threshold 閾値(デフォルトは20000)
* @param interval_ms 前回の操作からの入力を受け付けるフレーム数(デフォルトは0)
* @return 閾値を超えているかどうか
*/
StickDirection InputControl::GetLStickDirection(int threshold, int interval_ms)
{
	//XIputコントローラーの入力値を取得する
	XINPUT_STATE input_state = {};
	GetJoypadXInputState(DX_INPUT_PAD1, &input_state);

	frame_counter++;

	// フレームカウンタが一定の値を超えたらリセット
	if (frame_counter > INT_MAX / 2) {
		InputControl::frame_counter = 0;
	}

	if (InputControl::frame_counter < interval_ms) {
		return StickDirection::None;
	}

	StickDirection direction = StickDirection::None;

	if (std::abs(input_state.ThumbLX) > threshold) {
		direction = input_state.ThumbLX > 0 ? StickDirection::Right : StickDirection::Left;
	}
	else if (std::abs(input_state.ThumbLY) > threshold) {
		direction = input_state.ThumbLY > 0 ? StickDirection::Up : StickDirection::Down;
	}

	if (direction != StickDirection::None) {
		frame_counter = 0;
	}


	return direction;
}