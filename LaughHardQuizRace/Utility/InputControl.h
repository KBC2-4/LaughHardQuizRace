#pragma once

#include "Vector2D.h"

// スティックの方向
enum class StickDirection {
	None,
	Up,
	Down,
	Left,
	Right
};

//入力管理機能

class InputControl
{
private:
	static bool now_button[16];  //現在フレーム入力値
	static bool old_button[16];  //過去フレーム入力値
	static float trigger[2];     //左右トリガー入力値
	static Vector2D stick[2];    //左右スティック入力値
	static int frame_counter;  // フレームカウンター

public:
	static void Update();     //更新処理

	//ボタン入力取得処理
	static bool GetButton(int button);       //押し続けている間
	static bool GetButtonDown(int button);   //押した瞬間
	static bool GetButtonUp(int button);     //離した瞬間

	//トリガー入力取得処理
	static float GetLeftTrigger();   //左トリガー
	static float GetRightTrigger();  //右トリガー

	//スティック入力取得処理
	static Vector2D GetLeftStick();   //左スティック
	static Vector2D GetRightStick();  //右スティック
	/**
	* @brief 左スティックが閾値を超えているかどうかを返す
	* @param threshold 閾値(デフォルトは0)
	* @param interval_ms 前回の操作からの入力を受け付けるフレーム数(デフォルトは0)
	* @return 閾値を超えているかどうか
	* 使用例
	*	const StickDirection direction = InputControl::GetLStickDirection(20000, 0);
		if (direction == StickDirection::Down) {
		menu_cursor = (menu_cursor + 1) % 4;
		}
	*/
	static StickDirection GetLStickDirection(int threshold = 20000, int interval_ms = 0);

private:
	//ボタン配列範囲チェック
	static bool CheckButtonRange(int button);
};