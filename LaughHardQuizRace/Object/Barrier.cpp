#include "Barrier.h"
#include "DxLib.h"

Barrier::Barrier() :image(NULL), life_span(1000)
{
	//画像の読み込み
	image = LoadGraph("Resource/images/barrier.png");

	//エラーチェック
	if (image == -1)
	{
		throw("Resource/images/barrier.pngがありません\n");
	}
}


Barrier::~Barrier()
{
	//読み込んだ画像を削除
	DeleteGraph(image);
}


//描画処理
void Barrier::Draw(const Vector2D& location)
{
	DrawRotaGraphF(location.x, location.y, 1.0f, 0.0, image, TRUE);
}


//寿命が尽きたか？
bool Barrier::IsFinished(float speed)
{
	life_span -= speed;
	return (life_span <= 0);
}