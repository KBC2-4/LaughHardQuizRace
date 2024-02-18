#include "RankingData.h"
#include <stdio.h>
#include "../Utility/SpreadsheetClient.h"
#include "../Utility/json.hpp"

RankingData::RankingData() : data_loaded(false), data(L"")
{

}


RankingData::~RankingData()
{

}


//初期化処理
void RankingData::Initialize()
{
	// スクリプトIDを設定
	SpreadsheetClient client(L"AKfycbyCbERdoRMfYxiJLrMaQMS1grPSBHRDSPtoPvH0yrY-7QTZjRLeBYgBqA8zTUY56GtL");
	//auto task = client.GetRanking();
	//data = task.get(); // 非同期タスクの結果を取得

	client.GetRanking().then([this](pplx::task<std::wstring> task) {
		try {
			// タスクからランキングデータを取得し、例外が発生しなかった場合にはデータがロードされたとみなす
			this->data = task.get();
			this->data_loaded = true;
		}
		catch (...) {
			// エラー処理: 例外が発生した場合、データロードフラグをfalseに保つ
			this->data_loaded = false;
		}
		});

}


//終了処理
void RankingData::Finalize()
{

}

const std::wstring RankingData::GetData() const
{
	if (!data_loaded)
	{
		return L"";
	}
	return data;
}

