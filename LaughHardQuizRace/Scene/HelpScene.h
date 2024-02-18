#pragma once

#include "SceneBase.h"

class HelpScene:public SceneBase
{
private:
	int Help_image; //�w�i�摜
	int background_image;
	int scroll;
	int background_sound;
	int enter_se;        //���� �����n���h��
public:
	HelpScene();
	virtual ~HelpScene();

	virtual void Initialize() override;
	virtual eSceneType Update() override;
	virtual void Draw() const override;
	virtual void Finalize() override;

	virtual eSceneType GetNowScene() const override;
};