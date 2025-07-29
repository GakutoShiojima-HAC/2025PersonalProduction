#include "Engine/Core/Timeline/Parameters/CameraTimeline.h"
#include "Engine/Core/World/World.h"
#include "Engine/Core/Actor/Actor.h"
#include "Lib/json.hpp"
#include "Engine/Core/Camera/Camera.h"
#include "Engine/Utils/Check.h"

using json = nlohmann::json;

CameraTimeline::CameraTimeline(World* world, const string& list_json) {
	world_ = world;

	ifstream file(list_json);
	if (!file.is_open()) return;
	json j;
	file >> j;

	// 識別名とタイムラインデータファイルパスを渡して読み込む
	for (auto it = j.begin(); it != j.end(); ++it) {
		add(it.key(), load(it.value()));
	}
}

void CameraTimeline::update(float delta_time) {
	if (!is_playing_) {
		return;
	}
	if (current_ == nullptr || camera_ == nullptr) {
		end();
		return;
	}

	// playの後にupdateが呼ばれる都合上先に処理
	CameraKeyFrame* current{ nullptr };
	MyLib::is_valid_index(current_->get(), play_frame_, &current);
	CameraKeyFrame* next{ nullptr };
	MyLib::is_valid_index(current_->get(), play_frame_ + 1, &next);

	// 次のキーフレームがなくなったら終了
	if (current == nullptr || next == nullptr) {
		end();
		return;
	}
	// 最初のキーフレームを再生中かつ最初のキーフレーム開始時間が始まっていなかったらタイマーを更新して終了
	if (play_frame_ == 0 && current->time > timer_) {
		timer_ += delta_time / cFPS;
		if (current->time <= timer_) {
			world_->camera_transition(camera_, current_->start_transition_time());
		}
		return;
	}
	// 開始フレームならカメラを切り替える
	if (play_frame_ == 0 && current->time >= timer_) {
		world_->camera_transition(camera_, current_->start_transition_time());
	}

	// カメラパラメータを更新
	const float progress = CLAMP((timer_ - current->time) / (next->time - current->time), 0.0f, 1.0f);
	Actor* current_target = world_->find_actor(current->target);
	Actor* next_target = world_->find_actor(next->target);

	auto get_position = [](Actor* target, const GSvector3& local_position) {
		if (target == nullptr) return local_position;
		// 指定されたTranslate, Rotation, Scaleの行列を作成する
		GSmatrix4 local_matrix = GSmatrix4::TRS(local_position, GSquaternion::euler(GSvector3{ 0.0f, 0.0f, 0.0f }), GSvector3{ 1.0f, 1.0f, 1.0f });
		return (local_matrix * target->transform().localToWorldMatrix()).position();
	};

	// 座標
	const GSvector3 current_position = get_position(current_target, current->position);
	const GSvector3 next_position = get_position(next_target, next->position);
	const GSvector3 position = GSvector3::lerp(current_position, next_position, progress);
	camera_->transform().position(position);
	// 注視点
	const GSvector3 current_lookat = get_position(current_target, current->lookat);
	const GSvector3 next_lookat = get_position(next_target, next->lookat);
	const GSvector3 lookat = GSvector3::lerp(current_lookat, next_lookat, progress);
	camera_->transform().lookAt(lookat, GSvector3::up());
	// 角度
	auto lerp = [](float a, float b, float t) { return a + (b - a) * t; };
	camera_->transform().rotation(camera_->transform().rotation() * GSquaternion::angleAxis(lerp(current->angle, next->angle, progress), GSvector3::forward()));

	// 再生キーフレームを更新
	timer_ += delta_time / cFPS;
	if (progress >= 1.0f) {
		++play_frame_;
	}
}

void CameraTimeline::clear() {
	end();
	for (auto i : timelines_) {
		i.second->clear();
		delete i.second;
		i.second = nullptr;
	}
	timelines_.clear();
}

void CameraTimeline::play(const string& name) {
	// 再生中なら一度終了
	if (is_playing_) end();
	// 再生するタイムラインデータを取得
	current_ = find(name);
	// 見つからなければ再生せず終了
	if (current_ == nullptr) {
		end();
		return;
	}

	// 初期化
	is_playing_ = true;
	play_frame_ = 0;
	timer_ = 0.0f;
	prev_camera_ = world_->get_camera();
	camera_ = world_->find_camera(CameraTag::Timeline);
}

void CameraTimeline::stop() {
	is_playing_ = false;
}

void CameraTimeline::end() {
	if (prev_camera_ != nullptr) world_->camera_transition(prev_camera_, current_ != nullptr ? current_->end_transition_time() : 0.0f);
	stop();
	current_ = nullptr;
	camera_ = nullptr;
	prev_camera_ = nullptr;
}

CameraTimeline::CameraTimelineData* CameraTimeline::load(const string& load_json_path) {
	ifstream file(load_json_path);
	if (!file.is_open()) return;
	json j;
	file >> j;

	// エラーの出ないタイム取得
	auto get_time = [](const json& json, const string& key) {
		return json.contains(key) && json[key].is_number() ? json[key].get<float>() : 0.0f;
	};

	// タイムライン用カメラに遷移するまでの時間
	const float start = get_time(j, "start");
	// 元のカメラに遷移するまでの時間
	const float end = get_time(j, "end");
	// タイムラインデータ
	vector<CameraKeyFrame*> data;
	if (j.contains("timeline") && j["timeline"].is_array()) {
		for (const auto& item : j["timeline"]) {
			CameraKeyFrame* keyframe = new CameraKeyFrame{
			item["time"],
			item["target"],
			GSvector3{ item["position"][0], item["position"][1], item["position"][2] },
			GSvector3{ item["lookat"][0], item["lookat"][1], item["lookat"][2] },
			item["angle"]
			};
			data.push_back(keyframe);
		}
	}

	return new CameraTimelineData{ data, start, end };
}

void CameraTimeline::add(const string& name, CameraTimelineData* data) {
	if (data == nullptr) return;

	// 既に存在するかどうか
	CameraTimelineData* current_data = find(name);
	// 存在しない
	if (current_data == nullptr) {
		timelines_[name] = data;
	}
	// 存在する
	else {
		// 再生中なら終了
		if (current_ == current_data) end();
		current_data->clear();	// 既存のを消去	
		timelines_[name] = data;
	}
}

CameraTimeline::CameraTimelineData* CameraTimeline::find(const string& name) {
	auto it = timelines_.find(name);
	if (it != timelines_.end()) {
		return it->second;
	}
	return nullptr;
}

CameraTimeline::CameraTimelineData::CameraTimelineData(const vector<CameraKeyFrame*>& timeline, float start_transition_time, float end_transition_time) {
	timeline_ = timeline;
	start_transition_time_ = start_transition_time;
	end_transition_time_ = end_transition_time;
}

CameraTimeline::CameraTimelineData::~CameraTimelineData() {
	clear();
}

void CameraTimeline::CameraTimelineData::clear() {
	for (auto i : timeline_) {
		delete i;
		i = nullptr;
	}
	timeline_.clear();
}

const vector<CameraTimeline::CameraKeyFrame*>& CameraTimeline::CameraTimelineData::get() {
	return timeline_;
}

float CameraTimeline::CameraTimelineData::start_transition_time() const {
	return start_transition_time_;
}

float CameraTimeline::CameraTimelineData::end_transition_time() const {
	return end_transition_time_;
}
