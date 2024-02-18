#pragma once

#include "SceneBase.h"
#include "../Object/RankingData.h"

class RankingInputScene:public SceneBase
{
private:
	int backgrouond_image;   //背景画像
	int font_handle_h2;     // フォントハンドル
	int font_handle_h3;     // フォントハンドル
	int font_handle_h1;     // フォントハンドル
	int char_font_handle;     // 入力文字表示用フォントハンドル
	int buttonGuidFont;     // ガイド表示用フォントハンドル
	int background_sound;		//BGM
	int cursor_move_se;       //カーソル移動 音声ハンドル
	int enter_se;        //決定 音声ハンドル
	RankingData* ranking;    //ランキング情報
	int score;               //スコア
	char name[15];           //名前
	int name_num;            //名前入力数
	int cursor_x;            //カーソルx座標
	int cursor_y;            //カーソルy座標

public:
	RankingInputScene();
	virtual ~RankingInputScene();

	virtual void Initialize() override;
	virtual eSceneType Update() override;
	virtual void Draw() const override;
	virtual void Finalize() override;

	virtual eSceneType GetNowScene() const override;

private:
	//名前入力処理
	bool InputName();
	// char配列をstd::wstringに変換する
	std::wstring CharArrayToWString(const char* charArray);
};

