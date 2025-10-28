#include "Scene/MenuScene.h"
#include "Engine/Core/Assets/AssetsManager.h"
#include "Engine/Core/Assets/AssetsLoader.h"
#include "Assets.h"
#include "Engine/Graphics/Canvas/Canvas.h"
#include "Engine/Core/Tween/Tween.h"

#ifdef _DEBUG
#include <imgui/imgui.h>
#endif

#include "State/Scene/SceneState.h"
#include "State/Scene/SceneOriginalState.h"
#include "State/Scene/SceneSettingState.h"
#include "State/Scene/SceneMenuState.h"

MenuScene::MenuScene() {
    add_state();
}

void MenuScene::load() {
    // ������
    is_load_end_ = false;
    load_progress_ = 0.0f;

    load_data();

    // �I��
    is_load_end_ = true;
    load_progress_ = 1.0f;
}

void MenuScene::start() {
	is_end_ = false;
    is_app_end_ = false;
    next_scene_tag_ = SceneTag::Game;

    change_state((GSuint)SceneStateType::MenuScene);
}

void MenuScene::update(float delta_time) {
    state_.update(delta_time);

#ifdef _DEBUG
    ImGui::Begin("Scene Select Window");
    if (ImGui::Button("TimelineEditorScene")) {
        is_end_ = true;
        next_scene_tag_ = SceneTag::TimelineEditor;
    }
    ImGui::End();
#endif
}

void MenuScene::draw() const {
    state_.draw();
}

void MenuScene::end() {
	// Tween�̏I��
	Tween::clear();

	// �A�Z�b�g�̊J��
	AssetsManager::get_instance().delete_asset(AssetsLoader::MENU_ASSET_NAME);

    // ������
    is_load_end_ = false;
    load_progress_ = 0.0f;

    // ���̃V�[���̏���n��
    std::any data = next_scene_tag_;
    scene_manager_.send_message(SceneTag::Loading, "NextSceneTag", data);
}

SceneTag MenuScene::scene_tag() const {
	return SceneTag::Menu;
}

void MenuScene::reception_message(const std::string& message, std::any& param) {
	// �Ȃɂ��󂯎��Ȃ�
}

void MenuScene::add_state() {
    state_.add_state((GSuint)SceneStateType::Original, make_shared<SceneOriginalState>(*this));
    state_.add_state((GSuint)SceneStateType::Setting, make_shared<SceneSettingState>(*this, SceneStateType::MenuScene));
    state_.add_state((GSuint)SceneStateType::MenuScene, make_shared<SceneMenuState>(*this));
}

void MenuScene::original_update(float delta_time) {
    // ���j���[�V�[���ł̓I���W�i���X�e�[�g���g��Ȃ��̂őJ��
    if (state_.get_current_state() == (GSuint)SceneStateType::Original) change_state((GSuint)SceneStateType::MenuScene);
}

void MenuScene::original_draw() const {
    // TODO �w�i
}

void MenuScene::load_data() {
    // �ǂݍ��ݏ����̐������̏������̐i�������v�Z
    const int count = 1;
    const float progress = 1.0f / (float)count;

    // �A�Z�b�g�̓ǂݍ���
    AssetsLoader::load_by_json("Resource/Private/Common/Assets/menu.json", AssetsLoader::MENU_ASSET_NAME);
    load_progress_ += progress;
}
