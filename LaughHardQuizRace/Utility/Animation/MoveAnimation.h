#include "DxLib.h"
#include <cmath>


class MoveAnimation {

private:
    int image;		// 画像ハンドル
    float startX, startY; // 開始位置
    float endX, endY;     // 終了位置
    float startScale, endScale; // 開始スケールと終了スケール
    int totalTime;        // アニメーションの総時間（ミリ秒）
    int elapsedTime;      // 経過時間（ミリ秒）
    float currentX, currentY; // 現在の位置
    float currentScale;   // 現在のスケール

private:
    // イージング関数（線形補間）
    float Lerp(float start, float end, float t);

public:
	MoveAnimation() = default;
    MoveAnimation(int image, float startX, float startY, float endX, float endY, float startScale, float endScale, int totalTime);
    
    void Update(int deltaTime);
    void Draw() const;
    void Close();
};


//    // アニメーションの例: 3秒かけて(100,100)から(400,300)へ移動し、スケールを0.5から2.0へ変更
//    MoveAnimation anim(100, 100, 400, 300, 0.5f, 2.0f, 3000);
//
//        anim.Update(16); // 仮定: 1フレームあたり16ms
//        anim.Draw();
