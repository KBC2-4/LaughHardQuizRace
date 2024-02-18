#pragma once


class GameData
{
private:
	static int score; // スコア
	static int prev_bgm; // 最後に再生したBGMのハンドル

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

	// 最後に再生したBGMのハンドルを設定するメソッド
	static void SetPrevBGM(const int bgm)
	{
		GameData::prev_bgm = bgm;
	}

	// 最後に再生したBGMのハンドルを取得するメソッド
	static int GetPrevBGM()
	{
		return GameData::prev_bgm;
	}
};

