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


//����������
void RankingData::Initialize()
{
	// �X�N���v�gID��ݒ�
	SpreadsheetClient client(L"AKfycbyCbERdoRMfYxiJLrMaQMS1grPSBHRDSPtoPvH0yrY-7QTZjRLeBYgBqA8zTUY56GtL");
	auto task = client.GetRanking();
	data = task.get(); // �񓯊��^�X�N�̌��ʂ��擾

}


//�I������
void RankingData::Finalize()
{

}

const std::wstring RankingData::GetData() const
{
	return data;
}

