#pragma once
class Animation
{
private:
	float cycle; // �A�j���[�V�����̎����i�~���b�j
	float amplitude; // �A�j���[�V�����̐U���i�㉺���A���E���A�g��k���j

public:
	Animation() = default;
	Animation(float cycleDuration = 2000.0f, float amplitudeSize = 2.0f);
	~Animation() = default;


	/**
	 * \brief �㉺���A�j���[�V�����̃I�t�Z�b�g���擾
	 * \param currentTime 
	 * \return 
	 */
	float GetAnimationOffset(int currentTime) const;

	/**
	 * \brief ���E���A�j���[�V�����̃I�t�Z�b�g���擾
	 * \param currentTime
	 * \return
	 */
	float GetHorizontalAnimationOffset(int currentTime) const;

	/**
	 * \brief �g��k���X�P�[���A�j���[�V�����̃I�t�Z�b�g���擾
	 * \param currentTime
	 * \return
	 */
	float GetScaleAnimation(int currentTime) const;

	/**
	 * \brief ��]�A�j���[�V�����̃I�t�Z�b�g���擾
	 * \param currentTime
	 * \return
	 */
	float GetRotationAnimation(int currentTime) const;
};

