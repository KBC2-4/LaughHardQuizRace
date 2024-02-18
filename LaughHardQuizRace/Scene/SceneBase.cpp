#include "sceneBase.h"
#include <DxLib.h>
#include "../Utility/Guide.h"
#include <string>
#include <sstream>


int SceneBase::GetDrawCenterX(const char* str, int font_handle, int margin) const {
	constexpr int screenX = 1280;
	std::string string(str);
	std::istringstream stream(string);
	std::string line;
	std::vector<std::string> lines;
	int maxWidth = 0;

	// 改行で分割し、各行の前後の空白を削除
	while (std::getline(stream, line)) {
		line.erase(0, line.find_first_not_of(" \t\n\r\f\v")); // 左側の空白を削除
		line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1); // 右側の空白を削除
		lines.push_back(line);

		// この行の幅を取得し、最大幅を更新
		int lineWidth = GetDrawFormatStringWidthToHandle(font_handle, line.c_str());
		maxWidth = max(maxWidth, lineWidth);
	}

	// 最も長い行を基準に中央揃えのX座標を計算
	if (margin >= screenX || margin <= -screenX) {
		margin = 0;
	}

	if (font_handle == 0) {
		font_handle = DX_DEFAULT_FONT_HANDLE;
	}

	const int centerX = screenX / 2 + margin - (maxWidth / 2);
	return centerX;
}

void SceneBase::DrawGuides(const std::vector<guideElement>& guides, float x, float y, float padding, float elementSpacing) const
{

	float currentX = x;
	float centerY = y + 15;

	for (const auto& guide : guides)
	{
		int font = guide.font;
		if (font == NULL) {
			font = DX_DEFAULT_FONT_HANDLE;
		}

		DrawGuideElement(guide, currentX, y);

		int textWidth, textHeight;
		GetDrawStringSizeToHandle(&textWidth, &textHeight, nullptr, guide.description.c_str(), guide.description.length(), font);
		float stringY = centerY - (textHeight / 2) + guide.offsetStringY;
		DrawStringToHandle(currentX, stringY, guide.description.c_str(), guide.color, font, guide.edgeColor);
		currentX += textWidth + elementSpacing;
	}
}

void SceneBase::DrawGuideElement(const guideElement& guide, float& currentX, float y) const
{

	int font = guide.font;
	if (font == NULL) {
		font = DX_DEFAULT_FONT_HANDLE;
	}

	for (size_t i = 0; i < guide.buttons.size(); ++i)
	{
		const auto& button = guide.buttons[i];
		int textWidth, textHeight;
		GetDrawStringSizeToHandle(&textWidth, &textHeight, nullptr, button.c_str(), button.length(), font);

		float padding = 2.f;

		if (guide.shapeType == GUIDE_SHAPE_TYPE::DYNAMIC_CIRCLE) {
			// サークルの半径を文字列の幅に基づいて計算
			float radius = (std::max)(textHeight, textWidth) / 2 + padding + guide.radiusOffset;

			DrawCircleAA(currentX + radius, y + radius, radius, 32, guide.lineColor, FALSE, guide.lineWidth);
			DrawCircleAA(currentX + radius, y + radius, radius, 32, guide.buttonColor, TRUE);
			float stringY = y + radius - (textHeight / 2) + guide.offsetButtonStringY;
			// 文字列をサークルの中央に配置
			DrawStringToHandle(currentX + radius - textWidth / 2, stringY, button.c_str(), guide.buttonStringColor, font, 0xFFFFFF);
			currentX += radius * 2;
		}
		else if (guide.shapeType == GUIDE_SHAPE_TYPE::DYNAMIC_BOX) {
			float boxWidth = textWidth + 2 * padding;  // padding を両側に追加
			float boxHeight = textHeight + 2 * padding;  // padding を上下に追加

			// 1対1の比率の正方形に固定
			// // boxWidth と boxHeight のうち、大きい方を選ぶ
			// float maxDimension = max(boxWidth, boxHeight);
			//
			// // 正方形にするために、boxWidth と boxHeight を maxDimension に設定
			// boxWidth = maxDimension;
			// boxHeight = maxDimension;

			// 縦長にならないように制限する
			// 縦長になる場合は、boxWidth を boxHeight と同じにする
			if (boxHeight > boxWidth) {
				boxWidth = boxHeight;
			}

			DrawBoxAA(currentX, y, currentX + boxWidth, y + boxHeight, guide.lineColor, FALSE, guide.lineWidth);
			DrawBoxAA(currentX, y, currentX + boxWidth, y + boxHeight, guide.buttonColor, TRUE);
			float stringY = y + (boxHeight / 2 - textHeight / 2) + guide.offsetButtonStringY;
			DrawStringToHandle(currentX + boxWidth / 2 - textWidth / 2, stringY, button.c_str(), guide.buttonStringColor, font, 0xFFFFFF);
			currentX += boxWidth;
		}
		else if (guide.shapeType == GUIDE_SHAPE_TYPE::FIXED_CIRCLE) {
			float radius = guide.width / 2 + guide.radiusOffset;
			DrawCircleAA(currentX + radius, y + radius, radius, 32, guide.lineColor, FALSE, guide.lineWidth);
			DrawCircleAA(currentX + radius, y + radius, radius, 32, guide.buttonColor, TRUE);
			float stringY = y + radius - (textHeight / 2) + guide.offsetButtonStringY;
			DrawStringToHandle(currentX + radius - textWidth / 2, stringY, button.c_str(), guide.buttonStringColor, font, 0xFFFFFF);

			currentX += guide.width;
		}
		else if (guide.shapeType == GUIDE_SHAPE_TYPE::FIXED_BOX) {
			DrawBoxAA(currentX, y, currentX + guide.width, y + guide.height, guide.lineColor, FALSE, guide.lineWidth);
			DrawBoxAA(currentX, y, currentX + guide.width, y + guide.height, guide.buttonColor, TRUE);
			float stringY = y + guide.height / 2 - (textHeight / 2) + guide.offsetButtonStringY;
			DrawStringToHandle(currentX + guide.width / 2 - textWidth / 2, stringY, button.c_str(), guide.buttonStringColor, font, 0xFFFFFF);
			currentX += guide.width + guide.lineWidth;
		}
		else if (guide.shapeType == GUIDE_SHAPE_TYPE::ROUNDED_BOX) {

			// テキストのサイズを取得
			int textWidth, textHeight;
			GetDrawStringSizeToHandle(&textWidth, &textHeight, nullptr, button.c_str(), button.length(), font);

			// 図形のサイズを動的に計算
			float dynamicHeight = textHeight + 5;
			float dynamicWidth = textWidth + 5;
			float radius = dynamicHeight / 2;  // 高さに基づいて半径を設定

			float rectX = currentX;
			float rectY = y;

			// 矩形の左上と右下の座標を計算
			float rectLeft = rectX + radius;
			float rectTop = rectY;
			float rectRight = rectX + dynamicWidth - radius;
			float rectBottom = rectY + dynamicHeight + 1.0f;

			// 外枠の色と太さを設定
			unsigned int borderColor = 0xEFEAF6;
			float borderWidth = 4.0f; // 太さ

			// 外枠の矩形を描画
			DrawBoxAA(rectLeft, rectTop, rectRight, rectBottom, borderColor, FALSE, borderWidth);

			// 左側の円を描画
			float circleLeftCenterX = rectLeft;
			float circleLeftCenterY = rectY + radius;
			// 左側の円の外枠を描画
			DrawCircleAA(circleLeftCenterX, circleLeftCenterY, radius, 20, borderColor, FALSE, borderWidth);

			DrawCircleAA(circleLeftCenterX, circleLeftCenterY, radius, 20, guide.buttonColor, TRUE);

			// 右側の円を描画
			float circleRightCenterX = rectRight;
			float circleRightCenterY = rectY + radius;
			// 右側の円の外枠を描画
			DrawCircleAA(circleRightCenterX, circleRightCenterY, radius, 20, borderColor, FALSE, borderWidth);
			DrawCircleAA(circleRightCenterX, circleRightCenterY, radius, 20, guide.buttonColor, TRUE);

			// 矩形を描画
			DrawBoxAA(rectLeft, rectTop, rectRight, rectBottom, guide.buttonColor, TRUE);

			float stringY = rectY + dynamicHeight / 2 - (textHeight / 2) + guide.offsetButtonStringY;
			// テキストを描画
			DrawStringToHandle(rectX + dynamicWidth / 2 - textWidth / 2, stringY, button.c_str(), guide.buttonStringColor, font, 0xFFFFFF);

			currentX += dynamicWidth;
		}
		else if (guide.shapeType == GUIDE_SHAPE_TYPE::JOYSTICK) {
			float joystickX = currentX;
			float joystickY = y;
			DrawOvalAA(joystickX, joystickY + 6.0f, 18, 10, 20, guide.buttonColor, TRUE);
			DrawBoxAA(joystickX - 5.0f, joystickY, joystickX + 7, joystickY + 23, guide.buttonColor, TRUE);
			DrawOvalAA(joystickX, joystickY + 23.0f, 22, 8, 20, guide.buttonColor, TRUE);
			DrawString(joystickX - 2.0f, joystickY - 2.0f, button.c_str(), guide.buttonStringColor);
			currentX += 25.0f;  // ジョイスティックの幅

		}
		else { // Text
			DrawStringToHandle(currentX, y, button.c_str(), guide.buttonStringColor, font, 0xFFFFFF);
			currentX += textWidth;
		}

		//複数のボタンがある場合最後のボタン以外の後に内部パディングを追加
		if (i < guide.buttons.size() - 1)
		{
			currentX += guide.innerPadding;
		}
		else
		{
			// ボタンと説明の間のパディングを追加
			currentX += padding;
		}
	}
}
