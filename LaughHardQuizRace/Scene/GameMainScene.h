#pragma once

#include "SceneBase.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"
#include "../Object/Question.h"
#include "../Utility/Animation/AnimatedRectangle.h"

class GameMainScene:public SceneBase
{
private:
	int font_handle_h2;     //�t�H���g�n���h��
	int font_handle_h3;     //�t�H���g�n���h��
	int font_handle_h4;     //�t�H���g�n���h��
	int high_score;      //�n�C�X�R�A
	int back_ground;     //�w�i�摜
	int mileage;         //���s����
	int enemy_image[3];  //�G�摜
	int enemy_count[3];  //�ʂ�߂����G�J�E���g
	Player* player;      //�v���C���[
	Enemy** enemy;       //�G
	//Question question;  //���

	AnimatedRectangle animatedRect;

	bool hitEnemies[10] = { false }; // 10�̂̓G�ɑ΂��铖���蔻�茋�ʂ��ꎞ�I�ɕێ�����z��

public:
	GameMainScene();
	virtual ~GameMainScene();

	virtual void Initialize() override;
	virtual eSceneType Update() override;
	virtual void Draw() const override;
	virtual void Finalize() override;

	virtual eSceneType GetNowScene() const override;

private:
	//�n�C�X�R�A�ǂݍ��ݏ���
	void ReadHighScore();
	//�����蔻��
	bool IsHitCheck(Player* p, Enemy* e) const;
	void BubbleSort(Enemy enemies[], int n) const;
};

