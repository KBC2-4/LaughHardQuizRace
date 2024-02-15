#include "MoveAnimation.h"

MoveAnimation::MoveAnimation(int image, float startX, float startY, float endX, float endY, float startScale, float endScale, int totalTime)
    : image(image), startX(startX), startY(startY), endX(endX), endY(endY),
    startScale(startScale), endScale(endScale),
    totalTime(totalTime), elapsedTime(0) {}

void MoveAnimation::Update(int deltaTime) {
    if (elapsedTime < totalTime) {
        elapsedTime += deltaTime;
        const float t = static_cast<float>(elapsedTime) / totalTime;

        // ���݂̈ʒu�ƃX�P�[�����v�Z
        currentX = Lerp(startX, endX, t);
        currentY = Lerp(startY, endY, t);
        currentScale = Lerp(startScale, endScale, t);
    }
}

void MoveAnimation::Draw() const {
    // ��T�C�Y�ɃX�P�[����K�p
    const int radius = static_cast<int>(20 * currentScale);
    //DrawCircle(static_cast<int>(currentX), static_cast<int>(currentY), radius, GetColor(255, 0, 0), TRUE);
    DrawRotaGraph(static_cast<int>(currentX), static_cast<int>(currentY), radius, 1.0, image, true);

}

void MoveAnimation::Close()
{
	currentX = startX;
	currentY = startY;
	currentScale = startScale;
}


float MoveAnimation::Lerp(float start, float end, float t) {
    return start + (end - start) * t;
}
