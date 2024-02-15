#include "GameMainScene.h"
#include "../Object/RankingData.h"
#include "DxLib.h"
#include <math.h>
#include "../Utility/InputControl.h"
#include "../Utility/Guide.h"

#define QUESTION_NUM 40

GameMainScene::GameMainScene() :high_score(0), background_image(NULL), scroll(0), player(nullptr), answer_anim(0), idle_bgm(0), board_image(0), score(0),
/*question("Resource/dat/question.csv"), */ time_limit(0), start_count(GetNowCount() + 1000 * 100), clear_flg(false), selectMenu(0), clear_count(0), question_count(0),
size_anim_count(0), currentState(State::idle), difficulty(1), correct_se(0), cursor_move_se(0), wrong_se(0), enter_se(0), next_question_num(GetRand(QUESTION_NUM - 1))
{
	font_handle_h2 = CreateFontToHandle("Segoe UI", 50, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	font_handle_h3 = CreateFontToHandle("Segoe UI", 20, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);
	font_handle_h4 = CreateFontToHandle("Segoe UI", 10, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);
	answer_font_handle = CreateFontToHandle("Segoe UI", 40, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);
	buttonGuidFont = CreateFontToHandle("���C���I", 23, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

	for (int i = 0; i < 3; i++)
	{
		enemy_image[i] = NULL;
		enemy_count[i] = NULL;
	}
}

GameMainScene::~GameMainScene()
{
	// BGM���Đ�����Ă���ꍇ�A��~���폜
	if(CheckSoundMem(idle_bgm) == 1)
	{
		StopSoundMem(idle_bgm);
	}
	DeleteSoundMem(idle_bgm);

	// BGM���Đ�����Ă���ꍇ�A��~���폜
	if (CheckSoundMem(question_bgm) == 1)
	{
		StopSoundMem(question_bgm);
	}
	DeleteSoundMem(question_bgm);

	DeleteSoundMem(enter_se);
	DeleteSoundMem(cursor_move_se);
	DeleteSoundMem(wrong_se);
	DeleteSoundMem(correct_se);

	// �t�H���g�̍폜
	DeleteFontToHandle(font_handle_h2);
	DeleteFontToHandle(font_handle_h3);
	DeleteFontToHandle(font_handle_h4);
	DeleteFontToHandle(answer_font_handle);
	DeleteFontToHandle(buttonGuidFont);

	// �摜�̍폜
	DeleteGraph(background_image);
	DeleteGraph(board_image);
}

//����������
void GameMainScene::Initialize()
{
	// �����_�l��ǂݍ���
	ReadHighScore();

	// �摜�̓ǂݍ���
	background_image = LoadGraph("Resource/images/Scene/GameMain/background.png");
	//barrier_image = LoadGraph(      "Resource/images/barrier.png");
	const int result = LoadDivGraph("Resource/images/fish.png", 3, 1, 3, 120, 63,
		enemy_image);

	board_image = LoadGraph("Resource/images/Scene/GameMain/board.png");

	// BGM�̓ǂݍ���
	idle_bgm = LoadSoundMem("Resource/sounds/bgm/idle_time.mp3");
	question_bgm = LoadSoundMem("Resource/sounds/bgm/question_time.mp3");

	// SE�̓ǂݍ���
	enter_se = LoadSoundMem("Resource/sounds/se/enter.mp3");
	cursor_move_se = LoadSoundMem("Resource/sounds/se/cursor_move.mp3");
	wrong_se = LoadSoundMem("Resource/sounds/se/wrong.mp3");
	correct_se = LoadSoundMem("Resource/sounds/se/correct.mp3");

	//�G���[�`�F�b�N
	if (background_image == -1)
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

	if (idle_bgm == -1)
	{
		throw("Resource/sounds/bgm/idle_time.mp3������܂���\n");
	}

	if (enter_se == -1)
	{
		throw("Resource/sounds/se/enter.mp3������܂���\n");
	}

	if (cursor_move_se == -1)
	{
		throw("Resource/sounds/se/cursor_move.mp3������܂���\n");
	}

	if (wrong_se == -1)
	{
		throw("Resource/sounds/se/wrong.mp3������܂���\n");
	}

	if (correct_se == -1)
	{
		throw("Resource/sounds/se/correct.mp3������܂���\n");
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
	PlaySoundMem(idle_bgm, DX_PLAYTYPE_LOOP, FALSE);

	// �K�C�h�\����ݒ�
	gamepad_guides = {
		guideElement({"L"}, "�ړ�", GUIDE_SHAPE_TYPE::JOYSTICK, buttonGuidFont, 0x000000,0xFFFFFF, 0xFFFFFF),
	};
}

//�X�V����
eSceneType GameMainScene::Update()
{

	//�������Ԃ̌o��
	time_limit = GetNowCount() - start_count;

	//�������Ԃ�0�ȉ��ɂȂ����ꍇ�A���U���g��ʂ�
	if (time_limit > 0)
	{
		clear_flg = true;
		return eSceneType::E_RESULT;
	}

	if (currentState == State::question) {

		// ���X�e�B�b�N�̓|���ꂽ�������擾
		const StickDirection direction = InputControl::GetLStickDirection(20000, 10);

		if ((InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_UP) || direction == StickDirection::Up)
			|| (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_DOWN) || direction == StickDirection::Down))
		{
			// SE�Đ�
			PlaySoundMem(cursor_move_se, DX_PLAYTYPE_BACK, TRUE);
			selectMenu = (selectMenu + 1) % 2;
		}

		// A�{�^������������
		if (InputControl::GetButtonDown(XINPUT_BUTTON_A)) {

			// SE�Đ�
			PlaySoundMem(enter_se, DX_PLAYTYPE_BACK, TRUE);

			//�𓚏󋵂𖢉񓚂Ƀ��Z�b�g
			answer = Answer::unanswered;

			// �X�e�[�g���𓚏�ԂɕύX
			SetState(State::answer);

			if (answer_correct == selectMenu)
			{	// ����
				answer = Answer::correct;
			}
			else
			{	// �s����

				answer = Answer::wrong;
				player->SetActive(false);
			}



			//�s�����������ꍇ�A�������Ԃ����Z������
			if (answer == Answer::wrong) {
				start_count -= 1000 * 5;
				// SE�Đ�
				PlaySoundMem(wrong_se, DX_PLAYTYPE_BACK, TRUE);
			}
			//�����������ꍇ�Aclear_count�����Z���A�X�R�A�����Z������
			else if (answer == Answer::correct) {

				time_limit += 1000 * 1;
				clear_count++;
				score += 50;
				// SE�Đ�
				PlaySoundMem(correct_se, DX_PLAYTYPE_BACK, TRUE);

			}

			selectMenu = 0;

			// ��萶��
			CreateQuestion();

			// question_bgm���Đ����̏ꍇ�A��~
			if (CheckSoundMem(question_bgm) == 1)
			{
				StopSoundMem(question_bgm);
			}
			//BGM�̍Đ�
			PlaySoundMem(idle_bgm, DX_PLAYTYPE_LOOP, FALSE);

			// �K�C�h�\�����X�V
			gamepad_guides = {
		guideElement({"L"}, "�ړ�", GUIDE_SHAPE_TYPE::JOYSTICK, buttonGuidFont, 0x000000,0xFFFFFF, 0xFFFFFF),
			};

			// �X�e�[�g��ҋ@��ԂɕύX
			SetState(State::idle);
		}

	}


	// ��蕶�̃X�P�[���A�j���[�V�����̍X�V
	if (size_anim_count < 60) {
		size_anim_count++;
	}

	if (IsStateChanged()) {

		// �ҋ@������ւ̏���
		if (previousState == State::idle && currentState == State::question) {

			// ���������G�l�~�[���폜
			for (int i = 0; i < 2; i++)
			{
				if (hitEnemies[i]) {

					enemy[i]->Finalize();
					delete enemy[i];
					enemy[i] = nullptr;
				}
			}

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
	scroll += static_cast<int>(player->GetSpeed()) - 5;


	//�G�̍X�V�Ɠ����蔻��`�F�b�N
	for (int i = 0; i < 2; i++)
	{
		// �����蔻�茋�ʂ����Z�b�g
		hitEnemies[i] = false;

		if (enemy[i] != nullptr)
		{
			enemy[i]->Updata(player->GetSpeed());

			//��ʊO�ɍs������A�G���폜
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

				// idle_bgm���Đ����̏ꍇ�A��~
				if (CheckSoundMem(idle_bgm) == 1)
				{
					StopSoundMem(idle_bgm);
				}
				//BGM�̍Đ�
				PlaySoundMem(question_bgm, DX_PLAYTYPE_LOOP, TRUE);
				

				// �K�C�h�\�����X�V
				gamepad_guides = {
					guideElement({"L"}, "�I��", GUIDE_SHAPE_TYPE::JOYSTICK, buttonGuidFont, 0x000000,0xFFFFFF, 0xFFFFFF),
					guideElement({"A"}, "����", GUIDE_SHAPE_TYPE::FIXED_CIRCLE,buttonGuidFont, 0xFFFFFF, 0xEBE146,0xEB3229, 0xFFFFFF),
				};

				// �G�l�~�[�ɓ��������ꍇ�A�X�e�[�g����ɕύX
				SetState(State::question);

				// �v���C���[�̓����𖳌���
				player->IsStop(true);
				enemy[i]->IsStop(true);
			}
		}
	}

	if (currentState == State::question) {
		board.Update(16);
	}

	// State�`�F�b�N
//switch (currentState)
//{
//	// �ҋ@���
//case State::idle:
//	printfDx("idle\n");
//	break;

//case State::question:
//	printfDx("question\n");
//	break;

//case State::answer:
//	printfDx("answer\n");
//	break;

//default:
//	break;
//}

	return GetNowScene();
}

//�`�揈��
void GameMainScene::Draw()const
{
	// �w�i�摜�̕`��
	DrawGraph(scroll % 1280 + 1280, 0, background_image, TRUE);
	DrawGraph(scroll % 1280, 0, background_image, TRUE);

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

			// �I���� �`��
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
	}

	// �{�^���K�C�h�̕`��
	DrawGuides(gamepad_guides, 505.0f, 660.0f, 5.0f, 60.0f);

	// ����A�j���[�V������`��
	DrawAnswer();

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

	// ���������Ɠ��_��ۑ�
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
	constexpr int screenX = 1280;

	if (margin >= screenX || margin <= -screenX) {
		margin = 0;
	}

	if (font_handle == 0) {
		font_handle = DX_DEFAULT_FONT_HANDLE;
	}


	const int w = screenX / 2 + margin - (GetDrawFormatStringWidthToHandle(font_handle, string) / 2);
	return w;
}

void GameMainScene::DrawAnswer() const
{
	// �A�j���[�V�����̌o�ߎ��Ԃ��v�Z
	const int elapsed_time = GetNowCount() - answer_anim;
	constexpr int fadein_time = 800; // 800�~���b
	constexpr int fadeout_time = 1000; // 1�b
	constexpr int anim_time = fadein_time + fadeout_time;

	// �A���t�@�l�̌v�Z
	int alpha = 0;
	if (elapsed_time <= fadein_time) { // �t�F�[�h�C��
		alpha = (elapsed_time * 255) / fadein_time;
	}
	else if (elapsed_time <= anim_time) { // �t�F�[�h�A�E�g
		alpha = 255 - ((elapsed_time - fadein_time) * 255) / fadeout_time;
	}
	else {
		// �A�j���[�V�����I��
		alpha = 0;
	}

	//����\���̍��W
	const int x = 660;
	const int y = 400;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha));
	//����\��

	switch (answer)
	{

		//case Answer::unanswered:
		//	break;
	case Answer::wrong:		// �s����

	{//�L�����o�X

		// �o�c��`��
		const float canvas_x1 = x;
		const float canvas_y1 = y;
		DrawCircleAA(canvas_x1 - 200, canvas_y1 - 200, 25, 30, 0x4171bb, 50.0F);
		DrawCircleAA(canvas_x1 + 200, canvas_y1 + 200, 25, 30, 0x4171bb, 50.0F);
		DrawCircleAA(canvas_x1 + 200, canvas_y1 - 200, 25, 30, 0x4171bb, 50.0F);
		DrawCircleAA(canvas_x1 - 200, canvas_y1 + 200, 25, 30, 0x4171bb, 50.0F);
		DrawLineAA(canvas_x1 - 200, canvas_y1 - 200, canvas_x1 + 200, canvas_y1 + 200, 0x4171bb, 50.0F);
		DrawLineAA(canvas_x1 + 200, canvas_y1 - 200, canvas_x1 - 200, canvas_y1 + 200, 0x4171bb, 50.0F);

		DrawStringToHandle(1000, 90, "- 5.000", 0xFF0000, font_handle_h3);
	}
	break;
	case Answer::correct:		// ����
		// �ۂ�`��
		DrawCircleAA(x, y, 200, 30, 0xd2672f, FALSE, 50.0f);
		DrawStringToHandle(1000, 90, "+ 1.000", 0xFFFF00, font_handle_h3);
		break;
	default:
		break;
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameMainScene::CreateEnemy() const
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
			break;
		}
	}
}

void GameMainScene::CreateQuestion()
{
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
	answer_anim = GetNowCount();

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
