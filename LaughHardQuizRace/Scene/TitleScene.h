#pragma once

#include <string>
#include "../Utility/SpreadsheetClient.h"
#include "SceneBase.h"
#include "../Object/enemyManager.h"
#include "../Object/RankingData.h"

class RankingData;

class TitleScene:public SceneBase
{
private:
	RankingData* ranking; //ランキング情報

private:
	int buttonGuidFont;     //ガイド表示用フォントハンドル
	int background_image;
	int background_sound;
	int cursor_move_se;       //カーソル移動 音声ハンドル
	int enter_se;        //決定 音声ハンドル
	int menu_image;
	int title_image;
	int back_ground;     //背景画像
	int scroll;         //走行距離
	int cursor_image;
	int menu_cursor;
	int play_count;             //プレイ回数

	SpreadsheetClient client;
	//EnemyManager enemyManager;

public:
	TitleScene();
	virtual ~TitleScene();

	virtual void  Initialize() override;
	virtual eSceneType Update() override;
	virtual void Draw() const override;
	virtual void Finalize() override;

	virtual eSceneType GetNowScene() const override;
};

