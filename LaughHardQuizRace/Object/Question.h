#pragma once
#include <map>
#include <string>
#include <vector>

struct QuizItem {
	std::string question;			// ����
	std::string answer;				// ����
	std::vector<std::string> wrongs;// �듚
	int difficultyLevel;			// ��Փx���x���i1�`5�j
};

class Question
{
private:
	std::string filePath;	// �t�@�C���p�X
	std::vector<QuizItem> quizItems;	// ��胊�X�g

private:
	/**
	 * \brief �w�b�_�[�s����񖼂Ƃ��̃C���f�b�N�X���}�b�s���O����֐�
	 * \param headerLine
	 * \return
	 */
	std::map<std::string, int> GetColumnIndices(const std::string& headerLine);

	/**
	 * \brief ��Փx�𐔒l�ɕϊ�����֐�
	 * \param difficulty
	 * \return
	 */
	int DifficultyToNumber(const std::string& difficulty);

public:
	Question() = default;
	Question(const std::string& filePath);
	~Question();

	void SortQuestionItemsByDifficulty();

	/**
	 * \brief ����ǂݍ��ފ֐�
	 * \param filePath �t�@�C���p�X
	 * \return
	 */
	void LoadItems(const std::string& filePath);


	/**
	 * \brief ��胊�X�g���擾����֐�
	 * \return
	 */
	const std::vector<QuizItem>& GetQuizItems() const;
};

