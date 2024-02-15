#pragma once

#include "../Utility/Vector2D.h"
#include "../Utility/Animation/Animation.h"
#include "Barrier.h"


class Player
{
private:
	unsigned int previous_time;	// 1�O�̃t���[���̌o�ߎ���
	bool is_active;     //�L����Ԃ��H
	int image;          //�摜�f�[�^
	Vector2D location;  //�ʒu���W
	Vector2D box_size;  //�����蔻��̑傫��
	float angle;        //�p�x
	float speed;        //����
	float gear_ratios[6] = { 0.0f, 2.5f, 2.0f, 1.5f, 1.0f, 0.5f }; // �M�A���Ƃ̉����x��
	Animation anim;   //�A�j���[�V����
	bool is_stop = false;       //�ꎞ��~�t���O

public:
	Player();
	~Player();

	void Initialize();   //����������
	void Update();       //�X�V����
	void Draw();         //�`�揈��
	void Finalize();     //�I��������

public:
	void SetActive(bool flg);        //�L���t���O�ݒ�
	Vector2D GetLocation()const;     //�ʒu���W�擾
	Vector2D GetBoxSize()const;      //�����蔻��̑傫���擾
	float GetSpeed()const;           //�����擾����
	void IsStop(bool result);        // �ꎞ��~����

private:
	void Movement();                 //�ړ�����
};


