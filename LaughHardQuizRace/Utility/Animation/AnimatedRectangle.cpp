#include "AnimatedRectangle.h"

#include <DxLib.h>

AnimatedRectangle::AnimatedRectangle(int image, int duration, int height, int width, int centerY)
    : image(image), animationDuration(duration), finalHeight(height), rectWidth(width), centerY(centerY) {
    StartAnimation();
}

void AnimatedRectangle::StartAnimation() {
    startTime = GetNowCount();
}

void AnimatedRectangle::Draw() const
{
    // 現在時刻を取得
    const int currentTime = GetNowCount();
    // 開始時刻からの経過時間を計算
    const int elapsedTime = currentTime - startTime;
    // アニメーションの進捗度を計算（0.0から1.0の範囲）
    float progress = static_cast<float>(elapsedTime) / animationDuration;
    // 進捗度が1.0を超えないように制限
    if (progress > 1.0f) progress = 1.0f;

    // 現在の高さを計算（アニメーションの進捗に応じて）
    const int currentHeight = static_cast<int>(finalHeight * progress);
    // 長方形の上端のY座標を計算（中心から上下に伸ばす）
    const int startY = centerY - (currentHeight / 2);
    // 長方形の下端のY座標を計算
    const int endY = centerY + (currentHeight / 2);

    // 長方形を描画（中心位置が基準）
    //DrawBox(640 - (rectWidth / 2), startY, 640 + (rectWidth / 2), endY, GetColor(65, 105, 225), TRUE); // 青色で長方形を描画
    DrawRectExtendGraphF2(640 - (rectWidth / 2), startY, 640 + (rectWidth / 2), endY, 640, centerY, rectWidth, finalHeight, image, true);
	//DrawRotaGraph(640 - (rectWidth / 2), startY, ,1.0 image, true);
}
