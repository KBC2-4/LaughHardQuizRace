#include "Animation.h"
#define _USE_MATH_DEFINES
#include <cmath> 
#include <DxLib.h>

Animation::Animation(float cycleDuration, float amplitudeSize)
    : cycle(cycleDuration), amplitude(amplitudeSize)
{
}

float Animation::GetAnimationOffset(int currentTime) const {
	// sin波
    return sin((currentTime % static_cast<int>(cycle)) * 2 * DX_PI_F / cycle) * amplitude;
}

float Animation::GetHorizontalAnimationOffset(int currentTime) const
{
    return cos((currentTime % static_cast<int>(cycle)) * 2 * M_PI / cycle) * amplitude;
}

float Animation::GetScaleAnimation(int currentTime) const
{
	// 拡大縮小のスケールは1.0を基準にする
    return 1.0f + sin((currentTime % static_cast<int>(cycle)) * 2 * M_PI / cycle) * (amplitude / 10.0f);
}

float Animation::GetRotationAnimation(int currentTime) const
{
    // 周期内で360度の回転を完了する
    return (currentTime % static_cast<int>(cycle)) * 2 * M_PI / cycle;
}
