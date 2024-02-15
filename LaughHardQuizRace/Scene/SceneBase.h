#pragma once
#include <vector>


struct guideElement;

//�V�[�����
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

//���V�[���N���X
class SceneBase
{
public:
	SceneBase() {}
	virtual ~SceneBase() {}

	//����������
	virtual void Initialize() {}
	//�X�V����
	virtual eSceneType Update()
	{
		return GetNowScene();
	}
	//�`�揈��
	virtual void Draw() const {}
	//�I��������
	virtual void Finalize() {}
	
	//���݂̃V�[�������擾
	virtual eSceneType GetNowScene() const = 0;

	
	/// <summary>
	/// �K�C�h�\������
	/// </summary>
	/// <param name="guides">�K�C�h�v�f</param>
	/// <param name="x">�`��̊�ƂȂ�X���W</param>
	/// <param name="y">�`��̊�ƂȂ�Y���W</param>
	/// <param name="padding">�{�^���Ɛ����̊Ԃ̊Ԋu</param>
	/// <param name="elementSpacing">�e�v�f�̊Ԋu</param>
	void DrawGuides(const std::vector<guideElement>& guides, float x, float y, float padding, float elementSpacing) const;
	void DrawGuideElement(const guideElement& guide, float& currentX, float y) const;
};