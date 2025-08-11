// PC���͂�PAD�ڑ����Ȃ��ꍇ�̂ݓK�p�����
//
//

#ifndef INPUT_H_
#define INPUT_H_

#include <gslib.h>

enum class InputAction {
    APP_Pause,          // �|�[�Y���
    APP_ForceEnd,       // �A�v���P�[�V���������I��

    MENU_Decision,      // ����
    MENU_UP,            // ��
    MENU_DOWN,          // ��
    MENU_LEFT,          // ��
    MENU_RIGHT,         // �E

    GAME_Attack,        // �U��
    GAME_Jump,          // �W�����v
    GAME_Avoid,         // ���
    GAME_Skill,         // �X�L������
    GAME_Lockon,        // ���b�N�I��
    GAME_Sprint,        // ����

    GAME_Interact,      // �C���^���N�g
    GAME_Inventory,     // �C���x���g��

    DEBUG_CameraActive, // �J�����L����
    DEBUG_Up,           // �㏸
    DEBUG_Down,         // ���~


};

class Input {
private:
    // �R���X�g���N�^�i�O������̃C���X�^���X���֎~�j
    Input() = default;

public:
    ~Input() = default;

public:
    /// <summary>
    /// �C���X�^���X���擾���邽�߂�static�����o�֐�
    /// </summary>
    /// <returns>�C���X�^���X</returns>
    static Input& get_instance();

public:
    void update(float delta_time);

public:
    /// <summary>
    /// �����̏�Ԃ�ԋp
    /// </summary>
    /// <returns>1.0f ~ -1.0f</returns>
    const GSvector2& left_axis();

    /// <summary>
    /// �E���̏�Ԃ�ԋp
    /// </summary>
    /// <returns>1.0f ~ -1.0f</returns>
    const GSvector2& right_axis();

    /// <summary>
    /// �J�[�\�����W��ԋp
    /// </summary>
    /// <returns>���W</returns>
    const GSvector2& cursor_position();

    /// <summary>
    /// �J�[�\�����W�����Z�b�g
    /// </summary>
    void reset_cursor_position(const GSvector2& reset_position);

    /// <summary>
    /// �J�[�\�����W�̍X�V�t���O���擾
    /// </summary>
    /// <returns>�^�ōX�V</returns>
    bool& enable_update_cursor_position();

    /// <summary>
    /// PAD��Ԃ̃J�[�\���ړ����x���擾
    /// </summary>
    /// <returns>�ړ����x</returns>
    float& pad_cursor_speed();

public:
    /// <summary>
    /// �w�肵���A�N�V�������s���Ă��邩�ǂ���
    /// </summary>
    /// <param name="action">= �A�N�V����</param>
    /// <returns>�s���Ă���Ȃ�^��ԋp</returns>
    bool action(InputAction action) const;

private:
    // PAD�ڑ������ǂ���
    bool is_pad_{ false };

    // PAD�̍��X�e�B�b�N�EPC��WASD���
    GSvector2 left_axis_;
    // PAD�̉E�X�e�B�b�N�EPC�̃}�E�X���
    GSvector2 right_axis_;

    // �J�[�\�����W�̍X�V���s�����ǂ���
    bool is_update_cursor_position_{ false };
    // �J�[�\�����W
    GSvector2 cursor_position_;
    // PAD��Ԃ̃J�[�\���ړ����x
    float pad_cursor_speed_{ 1.0f };

public:
    // �R�s�[�֎~
    Input(const Input& other) = delete;
    Input& operator = (const Input& other) = delete;

};

#endif
