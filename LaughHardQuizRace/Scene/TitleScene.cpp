#include "TitleScene.h"
#include "../Utility/InputControl.h"
#include "DxLib.h"

TitleScene::TitleScene() :background_image(NULL), menu_image(NULL),
cursor_image(NULL), menu_cursor(0), client(L"AKfycbyoJ4KKmOTRqUji0Tycg6710ZE8SlRKMCuXO9YtUzQ0ZhPx2-WO5yCKKM8xMA8fbthB")
{

}

TitleScene::~TitleScene()
{

}


//����������
void TitleScene::Initialize()
{
	//�摜�̓ǂݍ���
	background_image = LoadGraph("Resource/images/Scene/Title/background.png");
	menu_image = LoadGraph("Resource/images/menu.bmp");
	cursor_image = LoadGraph("Resource/images/cone.bmp");

	//�G���[�`�F�b�N
	if (background_image == -1)
	{
		throw("Resource/images/Scene/Title/background.png������܂���\n");
	}
	if (menu_image == -1)
	{
		throw("Resource/images/menu.bmp������܂���\n");
	}
	if (cursor_image == -1)
	{
		throw("Resource/images/cursor.bmp������܂���\n");
	}

	// �X�v���b�h�V�[�g�̃f�[�^���擾
	client.GetSpreadsheetData();

	// �X�v���b�h�V�[�g�Ƀf�[�^����������
	client.PostSpreadsheetData(L"player123", 1000);

	const auto task = client.GetPlayCount();
	// �񓯊��^�X�N�̌��ʂ𓯊��I�ɑ҂�
	play_count = task.get();

	// �g�p���镶���R�[�h�� utf8 �ɐݒ�
	//SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);

	//printfDx("PlayCount: %s\n", play_count.c_str());
}


//�X�V����
eSceneType TitleScene::Update()
{
	//�J�[�\�����ړ�
	if (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_DOWN) || InputControl::GetLStickDirection() == StickDirection::Down)
	{
		menu_cursor++;
		//�P�ԉ��ɓ��B������A��ԏ�ɂ���
		if (menu_cursor > 3)
		{
			menu_cursor = 0;
		}
	}


	//�J�[�\����ړ�
	if (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_UP) || InputControl::GetLStickDirection() == StickDirection::Up)
	{
		menu_cursor--;
		//�P�ԏ�ɓ��B������A��ԉ��ɂ���
		if (menu_cursor < 0)
		{
			menu_cursor = 3;
		}
	}

	//�J�[�\������i���肵����ʂɑJ�ڂ���j
	if (InputControl::GetButtonDown(XINPUT_BUTTON_B))
	{
		switch (menu_cursor)
		{
			case 0:
				// �v���C�񐔂��C���N�������g
				client.IncrementPlayCount();
				return eSceneType::E_MAIN;
				
			case 1:
				return eSceneType::E_RANKING_DISP;

			case 2:
				return eSceneType::E_HELP;

			case 3:
				return eSceneType::E_END;
		}
	}

	//���݂̃V�[���^�C�v��Ԃ�
	return GetNowScene();
}


//�`�揈��
void TitleScene::Draw()const
{
	//�^�C�g���摜�̕`��
	DrawGraph(0, 0, background_image, FALSE);

	//���j���[�摜�̕`��
	DrawGraph(120, 200, menu_image, TRUE);

	//�J�[�\���摜�̕`��
	DrawRotaGraph(90, 220 + menu_cursor * 40, 0.7, DX_PI / 2.0, cursor_image, TRUE);

	//std::string shifted_play_count = WStringToShiftJIS(play_count);
	//DrawFormatString(0, 0, 0xffffff, "�v���C��: %s", shifted_play_count.c_str());
	DrawFormatString(0, 0, 0xffffff, "�v���C��: %d", play_count);
}


//�I��������
void TitleScene::Finalize()
{
	//�ǂݍ��񂾉摜�̍폜
	DeleteGraph(background_image);
	DeleteGraph(menu_image);
	DeleteGraph(cursor_image);
}


//���݂̃V�[�������擾
eSceneType TitleScene::GetNowScene()const
{
	return eSceneType::E_TITLE;
}