#pragma once

#include "Vector2D.h"

// �X�e�B�b�N�̕���
enum class StickDirection {
	None,
	Up,
	Down,
	Left,
	Right
};

//���͊Ǘ��@�\

class InputControl
{
private:
	static bool now_button[16];  //���݃t���[�����͒l
	static bool old_button[16];  //�ߋ��t���[�����͒l
	static float trigger[2];     //���E�g���K�[���͒l
	static Vector2D stick[2];    //���E�X�e�B�b�N���͒l
	static int frame_counter;  // �t���[���J�E���^�[

public:
	static void Update();     //�X�V����

	//�{�^�����͎擾����
	static bool GetButton(int button);       //���������Ă����
	static bool GetButtonDown(int button);   //�������u��
	static bool GetButtonUp(int button);     //�������u��

	//�g���K�[���͎擾����
	static float GetLeftTrigger();   //���g���K�[
	static float GetRightTrigger();  //�E�g���K�[

	//�X�e�B�b�N���͎擾����
	static Vector2D GetLeftStick();   //���X�e�B�b�N
	static Vector2D GetRightStick();  //�E�X�e�B�b�N
	/**
	* @brief ���X�e�B�b�N��臒l�𒴂��Ă��邩�ǂ�����Ԃ�
	* @param threshold 臒l(�f�t�H���g��0)
	* @param interval_ms �O��̑��삩��̓��͂��󂯕t����t���[����(�f�t�H���g��0)
	* @return 臒l�𒴂��Ă��邩�ǂ���
	* �g�p��
	*	const StickDirection direction = InputControl::GetLStickDirection(20000, 0);
		if (direction == StickDirection::Down) {
		menu_cursor = (menu_cursor + 1) % 4;
		}
	*/
	static StickDirection GetLStickDirection(int threshold = 20000, int interval_ms = 0);

private:
	//�{�^���z��͈̓`�F�b�N
	static bool CheckButtonRange(int button);
};