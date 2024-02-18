#include "sceneBase.h"
#include <DxLib.h>
#include "../Utility/Guide.h"

int SceneBase::GetDrawCenterX(const char* string, int font_handle, int margin) const
{
	//��ʕ�
	constexpr int screenX = 1280;

	if (margin >= screenX || margin <= -screenX) {
		margin = 0;
	}

	if (font_handle == 0) {
		font_handle = DX_DEFAULT_FONT_HANDLE;
	}


	const int w = screenX / 2 + margin - (GetDrawFormatStringWidthToHandle(font_handle, string) / 2);
	return w;
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
			// �T�[�N���̔��a�𕶎���̕��Ɋ�Â��Čv�Z
			float radius = (std::max)(textHeight, textWidth) / 2 + padding + guide.radiusOffset;

			DrawCircleAA(currentX + radius, y + radius, radius, 32, guide.lineColor, FALSE, guide.lineWidth);
			DrawCircleAA(currentX + radius, y + radius, radius, 32, guide.buttonColor, TRUE);
			float stringY = y + radius - (textHeight / 2) + guide.offsetButtonStringY;
			// ��������T�[�N���̒����ɔz�u
			DrawStringToHandle(currentX + radius - textWidth / 2, stringY, button.c_str(), guide.buttonStringColor, font, 0xFFFFFF);
			currentX += radius * 2;
		}
		else if (guide.shapeType == GUIDE_SHAPE_TYPE::DYNAMIC_BOX) {
			float boxWidth = textWidth + 2 * padding;  // padding �𗼑��ɒǉ�
			float boxHeight = textHeight + 2 * padding;  // padding ���㉺�ɒǉ�

			// 1��1�̔䗦�̐����`�ɌŒ�
			// // boxWidth �� boxHeight �̂����A�傫������I��
			// float maxDimension = max(boxWidth, boxHeight);
			//
			// // �����`�ɂ��邽�߂ɁAboxWidth �� boxHeight �� maxDimension �ɐݒ�
			// boxWidth = maxDimension;
			// boxHeight = maxDimension;

			// �c���ɂȂ�Ȃ��悤�ɐ�������
			// �c���ɂȂ�ꍇ�́AboxWidth �� boxHeight �Ɠ����ɂ���
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

			// �e�L�X�g�̃T�C�Y���擾
			int textWidth, textHeight;
			GetDrawStringSizeToHandle(&textWidth, &textHeight, nullptr, button.c_str(), button.length(), font);

			// �}�`�̃T�C�Y�𓮓I�Ɍv�Z
			float dynamicHeight = textHeight + 5;
			float dynamicWidth = textWidth + 5;
			float radius = dynamicHeight / 2;  // �����Ɋ�Â��Ĕ��a��ݒ�

			float rectX = currentX;
			float rectY = y;

			// ��`�̍���ƉE���̍��W���v�Z
			float rectLeft = rectX + radius;
			float rectTop = rectY;
			float rectRight = rectX + dynamicWidth - radius;
			float rectBottom = rectY + dynamicHeight + 1.0f;

			// �O�g�̐F�Ƒ�����ݒ�
			unsigned int borderColor = 0xEFEAF6;
			float borderWidth = 4.0f; // ����

			// �O�g�̋�`��`��
			DrawBoxAA(rectLeft, rectTop, rectRight, rectBottom, borderColor, FALSE, borderWidth);

			// �����̉~��`��
			float circleLeftCenterX = rectLeft;
			float circleLeftCenterY = rectY + radius;
			// �����̉~�̊O�g��`��
			DrawCircleAA(circleLeftCenterX, circleLeftCenterY, radius, 20, borderColor, FALSE, borderWidth);

			DrawCircleAA(circleLeftCenterX, circleLeftCenterY, radius, 20, guide.buttonColor, TRUE);

			// �E���̉~��`��
			float circleRightCenterX = rectRight;
			float circleRightCenterY = rectY + radius;
			// �E���̉~�̊O�g��`��
			DrawCircleAA(circleRightCenterX, circleRightCenterY, radius, 20, borderColor, FALSE, borderWidth);
			DrawCircleAA(circleRightCenterX, circleRightCenterY, radius, 20, guide.buttonColor, TRUE);

			// ��`��`��
			DrawBoxAA(rectLeft, rectTop, rectRight, rectBottom, guide.buttonColor, TRUE);

			float stringY = rectY + dynamicHeight / 2 - (textHeight / 2) + guide.offsetButtonStringY;
			// �e�L�X�g��`��
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
			currentX += 25.0f;  // �W���C�X�e�B�b�N�̕�

		}
		else { // Text
			DrawStringToHandle(currentX, y, button.c_str(), guide.buttonStringColor, font, 0xFFFFFF);
			currentX += textWidth;
		}

		//�����̃{�^��������ꍇ�Ō�̃{�^���ȊO�̌�ɓ����p�f�B���O��ǉ�
		if (i < guide.buttons.size() - 1)
		{
			currentX += guide.innerPadding;
		}
		else
		{
			// �{�^���Ɛ����̊Ԃ̃p�f�B���O��ǉ�
			currentX += padding;
		}
	}
}
