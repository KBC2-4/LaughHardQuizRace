//#include "Question.h"
//
//#include <boost/locale.hpp>
//#include <algorithm>
//#include <codecvt>
//#include <DxLib.h>
//#include <fstream>
//#include <iostream>
//#include <sstream>
//
//
//Question::Question(const std::string& filePath)
//{
//	LoadItems(filePath);
//	SortQuestionItemsByDifficulty();
//}
//
//Question::~Question()
//{
//}
//
//// ヘッダー行から列名とそのインデックスをマッピングする関数
//std::map<std::string, int> Question::GetColumnIndices(const std::string& headerLine) {
//	std::map<std::string, int> columnIndices;
//	std::stringstream ss(headerLine);
//	std::string columnName;
//	int columnIndex = 0;
//
//	while (std::getline(ss, columnName, ',')) {
//		columnIndices[columnName] = columnIndex++;
//	}
//
//	return columnIndices;
//}
//
//// 難易度を数値に変換する関数
//int Question::DifficultyToNumber(const std::string& difficulty) {
//
//	using namespace boost::locale;
//
//	// Boost.Localeのグローバルロケールを初期化
//	generator gen;
//	std::locale::global(gen(""));
//	// UTF-8用のロケールを生成
//	std::locale loc = gen("ja_JP.UTF-8");
//
//	// Shift-JISからUTF-8への変換
//	std::string utf8Difficulty = conv::between(difficulty, "UTF-8", "Shift_JIS");
//
//	// UTF-8エンコーディングされた文字列をワイド文字列に変換
//	std::wstring wide_difficulty = conv::utf_to_utf<wchar_t>(utf8Difficulty);
//
//	// ワイド文字列内での星（★）の数をカウント
//	return std::count(wide_difficulty.begin(), wide_difficulty.end(), L'★');
//}
//
//void Question::SortQuestionItemsByDifficulty() {
//	std::sort(quizItems.begin(), quizItems.end(), [](const QuizItem& a, const QuizItem& b) {
//		return a.difficultyLevel < b.difficultyLevel;
//		});
//}
//
//
//
//void Question::LoadItems(const std::string& filePath) {
//	using namespace boost::locale;
//
//	// Boost.Localeのグローバルロケールを初期化
//	generator gen;
//	// UTF-8用のロケールを生成
//	std::locale loc = gen("ja_JP.UTF-8");
//	std::locale::global(loc);
//
//	std::ifstream file(filePath);
//	//file.imbue(std::locale(std::locale(), new std::codecvt_utf8<char>));
//	if (!file.is_open()) {
//		std::cerr << "[Error]ファイルを開けませんでした: " << filePath << std::endl;
//		printfDx("[Error]ファイルを開けませんでした: %s\n", filePath);
//		LogFileFmtAdd("[Error]ファイルを開けませんでした: %s\n", filePath);
//		return;
//	}
//
//	// ファイルからの読み込みに使用するロケールを設定
//	file.imbue(loc);
//
//	std::string line;
//
//	//! デバッグ
//	//printfDx("問題の読み込みを開始します。\n");
//	//printfDx("ファイル: %s\n", filePath.c_str());
//	//printfDx("\n");
//
//	//! デバッグ
//	//printfDx("%d", DifficultyToNumber("★★★★★"));
//
//	//while (std::getline(file, line)) {
//	//	// 使用する文字コードを utf8 に設定
//	//	//SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
//
//	// Shift-JISからUTF-8へ変換
//	//	//std::string utf8line = boost::locale::conv::to_utf<char>(line, "Shift_JIS");
//
//	//	// UTF-8からShift-JISへ変換
//	//	std::string sjisLine = boost::locale::conv::from_utf(line, "Shift_JIS");
//
//	//	//! デバッグ用 ファイル内容をそのまま出力する
//	//	//printfDx("%s\n", sjisLine.c_str());
//	//}
//
//	// ヘッダー行を読み込み、列のインデックスを取得
//	if (std::getline(file, line)) {
//
//		// UTF-8からShift-JISへ変換
//		std::string sjisLine = boost::locale::conv::from_utf(line, "Shift_JIS");
//		//! デバッグ
//		//printfDx("%s\n", sjisLine.c_str());
//		auto columnIndices = GetColumnIndices(sjisLine);
//
//		while (std::getline(file, line)) {
//
//			// UTF-8からShift-JISへ変換
//			std::string sjisLine = boost::locale::conv::from_utf(line, "Shift_JIS");
//
//			std::stringstream ss(sjisLine);
//			QuizItem item;
//			std::string cell;
//			int columnIndex = 0;
//
//			while (std::getline(ss, cell, ',')) {
//				for (const auto& colIndex : columnIndices) {
//					if (colIndex.first == "問題" && columnIndex == colIndex.second) item.question = cell;
//					else if (colIndex.first == "答え" && columnIndex == colIndex.second) item.answer = cell;
//					else if (colIndex.first == "難易度" && columnIndex == colIndex.second) item.difficultyLevel = DifficultyToNumber(cell);
//					// プレフィックスが「誤り」である場合
//					else if (colIndex.first.find("誤り") == 0 && columnIndex == colIndex.second) item.wrongs.push_back(cell);
//				}
//				columnIndex++;
//			}
//			quizItems.push_back(item);
//		}
//	}
//}
//
//const std::vector<QuizItem>& Question::GetQuizItems() const {
//	return quizItems;
//}