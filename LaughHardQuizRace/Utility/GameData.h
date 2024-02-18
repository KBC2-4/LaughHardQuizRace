#pragma once


class GameData
{
private:
	static int score; // �X�R�A
	static int prev_bgm; // �Ō�ɍĐ�����BGM�̃n���h��

public:
	// �X�R�A��ݒ肷�郁�\�b�h
	static void SetScore(const int score) {
		GameData::score = score;
	}

	// �X�R�A���擾���郁�\�b�h
	static int GetScore()
	{
		return GameData::score;
	}

	// �Ō�ɍĐ�����BGM�̃n���h����ݒ肷�郁�\�b�h
	static void SetPrevBGM(const int bgm)
	{
		GameData::prev_bgm = bgm;
	}

	// �Ō�ɍĐ�����BGM�̃n���h�����擾���郁�\�b�h
	static int GetPrevBGM()
	{
		return GameData::prev_bgm;
	}
};

