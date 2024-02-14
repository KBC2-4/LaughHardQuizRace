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
    // ���ݎ������擾
    const int currentTime = GetNowCount();
    // �J�n��������̌o�ߎ��Ԃ��v�Z
    const int elapsedTime = currentTime - startTime;
    // �A�j���[�V�����̐i���x���v�Z�i0.0����1.0�͈̔́j
    float progress = static_cast<float>(elapsedTime) / animationDuration;
    // �i���x��1.0�𒴂��Ȃ��悤�ɐ���
    if (progress > 1.0f) progress = 1.0f;

    // ���݂̍������v�Z�i�A�j���[�V�����̐i���ɉ����āj
    const int currentHeight = static_cast<int>(finalHeight * progress);
    // �����`�̏�[��Y���W���v�Z�i���S����㉺�ɐL�΂��j
    const int startY = centerY - (currentHeight / 2);
    // �����`�̉��[��Y���W���v�Z
    const int endY = centerY + (currentHeight / 2);

    // �����`��`��i���S�ʒu����j
    //DrawBox(640 - (rectWidth / 2), startY, 640 + (rectWidth / 2), endY, GetColor(65, 105, 225), TRUE); // �F�Œ����`��`��
    DrawRectExtendGraphF2(640 - (rectWidth / 2), startY, 640 + (rectWidth / 2), endY, 640, centerY, rectWidth, finalHeight, image, true);
	//DrawRotaGraph(640 - (rectWidth / 2), startY, ,1.0 image, true);
}
