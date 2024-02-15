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
        throw("Resource/images/fish.pngがありません\n");
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
            //画面外に行ったら、敵を削除
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
    int edge = GetRand(4); // 0:左, 1:右, 2:上, 3:下

    int width, height;
    GetWindowSize(&width, &height); // ウィンドウサイズを取得

    float posX, posY;
    float moveX, moveY;

    // エネミーの出現位置と速度の設定
    switch (edge) {
    case 0: // 左辺から出現
        posX = 0.0f;
        posY = static_cast<float>(GetRand(height));
        moveX = 2.0f; // 右方向への速度
        moveY = 0.0f;
        break;
    case 1: // 右辺から出現
        posX = static_cast<float>(width);
        posY = static_cast<float>(GetRand(height));
        moveX = -2.0f; // 左方向への速度
        moveY = 0.0f;
        break;
    case 2: // 上辺から出現
        posX = static_cast<float>(GetRand(width));
        posY = 0.0f;
        moveX = 0.0f;
        moveY = 2.0f; // 下方向への速度
        break;
    case 3: // 下辺から出現
        posX = static_cast<float>(GetRand(width));
        posY = static_cast<float>(height);
        moveX = 0.0f;
        moveY = -2.0f; // 上方向への速度
        break;
    }

    // エネミーの種類と画像の選択
    int type = GetRand(14); // 仮に14種類のエネミーがいるとする
    int imageIndex = type % 15; // 15はエネミーの種類の数
    int image = enemy_image[imageIndex];

    // エネミーインスタンスの生成
    Enemy* enemy = new Enemy(type, image);
    enemy->SetPosition(posX, posY);
    enemy->SetMovement(moveX, moveY); // このメソッドをEnemyクラスに追加する必要があります
    enemy->Initialize();

    // エネミーリストに追加
    enemies.push_back(enemy);
}


void EnemyManager::ClearEnemies()
{
}
