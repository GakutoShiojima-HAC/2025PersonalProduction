#include "Scene/GameScene.h"
#include <GSeffect.h>
#include "Engine/Core/Field/Field.h"
#include "Engine/Core/World/Light.h"
#include <gslib.h>	// tmp
#include "Camera/FixedCamera.h"	// tmp

#define GS_ENABLE_MESH_SHADOW			// ���b�V���ɉe��t����
//#define GS_ENABLE_SKIN_MESH_SHADOW	// �X�L�j���O���b�V���ɉe��t����
//#define GS_ENABLE_SOFT_SHADOW			// �e�̗֊s���ڂ���
#include <GSstandard_shader.h>

void GameScene::start() {
	is_end_ = false;

	// ������J�����O��L���ɂ���
	gsEnable(GS_FRUSTUM_CULLING);
	// �V���h�E�}�b�v�̍쐬�i�Q���̃J�X�P�[�h�V���h�E�}�b�v�j
	static const GSuint shadow_map_size[] = { 2048, 2048 };
	gsCreateShadowMap(2, shadow_map_size, GS_TRUE);
	// �V���h�E�}�b�v��K�p���鋗��
	gsSetShadowMapDistance(60.0f);
	// �J�X�P�[�h�V���h�E�̃V���h�E�}�b�v�̕����ʒu�𒲐�
	gsSetShadowMapCascadeLamda(0.7f);
	// �V���h�E�ɂ����̌�������ݒ�(0.0:�����`1.0:�������Ȃ�)
	gsSetShadowMapAttenuation(0.0f);
	// �V���h�E�}�b�v�̃|���S���I�t�Z�b�g��ݒ肷��
	gsEnableShadowMapPolygonOffset();
	gsSetShadowMapPolygonOffset(2.5f, 1.0f);

	// tmp
	gsLoadOctree(0, "Resource/Assets/Octree/Octree.oct");
	gsLoadOctree(1, "Resource/Assets/Octree/Collider.oct");
	gsLoadTexture(0, "Resource/Assets/Skybox/default_skybox.dds");
	world_.add_field(new Field{ 0, 1, 0 });
	world_.add_light(new Light{});
	world_.add_camera(new FixedCamera{ &world_, GSvector3{ 0.0f, 3.0f, -10.0f }, GSvector3{ 0.0f, 2.0f, 0.0f } });

	// ����
	world_.update(0.0f);
}

void GameScene::update(float delta_time) {
	// tmp scene end
	if (gsGetKeyTrigger(GKEY_L)) is_end_ = true;

	world_.update(delta_time);
}

void GameScene::draw() const {
	world_.draw();
	
	gsDrawText("game");
}

void GameScene::end() {
	world_.clear();

	// �S�ẴG�t�F�N�g���~����
	gsStopAllEffects();

	// tmp
	gsDeleteOctree(0);
	gsDeleteOctree(1);
	gsDeleteTexture(0);
}

bool GameScene::is_end() const {
	return is_end_;
}

SceneTag GameScene::scene_tag() const {
	return SceneTag::Game;
}

SceneTag GameScene::next_scene_tag() const {
	return SceneTag::Menu;
}

bool GameScene::is_application_end() const {
	return false;
}

void GameScene::reception_message(const std::string& message, void* param) {
	// �Ȃɂ��󂯎��Ȃ�
}
