#include "HelpScene.h"
#include "../Utility/InputControl.h"
#include "DxLib.h"

HelpScene::HelpScene() :Help_image(NULL),background_image(NULL),scroll(0)
{

}

HelpScene::~HelpScene()
{

}


//初期化処理
void HelpScene::Initialize()
{
	//画像の読み込み
	Help_image = LoadGraph("Resource/images/Help.png");
	background_image = LoadGraph("Resource/images/Scene/Title/background.png");

	background_sound = LoadSoundMem("Resource/sounds/bgm/Electric_Shine.mp3");

	//エラーチェック
	if (Help_image == -1)
	{
		throw("Resource/images.Title.pngがありません\n");
	}

	PlaySoundMem(background_sound, DX_PLAYTYPE_LOOP, FALSE);
}


//更新処理
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

	//Bボタンが押されたら、タイトルに戻る
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


//描画処理
void HelpScene::Draw()const
{
	DrawGraph(scroll % 1280 - 1280, 0, background_image, TRUE);
	DrawGraph(scroll % 1280, 0, background_image, TRUE);

	//背景画像の描画
	DrawGraph(0, 0, Help_image, TRUE);
}


//終了処理
void HelpScene::Finalize()
{
	StopSoundMem(background_sound);
	DeleteSoundMem(background_sound);
	//読み込んだ画像の削除
	DeleteGraph(Help_image);
}

//現在のシーン情報を取得
eSceneType HelpScene::GetNowScene()const
{
	return eSceneType::E_HELP;
}