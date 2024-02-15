#pragma once
#include <vector>


struct guideElement;

//シーン情報
enum eSceneType
{
	E_TITLE,
	E_MAIN,
	E_HELP,
	E_RANKING_DISP,
	E_RANKING_INPUT,
	E_RESULT,
	E_END
};

//基底シーンクラス
class SceneBase
{
public:
	SceneBase() {}
	virtual ~SceneBase() {}

	//初期化処理
	virtual void Initialize() {}
	//更新処理
	virtual eSceneType Update()
	{
		return GetNowScene();
	}
	//描画処理
	virtual void Draw() const {}
	//終了時処理
	virtual void Finalize() {}
	
	//現在のシーン情報を取得
	virtual eSceneType GetNowScene() const = 0;

	
	/// <summary>
	/// ガイド表示する
	/// </summary>
	/// <param name="guides">ガイド要素</param>
	/// <param name="x">描画の基準となるX座標</param>
	/// <param name="y">描画の基準となるY座標</param>
	/// <param name="padding">ボタンと説明の間の間隔</param>
	/// <param name="elementSpacing">各要素の間隔</param>
	void DrawGuides(const std::vector<guideElement>& guides, float x, float y, float padding, float elementSpacing) const;
	void DrawGuideElement(const guideElement& guide, float& currentX, float y) const;
};