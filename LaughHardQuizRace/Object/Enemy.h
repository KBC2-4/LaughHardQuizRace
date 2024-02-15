#pragma once

#include "../Utility/Vector2D.h"

class Enemy
{private:
	int type;             //�^�C�v
	int image;            //�摜
	float speed_x;          //����X
	float speed_y;          //����Y
	bool Quizhide;
	Vector2D location;    //�ʒu���
	Vector2D box_size;    //�����蔻��̑傫��
	bool is_stop = false;       //�ꎞ��~�t���O
	float scale;		// �摜�̊g�嗦

public:
	Enemy(int type, int handle);
	~Enemy();

	void Initialize();            //����������
	void Updata(float speed);     //�X�V����
	void Update();				 // �X�V�����i�j���j
	void Draw()const;             //�`�揈��
	void Finalize();              //�I��������
	void QuizHide(bool flg);      //�N�C�Y�\�L����

	int GetType()const;           //�^�C�v�擾
	Vector2D GetLocation()const;  //�ʒu���̎擾
	Vector2D GetBoxSize()const;   //�����蔻��̑傫�����擾
	void IsStop(bool result);        // �ꎞ��~����
	void SetPosition(float x, float y);	// �ʒu���̐ݒ�
	void SetMovement(float speed_x, float speed_y);		// �����̐ݒ�
};

