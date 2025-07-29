// -----------------------------------------------------------------------------------------
//  File          : SceneTag.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/08
//  Updated       : 2025/07/08
//  Description   : �V�[���^�O�񋓗p
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef SCENE_TAG_H_
#define SCENE_TAG_H_

enum class SceneTag {
	Null,
	Title,	// �Q�[���^�C�g��
	Menu,	// �Q�[�����j���[
	Game,	// �Q�[���v���C
	TimelineEditor,

	MAX_SIZE	// �v�f��
};

#endif
