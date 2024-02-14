#pragma once
class AnimatedRectangle
{
private:
	int image;		// �摜�n���h��
	int startTime; // �A�j���[�V�����J�n����
	int animationDuration = 1000; // �A�j���[�V�����̑��������ԁi�~���b�j
	int finalHeight = 600; // �����`�̍ŏI�I�ȍ���
	int rectWidth = 800; // �����`�̕�
	int centerY = 340; // �����`�̒��S��Y���W

public:
	AnimatedRectangle() = default;
	AnimatedRectangle(int image, int duration, int height, int width, int centerY);
	~AnimatedRectangle() = default;
	void StartAnimation();
	void Draw() const;
};

