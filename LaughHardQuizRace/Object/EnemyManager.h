#pragma once
#include <vector>

#include "Enemy.h"

class EnemyManager
{
private:
	std::vector<Enemy*> enemies; // 管理するエネミーのリスト
	int enemy_image[15]; // エネミーの画像ハンドルを格納する配列

public:
	EnemyManager();
	~EnemyManager();
	void LoadImages(); // エネミー画像の読み込み
	void Update(); // すべてのエネミーを更新
	void Draw() const; // すべてのエネミーを描画
	void CreateEnemy(); // 新しいエネミーを生成
	void ClearEnemies(); // すべてのエネミーを削除
};

