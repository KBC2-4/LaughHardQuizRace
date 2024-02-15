#include "TitleScene.h"
#include "../Utility/InputControl.h"
#include "DxLib.h"

TitleScene::TitleScene() :background_image(NULL), menu_image(NULL), scroll(0), title_image(NULL), background_sound(NULL),
cursor_image(NULL), menu_cursor(0)/*, client(L"AKfycbyoJ4KKmOTRqUji0Tycg6710ZE8SlRKMCuXO9YtUzQ0ZhPx2-WO5yCKKM8xMA8fbthB")*/
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
	menu_image = LoadGraph("Resource/images/menu.png");
	cursor_image = LoadGraph("Resource/images/cone.png");
	title_image = LoadGraph("Resource/images/title2.png");

	//BGM�̓ǂݍ���
	background_sound = LoadSoundMem("Resource/sounds/bgm/Electric_Shine.mp3");

	//�G���[�`�F�b�N
	if (background_image == -1)
	{
		throw("Resource/images/Scene/Title/background.png������܂���\n");
	}
	if (menu_image == -1)
	{
		throw("Resource/images/menu.png������܂���\n");
	}
	if (cursor_image == -1)
	{
		throw("Resource/images/cursor.png������܂���\n");
	}
	if (title_image == -1)
	{
		throw("Resource/images/title2.png������܂���\n");
	}

	if (background_sound == -1)
	{
		throw("Resource/sounds/bgm/Electric_Shine.mp3������܂���\n");
	}
	// �X�v���b�h�V�[�g�̃f�[�^���擾
	//client.GetSpreadsheetData();

	//// �X�v���b�h�V�[�g�Ƀf�[�^����������
	//client.PostSpreadsheetData(L"player123", 1000);

	//const auto task = client.GetPlayCount();
	//// �񓯊��^�X�N�̌��ʂ𓯊��I�ɑ҂�
	//play_count = task.get();

	// �g�p���镶���R�[�h�� utf8 �ɐݒ�
	//SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);

	//printfDx("PlayCount: %s\n", play_count.c_str());

	//BGM�̍Đ�
	PlaySoundMem(background_sound, DX_PLAYTYPE_BACK, FALSE);
}


//�X�V����
eSceneType TitleScene::Update()
{

	//�J�[�\�����ړ�
	if (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_DOWN) || InputControl::GetLStickDirection() == StickDirection::Down)
	// �w�i�摜�𖳌��X�N���[��
	if(scroll <= 1280)
	{
		scroll++;
	}else
	{
		scroll = 0;
	}

	// ���X�e�B�b�N�̓|���ꂽ�������擾
	const StickDirection direction = InputControl::GetLStickDirection(20000, 0);

	//�J�[�\�����ړ�
	if (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_DOWN) 
		|| direction == StickDirection::Down)
	{
		menu_cursor = (menu_cursor + 1) % 4;
	}


	//�J�[�\����ړ�
	if (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_UP) 
		|| direction == StickDirection::Up)
	{
		menu_cursor = (menu_cursor + 3) % 4;
	}

	//�J�[�\������i���肵����ʂɑJ�ڂ���j
	if (InputControl::GetButtonDown(XINPUT_BUTTON_B))
	{
		switch (menu_cursor)
		{
			case 0:
				// �v���C�񐔂��C���N�������g
				//client.IncrementPlayCount();
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
	DrawGraph(scroll % 1280 - 1280, 0, background_image, TRUE);
	DrawGraph(scroll % 1280, 0, background_image, TRUE);
	//DrawGraph(0, 0, background_image, FALSE);
	DrawGraph(300, 70, title_image, TRUE);

	//���j���[�摜�̕`��
	DrawGraph(420, 300, menu_image, TRUE);

	//�J�[�\���摜�̕`��
	DrawRotaGraph(430, 350 + menu_cursor * 75, 0.7, DX_PI / 0.1, cursor_image, TRUE);
	DrawRotaGraph(800, 350 + menu_cursor * 75, 0.7, DX_PI / 0.1, cursor_image, TRUE);
	//std::string shifted_play_count = WStringToShiftJIS(play_count);
	//DrawFormatString(0, 0, 0xffffff, "�v���C��: %s", shifted_play_count.c_str());
	DrawFormatString(0, 0, 0xffffff, "�v���C��: %d", play_count);

	
}


//�I��������
void TitleScene::Finalize()
{
	//�V�[���̐؂�ւ����s��ꂽ��BGM���~�߂�
	StopSoundMem(background_sound);
	DeleteSoundMem(background_sound);

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