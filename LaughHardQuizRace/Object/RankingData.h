#pragma once
#include <string>
#include <vector>

class RankingData
{
private:
	std::wstring data;	// ランキングデータ
	bool data_loaded;	// データが取得されたかどうかのフラグ


public:
	RankingData();
	~RankingData();

	void Initialize();   //初期化処理
	void Finalize();    //終了処理

	const std::wstring GetData()const; //データを取得する
};

