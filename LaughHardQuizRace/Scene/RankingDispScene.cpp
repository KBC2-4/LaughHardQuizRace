#include "RankingDispScene.h"

#include <iostream>

#include "../Utility/InputControl.h"
#include "DxLib.h"
#include "../Utility/json.hpp"
#include "../Utility/Guide.h"
using json = nlohmann::json;

RankingDispScene::RankingDispScene() :background_image(NULL), ranking(nullptr)
{

}

RankingDispScene::~RankingDispScene()
{

}


//����������
void RankingDispScene::Initialize()
{
	//�摜�̓ǂݍ���
	background_image = LoadGraph("Resource/images/Scene/Ranking/background.png");

	font_handle_h2 = CreateFontToHandle("Segoe UI", 50, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	font_handle_h3 = CreateFontToHandle("Segoe UI", 20, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);
	font_handle_h1 = CreateFontToHandle("Segoe UI", 100, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);
	buttonGuidFont = CreateFontToHandle("���C���I", 23, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

	const int result = LoadDivGraph("Resource/images/Scene/Ranking/crowns.png", 3, 3, 1, 66, 48,
		crown_image);

	//�G���[�`�F�b�N
	if (background_image == -1)
	{
		throw("Resource/images/Scene/Ranking/background.png������܂���\n");
	}

	if (result == -1)
	{
		throw("Resource/images/Scene/Ranking/crowns.png������܂���\n");
	}

	//�����L���O�����擾
	ranking = new RankingData;
	ranking->Initialize();
}

//�X�V����
eSceneType RankingDispScene::Update()
{
	//B�{�^���������ꂽ��A�^�C�g���ɖ߂�
	if (InputControl::GetButtonDown(XINPUT_BUTTON_A))
	{
		return eSceneType::E_TITLE;
	}



	return GetNowScene();
}

// std::wstring �� Shift_JIS �ɕϊ�����֐�
std::string WStringToShiftJIS(const std::wstring& wstr) {
	int size_needed = WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL) + 1; // +1 for null terminator
	std::string result(size_needed, '\0');
	WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), &result[0], size_needed, NULL, NULL);
	return result;
}

//�`�揈��
void RankingDispScene::Draw()const
{
	//�w�i�摜�̕`��
	DrawGraph(0, 0, background_image, FALSE);

	//DrawStringToHandle(400, 30, "�����L���O", 0x0C74FF, font_handle_h1, 0xffffff);

	//std::wstring wstrData = ranking->GetData();
	//std::string strData = WStringToShiftJIS(wstrData);
	//clsDx();
	//printfDx("Ranking Data: %s\n", strData.c_str());

	// �f�[�^�擾��
	std::wstring rawData = ranking->GetData();
	//{	//! �f�o�b�O�p
	//printfDx("Raw Data Length: %d\n", rawData.length());

	//// �ϊ���
	//std::string shiftJISData = WStringToShiftJIS(rawData);
	//printfDx("Converted Data Length: %d\n", shiftJISData.length());
	//printfDx("Converted Data: %s\n", shiftJISData.c_str());

	//// �p�[�X�O
	//if (!shiftJISData.empty() && shiftJISData.back() != '}' && shiftJISData.back() != ']') {
	//	printfDx("Warning: JSON data might be incomplete.\n");
	//}
	//}

	constexpr int base_x = 240;
	constexpr int base_y = 60;

	try {
		auto json = nlohmann::json::parse(rawData);

		// JSON�f�[�^���z��܂��̓I�u�W�F�N�g�̏ꍇ
		if (json.is_array() || json.is_object()) {
			int i = 1; // �����L���O����
			int y = base_y; // Y���W�̏����l
			for (auto& element : json) {
				// PlayerID��Score���擾
				std::string playerID = element["PlayerID"].get<std::string>();
				int score = element["Score"].get<int>();
				std::string addDate = element["Date"].get<std::string>();


				// UTF-8����Shift-JIS�֕ϊ�
				// �g�p���镶���R�[�h�� utf8 �ɐݒ�
				SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
				if (i <= 3)
				{
					DrawGraph(base_x - 60, y + 5, crown_image[i - 1], TRUE);
				}
				DrawFormatString2ToHandle(base_x, y, 0x0C74FF, 0xffffff, font_handle_h2, u8"%2d��", i);
				DrawStringToHandle(base_x + 140, y, playerID.c_str(), 0x0C74FF, font_handle_h2, 0xffffff);
				DrawFormatString2ToHandle(base_x + 440, y, 0x0C74FF, 0xffffff, font_handle_h2, u8"%-d", score);
				DrawStringToHandle(base_x + 600, y, addDate.c_str(), 0x0C74FF, font_handle_h3, 0xffffff);
				y += 60; // ���̍��ڂ̂��߂�Y���W��������
				i++;

				// �g�p���镶���R�[�h�� shift-jis �ɐݒ�
				SetUseCharCodeFormat(DX_CHARCODEFORMAT_SHIFTJIS);
			}
		}
	}
	catch (nlohmann::json::parse_error& e) {
		// �p�[�X�G���[
		printfDx("JSON Parse Error: %s\n", e.what());
	}
	catch (...) {
		// ���̑��̃G���[
		printfDx("�G���[���������܂����B\n");
	}



	const std::vector<guideElement> gamepad_guides = {
guideElement({"A"}, "�^�C�g����", GUIDE_SHAPE_TYPE::FIXED_CIRCLE,
		 buttonGuidFont, 0xFFFFFF, 0xEBE146,
		 0xEB3229, 0xFFFFFF),
	};

	// �{�^���K�C�h�̕`��
	DrawGuides(gamepad_guides, 505.0f, 660.0f, 5.0f, 60.0f);
}


//�I��������
void RankingDispScene::Finalize()
{
	//�ǂݍ��񂾉摜�̍폜
	DeleteGraph(background_image);

	for (int i = 0; i < 3; i++)
	{
		DeleteGraph(crown_image[i]);
	}

	// �t�H���g�̍폜
	DeleteFontToHandle(font_handle_h2);
	DeleteFontToHandle(font_handle_h3);
	DeleteFontToHandle(font_handle_h1);
	DeleteFontToHandle(buttonGuidFont);


	//���I�������̉��
	ranking->Finalize();
	delete ranking;
}


//���݂̃V�[�������擾
eSceneType RankingDispScene::GetNowScene()const
{
	return eSceneType::E_RANKING_DISP;
}