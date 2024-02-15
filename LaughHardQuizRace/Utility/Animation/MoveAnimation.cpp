#include "MoveAnimation.h"

MoveAnimation::MoveAnimation(int image, float startX, float startY, float endX, float endY, float startScale, float endScale, int totalTime)
	: image(image), startX(startX), startY(startY), endX(endX), endY(endY),
	startScale(startScale), endScale(endScale),opacity(1.0f),
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
	//DrawCircle(static_cast<int>(currentX), static_cast<int>(currentY), radius, GetColor(255, 0, 0), TRUE);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(opacity * 255));
	DrawRotaGraph(static_cast<int>(currentX), static_cast<int>(currentY), radius, 1.0, image, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

void MoveAnimation::Close()
{
	//if (elapsedTime < totalTime) {
	//	elapsedTime += 16;
	//	const float t = static_cast<float>(elapsedTime) / totalTime;

	//	// 徐々に消える
	//	opacity = Lerp(1.0f, 0.0f, t);


	//}
	//else
	//{
		// 各変数をリセット
		elapsedTime = 0;
		currentX = startX;
		currentY = startY;
		currentScale = startScale;
	//}
}


float MoveAnimation::Lerp(float start, float end, float t) {
	return start + (end - start) * t;
}
