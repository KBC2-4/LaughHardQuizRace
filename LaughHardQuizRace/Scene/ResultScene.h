#pragma once

#include "SceneBase.h"

class ResultScene:public SceneBase
{
private:
	int background_sound;		//BGM
	int background_image;      //背景画像
	int font_handle_h1;     //フォントハンドル
	int score_font_handle;     //フォントハンドル
	int font_handle_h3;     //フォントハンドル
	int buttonGuidFont;     //ガイド表示用フォントハンドル
	int enter_se;        //決定 音声ハンドル
	int score;            //スコア
	bool is_score_in_top10; //スコアがランキングインしたかどうか
	bool data_loaded; //データがロードされたかどうか

public:
	ResultScene();
	virtual ~ResultScene();

	virtual void Initialize() override;
	virtual eSceneType Update() override;
	virtual void Draw() const override;
	virtual void Finalize() override;

	virtual eSceneType GetNowScene() const override;
};

