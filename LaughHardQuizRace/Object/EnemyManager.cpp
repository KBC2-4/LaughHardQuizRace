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
            const int speed = GetRand(3) + 1;
            enemy->Update();

            //画面外に行ったら、敵を削除
            if (enemy->GetLocation().x <= 0.0f)
            {
                enemy->Finalize();
                delete enemy;
                enemy = nullptr;
            }
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
    //int edge = GetRand(4); // 0:左, 1:右, 2:上, 3:下

    int width, height;
    GetWindowSize(&width, &height); // ウィンドウサイズを取得

    float posX;
    float moveX = 0.0f;

    // Y座標の候補を配列に格納
    int yPosOptions[] = { 100, 200, 300, 400, 500, 600, 700 };
    int optionsCount = sizeof(yPosOptions) / sizeof(yPosOptions[0]); // 配列の要素数を計算

    // エネミーが左端か右端から出現するかランダムに決定
    int edge = GetRand(1); // 0で左端、1で右端

    if (edge == 0) {
        // 左端から出現
        posX = 0.0f; // 画面の左端
        moveX = 2.0f; // 右方向への速度
    }
    else {
        // 右端から出現
        posX = static_cast<float>(width); // 画面の右端
        moveX = -2.0f; // 左方向への速度
    }

    // Y座標をランダムに選択
    float posY = static_cast<float>(yPosOptions[GetRand(optionsCount - 1)]);

    //float posX, posY;
    //float moveX, moveY;

    //// エネミーの出現位置と速度の設定
    //switch (edge) {
    //case 0: // 左辺から出現
    //    posX = 0.0f;
    //    posY = static_cast<float>(GetRand(height));
    //    moveX = 2.0f; // 右方向への速度
    //    moveY = 0.0f;
    //    break;
    //case 1: // 右辺から出現
    //    posX = static_cast<float>(width);
    //    posY = static_cast<float>(GetRand(height));
    //    moveX = -2.0f; // 左方向への速度
    //    moveY = 0.0f;
    //    break;
    //case 2: // 上辺から出現
    //    posX = static_cast<float>(GetRand(width));
    //    posY = 0.0f;
    //    moveX = 0.0f;
    //    moveY = 2.0f; // 下方向への速度
    //    break;
    //case 3: // 下辺から出現
    //    posX = static_cast<float>(GetRand(width));
    //    posY = static_cast<float>(height);
    //    moveX = 0.0f;
    //    moveY = -2.0f; // 上方向への速度
    //    break;
    //}

    // エネミーの種類と画像の選択
    const int type = GetRand(14);
    const int imageIndex = type % 15;
    const int image = enemy_image[imageIndex];

    // エネミーインスタンスの生成
    Enemy* enemy = new Enemy(type, image, true);
    enemy->SetPosition(posX, posY);
    enemy->SetMovement(moveX, 0.0f);
    enemy->Initialize();

    // エネミーリストに追加
    enemies.push_back(enemy);
}


void EnemyManager::ClearEnemies()
{
}
