#pragma once

#include "SceneBase.h"
#include "../Object/Player.h"
#include "../Object/Enemy.h"
#include "../Object/QuestionItems.h"
#include "../Utility/Animation/AnimatedRectangle.h"
#include "../Utility/Animation/MoveAnimation.h"

class GameMainScene :public SceneBase
{
private:
	int font_handle_h2;     //�t�H���g�n���h��
	int font_handle_h3;     //�t�H���g�n���h��
	int font_handle_h4;     //�t�H���g�n���h��
	int buttonGuidFont;     //�K�C�h�\���p�t�H���g�n���h��
	int answer_font_handle;     //�I�����\���p�t�H���g�n���h��
	int score;            // �X�R�A
	int add_score;       // �ǉ�����X�R�A
	int background_image;     //�w�i�摜
	int idle_bgm;     //�A�C�h������BGM
	int question_bgm;     //����ʎ���BGM
	int wrong_se;        //�s���� �����n���h��
	int correct_se;      //���� �����n���h��
	int cursor_move_se;       //�J�[�\���ړ� �����n���h��
	int enter_se;        //���� �����n���h��

	int board_image;     //�{�[�h�摜
	int scroll;         //���s����
	int enemy_image[15];  //�G�摜
	int time_limit;      //��������
	int start_count;     //�Q�[���J�n���̃J�E���g
	std::vector<guideElement> gamepad_guides; // �{�^���K�C�h�\���p

	short current_question_num;		//�o�肵�Ă�����ԍ�
	std::vector<short> question_num;//�o�肵�����ԍ�
	short question_count;			//����肩�̃J�E���g
	bool answer_correct;			//���k�̉𓚂̐���
	short clear_count;				//����������萔


	// �A�j���[�V�����p
	int answer_anim;				// ����̃A�j���[�V�����p
	int size_anim_count;			// �X�P�[���A�j���[�V�����p

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
	MoveAnimation board_effect;
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

private:
	// �X�R�A�����Z����
	void AddScore();
	// �����`�悷��
	void DrawAnswer() const;
	// �G�l�~�[��	����
	void CreateEnemy() const;
	// ���𐶐�
	void CreateQuestion();
	//�����蔻��
	bool IsHitCheck(Player* p, Enemy* e) const;
	void BubbleSort(Enemy enemies[], int n) const;
	// �X�e�[�g���X�V����֐�
	void SetState(State newState);

	// �X�e�[�g���؂�ւ�������ǂ������`�F�b�N����֐�
	bool IsStateChanged() const;
};

