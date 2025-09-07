#include "Scene/GameScene.h"
#include <GSeffect.h>
#include "Engine/Core/Assets/AssetsManager.h"
#include "Engine/Core/Field/Field.h"
#include "Engine/Core/World/Light.h"
#include "Engine/Core/Collision/AttackColliderPool.h"
#include "Engine/Core/Tween/Tween.h"
#include "Engine/Graphics/PostEffect/PostEffect.h"

#include <gslib.h>	// tmp
#include "Camera/FixedCamera.h"	// tmp
#include "Engine/Core/Timeline/Parameters/CameraTimeline.h"	// tmp
#include "Camera/TimelineCamera.h"	// tmp
#include "Camera/PlayerCamera.h"	// tmp
#include "Actor/Player/Player.h"	// tmp
#include "Camera/EditorCamera.h"	// tmp
#include "Actor/Enemy/DummyEnemy.h"	// tmp
#include "Assets.h"	// tmp

#define GS_ENABLE_MESH_SHADOW			// ���b�V���ɉe��t����
//#define GS_ENABLE_SKIN_MESH_SHADOW	// �X�L�j���O���b�V���ɉe��t����
//#define GS_ENABLE_SOFT_SHADOW			// �e�̗֊s���ڂ���
#include <GSstandard_shader.h>

void GameScene::start() {
	is_end_ = false;

	gsInitDefaultShader();
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

	// �ʏ�t�H�O�̐ݒ�
	GScolor4 color = world_.posteffect().fog_color();
	const float fog_color[4]{ color.r, color.g, color.b, color.a };
	const float fog_start{ 10.0f };
	const float fog_end{ 300.0f };
	glFogi(GL_FOG_MODE, GL_LINEAR);     // ���`�t�H�O
	glFogfv(GL_FOG_COLOR, fog_color);   // �t�H�O�̐F
	glFogf(GL_FOG_START, fog_start);    // �t�H�O�̊J�n�ʒu�i���_����̋����j
	glFogf(GL_FOG_END, fog_end);        // �t�H�O�̏I���ʒu�i���_����̋����j
	glEnable(GL_FOG);                   // �t�H�O��L���ɂ���

	// �|�X�g�G�t�F�N�g�̏�����
	world_.posteffect().init();

	// ���C�g�}�b�v�̓ǂݍ���
	gsLoadLightmap(0, "Resource/Assets/Octree/Stage1/Lightmap/Lightmap.txt");
	// ���t���N�V�����v���[�u�̓ǂݍ���
	gsLoadReflectionProbe(0, "Resource/Assets/Octree/Stage1/RefProbe/ReflectionProbe.txt");

	// tmp
	LoadAssets* asset = new LoadAssets{};
	asset->name = "Game";
	asset->octree.push_back({ (GSuint)OctreeID::Mesh, "Resource/Assets/Octree/Stage1/stage.oct" });
	asset->octree.push_back({ (GSuint)OctreeID::Collider, "Resource/Assets/Octree/Stage1/stage_collider.oct"});
	asset->texture.push_back({ (GSuint)TextureID::Skybox, "Resource/Assets/Skybox/stage1test.dds"});
	asset->skinmesh.push_back({ (GSuint)MeshID::Player, "Resource/Assets/Skinmesh/Player1/Player.mshb" });
	asset->skinmesh.push_back({ (GSuint)MeshID::DummyEnemy, "Resource/Assets/Skinmesh/DummyEnemy/DummyEnemy.mshb" });
	asset->texture.push_back({ (GSuint)TextureID::TmpUI, "Resource/Assets/Texture/kari.png" });
	AssetsManager::get_instance().load_assets(asset);

	world_.add_field(new Field{ (GSuint)OctreeID::Mesh, (GSuint)OctreeID::Collider, (GSuint)TextureID::Skybox });
	world_.add_light(new Light{});
	world_.add_attack_collider_pool(new AttackColliderPool{ &world_ });
	world_.add_navmesh(new NavMeshSurface{ "Resource/Assets/Octree/Stage1/navmesh.txt" });	// tmp
	
	// tmp
	world_.add_camera(new FixedCamera{ &world_, GSvector3{ 0.0f, 3.0f, -10.0f }, GSvector3{ 0.0f, 2.0f, 0.0f } });
	world_.add_camera(new TimelineCamera{ &world_ });
	world_.add_camera(new EditorCamera{ &world_ });

	// tmp
	world_.timeline().add(new CameraTimeline{ &world_, "Resource/Private/Timeline/Camera/List/world1.json" });

	// tmp
	PlayerCamera* player_camera = new PlayerCamera{ &world_ };
	world_.add_camera(player_camera);
	world_.add_character(new Player{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f }, GSvector3{ 0.0f, 1.0f, 0.0f }, player_camera });
	world_.camera_transition(player_camera);

	// tmp
	world_.add_character(new DummyEnemy{ &world_, GSvector3{ 0.0f, 0.0f, 2.0f } });

	// ����
	world_.update(0.0f);
}

void GameScene::update(float delta_time) {
	// tmp scene end
	if (gsGetKeyState(GKEY_LCONTROL) && gsGetKeyTrigger(GKEY_RETURN)) is_end_ = true;

	world_.update(delta_time);

	// TODO tmp scene end func
	{
		if (world_.count_actor_with_tag(ActorTag::Enemy) <= 0) {
			is_end_ = true;
			return;
		}
		if (world_.count_actor_with_tag(ActorTag::Player) <= 0) {
			is_end_ = true;
			return;
		}
	}
}

void GameScene::draw() const {
	world_.draw();	
}

void GameScene::end() {
	world_.clear();
	// Tween�̏I��
	Tween::clear();
	// �S�ẴG�t�F�N�g���~����
	gsStopAllEffects();

	// tmp
	AssetsManager::get_instance().delete_assets("Game");
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
