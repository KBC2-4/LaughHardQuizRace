#include "EnemyManager.h"

#include <DxLib.h>

EnemyManager::EnemyManager()
{
    for (int i = 0; i < 15; i++)
    {
        enemy_image[i] = NULL;
    }
}

EnemyManager::~EnemyManager()
{
    for (int i = 0; i < 15; i++)
    {
        DeleteGraph(enemy_image[i]);
    }
}

void EnemyManager::LoadImages()
{
    const int enemy_image_result = LoadDivGraph("Resource/images/fish.png", 15, 3, 5, 200, 138,
        enemy_image);

    if (enemy_image_result == -1)
    {
        throw("Resource/images/fish.png������܂���\n");
    }
}

void EnemyManager::Update()
{
	if (enemies.size() < 15)
	{
		CreateEnemy();
	}

    for (Enemy* enemy : enemies)
    {
        if (enemy != nullptr)
        {
            //��ʊO�ɍs������A�G���폜
            if (enemy->GetLocation().x <= 0.0f)
            {
                enemy->Finalize();
                delete enemy;
                enemy = nullptr;
            }


            const int speed = GetRand(3) + 1;
            enemy->Update();
        }
    }
}

void EnemyManager::Draw() const
{
	for (const Enemy* enemy : enemies)
	{
		if (enemy != nullptr)
		{
			enemy->Draw();
		}
	}
}

void EnemyManager::CreateEnemy() {
    int edge = GetRand(4); // 0:��, 1:�E, 2:��, 3:��

    int width, height;
    GetWindowSize(&width, &height); // �E�B���h�E�T�C�Y���擾

    float posX, posY;
    float moveX, moveY;

    // �G�l�~�[�̏o���ʒu�Ƒ��x�̐ݒ�
    switch (edge) {
    case 0: // ���ӂ���o��
        posX = 0.0f;
        posY = static_cast<float>(GetRand(height));
        moveX = 2.0f; // �E�����ւ̑��x
        moveY = 0.0f;
        break;
    case 1: // �E�ӂ���o��
        posX = static_cast<float>(width);
        posY = static_cast<float>(GetRand(height));
        moveX = -2.0f; // �������ւ̑��x
        moveY = 0.0f;
        break;
    case 2: // ��ӂ���o��
        posX = static_cast<float>(GetRand(width));
        posY = 0.0f;
        moveX = 0.0f;
        moveY = 2.0f; // �������ւ̑��x
        break;
    case 3: // ���ӂ���o��
        posX = static_cast<float>(GetRand(width));
        posY = static_cast<float>(height);
        moveX = 0.0f;
        moveY = -2.0f; // ������ւ̑��x
        break;
    }

    // �G�l�~�[�̎�ނƉ摜�̑I��
    int type = GetRand(14); // ����14��ނ̃G�l�~�[������Ƃ���
    int imageIndex = type % 15; // 15�̓G�l�~�[�̎�ނ̐�
    int image = enemy_image[imageIndex];

    // �G�l�~�[�C���X�^���X�̐���
    Enemy* enemy = new Enemy(type, image);
    enemy->SetPosition(posX, posY);
    enemy->SetMovement(moveX, moveY); // ���̃��\�b�h��Enemy�N���X�ɒǉ�����K�v������܂�
    enemy->Initialize();

    // �G�l�~�[���X�g�ɒǉ�
    enemies.push_back(enemy);
}


void EnemyManager::ClearEnemies()
{
}
