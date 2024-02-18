#pragma once
#include <vector>

#define A_BUTTON_COLOR 0xF03A32

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
	/// 文字の描画するX座標が中心になるX座標を取得する。
	/// </summary>
	/// <param name="string">描画する文字列</param>
	/// <param name="font_handle">描画に使用するフォントハンドル(未入力又は0の場合はデフォルトフォントハンドルを使用</param>
	/// <param name="margin">中央のX座標に間隔を空ける値</param>
	int GetDrawCenterX(const char* string, int font_handle = 0, int margin = 0) const;

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