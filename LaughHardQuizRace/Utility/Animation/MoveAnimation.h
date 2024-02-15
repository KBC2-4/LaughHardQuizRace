#include "DxLib.h"
#include <cmath>


class MoveAnimation {

private:
    int image;		// �摜�n���h��
    float startX, startY; // �J�n�ʒu
    float endX, endY;     // �I���ʒu
    float startScale, endScale; // �J�n�X�P�[���ƏI���X�P�[��
    int totalTime;        // �A�j���[�V�����̑����ԁi�~���b�j
    int elapsedTime;      // �o�ߎ��ԁi�~���b�j
    float currentX, currentY; // ���݂̈ʒu
    float currentScale;   // ���݂̃X�P�[��

private:
    // �C�[�W���O�֐��i���`��ԁj
    float Lerp(float start, float end, float t);

public:
	MoveAnimation() = default;
    MoveAnimation(int image, float startX, float startY, float endX, float endY, float startScale, float endScale, int totalTime);
    
    void Update(int deltaTime);
    void Draw() const;
    void Close();
};


//    // �A�j���[�V�����̗�: 3�b������(100,100)����(400,300)�ֈړ����A�X�P�[����0.5����2.0�֕ύX
//    MoveAnimation anim(100, 100, 400, 300, 0.5f, 2.0f, 3000);
//
//        anim.Update(16); // ����: 1�t���[��������16ms
//        anim.Draw();
