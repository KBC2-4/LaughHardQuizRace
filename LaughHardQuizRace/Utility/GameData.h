#pragma once


class GameData
{
private:
	static int score; // スコア

public:
    // スコアを設定するメソッド
	static void SetScore(const int score) {
	    GameData::score = score;
    }

    // スコアを取得するメソッド
	static int GetScore()
	{
        return GameData::score;
    }
};

