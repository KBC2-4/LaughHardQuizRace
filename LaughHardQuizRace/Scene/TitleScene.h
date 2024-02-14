#pragma once

#include <string>
#include "../Utility/SpreadsheetClient.h"
#include "SceneBase.h"

class TitleScene:public SceneBase
{
private:

private:
	int background_image;
	int menu_image;
	int title_image;
	int back_ground;     //�w�i�摜
	int scroll;         //���s����
	int cursor_image;
	int menu_cursor;
	int play_count;             //�v���C��

	//SpreadsheetClient client;

public:
	TitleScene();
	virtual ~TitleScene();

	virtual void  Initialize() override;
	virtual eSceneType Update() override;
	virtual void Draw() const override;
	virtual void Finalize() override;

	virtual eSceneType GetNowScene() const override;
};

