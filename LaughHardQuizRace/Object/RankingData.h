#pragma once
#include <string>
#include <vector>

class RankingData
{
private:
	std::wstring data;


public:
	RankingData();
	~RankingData();

	void Initialize();   //����������
	void Finalize();    //�I������

	const std::wstring GetData()const; //�f�[�^���擾����
};

