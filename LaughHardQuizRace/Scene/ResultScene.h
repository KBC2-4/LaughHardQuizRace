#pragma once

#include "SceneBase.h"

class ResultScene:public SceneBase
{
private:
	int background_sound;		//BGM
	int background_image;      //背景画像
	int font_handle_h1;     //フォントハンドル
	int font_handle_h2;     //フォントハンドル
	int buttonGuidFont;     //ガイド表示用フォントハンドル
	int score;            //スコア

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

