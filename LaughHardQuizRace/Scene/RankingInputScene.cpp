#include "DxLib.h"
#include "RankingInputScene.h"
#include "../Utility/InputControl.h"
#include "../Utility/GameData.h"
#include "../Utility/SpreadsheetClient.h"
#include "../Utility/Guide.h"

RankingInputScene::RankingInputScene() :backgrouond_image(NULL),
ranking(nullptr), score(0), name_num(0), cursor_x(0), cursor_y(0)
{
	memset(name, NULL, (sizeof(char) * 15));
}


RankingInputScene::~RankingInputScene()
{

}


//����������
void RankingInputScene::Initialize()
{
	//�摜�̓ǂݍ���
	backgrouond_image = LoadGraph("Resource/images/Scene/Ranking/background.png");

	//�G���[�`�F�b�N
	if (backgrouond_image == -1)
	{
		throw("Resource/images/Scene/Ranking/background.png������܂���\n");
	}

	char_font_handle = CreateFontToHandle("Segoe UI", 50, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	font_handle_h2 = CreateFontToHandle("Segoe UI", 80, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, 5);
	font_handle_h3 = CreateFontToHandle("Segoe UI", 20, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);
	font_handle_h1 = CreateFontToHandle("Segoe UI", 100, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);
	buttonGuidFont = CreateFontToHandle("���C���I", 23, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);


	// �����L���O���͉�ʂ����O�ɓ���BGM�͎g�p���Ă��Ȃ����߁A������
	GameData::SetPrevBGM(-1);

	//BGM�̓ǂݍ���
	// �O��̍Đ��r����BGM������Đ�
	if (GameData::GetPrevBGM() == -1)
	{
		background_sound = LoadSoundMem("Resource/sounds/bgm/ranking.mp3");
		if (background_sound == -1)
		{
			throw("Resource/sounds/bgm/ranking.mp3������܂���\n");
		}
	}
	else
	{
		background_sound = GameData::GetPrevBGM();
	}

	enter_se = LoadSoundMem("Resource/sounds/se/enter.mp3");
	cursor_move_se = LoadSoundMem("Resource/sounds/se/cursor_move.mp3");

	if (enter_se == -1)
	{
		throw("Resource/sounds/se/enter.mp3������܂���\n");
	}

	if (cursor_move_se == -1)
	{
		throw("Resource/sounds/se/cursor_move.mp3������܂���\n");
	}

	//�������̓��I�m��
	ranking = new RankingData;
	ranking->Initialize();

	// �X�R�A�̎擾
	this->score = GameData::GetScore();

	//BGM�̍Đ�
	PlaySoundMem(background_sound, DX_PLAYTYPE_LOOP, FALSE);
}


//�X�V����
eSceneType RankingInputScene::Update()
{
	bool is_change = false;

	//���O���͏���
	is_change = InputName();

	//�V�[���ύX�͉\��?
	if (is_change)
	{
		//// SE�Đ�
		//PlaySoundMem(enter_se, DX_PLAYTYPE_BACK, TRUE);
		//SE����I����Ă����ʐ��ڂ���B
		while (CheckSoundMem(enter_se)) {}
		// �O���BGM�̃n���h�����i�[
		GameData::SetPrevBGM(background_sound);

		//�����L���O�\���ɑJ��
		return eSceneType::E_RANKING_DISP;
	}
	else
	{
		return GetNowScene();
	}
}


//�`�揈��
void RankingInputScene::Draw()const
{
	//�w�i�摜�̕`��
	DrawGraph(0, 0, backgrouond_image, TRUE);

	//���O���͎w��������̕`��
	DrawStringToHandle(GetDrawCenterX("�����L���O�ɓo�^���܂�", font_handle_h2), 50, "�����L���O�ɓo�^���܂�",
		0xF0BD01, font_handle_h2, 0xffffff);
	DrawStringToHandle(GetDrawCenterX("�����10�ʈȓ��ɓ���Ȃ��ꍇ�ł��X�v���b�h�V�[�g�ɋL�^����܂��B",font_handle_h3),
		150, "�����10�ʈȓ��ɓ���Ȃ��ꍇ�ł��X�v���b�h�V�[�g�ɋL�^����܂��B", 0xffffff, font_handle_h3, 0xED7332);
	
	std::string result = ">" + std::string(name) + "�@";

	DrawFormatString2ToHandle(GetDrawCenterX(result.c_str(), font_handle_h2), 190, 0x000000, 0xffffff, font_handle_h2, ">%s", name);

	//�I��p������`��
	const int font_size = 55;
	for (int i = 0; i < 26; i++)
	{
		int x = (i % 13) * font_size + 305;
		int y = (i / 13) * font_size + 300;
		DrawFormatString2ToHandle(x, y, 0x329FED, 0xffffff, char_font_handle, "%-3c", 'a' + i);
		y = ((i / 13) + 2) * font_size + 300;
		DrawFormatString2ToHandle(x, y, 0x329FED, 0xffffff, char_font_handle, "%-3c", 'A' + i);
	}

	constexpr int enter_x = 580;
	constexpr int enter_y = 580;
	DrawStringToHandle(enter_x, enter_y, "����", 0xffffff, char_font_handle, 0x000000);

	//�I�𕶎����t�H�[�J�X����
	if (cursor_y < 4)
	{
		int x = cursor_x * font_size + 295;
		int y = cursor_y * font_size + 295;

		DrawBox(x, y, x + font_size, y + font_size, 0x0000ff, FALSE);
	}
	else
	{
		if (cursor_x == 0)
		{
			DrawBox(enter_x - 5, enter_y - 5, enter_x - 5 + font_size * 2, enter_y - 5 + font_size, 0x0000ff, FALSE);
		}
	}

	const std::vector<guideElement> gamepad_guides = {
guideElement({"L"}, "�I��", GUIDE_SHAPE_TYPE::JOYSTICK, buttonGuidFont, 0x000000,0xFFFFFF, 0xFFFFFF),
guideElement({"A"}, cursor_y == 4 ? "�����L���O�\����ʂ�" : "����", GUIDE_SHAPE_TYPE::FIXED_CIRCLE,buttonGuidFont, 0xFFFFFF, A_BUTTON_COLOR,0xEB3229, 0xFFFFFF),
guideElement({"B"}, "1�����폜", GUIDE_SHAPE_TYPE::FIXED_CIRCLE,buttonGuidFont, 0xFFFFFF, 0x00F080,0xEB3229, 0xFFFFFF),
	};

	// �{�^���K�C�h�̕`��
	DrawGuides(gamepad_guides, 505.0f, 660.0f, 5.0f, 60.0f);
}


//�I��������
void RankingInputScene::Finalize()
{
	//�V�[���̐؂�ւ����s��ꂽ��BGM���~�߂�
	StopSoundMem(background_sound);

	// �i�[���ꂽBGM�ƈقȂ�ꍇ�͍폜
	if (GameData::GetPrevBGM() != background_sound)
	{
		DeleteSoundMem(background_sound);
	}

	DeleteSoundMem(enter_se);
	DeleteSoundMem(cursor_move_se);

	// �X�N���v�gID��ݒ�
	SpreadsheetClient client(L"AKfycbyCbERdoRMfYxiJLrMaQMS1grPSBHRDSPtoPvH0yrY-7QTZjRLeBYgBqA8zTUY56GtL");

	// char�z�񂩂�std::wstring�ɕϊ�
	std::wstring wName = CharArrayToWString(name);

	// �X�v���b�h�V�[�g�Ƀf�[�^����������
	client.PostSpreadsheetData(wName, score);

	//�ǂݍ��񂾉摜���폜
	DeleteGraph(backgrouond_image);

	// �t�H���g�̍폜
	DeleteFontToHandle(char_font_handle);
	DeleteFontToHandle(font_handle_h2);
	DeleteFontToHandle(font_handle_h3);
	DeleteFontToHandle(font_handle_h1);
	DeleteFontToHandle(buttonGuidFont);

	//���I�������̉��
	delete ranking;
}


//���݂̃V�[�������擾
eSceneType RankingInputScene::GetNowScene()const
{
	return eSceneType::E_RANKING_INPUT;
}

// char�z���std::wstring�ɕϊ�����
std::wstring RankingInputScene::CharArrayToWString(const char* charArray) {
	size_t length = std::strlen(charArray);
	std::wstring wstr(length, L'\0'); // ������
	size_t convertedChars = 0; // �ϊ����ꂽ������

	// �}���`�o�C�g�����C�h�����ɕϊ�
	errno_t err = mbstowcs_s(&convertedChars, &wstr[0], length + 1, charArray, _TRUNCATE);

	if (err != 0) {
		// �G���[����
		throw std::runtime_error("char �z�񂩂� wstring �ւ̕ϊ��Ɏ��s���܂����B");
	}

	// �ϊ����ꂽ�����������wstring���k������
	wstr.resize(convertedChars - 1); // �I�[�̃k���������������ۂ̕������ɒ���

	return wstr;
}


//���O�̓��͏���
bool RankingInputScene::InputName()
{
	// ���X�e�B�b�N�̓|���ꂽ�������擾
	const StickDirection direction = InputControl::GetLStickDirection(20000, 10);

	//�J�[�\�����쏈��
	if (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_LEFT) || direction == StickDirection::Left)
	{
		// SE�Đ�
		PlaySoundMem(cursor_move_se, DX_PLAYTYPE_BACK, TRUE);

		if (cursor_y == 4) {
			// �������Ȃ�
		}
		else if (cursor_x > 0) {
			cursor_x--;
		}
		else {
			cursor_x = 12;
		}
	}
	if (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_RIGHT) || direction == StickDirection::Right)
	{
		// SE�Đ�
		PlaySoundMem(cursor_move_se, DX_PLAYTYPE_BACK, TRUE);

		if (cursor_y == 4 && cursor_x == 0) {
			// �������Ȃ�
		}
		else if (cursor_y == 4) {
			cursor_x = 0; // �u����v�Ƀt�H�[�J�X��߂�
		}
		else {
			// �ʏ�̕����I�����̏���
			if (cursor_x < 12) {
				cursor_x++;
			}
			else {
				cursor_x = 0;
			}
		}
	}
	if (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_UP) || direction == StickDirection::Up)
	{
		// SE�Đ�
		PlaySoundMem(cursor_move_se, DX_PLAYTYPE_BACK, TRUE);

		if (cursor_y > 0)
		{
			cursor_y--;
		}
		else
		{
			cursor_y = 4;
		}
	}
	if (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_DOWN) || direction == StickDirection::Down)
	{
		// SE�Đ�
		PlaySoundMem(cursor_move_se, DX_PLAYTYPE_BACK, TRUE);

		if (cursor_y < 4)
		{
			cursor_y++;
		}
		else
		{
			cursor_y = 0;
		}
	}

	//�J�[�\���ʒu�̕��������肷��
	if (InputControl::GetButtonDown(XINPUT_BUTTON_A))
	{
		// SE�Đ�
		PlaySoundMem(enter_se, DX_PLAYTYPE_BACK, TRUE);

		// ���O�̒�����6���������̏ꍇ�̂݁A������ǉ�
		if (name_num < 6) {
			if (cursor_y < 2)
			{
				name[name_num++] = 'a' + cursor_x + (cursor_y * 13);
				if (name_num == 14)
				{
					cursor_x = 0;
					cursor_y = 4;
				}
			}
			else if (cursor_y < 4)
			{
				name[name_num++] = 'A' + cursor_x + ((cursor_y - 2) * 13);
				if (name_num == 14)
				{
					cursor_x = 0;
					cursor_y = 4;
				}
			}
		}

		if (cursor_x == 0)
		{
			name[name_num] = '\0';
			return true;
		}
		//else
		//{
		//	name[--name_num] = NULL;
		//}

	}

	// B�{�^���������ꂽ�ꍇ�A���O�̍Ō�̕���������
	if (InputControl::GetButtonDown(XINPUT_BUTTON_B))
	{
		if (name_num > 0) // ���O����łȂ����Ƃ��m�F
		{
			name_num--;          // ���O�̒�����1���炷
			name[name_num] = '\0'; // �Ō�̕���������
		}
	}

	// cursor_y �� 4 �̂Ƃ��Acursor_x �� 0 �ɌŒ肷��
	if (cursor_y == 4) cursor_x = 0;
	//clsDx();
	//printfDx("Cursor:x %d  y %d\n", cursor_x, cursor_y);

	return false;
}
