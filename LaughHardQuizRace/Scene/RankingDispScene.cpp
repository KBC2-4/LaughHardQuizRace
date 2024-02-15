#include "RankingDispScene.h"
#include "../Utility/InputControl.h"
#include "DxLib.h"
#include "../Utility/json.hpp"
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

	//�G���[�`�F�b�N
	if (background_image == -1)
	{
		throw("Resource/images/Scene/Ranking/background.png������܂���\n");
	}

	//�����L���O�����擾
	ranking = new RankingData;
	ranking->Initialize();
}

//�X�V����
eSceneType RankingDispScene::Update()
{
	//B�{�^���������ꂽ��A�^�C�g���ɖ߂�
	if (InputControl::GetButtonDown(XINPUT_BUTTON_B))
	{
		return eSceneType::E_TITLE;
	}

	return GetNowScene();
}

// std::wstring �� Shift_JIS �ɕϊ�����֐�
std::string WStringToShiftJIS(const std::wstring& wstr) {
	int size_needed = WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string result(size_needed, 0);
	WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), &result[0], size_needed, NULL, NULL);
	return result;
}

//�`�揈��
void RankingDispScene::Draw()const
{
	//�w�i�摜�̕`��
	DrawGraph(0, 0, background_image, FALSE);

	//�擾���������L���O�f�[�^��`�悷��
	for (int i = 0; i < 5; i++)
	{
		DrawFormatString(50, 170 + i * 25, 0xffffff, "%2d %-15s %6d",
			ranking->GetRank(i), ranking->GetName(i), ranking->GetScore(i));
	}

	// JSON�f�[�^�̃p�[�X
	//try {
	//	json rankingData = json::parse(WStringToShiftJIS(ranking->GetData()).c_str());


	//	// �����L���O�f�[�^�̕`��
	//	for (int i = 0; i < rankingData.size() && i < 5; ++i) {
	//		// JSON�I�u�W�F�N�g����f�[�^���擾
	//		/*std::string playerName = rankingData[i]["PlayerID"];
	//		int score = rankingData[i]["Score"];*/
	//		std::string playerName;
	//		int score = 0;

	//		if (rankingData[i].contains("PlayerID") && rankingData[i]["PlayerID"].is_string()) {
	//			std::string playerName = rankingData[i].at("PlayerID").get<std::string>();
	//		}

	//		if (rankingData[i].contains("Score") && rankingData[i]["Score"].is_number_integer()) {
	//			int score = rankingData[i].at("Score").get<int>();
	//		}


	//		// DX���C�u�����ŕ������`��i�����ł�Shift_JIS�ɕϊ����Ă��܂��j
	//		std::wstring playerNameW(playerName.begin(), playerName.end());
	//		DrawFormatString(50, 170 + i * 25, GetColor(255, 255, 255), "%2d %-15s %6d",
	//			i + 1, playerNameW.c_str(), score);
	//	}
	//}
	//catch (json::parse_error& e) {
	//	// �p�[�X�G���[
	//}
	//catch (json::type_error& e) {
	//	// �^�G���[
	//}
	//catch (...) {
	//	// ���̑��̗�O
	//}

	//try {
	//	json rankingData = json::parse(WStringToShiftJIS(ranking->GetData()).c_str());

	//	for (int i = 0; i < rankingData.size() && i < 5; ++i) {
	//		// JSON�I�u�W�F�N�g����f�[�^���擾�i��̏ꍇ�̓f�t�H���g�l���g�p�j
	//		std::string playerName = rankingData[i].value("PlayerID", "");
	//		int score = rankingData[i].value("Score", 0); // �X�R�A����̏ꍇ��0�Ƃ���

	//		// ��̍s�𖳎�����
	//		if (playerName.empty() && score == 0) continue;

	//		// DX���C�u�����ŕ������`��
	//		std::wstring playerNameW(playerName.begin(), playerName.end());
	//		DrawFormatString(50, 170 + i * 25, GetColor(255, 255, 255), "%2d %-15s %6d",
	//			i + 1, playerNameW.c_str(), score);
	//	}
	//}
	//catch (const json::exception& e) {
	//	// JSON�̗�O����
	//}

	// JSON�`���̃����L���O�f�[�^
	//std::string jsonData = R"([
 //       {"PlayerID": "Player1", "Score": 100},
 //       {"PlayerID": "Player2", "Score": 95},
 //   ])";

	// �f�[�^�̃p�[�X�ƃ\�[�g
	try {
		auto rankingEntries = ranking->ParseAndSortRankingData(WStringToShiftJIS(ranking->GetData()).c_str());

		// �����L���O�f�[�^�̕`��
		for (size_t i = 0; i < rankingEntries.size() && i < 5; ++i) {
			std::wstring playerNameW(rankingEntries[i].playerName.begin(), rankingEntries[i].playerName.end());
			DrawFormatString(50, 170 + i * 25, GetColor(255, 255, 255), "%d %-15s %d",
				i + 1, playerNameW.c_str(), rankingEntries[i].score_);
		}
	}
	catch (const json::exception& e) {
	}
	// �擾���������L���O�f�[�^��\���i�����ł͒P���ɑS�f�[�^�𕶎���Ƃ��ĕ\���j
	//DrawString(0, 0, WStringToShiftJIS(ranking->GetData()).c_str(), GetColor(255, 255, 255));
}


//�I��������
void RankingDispScene::Finalize()
{
	//�ǂݍ��񂾉摜�̍폜
	DeleteGraph(background_image);

	//���I�������̉��
	ranking->Finalize();
	delete ranking;
}


//���݂̃V�[�������擾
eSceneType RankingDispScene::GetNowScene()const
{
	return eSceneType::E_RANKING_DISP;
}