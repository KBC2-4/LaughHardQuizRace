#pragma once
#include "SceneBase.h"
#include "../Object/RankingData.h"

class RankingDispScene:public SceneBase
{
private:
	int font_handle_h2;     // �t�H���g�n���h��
	int font_handle_h3;     // �t�H���g�n���h��
	int font_handle_h1;     // �t�H���g�n���h��
	int buttonGuidFont;     // �K�C�h�\���p�t�H���g�n���h��
	int background_image; // �w�i�摜
	int crown_image[3];  // �����摜
	int background_sound; // BGM
	int enter_se;        //���� �����n���h��
	RankingData* ranking; // �����L���O���
	std::wstring rawData; // �����L���O�f�[�^

public:
	RankingDispScene();
	virtual ~RankingDispScene();

	virtual void Initialize() override;
	virtual eSceneType Update() override;
	virtual void Draw() const override;
	virtual void Finalize() override;

	virtual eSceneType GetNowScene() const override;
};

