#pragma once
#include <string>
#include <vector>

class RankingData
{
private:
	std::wstring data;	// �����L���O�f�[�^
	bool data_loaded;	// �f�[�^���擾���ꂽ���ǂ����̃t���O


public:
	RankingData();
	~RankingData();

	void Initialize();   //����������
	void Finalize();    //�I������

	const std::wstring GetData()const; //�f�[�^���擾����
};

