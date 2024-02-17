#pragma once
#include "SceneBase.h"
#include "../Object/RankingData.h"

class RankingDispScene:public SceneBase
{
private:
	int font_handle_h2;     // フォントハンドル
	int font_handle_h3;     // フォントハンドル
	int font_handle_h1;     // フォントハンドル
	int buttonGuidFont;     // ガイド表示用フォントハンドル
	int background_image; // 背景画像
	int crown_image[3];  // 王冠画像
	RankingData* ranking; // ランキング情報

public:
	RankingDispScene();
	virtual ~RankingDispScene();

	virtual void Initialize() override;
	virtual eSceneType Update() override;
	virtual void Draw() const override;
	virtual void Finalize() override;

	virtual eSceneType GetNowScene() const override;
};

