#include "Engine/Core/Input/Input.h"

// PAD接続中判定に使用する識別番号
const int USE_PAD_NUM{ 0 };

Input& Input::get_instance() {
	// static変数のインスタンスは１つ
	// インスタンス化も１回のみ
	static Input self;
	return self;
}

void Input::update(float delta_time) {
	// PAD接続があるか更新
	is_pad_ = gsXBoxGetPadCount() > 0;

	// PAD状態の更新
	if (is_pad_) {
		// 左軸
		gsXBoxPadGetLeftAxis(USE_PAD_NUM, &left_axis_);
		// 右軸
		gsXBoxPadGetRightAxis(USE_PAD_NUM, &right_axis_);

		// カーソルは左スティックで移動とする
		if (!is_update_cursor_position_) return;
		cursor_position_.x += left_axis_.x * pad_cursor_speed_ * delta_time;
		cursor_position_.y += -left_axis_.y * pad_cursor_speed_ * delta_time;
	}
	// PC状態の更新
	else {
		// 左軸
		left_axis_ = GSvector2{ 0.0f, 0.0f };
		if (gsGetKeyState(GKEY_W)) left_axis_.y += 1;
		if (gsGetKeyState(GKEY_S)) left_axis_.y += -1;
		if (gsGetKeyState(GKEY_D)) left_axis_.x += 1;
		if (gsGetKeyState(GKEY_A)) left_axis_.x += -1;
		// 右軸
		int x, y;
		gsGetMouseVelocity(&x, &y, nullptr);
		right_axis_.x = x;
		right_axis_.y = -y;

		// カーソル座標
		if (!is_update_cursor_position_) return;
		gsGetMouseCursorPosition(&x, &y);
		cursor_position_.x = x;
		cursor_position_.y = y;
	}
}

const GSvector2& Input::left_axis() {
	return left_axis_;
}

const GSvector2& Input::right_axis() {
	return right_axis_;
}

const GSvector2 Input::debug_left_axis() const {
#ifdef _DEBUG
    GSvector2 left_axis{ 0.0f, 0.0f };
    if (gsGetKeyState(GKEY_W)) left_axis.y += 1;
    if (gsGetKeyState(GKEY_S)) left_axis.y += -1;
    if (gsGetKeyState(GKEY_D)) left_axis.x += 1;
    if (gsGetKeyState(GKEY_A)) left_axis.x += -1;
    return left_axis;
#else
    return GSvector2{ 0.0f, 0.0f };
#endif
}

const GSvector2 Input::debug_right_axis() const {
#ifdef _DEBUG
    int x, y;
    gsGetMouseVelocity(&x, &y, nullptr);
    return GSvector2{ (float)x, (float)-y };
#else
    return GSvector2{ 0.0f, 0.0f };
#endif
}

const GSvector2& Input::cursor_position() {
	return cursor_position_;
}

void Input::reset_cursor_position(const GSvector2& reset_position) {
	cursor_position_ = reset_position;
}

bool& Input::enable_update_cursor_position() {
	return is_update_cursor_position_;
}

float& Input::pad_cursor_speed() {
	return pad_cursor_speed_;
}

bool Input::is_pad() const {
	return is_pad_;
}

bool Input::action(InputAction action) const {
	switch (action) {
	case InputAction::APP_Pause:
		return is_pad_ ? gsXBoxPadButtonTrigger(USE_PAD_NUM, GS_XBOX_PAD_START) : gsGetKeyTrigger(GKEY_TAB);
	case InputAction::APP_ForceEnd:
		return gsGetKeyState(GKEY_ESCAPE) || (gsXBoxPadButtonState(USE_PAD_NUM, GS_XBOX_PAD_BACK) && gsXBoxPadButtonState(0, GS_XBOX_PAD_START));
	case InputAction::MENU_Decision:
		return is_pad_ ? gsXBoxPadButtonTrigger(USE_PAD_NUM, GS_XBOX_PAD_B) : (gsGetMouseButtonTrigger(GMOUSE_BUTTON_1) | gsGetKeyTrigger(GKEY_SPACE));
	case InputAction::MENU_UP:
        return is_pad_ ? gsXBoxPadButtonTrigger(USE_PAD_NUM, GS_XBOX_PAD_UP) : gsGetKeyTrigger(GKEY_W);
	case InputAction::MENU_DOWN:
        return is_pad_ ? gsXBoxPadButtonTrigger(USE_PAD_NUM, GS_XBOX_PAD_DOWN) : gsGetKeyTrigger(GKEY_S);
	case InputAction::MENU_LEFT:
		break;
	case InputAction::MENU_RIGHT:
		break;
	case InputAction::GAME_Attack:
		return is_pad_ ? gsXBoxPadButtonTrigger(USE_PAD_NUM, GS_XBOX_PAD_B) : gsGetMouseButtonTrigger(GMOUSE_BUTTON_1);
	case InputAction::GAME_Jump:
		return is_pad_ ? gsXBoxPadButtonTrigger(USE_PAD_NUM, GS_XBOX_PAD_A) : gsGetKeyTrigger(GKEY_SPACE);
	case InputAction::GAME_Avoid:
		return is_pad_ ? gsXBoxPadButtonTrigger(USE_PAD_NUM, GS_XBOX_PAD_LEFT_SHOULDER) : gsGetKeyTrigger(GKEY_LCONTROL);
	case InputAction::GAME_Skill:
		break;
	case InputAction::GAME_Lockon:
		return is_pad_ ? gsXBoxPadButtonTrigger(USE_PAD_NUM, GS_XBOX_PAD_RIGHT_SHOULDER) : gsGetKeyTrigger(GKEY_R);
	case InputAction::GAME_Sprint:
		break;
	case InputAction::GAME_Interact:
		break;
    case InputAction::GAME_Interact_Up:
        return gsXBoxPadButtonTrigger(USE_PAD_NUM, GS_XBOX_PAD_UP);
    case InputAction::GAME_Interact_Down:
        return gsXBoxPadButtonTrigger(USE_PAD_NUM, GS_XBOX_PAD_DOWN);
    case InputAction::GAME_Inventory:
		break;
	case InputAction::DEBUG_CameraActive:
		return gsGetMouseButtonState(GMOUSE_BUTTON_2);
	case InputAction::DEBUG_Up:
		return gsGetKeyState(GKEY_SPACE);
	case InputAction::DEBUG_Down:
		return gsGetKeyState(GKEY_LSHIFT);
	default:
		return false;
	}

	return false;
}
