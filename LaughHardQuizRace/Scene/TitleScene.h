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
	RankingData* ranking; //�����L���O���

private:
	int buttonGuidFont;     //�K�C�h�\���p�t�H���g�n���h��
	int background_image;
	int background_sound;
	int cursor_move_se;       //�J�[�\���ړ� �����n���h��
	int enter_se;        //���� �����n���h��
	int menu_image;
	int title_image;
	int back_ground;     //�w�i�摜
	int scroll;         //���s����
	int cursor_image;
	int menu_cursor;
	int play_count;             //�v���C��

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

