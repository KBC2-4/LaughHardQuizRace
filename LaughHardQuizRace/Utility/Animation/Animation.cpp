#include "Animation.h"
#define _USE_MATH_DEFINES
#include <cmath> 
#include <DxLib.h>

Animation::Animation(float cycleDuration, float amplitudeSize)
    : cycle(cycleDuration), amplitude(amplitudeSize)
{
}

float Animation::GetAnimationOffset(int currentTime) const {
	// sin”g
    return sin((currentTime % static_cast<int>(cycle)) * 2 * DX_PI_F / cycle) * amplitude;
}

float Animation::GetHorizontalAnimationOffset(int currentTime) const
{
    return cos((currentTime % static_cast<int>(cycle)) * 2 * M_PI / cycle) * amplitude;
}

float Animation::GetScaleAnimation(int currentTime) const
{
	// Šg‘åk¬‚ÌƒXƒP[ƒ‹‚Í1.0‚ğŠî€‚É‚·‚é
    return 1.0f + sin((currentTime % static_cast<int>(cycle)) * 2 * M_PI / cycle) * (amplitude / 10.0f);
}

float Animation::GetRotationAnimation(int currentTime) const
{
    // üŠú“à‚Å360“x‚Ì‰ñ“]‚ğŠ®—¹‚·‚é
    return (currentTime % static_cast<int>(cycle)) * 2 * M_PI / cycle;
}
