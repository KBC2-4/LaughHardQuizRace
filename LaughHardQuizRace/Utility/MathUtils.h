#pragma once
#include <cmath> // std::pow �Ȃǂ̐��w�֐��p

class MathUtils {
public:
    // ���`��ԁiLerp�j�֐�
    static float Lerp(float start, float end, float t) {
        return start + (end - start) * t;
    }

    // �C�[�W���O�֐��iEase-In-Out�j
    static float EaseInOut(float t) {
        if (t < 0.5f) {
            return 2.0f * t * t;
        }
        else {
            return -1.0f + (4.0f - 2.0f * t) * t;
        }
    }
};