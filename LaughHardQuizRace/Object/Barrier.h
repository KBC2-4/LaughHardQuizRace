#pragma once

#include "../Utility/Vector2D.h"

class Barrier
{
private:
	int image;
	float life_span;

public:
	Barrier();    //バリア画像
	~Barrier();   //バリアの寿命

	void Draw(const Vector2D& location);    //描画処理
	bool IsFinished(float speed);           //寿命が尽きたか？
};

