#include "DxLib.h"
#include "../Utility/InputControl.h"
#include "ResultScene.h"
#include "../Object/RankingData.h"
#include "../Utility/Guide.h"
#include "../Utility/GameData.h"
#include "../Utility/SpreadsheetClient.h"

ResultScene::ResultScene() :background_image(0), enter_se(0), score(GameData::GetScore()), is_score_in_top10(false), data_loaded(false)
{
}

ResultScene::~ResultScene()
{
}


//����������
void ResultScene::Initialize()
{
	//�摜�̓ǂݍ���
	background_image = LoadGraph("Resource/images/Scene/Result/background.png");

	//�G���[�`�F�b�N
	if (background_image == -1)
	{
		throw("Resource/images/Scene/Result/background.png������܂���\n");
	}

	font_handle_h1 = CreateFontToHandle("Segoe UI", 180, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, 5);
	score_font_handle = CreateFontToHandle("Segoe UI", 160, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, 3, 1);
	font_handle_h3 = CreateFontToHandle("Segoe UI", 60, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, 3, 1);
	buttonGuidFont = CreateFontToHandle("���C���I", 23, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

	//BGM�̓ǂݍ���
	background_sound = LoadSoundMem("Resource/sounds/bgm/title.mp3");

	enter_se = LoadSoundMem("Resource/sounds/se/enter.mp3");

	if (background_sound == -1)
	{
		throw("Resource/sounds/bgm/title.mp3������܂���\n");
	}

	if (enter_se == -1)
	{
		throw("Resource/sounds/se/enter.mp3������܂���\n");
	}

	//BGM�̍Đ�
	PlaySoundMem(background_sound, DX_PLAYTYPE_LOOP, FALSE);

	// �X�R�A�̎擾
	this->score = GameData::GetScore();

	// �X�N���v�gID��ݒ�
	SpreadsheetClient client(L"AKfycbwTncJE7fNBzWd4aZqG7Q7_ysP9XTNcV91VlStNzjDP3Xw95oEKgYwiNVQgU73AXxpv");

	client.IsScoreInTop10(this->score).then([this](pplx::task<bool> task) {
		try {
			// ��O���������Ȃ������ꍇ�ɂ͐���Ƀ��[�h���ꂽ�Ƃ݂Ȃ�
			this->is_score_in_top10 = task.get();
			this->data_loaded = true;
		}
		catch (...) {
			// �G���[����: ��O�����������ꍇ
			this->is_score_in_top10 = false;
			// ���[�h����Ă��Ȃ��Ă����s������
			this->data_loaded = true;
		}
	});
}


//�X�V����
eSceneType ResultScene::Update()
{
	//B�{�^���Ń����L���O�ɑJ�ڂ���
	if (InputControl::GetButtonDown(XINPUT_BUTTON_A))
	{

		// �f�[�^���[�h�̎��s�̌��ʂ��͂��Ă��Ȃ��ꍇ�́A���݂̃V�[����Ԃ�
		if (!data_loaded)
		{
			return GetNowScene();
		}
		// SE�Đ�
		PlaySoundMem(enter_se, DX_PLAYTYPE_BACK, TRUE);
		//SE����I����Ă����ʐ��ڂ���B
		while (CheckSoundMem(enter_se)) {}
		return eSceneType::E_RANKING_INPUT;
	}

	return GetNowScene();
}


//�`�揈��
void ResultScene::Draw()const
{
	//�w�i�摜��`��
	DrawGraph(0, 0, background_image, TRUE);

	DrawStringToHandle(GetDrawCenterX("�X�R�A", font_handle_h1), 30, "�X�R�A", 0x4532ED, font_handle_h1, 0xffffff);
	//const char score_char = score;
	DrawFormatString2ToHandle(GetDrawCenterX(std::to_string(score).c_str(), score_font_handle), 400, 0xE6106E, 0xffffff, score_font_handle, "%d", score);

	if (data_loaded == true) {
		if (is_score_in_top10)
		{
			DrawStringToHandle(GetDrawCenterX("���߂łƂ��������܂��I", font_handle_h3), 230,
				"���߂łƂ��������܂��I", 0xEF9309, font_handle_h3, 0xffffff);
			DrawStringToHandle(GetDrawCenterX("�����L���O10�ʈȓ��ɓ���܂����I", font_handle_h3), 310,
				"�����L���O10�ʈȓ��ɓ���܂����I", 0xEF9309, font_handle_h3, 0xffffff);
		}
		else
		{
			DrawStringToHandle(GetDrawCenterX("�����L���O10�ʈȓ��ɓ���܂���ł����B", font_handle_h3), 230,
				"�����L���O10�ʈȓ��ɓ���܂���ł����B", 0xED4132, font_handle_h3, 0xffffff);
			DrawStringToHandle(GetDrawCenterX("���A�����L���O�ɃX�R�A���c���܂��傤�I", font_handle_h3), 310,
				"���A�����L���O�ɃX�R�A���c���܂��傤�I", 0xED4132, font_handle_h3, 0xffffff);
		}
	}

	const std::vector<guideElement> gamepad_guides = {
guideElement({"A"}, "�����L���O���͉�ʂ�", GUIDE_SHAPE_TYPE::FIXED_CIRCLE,
		 buttonGuidFont, 0xFFFFFF, A_BUTTON_COLOR,
		 0xEB3229, 0xFFFFFF),
	};

	// �{�^���K�C�h�̕`��
	DrawGuides(gamepad_guides, 505.0f, 660.0f, 5.0f, 60.0f);
}


//�I��������
void ResultScene::Finalize()
{
	//�V�[���̐؂�ւ����s��ꂽ��BGM���~�߂�
	StopSoundMem(background_sound);
	DeleteSoundMem(background_sound);

	DeleteSoundMem(enter_se);

	//�ǂݍ��񂾉摜���폜
	DeleteGraph(background_image);

	// �t�H���g�̍폜
	DeleteFontToHandle(font_handle_h1);
	DeleteFontToHandle(score_font_handle);
	DeleteFontToHandle(font_handle_h3);
	DeleteFontToHandle(buttonGuidFont);
}


//���݂̃V�[�������擾
eSceneType ResultScene::GetNowScene()const
{
	return eSceneType::E_RESULT;
}