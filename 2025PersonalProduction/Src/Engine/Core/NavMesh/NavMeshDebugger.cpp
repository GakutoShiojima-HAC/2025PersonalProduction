#include "Engine/Core/NavMesh/NavMeshDebugger.h"
#include "Engine/Core/World/IWorld.h"

#ifdef _DEBUG
#include <imgui/imgui.h>
#include "Engine/Utils/DebugMarker.h"
#endif

NavMeshDebugger::NavMeshDebugger(IWorld* world) {
	world_ = world;
	tag_ = ActorTag::None;
	name_ = "NavMeshDebugger";

	navmesh_ = { this, world_->navmesh() };
}

void NavMeshDebugger::update(float delta_time) {
	update_move(delta_time);

	ImGui::Begin("NavMesh Window");

	ImGui::Text("NavMesh Debugger Option");

	if (ImGui::Button("Search Path")) navmesh_.find_path(start_, end_);
	
	ImGui::SameLine();
	if (ImGui::Button("Move Start")) move_start();

	ImGui::DragFloat3("start position", start_, 0.1f);
	ImGui::DragFloat3("end position", end_, 0.1f);

	if (ImGui::Button("Swap start and end position")) {
		GSvector3 tmp = start_;
		start_ = end_;
		end_ = tmp;
	}

#ifdef _DEBUG
	if (ImGui::Button("Toggle Draw Path Option")) draw_line_path_ = !draw_line_path_;
#endif

	ImGui::Separator();

	ImGui::End();
}

void NavMeshDebugger::draw() const {
#ifdef _DEBUG
	if (draw_line_path_) navmesh_.draw_line_path();
	else navmesh_.draw_path();
#else
	navmesh_.draw_path();
#endif

	// �o�H�T���̌��ʂŐF��ς���
	const GScolor color = navmesh_.found_path() ? GScolor{ 1.0f, 1.0f, 1.0f, 1.0f } : GScolor{ 1.0f, 0.0f, 0.0f, 1.0f };

	// �J�n�n�_��`��
	MyLib::draw_sphere(start_, 0.1f, color);
	// �I���n�_��`��
	MyLib::draw_sphere(end_, 0.1f, color);

	// ���g�̍��W��`��
	GSvector3 position = transform_.position();
	position.y += 1.0f;
	MyLib::draw_sphere(position, 0.25f, color);
}

void NavMeshDebugger::move_start() {
	if (!navmesh_.found_path()) return;

	// ������
	navmesh_.reset_move();
	is_move_waiting_ = false;
}

void NavMeshDebugger::update_move(float delta_time) {
	if (is_move_waiting_) return;

	const float MOVE_SPEED{ 0.085f };
	const float ROTATE_ANGLE{ 2.0f };
	navmesh_.update_move(delta_time, MOVE_SPEED, ROTATE_ANGLE);

	if (navmesh_.end_move()) is_move_waiting_ = true;
}
