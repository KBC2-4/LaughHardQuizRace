#include "SpreadsheetClient.h"
#include <iostream>

using namespace web;
using namespace web::http;
using namespace web::http::client;

SpreadsheetClient::SpreadsheetClient(const std::wstring& scriptId)
    : client(U("https://script.google.com/macros/s/") + scriptId + U("/exec")) {}

pplx::task<void> SpreadsheetClient::IncrementPlayCount() {
    return client.request(methods::GET, U("?action=increment")).then([](http_response response) {
        std::wcout << U("�v���C�񐔃J�E���g���܂����B") << std::endl;
        });
}

pplx::task<int> SpreadsheetClient::GetPlayCount() {
    return client.request(methods::GET, U("?action=getCount")).then([](http_response response) -> int {
        if (response.status_code() == status_codes::OK) {
            auto body = response.extract_string().get(); // ������Ƃ��Ď擾
            return std::stoi(body); // int�^�ɕϊ�
        }
        return -1; // �G���[����-1��Ԃ�
        });
}

void SpreadsheetClient::GetSpreadsheetData() {
    client.request(methods::GET).then([](http_response response) {
        if (response.status_code() == status_codes::OK) {
            auto body = response.extract_string().get();
            std::wcout << body << std::endl;
        }
        }).wait();
}

pplx::task<std::wstring> SpreadsheetClient::GetRanking() {
    return client.request(methods::GET, U("?action=getTop10Ranking")).then([](http_response response) -> std::wstring {
        if (response.status_code() == status_codes::OK) {
            // JSON�`���̃����L���O�f�[�^�𕶎���Ŏ擾
            return response.extract_string().get();
        }
        return L"Error";
        });
}


void SpreadsheetClient::PostSpreadsheetData(const std::wstring& playerId, int score) {
    json::value postData;
    postData[U("playerId")] = json::value::string(playerId);
    postData[U("score")] = json::value::number(score);

    client.request(methods::POST, U(""), postData.serialize(), U("application/json")).then([](http_response response) {
        if (response.status_code() == status_codes::OK) {
            std::wcout << U("�f�[�^�͐����POST����܂����B") << std::endl;
        }
        }).wait();
}
