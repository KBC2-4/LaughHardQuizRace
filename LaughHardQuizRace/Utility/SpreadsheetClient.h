#pragma once
#include <cpprest/http_client.h> // HTTPクライアント機能
#include <cpprest/filestream.h> // PPLXの非同期タスク関連の機能
#include <string>

class SpreadsheetClient {
public:
    SpreadsheetClient(const std::wstring& scriptId);
    // プレイ回数をインクリメントする非同期タスク
    pplx::task<void> IncrementPlayCount();
    // プレイ回数を取得する非同期タスク
    pplx::task<int> GetPlayCount();
    // スプレッドシートのデータを取得する非同期タスク
    void GetSpreadsheetData();
    // ランキングデータを取得する非同期タスク
    pplx::task<std::wstring> GetRanking();
    // ランキングデータをPOSTする非同期タスク
    void PostSpreadsheetData(const std::wstring& playerId, int score);
    // スコアが上位10位に入っているかどうかを判定する非同期タスク
    pplx::task<bool> IsScoreInTop10(int score);

private:
    web::http::client::http_client client;
};

