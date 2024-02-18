#pragma once
#include <vector>

#define A_BUTTON_COLOR 0xF03A32

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
	/// �����̕`�悷��X���W�����S�ɂȂ�X���W���擾����B
	/// </summary>
	/// <param name="string">�`�悷�镶����</param>
	/// <param name="font_handle">�`��Ɏg�p����t�H���g�n���h��(�����͖���0�̏ꍇ�̓f�t�H���g�t�H���g�n���h�����g�p</param>
	/// <param name="margin">������X���W�ɊԊu���󂯂�l</param>
	int GetDrawCenterX(const char* string, int font_handle = 0, int margin = 0) const;

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