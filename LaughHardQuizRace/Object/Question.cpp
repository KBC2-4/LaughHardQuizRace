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
//// �w�b�_�[�s����񖼂Ƃ��̃C���f�b�N�X���}�b�s���O����֐�
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
//// ��Փx�𐔒l�ɕϊ�����֐�
//int Question::DifficultyToNumber(const std::string& difficulty) {
//
//	using namespace boost::locale;
//
//	// Boost.Locale�̃O���[�o�����P�[����������
//	generator gen;
//	std::locale::global(gen(""));
//	// UTF-8�p�̃��P�[���𐶐�
//	std::locale loc = gen("ja_JP.UTF-8");
//
//	// Shift-JIS����UTF-8�ւ̕ϊ�
//	std::string utf8Difficulty = conv::between(difficulty, "UTF-8", "Shift_JIS");
//
//	// UTF-8�G���R�[�f�B���O���ꂽ����������C�h������ɕϊ�
//	std::wstring wide_difficulty = conv::utf_to_utf<wchar_t>(utf8Difficulty);
//
//	// ���C�h��������ł̐��i���j�̐����J�E���g
//	return std::count(wide_difficulty.begin(), wide_difficulty.end(), L'��');
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
//	// Boost.Locale�̃O���[�o�����P�[����������
//	generator gen;
//	// UTF-8�p�̃��P�[���𐶐�
//	std::locale loc = gen("ja_JP.UTF-8");
//	std::locale::global(loc);
//
//	std::ifstream file(filePath);
//	//file.imbue(std::locale(std::locale(), new std::codecvt_utf8<char>));
//	if (!file.is_open()) {
//		std::cerr << "[Error]�t�@�C�����J���܂���ł���: " << filePath << std::endl;
//		printfDx("[Error]�t�@�C�����J���܂���ł���: %s\n", filePath);
//		LogFileFmtAdd("[Error]�t�@�C�����J���܂���ł���: %s\n", filePath);
//		return;
//	}
//
//	// �t�@�C������̓ǂݍ��݂Ɏg�p���郍�P�[����ݒ�
//	file.imbue(loc);
//
//	std::string line;
//
//	//! �f�o�b�O
//	//printfDx("���̓ǂݍ��݂��J�n���܂��B\n");
//	//printfDx("�t�@�C��: %s\n", filePath.c_str());
//	//printfDx("\n");
//
//	//! �f�o�b�O
//	//printfDx("%d", DifficultyToNumber("����������"));
//
//	//while (std::getline(file, line)) {
//	//	// �g�p���镶���R�[�h�� utf8 �ɐݒ�
//	//	//SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
//
//	// Shift-JIS����UTF-8�֕ϊ�
//	//	//std::string utf8line = boost::locale::conv::to_utf<char>(line, "Shift_JIS");
//
//	//	// UTF-8����Shift-JIS�֕ϊ�
//	//	std::string sjisLine = boost::locale::conv::from_utf(line, "Shift_JIS");
//
//	//	//! �f�o�b�O�p �t�@�C�����e�����̂܂܏o�͂���
//	//	//printfDx("%s\n", sjisLine.c_str());
//	//}
//
//	// �w�b�_�[�s��ǂݍ��݁A��̃C���f�b�N�X���擾
//	if (std::getline(file, line)) {
//
//		// UTF-8����Shift-JIS�֕ϊ�
//		std::string sjisLine = boost::locale::conv::from_utf(line, "Shift_JIS");
//		//! �f�o�b�O
//		//printfDx("%s\n", sjisLine.c_str());
//		auto columnIndices = GetColumnIndices(sjisLine);
//
//		while (std::getline(file, line)) {
//
//			// UTF-8����Shift-JIS�֕ϊ�
//			std::string sjisLine = boost::locale::conv::from_utf(line, "Shift_JIS");
//
//			std::stringstream ss(sjisLine);
//			QuizItem item;
//			std::string cell;
//			int columnIndex = 0;
//
//			while (std::getline(ss, cell, ',')) {
//				for (const auto& colIndex : columnIndices) {
//					if (colIndex.first == "���" && columnIndex == colIndex.second) item.question = cell;
//					else if (colIndex.first == "����" && columnIndex == colIndex.second) item.answer = cell;
//					else if (colIndex.first == "��Փx" && columnIndex == colIndex.second) item.difficultyLevel = DifficultyToNumber(cell);
//					// �v���t�B�b�N�X���u���v�ł���ꍇ
//					else if (colIndex.first.find("���") == 0 && columnIndex == colIndex.second) item.wrongs.push_back(cell);
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