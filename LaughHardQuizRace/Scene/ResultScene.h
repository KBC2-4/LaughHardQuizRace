#pragma once

#include "SceneBase.h"

class ResultScene:public SceneBase
{
private:
	int background_image;      //”wŒi‰æ‘œ
	int score;            //ƒXƒRƒA
	int enemy_count[3];   //“G‚ÌƒJƒEƒ“ƒg
	int enemy_image[3];   //“G‰æ‘œ

public:
	ResultScene();
	virtual ~ResultScene();

	virtual void Initialize() override;
	virtual eSceneType Update() override;
	virtual void Draw() const override;
	virtual void Finalize() override;

	virtual eSceneType GetNowScene() const override;

private:
	void ReadResultData();
};

