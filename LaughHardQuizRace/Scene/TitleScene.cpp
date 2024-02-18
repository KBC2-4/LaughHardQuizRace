#include "TitleScene.h"
#include "../Utility/InputControl.h"
#include "DxLib.h"
#include "../Utility/Guide.h"
#include "../Utility/GameData.h"

TitleScene::TitleScene() :background_image(NULL), menu_image(NULL), scroll(0), title_image(NULL), background_sound(NULL),
cursor_image(NULL), menu_cursor(0), cursor_move_se(0), enter_se(0), play_count(-1), buttonGuidFont(0), ranking(nullptr),
client(L"AKfycbyCbERdoRMfYxiJLrMaQMS1grPSBHRDSPtoPvH0yrY-7QTZjRLeBYgBqA8zTUY56GtL")
{
	buttonGuidFont = CreateFontToHandle("���C���I", 23, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

	//enemyManager = EnemyManager();
}

TitleScene::~TitleScene()
{
}


//����������
void TitleScene::Initialize()
{
	//�摜�̓ǂݍ���
	background_image = LoadGraph("Resource/images/Scene/Title/background.png");
	menu_image = LoadGraph("Resource/images/Scene/Title/menu.png");
	cursor_image = LoadGraph("Resource/images/Scene/Title/cursor.png");
	title_image = LoadGraph("Resource/images/Scene/Title/title.png");

	//BGM�̓ǂݍ���
		// �O��̍Đ��r����BGM������Đ�
	if (GameData::GetPrevBGM() == -1)
	{
		background_sound = LoadSoundMem("Resource/sounds/bgm/title.mp3");
		if (background_sound == -1)
		{
			throw("Resource/sounds/bgm/title.mp3������܂���\n");
		}
	}
	else
	{
		background_sound = GameData::GetPrevBGM();
	}

	enter_se = LoadSoundMem("Resource/sounds/se/enter.mp3");
	cursor_move_se = LoadSoundMem("Resource/sounds/se/cursor_move.mp3");

	//�G���[�`�F�b�N
	if (background_image == -1)
	{
		throw("Resource/images/Scene/Title/background.png������܂���\n");
	}
	if (menu_image == -1)
	{
		throw("Resource/images/Scene/Title/menu.png������܂���\n");
	}
	if (cursor_image == -1)
	{
		throw("Resource/images/Scene/Title/cursor.png������܂���\n");
	}
	if (title_image == -1)
	{
		throw("Resource/images/Scene/Title/title.png������܂���\n");
	}

	if (enter_se == -1)
	{
		throw("Resource/sounds/se/enter.mp3������܂���\n");
	}

	if (cursor_move_se == -1)
	{
		throw("Resource/sounds/se/cursor_move.mp3������܂���\n");
	}

	// �X�v���b�h�V�[�g�̃f�[�^���擾
	//client.GetSpreadsheetData();

	//const auto task = client.GetPlayCount();
	//// �񓯊��^�X�N�̌��ʂ𓯊��I�ɑ҂�
	//play_count = task.get();

	// �񓯊��Ƀv���C�񐔂��擾
	auto task = client.GetPlayCount().then([this](int count) {
		play_count = count;
		});

	//BGM�̍Đ�
	PlaySoundMem(background_sound, DX_PLAYTYPE_LOOP, FALSE);

	//enemyManager.LoadImages();

	//�����L���O�����擾
	ranking = new RankingData;
	ranking->Initialize();
}


//�X�V����
eSceneType TitleScene::Update()
{

	// �w�i�摜�𖳌��X�N���[��
	if (scroll <= 1280)
	{
		scroll++;
	}
	else
	{
		scroll = 0;
	}

	// ���X�e�B�b�N�̓|���ꂽ�������擾
	const StickDirection direction = InputControl::GetLStickDirection(20000, 10);

	//�J�[�\�����ړ�
	if (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_DOWN)
		|| direction == StickDirection::Down)
	{
		// SE�Đ�
		PlaySoundMem(cursor_move_se, DX_PLAYTYPE_BACK, TRUE);
		menu_cursor = (menu_cursor + 1) % 4;
	}


	//�J�[�\����ړ�
	if (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_UP)
		|| direction == StickDirection::Up)
	{
		// SE�Đ�
		PlaySoundMem(cursor_move_se, DX_PLAYTYPE_BACK, TRUE);
		menu_cursor = (menu_cursor + 3) % 4;
	}

	//�J�[�\������i���肵����ʂɑJ�ڂ���j
	if (InputControl::GetButtonDown(XINPUT_BUTTON_A))
	{
		// SE�Đ�
		PlaySoundMem(enter_se, DX_PLAYTYPE_BACK, TRUE);
		//SE����I����Ă����ʐ��ڂ���B
		while (CheckSoundMem(enter_se)) {}

		switch (menu_cursor)
		{
		case 0:
			// �v���C�񐔂��C���N�������g
			//client.IncrementPlayCount();
			// �v���C�񐔂��C���N�������g�i�o�b�N�O���E���h�Ŏ��s�j
			client.IncrementPlayCount().then([](pplx::task<void> t) {
				try {
					t.get(); // �G���[���L���b�`
					std::wcout << U("�v���C�񐔂��C���N�������g���܂����B") << std::endl;
				}
				catch (...) {
					// �G���[����
					std::cerr << "�v���C�񐔂̃C���N�������g�Ɏ��s���܂����B" << std::endl;
				}
				});
			return eSceneType::E_MAIN;

		case 1:
			return eSceneType::E_RANKING_DISP;

		case 2:
			// �O���BGM�̃n���h�����i�[
			GameData::SetPrevBGM(background_sound);
			return eSceneType::E_HELP;

		case 3:
			return eSceneType::E_END;
		}
	}

	//enemyManager.Update();

	//���݂̃V�[���^�C�v��Ԃ�
	return GetNowScene();
}


//�`�揈��
void TitleScene::Draw()const
{

	//�^�C�g���摜�̕`��
	DrawGraph(scroll % 1280 - 1280, 0, background_image, TRUE);
	DrawGraph(scroll % 1280, 0, background_image, TRUE);

	//enemyManager.Draw();

	//DrawGraph(0, 0, background_image, FALSE);
	DrawGraph(300, 70, title_image, TRUE);

	//���j���[�摜�̕`��
	DrawGraph(420, 300, menu_image, TRUE);

	//�J�[�\���摜�̕`��
	DrawRotaGraph(430, 350 + menu_cursor * 75, 0.7, DX_PI / 0.1, cursor_image, TRUE);
	DrawRotaGraph(800, 350 + menu_cursor * 75, 0.7, DX_PI / 0.1, cursor_image, TRUE);
	//std::string shifted_play_count = WStringToShiftJIS(play_count);
	//DrawFormatString(0, 0, 0xffffff, "�v���C��: %s", shifted_play_count.c_str());

	/*�g�p���镶���R�[�h�� utf8 �ɐݒ�*/
	//SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);

	//DrawFormatStringToHandle(50, 100, 0xffffff, buttonGuidFont, "�v���C��: %d", play_count);

	// play_count��-1�ȊO�̏ꍇ�ɂ̂݃v���C�񐔂�\��
	if (play_count != -1) {
		// �v���C�񐔂̕\������
		DrawFormatStringToHandle(50, 100, 0xffffff, buttonGuidFont, "�v���C��: %d", play_count);
	}
	else
	{
		DrawFormatStringToHandle(50, 100, 0xffffff, buttonGuidFont, "�v���C��: �ǂݍ��ݒ�...");
	}
	/*�g�p���镶���R�[�h�� shift-jis �ɐݒ�*/
	//SetUseCharCodeFormat(DX_CHARCODEFORMAT_SHIFTJIS);

	const std::vector<guideElement> gamepad_guides = {
					guideElement({"L"}, "�ړ�", GUIDE_SHAPE_TYPE::JOYSTICK, buttonGuidFont, 0x000000,
						 0xFFFFFF, 0xFFFFFF),
guideElement({"A"}, "����", GUIDE_SHAPE_TYPE::FIXED_CIRCLE,
		 buttonGuidFont, 0xFFFFFF, A_BUTTON_COLOR,
		 0xEB3229, 0xFFFFFF),
	};

	// �{�^���K�C�h�̕`��
	DrawGuides(gamepad_guides, 505.0f, 660.0f, 5.0f, 60.0f);

	//�擾���������L���O�f�[�^��`�悷��
	//for (int i = 0; i < 1; i++)
	//{
	//	/*DrawFormatStringToHandle(50, 170 + i * 25, 0xffffff, buttonGuidFont, "%2d %-15s %6d",
	//		ranking->GetRank(i), ranking->GetName(i), ranking->GetScore(i));*/
	//		DrawFormatStringToHandle(50, 170 + i * 25, 0xffffff, buttonGuidFont, "�O��̃X�R�A�F%6d",
	//			ranking->GetScore(i));
	//}
}


//�I��������
void TitleScene::Finalize()
{
	//�V�[���̐؂�ւ����s��ꂽ��BGM���~�߂�
	StopSoundMem(background_sound);

	// �i�[���ꂽBGM�ƈقȂ�ꍇ�͍폜
	if (GameData::GetPrevBGM() != background_sound)
	{
		DeleteSoundMem(background_sound);
	}

	DeleteSoundMem(enter_se);
	DeleteSoundMem(cursor_move_se);

	DeleteFontToHandle(buttonGuidFont);

	//�ǂݍ��񂾉摜�̍폜
	DeleteGraph(background_image);
	DeleteGraph(title_image);
	DeleteGraph(menu_image);
	DeleteGraph(cursor_image);

}


//���݂̃V�[�������擾
eSceneType TitleScene::GetNowScene()const
{
	return eSceneType::E_TITLE;
}