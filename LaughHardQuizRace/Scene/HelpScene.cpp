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


//初期化処理
void HelpScene::Initialize()
{
	//画像の読み込み
	Help_image = LoadGraph("Resource/images/Scene/Help/Help.png");
	background_image = LoadGraph("Resource/images/Scene/Title/background.png");

	// 前回の再生途中のBGMそれを再生
	if (GameData::GetPrevBGM() == -1)
	{
		background_sound = LoadSoundMem("Resource/sounds/bgm/title.mp3");
		if (background_sound == -1)
		{
			throw("Resource/sounds/bgm/title.mp3がありません\n");
		}
	}
	else
	{
		background_sound = GameData::GetPrevBGM();
	}

	enter_se = LoadSoundMem("Resource/sounds/se/enter.mp3");

	//エラーチェック
	if (Help_image == -1)
	{
		throw("Resource/images/Scene/Help/Help.png\n");
	}

	if (enter_se == -1)
	{
		throw("Resource/sounds/se/enter.mp3がありません\n");
	}

	// BGMの再生
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
		// SE再生
		PlaySoundMem(enter_se, DX_PLAYTYPE_BACK, TRUE);
		//SEが鳴り終わってから画面推移する。
		while (CheckSoundMem(enter_se)) {}
		// 前回のBGMのハンドルを格納
		GameData::SetPrevBGM(background_sound);
		return eSceneType::E_TITLE;
	}
	if (InputControl::GetButtonDown(XINPUT_BUTTON_A))
	{
		// SE再生
		PlaySoundMem(enter_se, DX_PLAYTYPE_BACK, TRUE);
		//SEが鳴り終わってから画面推移する。
		while (CheckSoundMem(enter_se)) {}
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
	// 格納されたBGMと異なる場合は削除
	if (GameData::GetPrevBGM() != background_sound)
	{
		DeleteSoundMem(background_sound);
	}

	DeleteSoundMem(enter_se);

	//読み込んだ画像の削除
	DeleteGraph(Help_image);
}

//現在のシーン情報を取得
eSceneType HelpScene::GetNowScene()const
{
	return eSceneType::E_HELP;
}