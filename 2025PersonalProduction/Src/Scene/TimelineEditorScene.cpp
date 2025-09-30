#include "Scene/TimelineEditorScene.h"
#include <GSeffect.h>
#include "Engine/Core/Assets/AssetsManager.h"
#include "Engine/Core/Assets/AssetsLoader.h"
#include "Engine/Core/Field/Field.h"
#include "Engine/Core/World/Light.h"
#include "Engine/Core/Collision/AttackColliderPool.h"
#include "Engine/Core/Tween/Tween.h"
#include "Engine/Graphics/Shader/GameShader.h"
#include "Engine/Graphics/Shader/GamePostEffect.h"

#include <gslib.h>	// tmp
#include "Camera/FixedCamera.h"	// tmp
#include "Engine/Core/Timeline/Parameters/CameraTimeline.h"	// tmp
#include "Camera/TimelineCamera.h"	// tmp
#include "Camera/PlayerCamera.h"	// tmp
#include "Actor/Player/Player.h"	// tmp
#include "Camera/EditorCamera.h"	// tmp
#include "Actor/Enemy/DummyEnemy.h"	// tmp
#include "Assets.h"	// tmp

#include "Engine/Core/Timeline/Editor/CameraTimelineEditor.h"
#include "Camera/EditorCamera.h"

#include "Engine/Core/NavMesh/NavMeshDebugger.h"

TimelineEditorScene::TimelineEditorScene() {
    next_scene_tag_ = SceneTag::Menu;
}

void TimelineEditorScene::load() {
    // 初期化
    is_load_end_ = false;
    load_progress_ = 0.0f;

    load_data();

    // 終了
    is_load_end_ = true;
    load_progress_ = 1.0f;
}

void TimelineEditorScene::start() {
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

	world_.add_field(new Field{ (GSuint)OctreeID::Mesh, (GSuint)OctreeID::Collider, (GSuint)TextureID::Skybox });
	world_.add_light(new Light{});
	world_.add_attack_collider_pool(new AttackColliderPool{ &world_ });

	// tmp
	world_.add_camera(new FixedCamera{ &world_, GSvector3{ 0.0f, 3.0f, -10.0f }, GSvector3{ 0.0f, 2.0f, 0.0f } });
	world_.add_camera(new TimelineCamera{ &world_ });

	world_.add_camera(new EditorCamera{ &world_ });

	// tmp
	world_.add_actor(new NavMeshDebugger{ &world_ });

	// tmp
	editor_.add(new CameraTimelineEditor{ &world_ });

	// tmp
	PlayerCamera* player_camera = new PlayerCamera{ &world_ };
	world_.add_camera(player_camera);
	world_.add_character(new Player{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f }, GSvector3{ 0.0f, 1.0f, 0.0f }, player_camera });
	world_.camera_transition(player_camera);

	// tmp
	world_.add_pawn(new DummyEnemy{ &world_, GSvector3{ 0.0f, 0.0f, 2.0f } });

    // シェーダーの有効化
    GameShader::get_instance().start();
    // レンダーターゲットの作成
    GamePostEffect::get_instance().start();

	// 同期
	world_.update(0.0f);
}

void TimelineEditorScene::update(float delta_time) {
	// tmp scene end
	if (gsGetKeyState(GKEY_LCONTROL) && gsGetKeyTrigger(GKEY_RETURN)) is_end_ = true;

	world_.update(delta_time);
	editor_.update(delta_time);
}

void TimelineEditorScene::draw() const {
	world_.draw();

	gsDrawText("timeline editor");
}

void TimelineEditorScene::end() {
    // ワールドクリア
	world_.clear();
    // タイムラインエディタクリア
	editor_.clear();
	// Tweenの終了
	Tween::clear();
	// 全てのエフェクトを停止する
	gsStopAllEffects();

    // シェーダーの無効化
    GameShader::get_instance().end();
    // レンダーターゲットの削除
    GamePostEffect::get_instance().end();
	// アセットの開放
	AssetsManager::get_instance().delete_asset("TimelineEditor");

    // 初期化
    is_load_end_ = false;
    load_progress_ = 0.0f;

    // 次のシーンの情報を渡す
    std::any data = next_scene_tag_;
    scene_manager_.send_message(SceneTag::Loading, "NextSceneTag", data);
}


SceneTag TimelineEditorScene::scene_tag() const {
	return SceneTag::TimelineEditor;
}

bool TimelineEditorScene::is_application_end() const {
	return false;
}

void TimelineEditorScene::reception_message(const std::string& message, std::any& param) {
	// なにも受け取らない
}

void TimelineEditorScene::load_data() {
    // 読み込み処理の数から一つの処理分の進捗率を計算
    const int count = 5;
    const float progress = 1.0f / (float)count;

    // アセットの読み込み
    LoadAssets* asset = new LoadAssets{};
    asset->name = "TimelineEditor";
    asset->octree.push_back({ (GSuint)OctreeID::Mesh, "Resource/Assets/Octree/stage.oct" });
    asset->octree.push_back({ (GSuint)OctreeID::Collider, "Resource/Assets/Octree/stage_collider.oct" });
    asset->texture.push_back({ (GSuint)TextureID::Skybox, "Resource/Assets/Skybox/default_skybox.dds" });
    asset->skinmesh.push_back({ (GSuint)MeshID::Player, "Resource/Assets/Skinmesh/Player1/Player.mshb" });
    AssetsManager::get_instance().load_asset(asset);
    load_progress_ += progress;

    // ライトマップの読み込み
    gsLoadLightmap(0, "Resource/Assets/Octree/Stage1/Lightmap/Lightmap.txt");
    load_progress_ += progress;
    // リフレクションプローブの読み込み
    gsLoadReflectionProbe(0, "Resource/Assets/Octree/Stage1/RefProbe/ReflectionProbe.txt");
    load_progress_ += progress;

    // ナビメッシュデータの読み込み
    world_.add_navmesh(new NavMeshSurface{ "Resource/Assets/Octree/navmesh_export.txt" });
    load_progress_ += progress;

    // タイムラインデータの読み込み
    world_.timeline().add(new CameraTimeline{ &world_, "Resource/Private/Timeline/Camera/List/world1.json" });
    load_progress_ += progress;
}
