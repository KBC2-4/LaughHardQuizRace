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
	int gear;			// ���݂̃M�A
	const float base_acceleration = 0.5f; // ��{�����x
	float gear_ratios[6] = { 0.0f, 2.5f, 2.0f, 1.5f, 1.0f, 0.5f }; // �M�A���Ƃ̉����x��
	float hp;           //�̗�
	float fuel;         //�R��
	Barrier* barrier;   //�o���A
	Animation anim;   //�A�j���[�V����

public:
	Player();
	~Player();

	void Initialize();   //����������
	void Update();       //�X�V����
	void Draw();         //�`�揈��
	void Finalize();     //�I��������

public:
	void SetActive(bool flg);        //�L���t���O�ݒ�
	void DecreaseHp(float value);    //�̗͌�������
	Vector2D GetLocation()const;     //�ʒu���W�擾
	Vector2D GetBoxSize()const;      //�����蔻��̑傫���擾
	float GetSpeed()const;           //�����擾����
	float GetGear()const;           //�M�A�擾����
	float GetFuel()const;            //�R���擾
	float GetHp()const;              //�̗͎擾
private:
	void Movement();                 //�ړ�����
	void Acceleration();             //��������
	void UpdateGear();				 // �M�A�`�F���W����	
};


