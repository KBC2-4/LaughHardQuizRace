#pragma once

#include "SceneBase.h"

class ResultScene:public SceneBase
{
private:
	int background_sound;		//BGM
	int background_image;      //�w�i�摜
	int font_handle_h1;     //�t�H���g�n���h��
	int score_font_handle;     //�t�H���g�n���h��
	int font_handle_h3;     //�t�H���g�n���h��
	int buttonGuidFont;     //�K�C�h�\���p�t�H���g�n���h��
	int enter_se;        //���� �����n���h��
	int score;            //�X�R�A
	bool is_score_in_top10; //�X�R�A�������L���O�C���������ǂ���
	bool data_loaded; //�f�[�^�����[�h���ꂽ���ǂ���

public:
	ResultScene();
	virtual ~ResultScene();

	virtual void Initialize() override;
	virtual eSceneType Update() override;
	virtual void Draw() const override;
	virtual void Finalize() override;

	virtual eSceneType GetNowScene() const override;
};

