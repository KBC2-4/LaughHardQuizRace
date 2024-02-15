#include "Player.h"
#include "../Utility/InputControl.h"
#include "DxLib.h"
#include "../Utility/MathUtils.h"


Player::Player() :is_active(false), image(NULL), location(0.0f), box_size(0.0f), is_stop(false),
angle(0.0f),speed(0.0f), anim(2000.0f, 2.0f)
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
	box_size = Vector2D(140.0f, 200.0f);
	angle = 0.0f;
	speed = 1.0f;
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
		speed = 0.2f;
		if (angle >= DX_PI_F * 4.0f)
		{
			is_active = true;
		}
		return;
	}


	if (!is_stop) {

		//�ړ�����
		Movement();
	}

	// �A�C�h���A�j���[�V����

	 // ���݂̎��Ԃ��擾
	const int currentTime = GetNowCount();

	// �A�j���[�V�����̍X�V
	const float animationOffset = anim.GetAnimationOffset(currentTime);

	// �A�j���[�V�����ɂ��ʒu�̍X�V
	location.y += animationOffset;

}


//�`�揈��
void Player::Draw()
{
	//�v���C���[�摜�̕`��
	DrawRotaGraphF(location.x, location.y, 1.0f, angle, image, TRUE);

	// �����蔻���`��
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);;
	//DrawBoxAA(location.x - box_size.x / 2, location.y - box_size.y / 2, location.x + box_size.x / 2, location.y + box_size.y / 2, GetColor(255, 0, 0), TRUE, -1);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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
	//if (InputControl::GetButton(XINPUT_BUTTON_DPAD_UP) || InputControl::GetLStickDirection() == StickDirection::Up)
	//{
	//	move += Vector2D(0.0f, -1.0f);
	//}
	//if (InputControl::GetButton(XINPUT_BUTTON_DPAD_DOWN) || InputControl::GetLStickDirection() == StickDirection::Down)
	//{
	//	move += Vector2D(0.0f, 1.0f);
	//}

	location += move;

	//��ʊO�ɍs���Ȃ��悤�ɐ�������
	if ((location.x < box_size.x) || (location.x >= 1280.f - 180.0f) ||
		(location.y < box_size.y) || (location.y >= 720.0f - box_size.y))
	{
		location -= move;
	}
}

void Player::IsStop(bool result)
{
	this->is_stop = result;
}