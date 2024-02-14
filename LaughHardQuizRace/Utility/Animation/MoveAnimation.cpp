#include "MoveAnimation.h"

MoveAnimation::MoveAnimation(float startX, float startY, float endX, float endY, float startScale, float endScale, int totalTime)
    : startX(startX), startY(startY), endX(endX), endY(endY),
    startScale(startScale), endScale(endScale),
    totalTime(totalTime), elapsedTime(0) {}

void MoveAnimation::Update(int deltaTime) {
    if (elapsedTime < totalTime) {
        elapsedTime += deltaTime;
        const float t = static_cast<float>(elapsedTime) / totalTime;

        // 現在の位置とスケールを計算
        currentX = Lerp(startX, endX, t);
        currentY = Lerp(startY, endY, t);
        currentScale = Lerp(startScale, endScale, t);
    }
}

void MoveAnimation::Draw() const {
    // 基準サイズにスケールを適用
    const int radius = static_cast<int>(20 * currentScale);
    DrawCircle(static_cast<int>(currentX), static_cast<int>(currentY), radius, GetColor(255, 0, 0), TRUE);
}


float MoveAnimation::Lerp(float start, float end, float t) {
    return start + (end - start) * t;
}