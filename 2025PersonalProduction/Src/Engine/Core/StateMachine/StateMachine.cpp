#include "Engine/Core/StateMachine/StateMachine.h"
#include "Engine/Core/StateMachine/NullState.h"

static shared_ptr<NullState> null_state_ = make_shared<NullState>();

StateMachine::StateMachine() :
	current_state_{ null_state_ } {
}

StateMachine::~StateMachine() {
	clear();
}

void StateMachine::update(float delta_time) {
	// 状態の更新
	current_state_->update(delta_time);
}

void StateMachine::draw() const {
	// 状態の描画
	current_state_->draw();
}

void StateMachine::draw_gui() const {
	// 状態のGUI描画
	current_state_->draw_gui();
}

void StateMachine::add_state(const GSuint state_num, shared_ptr<IState> state) {
	state_list_[state_num] = state;
}

void StateMachine::change_state(const GSuint state_num) {
	// 同じ状態なら更新しない
	if (current_state_num_ == state_num) return;

	// 状態を検索
	shared_ptr<IState> state = find(state_num);
	if (state == nullptr) return;

	current_state_->exit();			// 元の状態を終了
	current_state_ = state;			// 状態を更新
	current_state_->enter();		// 新しい状態を開始
	current_state_num_ = state_num;	// 番号を更新
}

bool StateMachine::is_current_state(const GSuint state_num) const {
	return state_num == current_state_num_;
}

GSuint StateMachine::get_current_state() const {
	return current_state_num_;
}

IState* StateMachine::get_state(const GSuint state_num) const {
	shared_ptr<IState> state = find(state_num);
	if (state == nullptr) return nullptr;
	
	return state.get();
}

shared_ptr<IState> StateMachine::find(const GSuint state_num) const {
	// 状態を検索
	auto it = state_list_.find(state_num);
	// 状態が存在していたら
	if (it != state_list_.end()) {
		return it->second;
	}

	return nullptr;
}

void StateMachine::clear() {
	state_list_.clear();
}
