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


//����������
void RankingData::Initialize()
{
	// �X�N���v�gID��ݒ�
	SpreadsheetClient client(L"AKfycbyCbERdoRMfYxiJLrMaQMS1grPSBHRDSPtoPvH0yrY-7QTZjRLeBYgBqA8zTUY56GtL");
	//auto task = client.GetRanking();
	//data = task.get(); // �񓯊��^�X�N�̌��ʂ��擾

	client.GetRanking().then([this](pplx::task<std::wstring> task) {
		try {
			// �^�X�N���烉���L���O�f�[�^���擾���A��O���������Ȃ������ꍇ�ɂ̓f�[�^�����[�h���ꂽ�Ƃ݂Ȃ�
			this->data = task.get();
			this->data_loaded = true;
		}
		catch (...) {
			// �G���[����: ��O�����������ꍇ�A�f�[�^���[�h�t���O��false�ɕۂ�
			this->data_loaded = false;
		}
		});

}


//�I������
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

