#pragma once


class GameData
{
private:
	static int score; // �X�R�A

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
};

