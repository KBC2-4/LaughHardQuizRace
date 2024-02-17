#include "DxLib.h"
#include "../Utility/InputControl.h"
#include "ResultScene.h"
#include "../Object/RankingData.h"
#include "../Utility/Guide.h"
#include "../Utility/GameData.h"

ResultScene::ResultScene() :background_image(0), score(GameData::GetScore())
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

	font_handle_h1 = CreateFontToHandle("Segoe UI", 180, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	font_handle_h2 = CreateFontToHandle("Segoe UI", 160, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);
	buttonGuidFont = CreateFontToHandle("���C���I", 23, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

	//BGM�̓ǂݍ���
	background_sound = LoadSoundMem("Resource/sounds/bgm/title.mp3");

	if (background_sound == -1)
	{
		throw("Resource/sounds/bgm/title.mp3������܂���\n");
	}

	//BGM�̍Đ�
	PlaySoundMem(background_sound, DX_PLAYTYPE_LOOP, FALSE);

	// �X�R�A�̎擾
	this->score = GameData::GetScore();;
}


//�X�V����
eSceneType ResultScene::Update()
{
	//B�{�^���Ń����L���O�ɑJ�ڂ���
	if (InputControl::GetButtonDown(XINPUT_BUTTON_A))
	{
		//return eSceneType::E_TITLE;
		//return eSceneType::E_RANKING_INPUT;
	}

	return GetNowScene();
}


//�`�揈��
void ResultScene::Draw()const
{
	//�w�i�摜��`��
	DrawGraph(0, 0, background_image, TRUE);

	DrawStringToHandle(440, 80, "�X�R�A", 0x10B3E6, font_handle_h1, 0xE67B10);
	DrawFormatString2ToHandle(370, 430, 0xE6106E, 0x10E617, font_handle_h2, "%6d", score);

	const std::vector<guideElement> gamepad_guides = {
guideElement({"A"}, "�^�C�g����", GUIDE_SHAPE_TYPE::FIXED_CIRCLE,
		 buttonGuidFont, 0xFFFFFF, 0xEBE146,
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

	//�ǂݍ��񂾉摜���폜
	DeleteGraph(background_image);

	// �t�H���g�̍폜
	DeleteFontToHandle(font_handle_h1);
	DeleteFontToHandle(font_handle_h2);
	DeleteFontToHandle(buttonGuidFont);
}


//���݂̃V�[�������擾
eSceneType ResultScene::GetNowScene()const
{
	return eSceneType::E_RESULT;
}