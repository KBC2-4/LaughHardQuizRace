#pragma once
#include <vector>

#include "Enemy.h"

class EnemyManager
{
private:
	std::vector<Enemy*> enemies; // �Ǘ�����G�l�~�[�̃��X�g
	int enemy_image[15]; // �G�l�~�[�̉摜�n���h�����i�[����z��

public:
	EnemyManager();
	~EnemyManager();
	void LoadImages(); // �G�l�~�[�摜�̓ǂݍ���
	void Update(); // ���ׂẴG�l�~�[���X�V
	void Draw() const; // ���ׂẴG�l�~�[��`��
	void CreateEnemy(); // �V�����G�l�~�[�𐶐�
	void ClearEnemies(); // ���ׂẴG�l�~�[���폜
};

