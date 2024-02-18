#include "DxLib.h"
#include "RankingInputScene.h"
#include "../Utility/InputControl.h"
#include "../Utility/GameData.h"
#include "../Utility/SpreadsheetClient.h"
#include "../Utility/Guide.h"

RankingInputScene::RankingInputScene() :backgrouond_image(NULL),
ranking(nullptr), score(0), name_num(0), cursor_x(0), cursor_y(0)
{
	memset(name, NULL, (sizeof(char) * 15));
}


RankingInputScene::~RankingInputScene()
{

}


//初期化処理
void RankingInputScene::Initialize()
{
	//画像の読み込み
	backgrouond_image = LoadGraph("Resource/images/Scene/Ranking/background.png");

	//エラーチェック
	if (backgrouond_image == -1)
	{
		throw("Resource/images/Scene/Ranking/background.pngがありません\n");
	}

	char_font_handle = CreateFontToHandle("Segoe UI", 50, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	font_handle_h2 = CreateFontToHandle("Segoe UI", 80, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, 5);
	font_handle_h3 = CreateFontToHandle("Segoe UI", 20, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);
	font_handle_h1 = CreateFontToHandle("Segoe UI", 100, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);
	buttonGuidFont = CreateFontToHandle("メイリオ", 23, 1, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);


	// ランキング入力画面よりも前に同じBGMは使用していないため、初期化
	GameData::SetPrevBGM(-1);

	//BGMの読み込み
	// 前回の再生途中のBGMそれを再生
	if (GameData::GetPrevBGM() == -1)
	{
		background_sound = LoadSoundMem("Resource/sounds/bgm/ranking.mp3");
		if (background_sound == -1)
		{
			throw("Resource/sounds/bgm/ranking.mp3がありません\n");
		}
	}
	else
	{
		background_sound = GameData::GetPrevBGM();
	}

	enter_se = LoadSoundMem("Resource/sounds/se/enter.mp3");
	cursor_move_se = LoadSoundMem("Resource/sounds/se/cursor_move.mp3");

	if (enter_se == -1)
	{
		throw("Resource/sounds/se/enter.mp3がありません\n");
	}

	if (cursor_move_se == -1)
	{
		throw("Resource/sounds/se/cursor_move.mp3がありません\n");
	}

	//メモリの動的確保
	ranking = new RankingData;
	ranking->Initialize();

	// スコアの取得
	this->score = GameData::GetScore();

	//BGMの再生
	PlaySoundMem(background_sound, DX_PLAYTYPE_LOOP, FALSE);
}


//更新処理
eSceneType RankingInputScene::Update()
{
	bool is_change = false;

	//名前入力処理
	is_change = InputName();

	//シーン変更は可能か?
	if (is_change)
	{
		//// SE再生
		//PlaySoundMem(enter_se, DX_PLAYTYPE_BACK, TRUE);
		//SEが鳴り終わってから画面推移する。
		while (CheckSoundMem(enter_se)) {}
		// 前回のBGMのハンドルを格納
		GameData::SetPrevBGM(background_sound);

		//ランキング表示に遷移
		return eSceneType::E_RANKING_DISP;
	}
	else
	{
		return GetNowScene();
	}
}


//描画処理
void RankingInputScene::Draw()const
{
	//背景画像の描画
	DrawGraph(0, 0, backgrouond_image, TRUE);

	//名前入力指示文字列の描画
	DrawStringToHandle(GetDrawCenterX("ランキングに登録します", font_handle_h2), 50, "ランキングに登録します",
		0xF0BD01, font_handle_h2, 0xffffff);
	DrawStringToHandle(GetDrawCenterX("※上位10位以内に入らない場合でもスプレッドシートに記録されます。",font_handle_h3),
		150, "※上位10位以内に入らない場合でもスプレッドシートに記録されます。", 0xffffff, font_handle_h3, 0xED7332);
	
	std::string result = ">" + std::string(name) + "　";

	DrawFormatString2ToHandle(GetDrawCenterX(result.c_str(), font_handle_h2), 190, 0x000000, 0xffffff, font_handle_h2, ">%s", name);

	//選択用文字を描画
	const int font_size = 55;
	for (int i = 0; i < 26; i++)
	{
		int x = (i % 13) * font_size + 305;
		int y = (i / 13) * font_size + 300;
		DrawFormatString2ToHandle(x, y, 0x329FED, 0xffffff, char_font_handle, "%-3c", 'a' + i);
		y = ((i / 13) + 2) * font_size + 300;
		DrawFormatString2ToHandle(x, y, 0x329FED, 0xffffff, char_font_handle, "%-3c", 'A' + i);
	}

	constexpr int enter_x = 580;
	constexpr int enter_y = 580;
	DrawStringToHandle(enter_x, enter_y, "決定", 0xffffff, char_font_handle, 0x000000);

	//選択文字をフォーカスする
	if (cursor_y < 4)
	{
		int x = cursor_x * font_size + 295;
		int y = cursor_y * font_size + 295;

		DrawBox(x, y, x + font_size, y + font_size, 0x0000ff, FALSE);
	}
	else
	{
		if (cursor_x == 0)
		{
			DrawBox(enter_x - 5, enter_y - 5, enter_x - 5 + font_size * 2, enter_y - 5 + font_size, 0x0000ff, FALSE);
		}
	}

	const std::vector<guideElement> gamepad_guides = {
guideElement({"L"}, "選択", GUIDE_SHAPE_TYPE::JOYSTICK, buttonGuidFont, 0x000000,0xFFFFFF, 0xFFFFFF),
guideElement({"A"}, cursor_y == 4 ? "ランキング表示画面へ" : "入力", GUIDE_SHAPE_TYPE::FIXED_CIRCLE,buttonGuidFont, 0xFFFFFF, A_BUTTON_COLOR,0xEB3229, 0xFFFFFF),
guideElement({"B"}, "1文字削除", GUIDE_SHAPE_TYPE::FIXED_CIRCLE,buttonGuidFont, 0xFFFFFF, 0x00F080,0xEB3229, 0xFFFFFF),
	};

	// ボタンガイドの描画
	DrawGuides(gamepad_guides, 505.0f, 660.0f, 5.0f, 60.0f);
}


//終了時処理
void RankingInputScene::Finalize()
{
	//シーンの切り替えが行われたらBGMを止める
	StopSoundMem(background_sound);

	// 格納されたBGMと異なる場合は削除
	if (GameData::GetPrevBGM() != background_sound)
	{
		DeleteSoundMem(background_sound);
	}

	DeleteSoundMem(enter_se);
	DeleteSoundMem(cursor_move_se);

	// スクリプトIDを設定
	SpreadsheetClient client(L"AKfycbyCbERdoRMfYxiJLrMaQMS1grPSBHRDSPtoPvH0yrY-7QTZjRLeBYgBqA8zTUY56GtL");

	// char配列からstd::wstringに変換
	std::wstring wName = CharArrayToWString(name);

	// スプレッドシートにデータを書き込み
	client.PostSpreadsheetData(wName, score);

	//読み込んだ画像を削除
	DeleteGraph(backgrouond_image);

	// フォントの削除
	DeleteFontToHandle(char_font_handle);
	DeleteFontToHandle(font_handle_h2);
	DeleteFontToHandle(font_handle_h3);
	DeleteFontToHandle(font_handle_h1);
	DeleteFontToHandle(buttonGuidFont);

	//動的メモリの解放
	delete ranking;
}


//現在のシーン情報を取得
eSceneType RankingInputScene::GetNowScene()const
{
	return eSceneType::E_RANKING_INPUT;
}

// char配列をstd::wstringに変換する
std::wstring RankingInputScene::CharArrayToWString(const char* charArray) {
	size_t length = std::strlen(charArray);
	std::wstring wstr(length, L'\0'); // 初期化
	size_t convertedChars = 0; // 変換された文字数

	// マルチバイトをワイド文字に変換
	errno_t err = mbstowcs_s(&convertedChars, &wstr[0], length + 1, charArray, _TRUNCATE);

	if (err != 0) {
		// エラー処理
		throw std::runtime_error("char 配列から wstring への変換に失敗しました。");
	}

	// 変換された文字数を基にwstringを縮小調整
	wstr.resize(convertedChars - 1); // 終端のヌル文字を除く実際の文字数に調整

	return wstr;
}


//名前の入力処理
bool RankingInputScene::InputName()
{
	// 左スティックの倒された方向を取得
	const StickDirection direction = InputControl::GetLStickDirection(20000, 10);

	//カーソル操作処理
	if (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_LEFT) || direction == StickDirection::Left)
	{
		// SE再生
		PlaySoundMem(cursor_move_se, DX_PLAYTYPE_BACK, TRUE);

		if (cursor_y == 4) {
			// 何もしない
		}
		else if (cursor_x > 0) {
			cursor_x--;
		}
		else {
			cursor_x = 12;
		}
	}
	if (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_RIGHT) || direction == StickDirection::Right)
	{
		// SE再生
		PlaySoundMem(cursor_move_se, DX_PLAYTYPE_BACK, TRUE);

		if (cursor_y == 4 && cursor_x == 0) {
			// 何もしない
		}
		else if (cursor_y == 4) {
			cursor_x = 0; // 「決定」にフォーカスを戻す
		}
		else {
			// 通常の文字選択時の処理
			if (cursor_x < 12) {
				cursor_x++;
			}
			else {
				cursor_x = 0;
			}
		}
	}
	if (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_UP) || direction == StickDirection::Up)
	{
		// SE再生
		PlaySoundMem(cursor_move_se, DX_PLAYTYPE_BACK, TRUE);

		if (cursor_y > 0)
		{
			cursor_y--;
		}
		else
		{
			cursor_y = 4;
		}
	}
	if (InputControl::GetButtonDown(XINPUT_BUTTON_DPAD_DOWN) || direction == StickDirection::Down)
	{
		// SE再生
		PlaySoundMem(cursor_move_se, DX_PLAYTYPE_BACK, TRUE);

		if (cursor_y < 4)
		{
			cursor_y++;
		}
		else
		{
			cursor_y = 0;
		}
	}

	//カーソル位置の文字を決定する
	if (InputControl::GetButtonDown(XINPUT_BUTTON_A))
	{
		// SE再生
		PlaySoundMem(enter_se, DX_PLAYTYPE_BACK, TRUE);

		// 名前の長さが6文字未満の場合のみ、文字を追加
		if (name_num < 6) {
			if (cursor_y < 2)
			{
				name[name_num++] = 'a' + cursor_x + (cursor_y * 13);
				if (name_num == 14)
				{
					cursor_x = 0;
					cursor_y = 4;
				}
			}
			else if (cursor_y < 4)
			{
				name[name_num++] = 'A' + cursor_x + ((cursor_y - 2) * 13);
				if (name_num == 14)
				{
					cursor_x = 0;
					cursor_y = 4;
				}
			}
		}

		if (cursor_x == 0)
		{
			name[name_num] = '\0';
			return true;
		}
		//else
		//{
		//	name[--name_num] = NULL;
		//}

	}

	// Bボタンが押された場合、名前の最後の文字を消す
	if (InputControl::GetButtonDown(XINPUT_BUTTON_B))
	{
		if (name_num > 0) // 名前が空でないことを確認
		{
			name_num--;          // 名前の長さを1減らす
			name[name_num] = '\0'; // 最後の文字を消去
		}
	}

	// cursor_y が 4 のとき、cursor_x は 0 に固定する
	if (cursor_y == 4) cursor_x = 0;
	//clsDx();
	//printfDx("Cursor:x %d  y %d\n", cursor_x, cursor_y);

	return false;
}
