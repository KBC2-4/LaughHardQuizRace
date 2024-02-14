#pragma once
#include <string>
#include <vector>

class RankingData
{
private:
	int score[6];       //�X�R�A�f�[�^
	int rank[6];        //�����N�f�[�^
	char name[6][15];   //���O�f�[�^
	std::wstring data;




public:
	RankingData();
	RankingData(const std::string& name, int sc);
	~RankingData();

	void Initialize();   //����������
	void Finalize();    //�I������

public:
	std::vector<RankingData> ParseAndSortRankingData(const std::string& jsonData);
	//�����L���O�f�[�^�̐ݒ�
	void SetRankingData(int score, const char* name);
	//�X�R�A�擾����
	int GetScore(int value)const;
	//�����N�擾����
	int GetRank(int value)const;
	//���O�擾����
	const char* GetName(int value)const;
	const std::wstring GetData()const;

	std::string playerName;
	int score_;

private:
	//�f�[�^����ւ�����
	void SortData();
};

