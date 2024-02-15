#pragma once

#include "SceneBase.h"

class ResultScene:public SceneBase
{
private:
	int background_image;      //背景画像
	int font_handle_h2;     //フォントハンドル
	int font_handle_h3;     //フォントハンドル
	int score;            //スコア
	int enemy_count[3];   //敵のカウント
	int enemy_image[3];   //敵画像

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

