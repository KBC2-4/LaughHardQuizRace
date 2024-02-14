#pragma once
#include <cpprest/http_client.h> // HTTPクライアント機能
#include <cpprest/filestream.h> // PPLXの非同期タスク関連の機能
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

