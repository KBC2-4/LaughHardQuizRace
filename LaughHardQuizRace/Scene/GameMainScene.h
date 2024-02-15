#pragma once

#include "SceneBase.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"
#include "../Object/Question.h"
#include "../Utility/Animation/AnimatedRectangle.h"
#include "../Utility/Animation/MoveAnimation.h"

class GameMainScene :public SceneBase
{
private:
	int font_handle_h2;     //�t�H���g�n���h��
	int font_handle_h3;     //�t�H���g�n���h��
	int font_handle_h4;     //�t�H���g�n���h��
	int score;            //�X�R�A
	int high_score;      //�n�C�X�R�A
	int back_ground;     //�w�i�摜
	int board_image;     //�{�[�h�摜
	int mileage;         //���s����
	int enemy_image[3];  //�G�摜
	int enemy_count[3];  //�ʂ�߂����G�J�E���g
	int time_limit;      //��������
	int start_count;     //�Q�[���J�n���̃J�E���g


	int difficulty = 1; //�o�肷����̓�Փx
	short next_question_num;		//���ɏo�肷����ԍ�
	std::vector<short> question_num;//�o�肵�����ԍ�
	bool next_question;				//���̖��ֈڍs����t���O
	short question_count;			//���̃J�E���g
	bool answer_correct;			//���k�̉𓚂̐���
	short clear_count;				//����������萔

	int size_anim_count;

	enum class Answer {
		unanswered, //����
		wrong,		//�s����
		correct,	//����
	}answer;

	// �Q�[���̏��
	enum class State {
		idle,		//�ҋ@
		question,	//���
		answer,		//��
		result,		//���U���g��ʂ�
	};

	State currentState = State::idle; // ���݂̃X�e�[�g
	State previousState = State::idle; // �O�̃X�e�[�g

	bool clear_flg;   //�Q�[���N���A���̃t���O
	Player* player;      //�v���C���[
	Enemy** enemy;       //�G
	QuestionItems* question;  //���

	int selectMenu; // ���ւ̉𓚂�����I�𒆂̃��j���[
	AnimatedRectangle animatedRect;
	MoveAnimation board;

	bool hitEnemies[10] = { false }; // 10�̂̓G�ɑ΂��铖���蔻�茋�ʂ��ꎞ�I�ɕێ�����z��

public:
	GameMainScene();
	virtual ~GameMainScene();

	virtual void Initialize() override;
	virtual eSceneType Update() override;
	virtual void Draw() const override;
	virtual void Finalize() override;

	virtual eSceneType GetNowScene() const override;

	/// <summary>
	/// �����̕`�悷��X���W�����S�ɂȂ�X���W���擾����B
	/// </summary>
	/// <param name="string">�`�悷�镶����</param>
	/// <param name="font_handle">�`��Ɏg�p����t�H���g�n���h��(�����͖���0�̏ꍇ�̓f�t�H���g�t�H���g�n���h�����g�p</param>
	/// <param name="margin">������X���W�ɊԊu���󂯂�l</param>
	int GetDrawCenterX(const char* string, int font_handle = 0, int margin = 0)const;

private:
	// �G�l�~�[��	����
	void CreateEnemy();
	// ���𐶐�
	void CreateQuestion();
	//�n�C�X�R�A�ǂݍ��ݏ���
	void ReadHighScore();
	//�����蔻��
	bool IsHitCheck(Player* p, Enemy* e) const;
	void BubbleSort(Enemy enemies[], int n) const;
	// �X�e�[�g���X�V����֐�
	void SetState(State newState);

	// �X�e�[�g���؂�ւ�������ǂ������`�F�b�N����֐�
	bool IsStateChanged() const;
};

