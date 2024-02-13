#pragma once
#include <cmath> // std::pow などの数学関数用

class MathUtils {
public:
    // 線形補間（Lerp）関数
    static float Lerp(float start, float end, float t) {
        return start + (end - start) * t;
    }

    // イージング関数（Ease-In-Out）
    static float EaseInOut(float t) {
        if (t < 0.5f) {
            return 2.0f * t * t;
        }
        else {
            return -1.0f + (4.0f - 2.0f * t) * t;
        }
    }
};