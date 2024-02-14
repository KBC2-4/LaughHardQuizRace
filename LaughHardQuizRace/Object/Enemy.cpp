#include "Enemy.h"
#include "DxLib.h"

Enemy::Enemy(int type, int handle) :type(type), image(handle), speed(0.0f),
location(0.0f), box_size(0.0f)
{

}

Enemy::~Enemy()
{

}

void Enemy::Initialize()
{
	//�o��������X���W�p�^�[�����擾
	const float random_y = static_cast<float>(GetRand(4) * 105 + 40);
	//�����ʒu�̐ݒ�
	location = Vector2D(1380.0f, random_y);
	//�����蔻��̐ݒ�
	box_size = Vector2D(31.0f, 60.0f);
	//�����̐ݒ�
	speed = static_cast<float>(this->type * 2);
}


void Enemy::Updata(float speed)
{
	//�ʒu���Ɉړ��ʂ����Z����
	location += Vector2D(this->speed + speed - 6, 0.0f);
}


void Enemy::Draw()const
{
	//�G�摜�̕`��
	DrawRotaGraphF(location.x, location.y, 1.0f, 0.0, image, TRUE);
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