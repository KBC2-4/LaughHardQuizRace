#include "Enemy.h"
#include "DxLib.h"

Enemy::Enemy(int type, int handle, bool appreciation) :type(type), image(handle), speed_x(0.0f),
location(0.0f), box_size(0.0f), is_stop(false), scale(0.7f)
{
	if (appreciation) {
		
		//scale = 1.5f; // �X�P�[����1.5f�ɐݒ�
		location = Vector2D(1380.0f, 400.0f);
		return;
	}
	// type��9�����傫���ꍇ
	if (this->type > 9) {
		scale = 1.0f; // �X�P�[����1.0f�ɐݒ�

		//�����蔻��̐ݒ�
		box_size = Vector2D(42.0f, 60.0f);

		// �X�s�[�h��type��9�𒴂���䗦��������
		speed_x += (this->type - 9) * (2.0f / (this->type - 9));
	}
}
	

Enemy::~Enemy()
{

}

void Enemy::Initialize()
{
	//�o��������X���W�p�^�[�����擾
	//const float random_y = static_cast<float>(GetRand(4) * 105 + 40);
	//�����ʒu�̐ݒ�
	//location = Vector2D(1380.0f, random_y);
	location = Vector2D(1380.0f, 400.0f);
	//�����蔻��̐ݒ�
	box_size = Vector2D(31.0f, 60.0f);
	//�����̐ݒ�
	speed_x = 2;

}


void Enemy::Updata(float speed)
{

	if (is_stop) { return; }
	//�ʒu���Ɉړ��ʂ����Z����
	location += Vector2D(this->speed_x + speed - 6, 0.0f);
}

void Enemy::Update()
{
	location += Vector2D(this->speed_x * GetRand(1), this->speed_y);

	//// �E�B���h�E�T�C�Y���擾
	//int width, height;
	//width = 1280;
	//height = 720;

	//// ��ʂ̍��[�𒴂����ꍇ
	//if (location.x < 0) {
	//	location.x = static_cast<float>(width); // ��ʂ̉E�[����ďo��
	//}
	//// ��ʂ̉E�[�𒴂����ꍇ
	//else if (location.x > static_cast<float>(width)) {
	//	location.x = 0; // ��ʂ̍��[����ďo��
	//}

	//// ��ʂ̏�[�𒴂����ꍇ
	//if (location.y < 0) {
	//	location.y = static_cast<float>(height); // ��ʂ̉��[����ďo��
	//}
	//// ��ʂ̉��[�𒴂����ꍇ
	//else if (location.y > static_cast<float>(height)) {
	//	location.y = 0; // ��ʂ̏�[����ďo��
	//}

}


void Enemy::Draw()const
{
	//�G�摜�̕`��
	DrawRotaGraphF(location.x, location.y, scale, 0.0, image, TRUE);

	// �����蔻���`��
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);;
	//DrawBoxAA(location.x - box_size.x / 2, location.y - box_size.y / 2, location.x + box_size.x / 2, location.y + box_size.y / 2, GetColor(255, 0, 0), TRUE, -1);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


void Enemy::Finalize()
{

}


//�G�̃^�C�v���擾
int Enemy::GetType()const
{
	return type;
}


//�ʒu�����擾
Vector2D Enemy::GetLocation()const
{
	return location;
}


//�����蔻��̑傫�����擾
Vector2D Enemy::GetBoxSize()const
{
	return box_size;
}

void Enemy::IsStop(bool result)
{
	this->is_stop = result;
}

void Enemy::SetPosition(const float x, const float y)
{
	this->location.x = x;
	this->location.y = y;
}

void Enemy::SetMovement(float speed_x, float speed_y) {
	this->speed_x = speed_x;
	this->speed_y = speed_y;
}
