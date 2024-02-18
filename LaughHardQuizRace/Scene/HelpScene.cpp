#include "../Utility/InputControl.h"
#include "DxLib.h"
#include "HelpScene.h"
#include "../Utility/GameData.h"

HelpScene::HelpScene() :Help_image(NULL), background_image(NULL), scroll(0)
{

}

HelpScene::~HelpScene()
{

}


//����������
void HelpScene::Initialize()
{
	//�摜�̓ǂݍ���
	Help_image = LoadGraph("Resource/images/Scene/Help/Help.png");
	background_image = LoadGraph("Resource/images/Scene/Title/background.png");

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

	//�G���[�`�F�b�N
	if (Help_image == -1)
	{
		throw("Resource/images/Scene/Help/Help.png\n");
	}

	if (enter_se == -1)
	{
		throw("Resource/sounds/se/enter.mp3������܂���\n");
	}

	// BGM�̍Đ�
	PlaySoundMem(background_sound, DX_PLAYTYPE_LOOP, FALSE);
}


//�X�V����
eSceneType HelpScene::Update()
{
	if (scroll <= 1280)
	{
		scroll++;
	}
	else
	{
		scroll = 0;
	}

	//B�{�^���������ꂽ��A�^�C�g���ɖ߂�
	if (InputControl::GetButtonDown(XINPUT_BUTTON_B))
	{
		// SE�Đ�
		PlaySoundMem(enter_se, DX_PLAYTYPE_BACK, TRUE);
		//SE����I����Ă����ʐ��ڂ���B
		while (CheckSoundMem(enter_se)) {}
		// �O���BGM�̃n���h�����i�[
		GameData::SetPrevBGM(background_sound);
		return eSceneType::E_TITLE;
	}
	if (InputControl::GetButtonDown(XINPUT_BUTTON_A))
	{
		// SE�Đ�
		PlaySoundMem(enter_se, DX_PLAYTYPE_BACK, TRUE);
		//SE����I����Ă����ʐ��ڂ���B
		while (CheckSoundMem(enter_se)) {}
		return eSceneType::E_MAIN;
	}
	return GetNowScene();
}


//�`�揈��
void HelpScene::Draw()const
{
	DrawGraph(scroll % 1280 - 1280, 0, background_image, TRUE);
	DrawGraph(scroll % 1280, 0, background_image, TRUE);

	//�w�i�摜�̕`��
	DrawGraph(0, 0, Help_image, TRUE);
}


//�I������
void HelpScene::Finalize()
{
	StopSoundMem(background_sound);
	// �i�[���ꂽBGM�ƈقȂ�ꍇ�͍폜
	if (GameData::GetPrevBGM() != background_sound)
	{
		DeleteSoundMem(background_sound);
	}

	DeleteSoundMem(enter_se);

	//�ǂݍ��񂾉摜�̍폜
	DeleteGraph(Help_image);
}

//���݂̃V�[�������擾
eSceneType HelpScene::GetNowScene()const
{
	return eSceneType::E_HELP;
}