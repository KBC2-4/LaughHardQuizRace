#include "DxLib.h"
#include "../Utility/InputControl.h"
#include "ResultScene.h"
#include "../Object/RankingData.h"

ResultScene::ResultScene() :background_image(0), score(0)
{
}

ResultScene::~ResultScene()
{
}	


//����������
void ResultScene::Initialize()
{
	//�摜�̓ǂݍ���
	background_image = LoadGraph("Resource/images/Scene/Result/background.png");

	//�G���[�`�F�b�N
	if (background_image == -1)
	{
		throw("Resource/images/Scene/Result/background.png������܂���\n");
	}

	//�Q�[�����ʂ̓ǂݍ���
	ReadResultData();
}


//�X�V����
eSceneType ResultScene::Update()
{
	//B�{�^���Ń����L���O�ɑJ�ڂ���
	if (InputControl::GetButtonDown(XINPUT_BUTTON_B))
	{
		return eSceneType::E_TITLE;
		//return eSceneType::E_RANKING_INPUT;
	}

	return GetNowScene();
}


//�`�揈��
void ResultScene::Draw()const
{
	//�w�i�摜��`��
	DrawGraph(0, 0, background_image, TRUE);

	//for (int i = 0; i < 3; i++)
	//{
	//	DrawRotaGraph(230, 230 + (i * 20), 0.3f, DX_PI_F / 2, enemy_image[i], TRUE);

	//	DrawFormatString(260, 222 + (i * 21), GetColor(255, 255, 255), "%6d x%4d=%6d",
	//		enemy_count[i], (i + 1) * 50, (i + 1) * 50 * enemy_count[i]);
	//}
	DrawString(600, 290, "�X�R�A", 0xEB3229);
	DrawFormatString(180, 290, 0xEB3229, "         =%6d", score);
}


//�I��������
void ResultScene::Finalize()
{
	//�ǂݍ��񂾉摜���폜
	DeleteGraph(background_image);
}


//���݂̃V�[�������擾
eSceneType ResultScene::GetNowScene()const
{
	return eSceneType::E_RESULT;
}

//���U���g�f�[�^�̓ǂݍ���
void ResultScene::ReadResultData()
{
	//�t�@�C���I�[�v��
	FILE* fp = nullptr;
	errno_t result = fopen_s(&fp, "Resource/dat/result_data.csv", "r");

	//�G���[�`�F�b�N
	if (result != 0)
	{
		throw("Resource/dat/result_data.csv���ǂݍ��߂܂���\n");
	}

	//���ʂ�ǂݍ���
	fscanf_s(fp, "%d,\n", &score);

	//���������Ɠ��_���擾
	for (int i = 0; i < 3; i++)
	{
		fscanf_s(fp, "%d,\n", &enemy_count[i]);
	}

	//�t�@�C���N���[�Y
	fclose(fp);
}
