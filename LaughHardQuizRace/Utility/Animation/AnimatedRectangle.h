#pragma once
class AnimatedRectangle
{
private:
	int image;		// 画像ハンドル
	int startTime; // アニメーション開始時間
	int animationDuration = 1000; // アニメーションの総持続時間（ミリ秒）
	int finalHeight = 600; // 長方形の最終的な高さ
	int rectWidth = 800; // 長方形の幅
	int centerY = 340; // 長方形の中心のY座標

public:
	AnimatedRectangle() = default;
	AnimatedRectangle(int image, int duration, int height, int width, int centerY);
	~AnimatedRectangle() = default;
	void StartAnimation();
	void Draw() const;
};

