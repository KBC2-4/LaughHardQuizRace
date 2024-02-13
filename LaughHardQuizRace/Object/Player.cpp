#include "Player.h"
#include "../Utility/InputControl.h"
#include "DxLib.h"
#include "../Utility/MathUtils.h"


Player::Player() :is_active(false), image(NULL), location(0.0f), box_size(0.0f),
angle(0.0f),speed(0.0f), gear(0), hp(0.0f), fuel(0.0f)
{
	// ���݂̌o�ߎ��Ԃ��擾
	previous_time = GetNowCount();

}


Player::~Player()
{

}


//����������
void Player::Initialize()
{
	is_active = true;
	location = Vector2D(320.0f, 380.0f);
	box_size = Vector2D(31.0f, 60.0f);
	angle = 0.0f;
	speed = 1.0f;
	gear = 0;
	hp = 1000;
	fuel = 6000;

	//�摜�̓ǂݍ���
	image = LoadGraph("Resource/images/Scene/GameMain/player.png");

	//�G���[�`�F�b�N
	if (image == -1)
	{
		throw("Resource/images/Scene/GameMain/player.png������܂���\n");
	}
}



//�X�V����
void Player::Update()
{
	//����s��Ԃł���΁A���g����]������
	if (!is_active)
	{
		angle += DX_PI_F / 24.0f;
		speed = 1.0f;
		if (angle >= DX_PI_F * 4.0f)
		{
			is_active = true;
		}
		return;
	}

	//�R���̏���
	fuel -= speed;

	//�ړ�����
	Movement();

	//����������
	Acceleration();

	//�o���A����
	//if (InputControl::GetButtonDown(XINPUT_BUTTON_B) && barrier_count > 0)
	//{
	//	if (barrier == nullptr)
	//	{
	//		barrier_count--;
	//		barrier = new Barrier;
	//	}
	//}

	////�o���A����������Ă�����A�X�V���s��
	//if (barrier != nullptr)
	//{
	//	//�o���A���Ԃ��o�߂������H���Ă�����A�폜����
	//	if (barrier->IsFinished(this->speed))
	//	{
	//		delete barrier;
	//		barrier = nullptr;
	//	}
	//}
}


//�`�揈��
void Player::Draw()
{
	//�v���C���[�摜�̕`��
	DrawRotaGraphF(location.x, location.y, 1.0f, angle, image, TRUE);

}


//�I��������
void Player::Finalize()
{
	//�ǂݍ��񂾉摜���폜
	DeleteGraph(image);

}


//��Ԑݒ菈��
void Player::SetActive(bool flg)
{
	this->is_active = flg;
}


//�̗͌�������
void Player::DecreaseHp(float value)
{
	this->hp += value;
}


//�ʒu���擾����
Vector2D Player::GetLocation()const
{
	return this->location;
}


//�����蔻��̑傫���擾����
Vector2D Player::GetBoxSize()const
{
	return this->box_size;
}


//�����擾����
float Player::GetSpeed()const
{
	return this->speed;
}

float Player::GetGear() const
{
	return this->gear;
}


//�R���擾����
float Player::GetFuel()const
{
	return this->fuel;
}


//�̗͎擾����
float Player::GetHp()const
{
	return this->hp;
}


//�ړ�����
void Player::Movement()
{
	Vector2D move = Vector2D(0.0f);
	angle = 0.0f;

	//�\���ړ�����
	if (InputControl::GetButton(XINPUT_BUTTON_DPAD_LEFT) || InputControl::GetLStickDirection() == StickDirection::Left)
	{
		move += Vector2D(-1.0f, 0.0f);
		angle = -DX_PI_F / 18;
	}
	if (InputControl::GetButton(XINPUT_BUTTON_DPAD_RIGHT) || InputControl::GetLStickDirection() == StickDirection::Right)
	{
		move += Vector2D(1.0f, 0.0f);
		angle = DX_PI_F / 18;
	}
	if (InputControl::GetButton(XINPUT_BUTTON_DPAD_UP) || InputControl::GetLStickDirection() == StickDirection::Up)
	{
		move += Vector2D(0.0f, -1.0f);
	}
	if (InputControl::GetButton(XINPUT_BUTTON_DPAD_DOWN) || InputControl::GetLStickDirection() == StickDirection::Down)
	{
		move += Vector2D(0.0f, 1.0f);
	}

	location += move;

	//��ʊO�ɍs���Ȃ��悤�ɐ�������
	if ((location.x < box_size.x) || (location.x >= 1280.f - 180.0f) ||
		(location.y < box_size.y) || (location.y >= 720.0f - box_size.y))
	{
		location -= move;
	}
}

// �M�A�`�F���W����
void Player::UpdateGear()
{
	if (InputControl::GetButton(XINPUT_BUTTON_A) && gear < 5) {
		gear++; // �M�A�A�b�v
	}
	if (InputControl::GetButton(XINPUT_BUTTON_B) && gear > 1) {
		gear--; // �M�A�_�E��
	}
}



// //���������� (�������Ή���)
//void Player::Acceleration()
//{
//	// �o�ߎ���(�b)
//	// ���݂̎������擾
//	const unsigned int current_time = GetNowCount();
//
//	// �o�ߎ���(�b)���v�Z
//	float deltaTime = static_cast<float>(current_time - previous_time) * 0.001f; // �~���b����b�ւ̕ϊ�
//
//	// ���̃t���[���̂��߂Ɍ��݂̎�����ۑ�
//	previous_time = current_time;
//
//	const float acceleration_amount = 10.0f * deltaTime; // �b��10�X�s�[�h����
//	const float deceleration_amount = 10.0f * deltaTime; // �b��10�X�s�[�h����
//
//	//LB�{�^���������ꂽ��A��������
//	if (InputControl::GetButton(XINPUT_BUTTON_LEFT_SHOULDER) && speed > 1.0f)
//	{
//		speed -= deceleration_amount;
//		if (speed < 1.0f) speed = 1.0f; // �ŏ����x��ۏ�
//	}
//
//	//RB�{�^���������ꂽ��A��������
//	if (InputControl::GetButton(XINPUT_BUTTON_RIGHT_SHOULDER) && speed < 100.0f)
//	{
//		speed += acceleration_amount;
//		if (speed > 100.0f) speed = 100.0f; // �ő呬�x��ۏ�
//	}
//}


//���������� (�^���X�s�[�h���[�^�[)
void Player::Acceleration()
{
	// �o�ߎ���(�b)
	// ���݂̎������擾
	const unsigned int current_time = GetNowCount();

	// �o�ߎ���(�b)���v�Z
	float deltaTime = static_cast<float>(current_time - previous_time) * 0.001f; // �~���b����b�ւ̕ϊ�

	// ���̃t���[���̂��߂Ɍ��݂̎�����ۑ�
	previous_time = current_time;

	// �����x�ƌ����x�𒲐�
	const float acceleration_rate = 0.5f; // �������i�b�Ԃ̉����x�j
	const float deceleration_rate = 0.5f; // �������i�b�Ԃ̌����x�j

	// ����
	if (InputControl::GetButton(XINPUT_BUTTON_RIGHT_SHOULDER) && speed < 100.0f) {
		// �����ʂ�deltaTime�ɉ����Ē���
		float acceleration_amount = acceleration_rate * deltaTime;
		speed += acceleration_amount;
		if (speed > 100.0f) speed = 100.0f; // �ő呬�x��ۏ�
	}

	// ����
	if (InputControl::GetButton(XINPUT_BUTTON_LEFT_SHOULDER) && speed > 1.0f) {
		// �����ʂ�deltaTime�ɉ����Ē���
		float deceleration_amount = deceleration_rate * deltaTime;
		speed -= deceleration_amount;
		if (speed < 1.0f) speed = 1.0f; // �ŏ����x��ۏ�
	}

}