#pragma once
#include <map>
#include <string>
#include <vector>

struct QuizItem {
	std::string question;			// 質問
	std::string answer;				// 答え
	std::vector<std::string> wrongs;// 誤答
	int difficultyLevel;			// 難易度レベル（1～5）
};

class Question
{
private:
	std::string filePath;	// ファイルパス
	std::vector<QuizItem> quizItems;	// 問題リスト

private:
	/**
	 * \brief ヘッダー行から列名とそのインデックスをマッピングする関数
	 * \param headerLine
	 * \return
	 */
	std::map<std::string, int> GetColumnIndices(const std::string& headerLine);

	/**
	 * \brief 難易度を数値に変換する関数
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
	 * \brief 問題を読み込む関数
	 * \param filePath ファイルパス
	 * \return
	 */
	void LoadItems(const std::string& filePath);


	/**
	 * \brief 問題リストを取得する関数
	 * \return
	 */
	const std::vector<QuizItem>& GetQuizItems() const;
};

