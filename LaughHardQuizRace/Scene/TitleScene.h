#pragma once

#include "SceneBase.h"

class TitleScene:public SceneBase
{
private:

private:
	int background_image;
	int menu_image;
	int cursor_image;
	int menu_cursor;

public:
	TitleScene();
	virtual ~TitleScene();

	virtual void  Initialize() override;
	virtual eSceneType Update() override;
	virtual void Draw() const override;
	virtual void Finalize() override;

	virtual eSceneType GetNowScene() const override;
};

