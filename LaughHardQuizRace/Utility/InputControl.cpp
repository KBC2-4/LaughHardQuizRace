#include "InputControl.h"
#include "DxLib.h"
#include <iostream>

//�ÓI�����o�ϐ���`
bool InputControl::now_button[16] = {};
bool InputControl::old_button[16] = {};
float InputControl::trigger[2] = {};
Vector2D InputControl::stick[2] = {};
int InputControl::frame_counter = 0;

//���͋@�\�F�X�V����
void InputControl::Update()
{
	//XIput�R���g���[���[�̓��͒l���擾����
	XINPUT_STATE input_state = {};
	GetJoypadXInputState(DX_INPUT_PAD1, &input_state);

	//�{�^�����͒l�̍X�V
	for (int i = 0; i < 16; i++)
	{
		old_button[i] = now_button[i];
		now_button[i] = static_cast<bool>(input_state.Buttons[i]);
	}

	//�g���K�[���͂̍X�V(0.0f�`1.0f�ɔ͈͂𐧌�����)
	trigger[0] = static_cast<float>(input_state.LeftTrigger) / static_cast<float>(UCHAR_MAX);
	trigger[1] = static_cast<float>(input_state.RightTrigger) / static_cast<float>(UCHAR_MAX);

	//���X�e�B�b�N���͒l�̍X�V(-1.0f�`1.0f�ɔ͈͂𐧌�����)
	if (input_state.ThumbLX > 0.0f)
	{
		stick[0].x = static_cast<float>(input_state.ThumbLX) / static_cast<float>(SHRT_MAX);
	}
	else
	{
		stick[0].x = -(static_cast<float>(input_state.ThumbLX) / static_cast<float>(SHRT_MIN));
	}
	if (input_state.ThumbLY > 0.0f)
	{
		stick[0].y = static_cast<float>(input_state.ThumbLY) / static_cast<float>(SHRT_MAX);
	}
	else
	{
		stick[0].y = static_cast<float>(input_state.ThumbLY) / static_cast<float>(SHRT_MIN);
	}

	//�E�X�e�B�b�N���͒l�̍X�V(-1.0f�`1.0f�ɔ͈͂𐧌�����)
	if (input_state.ThumbRX > 0.0f)
	{
		stick[1].x = static_cast<float>(input_state.ThumbRX) / static_cast<float>(SHRT_MAX);
	}
	else
	{
		stick[1].x = -(static_cast<float>(input_state.ThumbRX) / static_cast<float>(SHRT_MIN));
	}
	if (input_state.ThumbRY > 0.0f)
	{
		stick[1].y = static_cast<float>(input_state.ThumbRY) / static_cast<float>(SHRT_MAX);
	}
	else
	{
		stick[1].y = static_cast<float>(input_state.ThumbRY) / static_cast<float>(SHRT_MIN);
	}
}

//�{�^���擾�F�����Ă��
bool InputControl::GetButton(int button)
{
	return CheckButtonRange(button) && (now_button[button] &&
		old_button[button]);
}

//�{�^���擾�F�������u��
bool InputControl::GetButtonDown(int button)
{
	return CheckButtonRange(button) && (now_button[button] &&
		!old_button[button]);
}

//�{�^���擾�F�������u��
bool InputControl::GetButtonUp(int button)
{
	return CheckButtonRange(button) && (!now_button[button] &&
		old_button[button]);
}

//���g���K�[�擾
float InputControl::GetLeftTrigger()
{
	return trigger[0];
}

//�E�g���K�[�擾
float InputControl::GetRightTrigger()
{
	return trigger[1];
}

//���X�e�B�b�N�擾
Vector2D InputControl::GetLeftStick()
{
	return stick[0];
}

//�E�X�e�B�b�N�擾
Vector2D InputControl::GetRightStick()
{
	return stick[1];
}

//�{�^���z��͈̓`�F�b�N
bool InputControl::CheckButtonRange(int button)
{
	return(0 <= button && button < 16);
}

/**
* @brief ���X�e�B�b�N��臒l�𒴂��Ă��邩�ǂ�����Ԃ�
* @param threshold 臒l(�f�t�H���g��20000)
* @param interval_ms �O��̑��삩��̓��͂��󂯕t����t���[����(�f�t�H���g��0)
* @return 臒l�𒴂��Ă��邩�ǂ���
*/
StickDirection InputControl::GetLStickDirection(int threshold, int interval_ms)
{
	//XIput�R���g���[���[�̓��͒l���擾����
	XINPUT_STATE input_state = {};
	GetJoypadXInputState(DX_INPUT_PAD1, &input_state);

	frame_counter++;

	// �t���[���J�E���^�����̒l�𒴂����烊�Z�b�g
	if (frame_counter > INT_MAX / 2) {
		InputControl::frame_counter = 0;
	}

	if (InputControl::frame_counter < interval_ms) {
		return StickDirection::None;
	}

	StickDirection direction = StickDirection::None;

	if (std::abs(input_state.ThumbLX) > threshold) {
		direction = input_state.ThumbLX > 0 ? StickDirection::Right : StickDirection::Left;
	}
	else if (std::abs(input_state.ThumbLY) > threshold) {
		direction = input_state.ThumbLY > 0 ? StickDirection::Up : StickDirection::Down;
	}

	if (direction != StickDirection::None) {
		frame_counter = 0;
	}


	return direction;
}