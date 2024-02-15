#include "GameMainScene.h"
#include "../Object/RankingData.h"
#include "DxLib.h"
#include <math.h>
#include "../Utility/InputControl.h"

#define QUESTION_NUM 40

GameMainScene::GameMainScene() :high_score(0), back_ground(NULL), mileage(0), player(nullptr), answer_anim(0),
/*question("Resource/dat/question.csv"), */ time_limit(0), start_count(GetNowCount() + 1000 * 100), clear_flg(false),
size_anim_count(0), currentState(State::idle), difficulty(1)
{
	font_handle_h2 = CreateFontToHandle("Segoe UI", 50, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	font_handle_h3 = CreateFontToHandle("Segoe UI", 20, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);
	font_handle_h4 = CreateFontToHandle("Segoe UI", 10, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);
	answer_font_handle = CreateFontToHandle("Segoe UI", 40, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);

	for (int i = 0; i < 3; i++)
	{
		enemy_image[i] = NULL;
		enemy_count[i] = NULL;
	}
}

GameMainScene::~GameMainScene()
{
	StopSoundMem(background_sound);
	DeleteSoundMem(background_sound);
	DeleteFontToHandle(font_handle_h2);
	DeleteFontToHandle(font_handle_h3);
	DeleteFontToHandle(font_handle_h4);
	DeleteFontToHandle(answer_font_handle);
}

//����������
void GameMainScene::Initialize()
{
	//�����_�l��ǂݍ���
	ReadHighScore();

	//�摜�̓ǂݍ���
	back_ground = LoadGraph("Resource/images/Scene/GameMain/background.png");
	//barrier_image = LoadGraph(      "Resource/images/barrier.png");
	const int result = LoadDivGraph("Resource/images/fish.png", 3, 1, 3, 120, 63,
		enemy_image);

	board_image = LoadGraph("Resource/images/Scene/GameMain/board.png");

	//BGM�̓ǂݍ���
	background_sound = LoadSoundMem("Resource/sounds/bgm/Electric_Shine.mp3");

	//�G���[�`�F�b�N
	if (back_ground == -1)
	{
		throw("Resource/images/Scene/GameMain/background.png������܂���\n");
	}

	if (board_image == -1)
	{
		throw("Resource/images/Scene/GameMain/board.png������܂���\n");
	}

	if (result == -1)
	{
		throw("Resource/images/fish.png������܂���\n");
	}

	if (background_sound == -1)
	{
		throw("Resource/sounds/bgm/Electric_Shine.mp3������܂���\n");
	}

	//�I�u�W�F�N�g�̐���
	player = new Player;
	enemy = new Enemy * [2];
	question = new QuestionItems;

	animatedRect = AnimatedRectangle(board_image, 3000, 600, 800, 340);
	board = MoveAnimation(board_image, 2000, 400, 800, 400, 0.1f, 1.0f, 1000);

	//�I�u�W�F�N�g�̏�����
	player->Initialize();

	for (int i = 0; i < 2; i++)
	{
		enemy[i] = nullptr;
	}

	CreateEnemy();

	//BGM�̍Đ�
	PlaySoundMem(background_sound, DX_PLAYTYPE_LOOP, FALSE);
}

//�X�V����
eSceneType GameMainScene::Update()
{


	//�������Ԃ̌o��
	time_limit = GetNowCount() - start_count;

	//�������Ԃ�0�ȏ�ɂȂ����ꍇ�A���U���g��ʂ�
	if (time_limit > 0) { clear_flg = true; }

	if (currentState == State::question) {

		// ���X�e�B�b�N�̓|���ꂽ�������擾
		const StickDirection direction = InputControl::GetLStickDirection(20000, 5);

		if ((InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_UP) || direction == StickDirection::Up)
			|| (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_DOWN) || direction == StickDirection::Down))
		{
			selectMenu = (selectMenu + 1) % 2;
		}

		if (InputControl::GetButtonDown(XINPUT_BUTTON_A)) {

			// �X�e�[�g���𓚏�ԂɕύX
			SetState(State::answer);

			if (answer_correct == selectMenu)
			{
				answer = Answer::correct;
			}
			else
			{
				answer = Answer::wrong;
			}
		}

		/*if (InputControl::GetButtonDown(XINPUT_BUTTON_A)) {
			if (answer_correct == 0) { answer = GameMainScene::Answer::wrong; }
			else { answer = Answer::correct; }
		}
		else if (InputControl::GetButtonDown(XINPUT_BUTTON_B)) {
			if (answer_correct == 1) { answer = GameMainScene::Answer::wrong; }
			else { answer = Answer::correct; }
		}*/


		//�s�����������ꍇ�A�������Ԃ����Z������
		if (answer == Answer::wrong) {
			time_limit -= 1000 * 5;
			//PlaySoundMem(wrong_se, DX_PLAYTYPE_BACK, TRUE);
		}
		//�����������ꍇ�Aclear_count�����Z���A�X�R�A�����Z������
		else if (answer == Answer::correct) {
			time_limit += 1000 * 1;
			clear_count++;
			score += 50;
			//PlaySoundMem(correct_se, DX_PLAYTYPE_BACK, TRUE);
		}

		//B�{�^���܂���A�{�^������������
		if ((InputControl::GetButtonDown(XINPUT_BUTTON_A))
			|| InputControl::GetButtonDown(XINPUT_BUTTON_B)) {
			next_question = true;

			selectMenu = 0;

			// ��萶��
			CreateQuestion();

			// �X�e�[�g��ҋ@��ԂɕύX
			SetState(State::idle);
			//����̃A�j���[�V�������J�n
			//answer_anim_count = 0;
			//�^�C�������Z�̃A�j���[�V�������J�n
			//addtime_anim_count = 0;
		}

	}

	if (size_anim_count < 60) {
		size_anim_count++;

	}

	if (IsStateChanged()) {

		// �ҋ@������ւ̏���
		if (previousState == State::idle && currentState == State::question) {

		}

		// ��肩��𓚂ւ̏���
		if (previousState == State::question && currentState == State::answer) {

		}

		// �𓚂���ҋ@�ւ̏���
		if (previousState == State::answer && currentState == State::idle) {

			player->IsStop(false);
			player->SetActive(true);
			board.Close();

			//�G����
			CreateEnemy();
		}

	}

	//�v���C���[�̍X�V
	player->Update();

	//�ړ������̍X�V
	mileage += static_cast<int>(player->GetSpeed()) - 5;


	//�G�̍X�V�Ɠ����蔻��`�F�b�N
	for (int i = 0; i < 2; i++)
	{
		// �����蔻�茋�ʂ����Z�b�g
		hitEnemies[i] = false;

		if (enemy[i] != nullptr)
		{
			enemy[i]->Updata(player->GetSpeed());

			//��ʊO�ɍs������A�G���폜���ăX�R�A���Z
			if (enemy[i]->GetLocation().x <= 0.0f)
			{
				enemy_count[enemy[i]->GetType()]++;
				enemy[i]->Finalize();
				delete enemy[i];
				enemy[i] = nullptr;
			}

			//�����蔻��̊m�F
			// �����蔻��̌��ʂ�z��ɕێ�
			hitEnemies[i] = IsHitCheck(player, enemy[i]);
			if (hitEnemies[i]) {

				SetState(State::question);
				player->IsStop(true);
				enemy[i]->IsStop(true);
				player->SetActive(false);

				enemy[i]->Finalize();
				delete enemy[i];
				enemy[i] = nullptr;
			}
		}
	}

	if (currentState == State::question) {
		board.Update(16);
	}

	//if (currentState == State::idle) {

	//	if (time_limit < answer_anim - 3000) {
	//		//�𓚏󋵂𖢉񓚂Ƀ��Z�b�g
	//		answer = Answer::unanswered;

	//		printfDx("%d\n", time_limit);
	//	}
	//}


	//�v���C���[�̔R�����̗͂��O�����Ȃ�A���U���g�ɑJ�ڂ���
	if (/*player->GetFuel() < 0.0f || */player->GetHp() < 0.0f)
	{
		return eSceneType::E_RESULT;
	}
	return GetNowScene();
}

//�`�揈��
void GameMainScene::Draw()const
{
	// �w�i�摜�̕`��
	DrawGraph(mileage % 1280 + 1280, 0, back_ground, TRUE);
	DrawGraph(mileage % 1280, 0, back_ground, TRUE);

	DrawFormatString2ToHandle(770, 50, 0x4E75ED, 0xFFFFFF, font_handle_h2, "�c�莞�ԁF%5d.%.3d", -time_limit / 1000, -time_limit % 1000);
	DrawFormatString2ToHandle(50, 50, 0xFF0000, 0xFFFFFF, font_handle_h2, "�X�R�A�F%2d", score);
	// �G�̕`��
	for (int i = 0; i < 2; i++)
	{
		if (enemy[i] != nullptr)
		{
			enemy[i]->Draw();
		}
	}

	//�v���C���[�̕`��
	player->Draw();

	//for (int i = 0; i < 2; i++) {
	//	// Update�ł̓����蔻�茋�ʂ�true�̏ꍇ
	//	if (hitEnemies[i]) {

	//	}
	//}

	if (currentState == State::question)
	{
		board.Draw();
	}

	{//�L�����o�X

		// ��ʃT�C�Y
		constexpr int screenWidth = 1280;
		constexpr int screenHeight = 720;

		// �L�����o�X�T�C�Y
		constexpr int canvasWidth = 800;
		constexpr int canvasHeight = 400;

		int canvas_x1;
		int canvas_x2;
		int canvas_y1;
		int canvas_y2;
		short speed = 5;

		canvas_x1 = 490;
		canvas_y1 = 150;
		//�L�����o�X �`��
		//DrawBoxAA(canvas_x1, canvas_y1, canvas_x2, canvas_y2, 0xFFFFFF, TRUE);
		//DrawExtendGraph(canvas_x1, canvas_y1, canvas_x2, canvas_y2, paper_image, TRUE);


		DrawExtendFormatString2ToHandle(canvas_x1 + 30, canvas_y1, size_anim_count * 0.01 + 0.4, size_anim_count * 0.01 + 0.4,
			0xFF0000, 0xFFFFFF, font_handle_h2, "%2d���", question_count + 1);

		//��� �`��
		DrawExtendFormatString2ToHandle(GetDrawCenterX(question->GetQuestion(next_question_num).c_str(), font_handle_h2), canvas_y1 + 100,
			/*size_anim_count * 0.01 + 0.2*/1.0f, size_anim_count * 0.01 + 0.2, 0xF5A000, 0xEFBD00, font_handle_h2, "%s",
			question->GetQuestion(next_question_num).c_str());

		//���k�̉� �`��
		//DrawExtendStringToHandle(canvas_x1 - 200, 370, size_anim_count * 0.01 + 0.4, size_anim_count * 0.01 + 0.4, "�I����",
		//	0xFFFFFF, font_handle_h3, 0x000000);
		//int char_num = GetDrawFormatStringWidthToHandle(font_handle_h4, question->GetAnswer(next_question_num, answer_correct).c_str());
		//for (int i = 0; i < char_num / 20; i++) {
		//	DrawExtendStringToHandle(canvas_x1 + 100 + i * 20, 580, size_anim_count * 0.01 + 0.4, size_anim_count * 0.01 + 0.4, "_", 0xFFFFFF, font_handle_h2, 0x000000);
		//	//DrawLineAA(canvas_x1 + 300, 640, canvas_x1 + 300 + i * 20, 580, 0x000000, size_anim_count * 0.01 + 0.8);
		//}

		//const bool question_num = GetRand(1);

		// �I����
		const std::string a1 = question->GetAnswer(next_question_num, answer_correct);
		const std::string a2 = question->GetAnswer(next_question_num, !answer_correct);

		// �e�I�����̃t�H���g
		int a_font = font_handle_h2;

		// �I�����̕�������10�����ȏ�̏ꍇ
		if (a1.length() > 10) {
			a_font = answer_font_handle;
		}

		DrawExtendFormatString2ToHandle(canvas_x1 + 30, 400, (size_anim_count * 0.01 + 0.4), (size_anim_count * 0.01 + 0.4),
			0x00bfff, selectMenu == 0 ? 0x00FFE1 : 0x0000cd, a_font, "%6s",
			a1.c_str());
		DrawExtendFormatString2ToHandle(canvas_x1 + 30, 470, (size_anim_count * 0.01 + 0.4), (size_anim_count * 0.01 + 0.4),
			0x00bfff, selectMenu == 1 ? 0x00FFE1 : 0x0000cd, a_font, "%6s",
			a2.c_str());
	}

	//����\���̍��W
	int x = 660;
	int y = 400;
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(answer_anim_count));
	//����\��

	switch (answer)
	{

		/*case GameMain::Answer::unanswered:
			break;*/
	case Answer::wrong:

	{//�L�����o�X
		float canvas_x1 = x;
		float canvas_x2;
		float canvas_y1 = y;
		float canvas_y2;
		DrawCircleAA(canvas_x1 - 200, canvas_y1 - 200, 25, 30, 0x4171bb, 50.0F);
		DrawCircleAA(canvas_x1 + 200, canvas_y1 + 200, 25, 30, 0x4171bb, 50.0F);
		DrawCircleAA(canvas_x1 + 200, canvas_y1 - 200, 25, 30, 0x4171bb, 50.0F);
		DrawCircleAA(canvas_x1 - 200, canvas_y1 + 200, 25, 30, 0x4171bb, 50.0F);
		DrawLineAA(canvas_x1 - 200, canvas_y1 - 200, canvas_x1 + 200, canvas_y1 + 200, 0x4171bb, 50.0F);
		DrawLineAA(canvas_x1 + 200, canvas_y1 - 200, canvas_x1 - 200, canvas_y1 + 200, 0x4171bb, 50.0F);

		DrawStringToHandle(1000, 90, "- 5.000", 0xFF0000, font_handle_h3);
	}
	break;
	case Answer::correct:
		DrawCircleAA(x, y, 200, 30, 0xd2672f, FALSE, 50.0F);
		DrawStringToHandle(1000, 90, "+ 1.000", 0xFFFF00, font_handle_h3);
		break;
	default:
		break;
	}

	//! �f�o�b�O ���̕\��
	//for (const auto& item : question.GetQuizItems()) {
	//	printfDx("Question: %s\n", item.question.c_str());
	//	printfDx("Difficulty: %d\n", item.difficultyLevel);
	//	printfDx("Answer: %s\n", item.answer.c_str());

	//	if (!item.wrongs.empty()) {
	//		for (const auto& wrong : item.wrongs) {
	//			printfDx("Wrong: %s\n", wrong.c_str());
	//		}
	//	}
	//	else {
	//		// wrongs����̏ꍇ
	//		printfDx("Wrongs: None\n");
	//	}

	//	printfDx("-----------------------\n");
	//}




	//! DEBUG �G�l�~�[
	//for (int i = 0; i < 2; i++)
	//{
	//	if (enemy[i] != nullptr) {
	//		const int lane = static_cast<int>((enemy[i]->GetLocation().x - 40) / 105) + 1;
	//		DrawFormatString(100, 20 + i * 20, 0xffffff, "Enemy: ���[���F%d x %f , y %f", lane, enemy[i]->GetLocation().x, enemy[i]->GetLocation().y);
	//	}
	//}
}


//�I��������
void GameMainScene::Finalize()
{
	//�X�R�A���v�Z����
	int score = (mileage / 10 * 10);
	for (int i = 0; i < 3; i++)
	{
		score += (i + 1) * 50 * enemy_count[i];
	}

	//���U���g�f�[�^�̏�������
	FILE* fp = nullptr;
	//�t�@�C���I�[�v��
	errno_t result = fopen_s(&fp, "Resource/dat/result_data.csv", "w");

	//�G���[�`�F�b�N
	if (result != 0)
	{
		throw("Resource/dat/result_data.csv���J���܂���\n");
	}

	//�X�R�A��ۑ�
	fprintf(fp, "%d,\n", score);

	//���������Ɠ��_��ۑ�
	for (int i = 0; i < 3; i++)
	{
		fprintf(fp, "%d,\n", enemy_count[i]);
	}

	//�t�@�C���N���[�Y
	fclose(fp);

	//���I�m�ۂ����I�u�W�F�N�g���폜����
	player->Finalize();
	delete player;

	for (int i = 0; i < 2; i++)
	{
		if (enemy[i] != nullptr)
		{
			enemy[i]->Finalize();
			delete enemy[i];
			enemy[i] = nullptr;
		}
	}

	delete[] enemy;
}

//���݂̃V�[�������擾
eSceneType GameMainScene::GetNowScene()const
{
	return eSceneType::E_MAIN;
}


int GameMainScene::GetDrawCenterX(const char* string, int font_handle, int margin)const {

	//��ʕ�
	const int screenX = 1280;

	if (margin >= screenX || margin <= -screenX) {
		margin = 0;
	}

	if (font_handle == 0) {
		font_handle = DX_DEFAULT_FONT_HANDLE;
	}


	const int w = screenX / 2 + margin - (GetDrawFormatStringWidthToHandle(font_handle, string) / 2);
	return w;
}

void GameMainScene::CreateEnemy()
{
	for (int i = 0; i < 1; i++)
	{
		if (enemy[i] == nullptr)
		{
			const int type = GetRand(3) % 3;

			// ��Փx�ŐV������ނ̃G�l�~�[�𐶐�����
			//const int type = (GetRand(3) % 3) + difficulty;
			enemy[i] = new Enemy(type, enemy_image[type]);
			enemy[i]->Initialize();
			printfDx("CreateEnemy");
			break;
		}
	}
}

void GameMainScene::CreateQuestion()
{
	next_question = false;
	//���̃J�E���g�����Z
	++question_count;
	//���k�̐���𗐐��Ń����_���Ŏ擾
	answer_correct = GetRand(1);
	//�܂��o����Ă��Ȃ��������ɏo��
	next_question_num = GetRand(QUESTION_NUM - 1);

	while (std::find(question_num.begin(), question_num.end(), next_question_num) != question_num.end()) {

		next_question_num = GetRand(QUESTION_NUM - 1);

		//printfDx("next_question_num:%d", next_question_num);
		////�f�o�b�O
		//for (int num : question_num) {
		//	printfDx("[%d]", num);

					//�𓚂����Z�b�g

	}

	// �𓚂̃A�j���[�V�����p�Ɍ��݂̌o�ߎ��Ԃ��i�[
	answer_anim = time_limit;

	//�𓚏󋵂𖢉񓚂Ƀ��Z�b�g
	answer = Answer::unanswered;

	//���̖��ԍ����v�b�V��
	question_num.push_back(next_question_num);
}

//�n�C�X�R�A�ǂݍ���
void GameMainScene::ReadHighScore()
{
	RankingData data;
	data.Initialize();

	high_score = data.GetScore(0);

	data.Finalize();
}


//�����蔻�菈���i�v���C���[�ƓG�j
bool GameMainScene::IsHitCheck(Player* p, Enemy* e) const
{
	//�v���C���[���o���A�𒣂��Ă�����A�����蔻��𖳎�����
	//if (p->IsBarrier())
	//{
	//	return false;
	//}

	//�G��񂪂Ȃ���΁A�����蔻��𖳎�����
	if (e == nullptr)
	{
		return false;
	}

	//�ʒu���̍������擾
	const Vector2D diff_location = p->GetLocation() - e->GetLocation();

	//�����蔻��T�C�Y�̑傫�����擾
	const Vector2D box_ex = p->GetBoxSize() + e->GetBoxSize();

	//�R���W�����f�[�^���ʒu���̍������������Ȃ�A�q�b�g����Ƃ���
	return ((fabsf(diff_location.x) < box_ex.x) && (fabsf(diff_location.y) < box_ex.y));
}

void GameMainScene::SetState(State newState)
{
	previousState = currentState; // ���݂̃X�e�[�g��O�̃X�e�[�g�ɐݒ�
	currentState = newState; // �V�����X�e�[�g�����݂̃X�e�[�g�ɐݒ�
}

bool GameMainScene::IsStateChanged() const
{
	return currentState != previousState; // ���݂̃X�e�[�g�ƑO�̃X�e�[�g���قȂ邩�ǂ�����Ԃ�
}
