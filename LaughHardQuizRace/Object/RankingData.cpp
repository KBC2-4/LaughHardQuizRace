#include "RankingData.h"
#include <stdio.h>
#include "../Utility/SpreadsheetClient.h"
#include "../Utility/json.hpp"

RankingData::RankingData()
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
	auto task = client.GetRanking();
	data = task.get(); // 非同期タスクの結果を取得

}


//終了処理
void RankingData::Finalize()
{

}

const std::wstring RankingData::GetData() const
{
	return data;
}

