#include "GameMainScene.h"
#include "../Object/RankingData.h"
#include "DxLib.h"
#include <math.h>

GameMainScene::GameMainScene() :high_score(0), back_ground(NULL), mileage(0), player(nullptr),enemy(nullptr)
{
	font_handle_h2 = CreateFontToHandle("Segoe UI", 50, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	font_handle_h3 = CreateFontToHandle("Segoe UI", 20, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);
	font_handle_h4 = CreateFontToHandle("Segoe UI", 10, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, -1, 1);

	for (int i = 0; i < 3; i++)
	{
		enemy_image[i] = NULL;
		enemy_count[i] = NULL;
	}
}

GameMainScene::~GameMainScene()
{
	DeleteFontToHandle(font_handle_h2);
	DeleteFontToHandle(font_handle_h3);
	DeleteFontToHandle(font_handle_h4);
}

//����������
void GameMainScene::Initialize()
{
	//�����_�l��ǂݍ���
	ReadHighScore();

	//�摜�̓ǂݍ���
	back_ground = LoadGraph(        "Resource/images/Scene/GameMain/background.png");
	//barrier_image = LoadGraph(      "Resource/images/barrier.png");
	const int result = LoadDivGraph("Resource/images/fish.png", 3, 1, 3, 120, 63,
		enemy_image);

	//�G���[�`�F�b�N
	if (back_ground == -1)
	{
		throw("Resource/images/Scene/GameMain/background.png������܂���\n");
	}

	if (result == -1)
	{
		throw("Resource/images/fish.png������܂���\n");
	}

	//�I�u�W�F�N�g�̐���
	player = new Player;
	enemy = new Enemy * [10];

	//�I�u�W�F�N�g�̏�����
	player->Initialize();

	for (int i = 0; i < 10; i++)
	{
		enemy[i] = nullptr;
	}
}

//�X�V����
eSceneType GameMainScene::Update()
{
	//�v���C���[�̍X�V
	player->Update();

	//�ړ������̍X�V
	mileage += static_cast<int>(player->GetSpeed()) - 5;

	//�G��������
	//if (mileage / 20 % 100 == 0)
	// 100�}�C�����ƂɓG�𐶐�
	if (mileage % 100 == 0)
	{
		//! �f�o�b�O
		//printfDx("%d\n", mileage);
		for (int i = 0; i < 10; i++)
		{
			if (enemy[i] == nullptr)
			{
				const int type = GetRand(3) % 3;
				enemy[i] = new Enemy(type, enemy_image[type]);
				enemy[i]->Initialize();
				break;
			}
		}
	}

	//�G�̍X�V�Ɠ����蔻��`�F�b�N
	for (int i = 0; i < 10; i++)
	{
		if (enemy[i] != nullptr)
		{
			enemy[i]->Updata(player->GetSpeed());

			//��ʊO�ɍs������A�G���폜���ăX�R�A���Z
			if (enemy[i]->GetLocation().y >= 640.0f)
			{
				enemy_count[enemy[i]->GetType()]++;
				enemy[i]->Finalize();
				delete enemy[i];
				enemy[i] = nullptr;
			}

			//�����蔻��̊m�F
			if (IsHitCheck(player, enemy[i]))
			{
				player->SetActive(false);
				player->Draw_Qizu(false);
				player->DecreaseHp(-50.0f);
				enemy[i]->QuizHide(false);
				enemy[i]->Finalize();
				delete enemy[i];
				enemy[i] = nullptr;
			}
		}
	}

	//�v���C���[�̔R�����̗͂��O�����Ȃ�A���U���g�ɑJ�ڂ���
	if (/*player->GetFuel() < 0.0f || */player->GetHp() < 0.0f)
	{
		return eSceneType::E_RESULT;
	}
	return GetNowScene();
}

//�`�揈��
void GameMainScene::Draw()const
{
	//�w�i�摜�̕`��
	DrawGraph(mileage % 1280 + 1280, 0, back_ground, TRUE);
	DrawGraph(mileage % 1280, 0, back_ground, TRUE);

	//�G�̕`��
	for (int i = 0; i < 10; i++)
	{
		if (enemy[i] != nullptr)
		{
			enemy[i]->Draw();
		}
	}

	//�v���C���[�̕`��
	player->Draw();

	//UI�̕`��
	//DrawBox(500, 0, 640, 480, GetColor(0, 153, 0), TRUE);
	//SetFontSize(16);
	//DrawFormatString2ToHandle(510, 20, 0xffffff, 0x000000, font_handle_h3, "�n�C�X�R�A");
	//DrawFormatString2ToHandle(505, 50, 0xffffff, 0xff8c00, font_handle_h3, "%08dM", high_score);
	//DrawFormatString2ToHandle(510, 80, 0xffffff, 0x000000, font_handle_h3, "��������");
	//for (int i = 0; i < 3; i++)
	//{
	//	DrawRotaGraph(523 + (i * 50), 130, 0.3, 0, enemy_image[i], TRUE, FALSE);
	//	DrawFormatString(510 + (i * 50), 150, GetColor(255, 255, 255), "%03d", enemy_count[i]);
	//}
	//DrawFormatString2ToHandle(510, 180, 0xffffff, 0x000000, font_handle_h3, "���s����");
	//DrawFormatString2ToHandle(505, 220, 0xffffff, 0x00fa9a, font_handle_h3,  "%08dM", mileage / 10);
	//DrawFormatString2ToHandle(510, 260, 0xffffff, 0x000000, font_handle_h3, "�X�s�[�h");
	////DrawFormatString2ToHandle(555, 280, 0xffffff, 0x0000ff, font_handle_h2, "%0.f", player->GetSpeed());
	//DrawFormatString2ToHandle(500, 280, 0xffffff, 0x0000ff, font_handle_h2, "%3.f km/h", player->GetSpeed() * 15.0f);
	//DrawFormatString2ToHandle(470, 280, 0xffffff, 0x0000ff, font_handle_h2, "%d", player->GetGear());

	////�o���A�����̕`��
	//for (int i = 0; i < player->GetBarrierCount(); i++)
	//{
	//	DrawRotaGraph(520 + i * 50, 380, 0.32f, 0, barrier_image, TRUE, FALSE);
	//}

	//�R���Q�[�W�̕`��
	//float fx = 510.0f;
	//float fy = 390.0f;
	//DrawFormatString(fx, fy, GetColor(0, 0, 0), "FUEL METER");
	//DrawBoxAA(fx, fy + 20.0f, fx + (player->GetFuel() * 100 / 20000), fy + 40.0f,
	//    GetColor(0, 102, 204), TRUE);
	//DrawBoxAA(fx, fy + 20.0f, fx + 100.0f, fy + 40.0f, GetColor(0, 0, 0), FALSE);

	//�̗̓Q�[�W�̕`��
	//float fx = 510.0f;
	//float fy = 430.0f;
	//DrawFormatString(fx, fy, GetColor(0, 0, 0), "PLAYER HP");
	//DrawBoxAA(fx, fy + 20.0f, fx + (player->GetHp() * 100 / 1000), fy + 40.0f,
	//	GetColor(255, 0, 0), TRUE);
	//DrawBoxAA(fx, fy + 20.0f, fx + 100.0f, fy + 40.0f, GetColor(0, 0, 0),
	//	FALSE);




	//! DEBUG
	//for (int i = 0; i < 10; i++)
	//{
	//	if (enemy[i] != nullptr) {
	//		const int lane = static_cast<int>((enemy[i]->GetLocation().x - 40) / 105) + 1;
	//		DrawFormatString(100, 20 + i * 20, 0xffffff, "Enemy: ���[���F%d x %f , y %f", lane, enemy[i]->GetLocation().x, enemy[i]->GetLocation().y);
	//	}
	//}
}


//�I��������
void GameMainScene::Finalize()
{
	//�X�R�A���v�Z����
	int score = (mileage / 10 * 10);
	for (int i = 0; i < 3; i++)
	{
		score += (i + 1) * 50 * enemy_count[i];
	}

	//���U���g�f�[�^�̏�������
	FILE* fp = nullptr;
	//�t�@�C���I�[�v��
	errno_t result = fopen_s(&fp, "Resource/dat/result_data.csv", "w");

	//�G���[�`�F�b�N
	if (result != 0)
	{
		throw("Resource/dat/result_data.csv���J���܂���\n");
	}

	//�X�R�A��ۑ�
	fprintf(fp, "%d,\n", score);

	//���������Ɠ��_��ۑ�
	for (int i = 0; i < 3; i++)
	{
		fprintf(fp, "%d,\n", enemy_count[i]);
	}

	//�t�@�C���N���[�Y
	fclose(fp);

	//���I�m�ۂ����I�u�W�F�N�g���폜����
	player->Finalize();
	delete player;

	for (int i = 0; i < 10; i++)
	{
		if (enemy[i] != nullptr)
		{
			enemy[i]->Finalize();
			delete enemy[i];
			enemy[i] = nullptr;
		}
	}

	delete[] enemy;
}

//���݂̃V�[�������擾
eSceneType GameMainScene::GetNowScene()const
{
	return eSceneType::E_MAIN;
}


//�n�C�X�R�A�ǂݍ���
void GameMainScene::ReadHighScore()
{
	RankingData data;
	data.Initialize();

	high_score = data.GetScore(0);

	data.Finalize();
}


//�����蔻�菈���i�v���C���[�ƓG�j
bool GameMainScene::IsHitCheck(Player* p, Enemy* e)
{
	//�v���C���[���o���A�𒣂��Ă�����A�����蔻��𖳎�����
	//if (p->IsBarrier())
	//{
	//	return false;
	//}

	//�G��񂪂Ȃ���΁A�����蔻��𖳎�����
	if (e == nullptr)
	{
		return false;
	}

	//�ʒu���̍������擾
	Vector2D diff_location = p->GetLocation() - e->GetLocation();

	//�����蔻��T�C�Y�̑傫�����擾
	Vector2D box_ex = p->GetBoxSize() + e->GetBoxSize();

	//�R���W�����f�[�^���ʒu���̍������������Ȃ�A�q�b�g����Ƃ���
	return ((fabsf(diff_location.x) < box_ex.x) && (fabsf(diff_location.y) < box_ex.y));
}