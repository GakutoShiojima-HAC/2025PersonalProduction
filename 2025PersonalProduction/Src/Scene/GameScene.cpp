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

#define GS_ENABLE_MESH_SHADOW			// メッシュに影を付ける
//#define GS_ENABLE_SKIN_MESH_SHADOW	// スキニングメッシュに影を付ける
//#define GS_ENABLE_SOFT_SHADOW			// 影の輪郭をぼかす
#include <GSstandard_shader.h>

void GameScene::start() {
	is_end_ = false;

	gsInitDefaultShader();
	// 視錐台カリングを有効にする
	gsEnable(GS_FRUSTUM_CULLING);
	// シャドウマップの作成（２枚のカスケードシャドウマップ）
	static const GSuint shadow_map_size[] = { 2048, 2048 };
	gsCreateShadowMap(2, shadow_map_size, GS_TRUE);
	// シャドウマップを適用する距離
	gsSetShadowMapDistance(60.0f);
	// カスケードシャドウのシャドウマップの分割位置を調整
	gsSetShadowMapCascadeLamda(0.7f);
	// シャドウによる光の減衰率を設定(0.0:減衰～1.0:減衰しない)
	gsSetShadowMapAttenuation(0.0f);
	// シャドウマップのポリゴンオフセットを設定する
	gsEnableShadowMapPolygonOffset();
	gsSetShadowMapPolygonOffset(2.5f, 1.0f);

	// 通常フォグの設定
	GScolor4 color = world_.posteffect().fog_color();
	const float fog_color[4]{ color.r, color.g, color.b, color.a };
	const float fog_start{ 10.0f };
	const float fog_end{ 300.0f };
	glFogi(GL_FOG_MODE, GL_LINEAR);     // 線形フォグ
	glFogfv(GL_FOG_COLOR, fog_color);   // フォグの色
	glFogf(GL_FOG_START, fog_start);    // フォグの開始位置（視点からの距離）
	glFogf(GL_FOG_END, fog_end);        // フォグの終了位置（視点からの距離）
	glEnable(GL_FOG);                   // フォグを有効にする

	// ポストエフェクトの初期化
	world_.posteffect().init();

	// ライトマップの読み込み
	gsLoadLightmap(0, "Resource/Assets/Octree/Stage1/Lightmap/Lightmap.txt");
	// リフレクションプローブの読み込み
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

	// 同期
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
	// Tweenの終了
	Tween::clear();
	// 全てのエフェクトを停止する
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
	// なにも受け取らない
}
