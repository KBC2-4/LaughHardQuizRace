#pragma once
#include <cpprest/http_client.h> // HTTP�N���C�A���g�@�\
#include <cpprest/filestream.h> // PPLX�̔񓯊��^�X�N�֘A�̋@�\
#include <string>

class SpreadsheetClient {
public:
    SpreadsheetClient(const std::wstring& scriptId);
    // �v���C�񐔂��C���N�������g����񓯊��^�X�N
    pplx::task<void> IncrementPlayCount();
    // �v���C�񐔂��擾����񓯊��^�X�N
    pplx::task<int> GetPlayCount();
    // �X�v���b�h�V�[�g�̃f�[�^���擾����񓯊��^�X�N
    void GetSpreadsheetData();
    // �����L���O�f�[�^���擾����񓯊��^�X�N
    pplx::task<std::wstring> GetRanking();
    // �����L���O�f�[�^��POST����񓯊��^�X�N
    void PostSpreadsheetData(const std::wstring& playerId, int score);
    // �X�R�A�����10�ʂɓ����Ă��邩�ǂ����𔻒肷��񓯊��^�X�N
    pplx::task<bool> IsScoreInTop10(int score);

private:
    web::http::client::http_client client;
};

