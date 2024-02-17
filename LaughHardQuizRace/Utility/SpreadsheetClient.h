#pragma once
#include <cpprest/http_client.h> // HTTP�N���C�A���g�@�\
#include <cpprest/filestream.h> // PPLX�̔񓯊��^�X�N�֘A�̋@�\
#include <string>

class SpreadsheetClient {
public:
    SpreadsheetClient(const std::wstring& scriptId);
    pplx::task<void> IncrementPlayCount();
    pplx::task<int> GetPlayCount();
    void GetSpreadsheetData();
    pplx::task<std::wstring> GetRanking();
    void PostSpreadsheetData(const std::wstring& playerId, int score);

private:
    web::http::client::http_client client;
};

