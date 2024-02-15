#include "HelpScene.h"
#include "../Utility/InputControl.h"
#include "DxLib.h"

HelpScene::HelpScene() :Help_image(NULL),background_image(NULL),scroll(0)
{

}

HelpScene::~HelpScene()
{

}


//����������
void HelpScene::Initialize()
{
	//�摜�̓ǂݍ���
	Help_image = LoadGraph("Resource/images/Help.png");
	background_image = LoadGraph("Resource/images/Scene/Title/background.png");

	background_sound = LoadSoundMem("Resource/sounds/bgm/Electric_Shine.mp3");

	//�G���[�`�F�b�N
	if (Help_image == -1)
	{
		throw("Resource/images.Title.png������܂���\n");
	}

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
		return eSceneType::E_TITLE;
	}
	if (InputControl::GetButtonDown(XINPUT_BUTTON_A))
	{
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
	DeleteSoundMem(background_sound);
	//�ǂݍ��񂾉摜�̍폜
	DeleteGraph(Help_image);
}

//���݂̃V�[�������擾
eSceneType HelpScene::GetNowScene()const
{
	return eSceneType::E_HELP;
}