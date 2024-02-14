#pragma once
class Animation
{
private:
	float cycle; // アニメーションの周期（ミリ秒）
	float amplitude; // アニメーションの振幅（上下動、左右動、拡大縮小）

public:
	Animation() = default;
	Animation(float cycleDuration = 2000.0f, float amplitudeSize = 2.0f);
	~Animation() = default;


	/**
	 * \brief 上下動アニメーションのオフセットを取得
	 * \param currentTime 
	 * \return 
	 */
	float GetAnimationOffset(int currentTime) const;

	/**
	 * \brief 左右動アニメーションのオフセットを取得
	 * \param currentTime
	 * \return
	 */
	float GetHorizontalAnimationOffset(int currentTime) const;

	/**
	 * \brief 拡大縮小スケールアニメーションのオフセットを取得
	 * \param currentTime
	 * \return
	 */
	float GetScaleAnimation(int currentTime) const;

	/**
	 * \brief 回転アニメーションのオフセットを取得
	 * \param currentTime
	 * \return
	 */
	float GetRotationAnimation(int currentTime) const;
};

