#include "Scene/GameScene.h"
#include <GSeffect.h>
#include "Engine/Core/Assets/AssetsManager.h"
#include "Engine/Core/Field/Field.h"
#include "Engine/Core/World/Light.h"

#include <gslib.h>	// tmp
#include "Camera/FixedCamera.h"	// tmp
#include "Engine/Core/Timeline/Parameters/CameraTimeline.h"	// tmp
#include "Camera/TimelineCamera.h"	// tmp
#include "Camera/PlayerCamera.h"	// tmp
#include "Actor/Player/Player.h"	// tmp
#include "Camera/EditorCamera.h"	// tmp

#define GS_ENABLE_MESH_SHADOW			// メッシュに影を付ける
//#define GS_ENABLE_SKIN_MESH_SHADOW	// スキニングメッシュに影を付ける
//#define GS_ENABLE_SOFT_SHADOW			// 影の輪郭をぼかす
#include <GSstandard_shader.h>

void GameScene::start() {
	is_end_ = false;

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

	// tmp
	LoadAssets* asset = new LoadAssets{};
	asset->name = "Game";
	asset->octree.push_back({ 0, "Resource/Assets/Octree/stage.oct" });
	asset->octree.push_back({ 1, "Resource/Assets/Octree/stage_collider.oct" });
	asset->texture.push_back({ 0, "Resource/Assets/Skybox/default_skybox.dds" });
	AssetsManager::get_instance().load_assets(asset);

	world_.add_field(new Field{ 0, 1, 0 });
	world_.add_light(new Light{});
	
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

	// 同期
	world_.update(0.0f);
}

void GameScene::update(float delta_time) {
	// tmp scene end
	if (gsGetKeyTrigger(GKEY_L)) is_end_ = true;

	// tmp timeline play
	if (gsGetKeyTrigger(GKEY_T)) world_.timeline().camera_timeline()->play("test");

	world_.update(delta_time);
}

void GameScene::draw() const {
	world_.draw();
	
	gsDrawText("game");
}

void GameScene::end() {
	world_.clear();

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
