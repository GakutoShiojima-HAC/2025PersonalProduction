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
	// ��Ԃ̍X�V
	current_state_->update(delta_time);
}

void StateMachine::draw() const {
	// ��Ԃ̕`��
	current_state_->draw();
}

void StateMachine::draw_gui() const {
	// ��Ԃ�GUI�`��
	current_state_->draw_gui();
}

void StateMachine::add_state(const GSuint state_num, shared_ptr<IState> state) {
	state_list_[state_num] = state;
}

void StateMachine::change_state(const GSuint state_num) {
	// ������ԂȂ�X�V���Ȃ�
	if (current_state_num_ == state_num) return;

	// ��Ԃ�����
	shared_ptr<IState> state = find(state_num);
	if (state == nullptr) return;

	current_state_->exit();			// ���̏�Ԃ��I��
	current_state_ = state;			// ��Ԃ��X�V
	current_state_->enter();		// �V������Ԃ��J�n
	current_state_num_ = state_num;	// �ԍ����X�V
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
	// ��Ԃ�����
	auto it = state_list_.find(state_num);
	// ��Ԃ����݂��Ă�����
	if (it != state_list_.end()) {
		return it->second;
	}

	return nullptr;
}

void StateMachine::clear() {
	state_list_.clear();
}
