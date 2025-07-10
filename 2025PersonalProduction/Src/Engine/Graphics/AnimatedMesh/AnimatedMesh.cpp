#include "Engine/Graphics/AnimatedMesh/AnimatedMesh.h"
#include <algorithm>
#include <cmath>
#include <GStransform.h>

// ��ԃt���[����
const GSfloat cLerpTime{ 10.0f };

AnimatedMesh::AnimatedMesh(GSuint mesh, GSuint skeleton, GSuint animation, GSuint motion, bool loop, GSuint layer_count, GSuint bone_count) :
    mesh_{ mesh },
    skeleton_{ skeleton },
    animation_layers_(layer_count, Animation{ animation, motion, loop }),
    animation_layer_indices_(bone_count, 0),
    local_bone_matrices_(bone_count, GS_MATRIX4_IDENTITY),
    bone_matrices_(bone_count, GS_MATRIX4_IDENTITY),
    transform_{ GS_MATRIX4_IDENTITY } {
}

void AnimatedMesh::update(float delta_time) {
    // �A�j���[�V�������C���[���X�V
    for (auto& layer : animation_layers_) {
        layer.update(delta_time);
    }
}

void AnimatedMesh::draw() const {
    // �X�P���g���̎����v�Z�𖳌��ɂ���
    gsDisable(GS_CALC_SKELETON);
    // �X�P���g���̃o�C���h
    gsBindSkeleton(skeleton_);
    // �{�[���̃��[���h�ϊ��s���ݒ�
    gsSetMatrixSkeleton(bone_matrices_.data());

    glPushMatrix();
    // ���W�ϊ��s���ݒ肷��
    glMultMatrixf(transform_);
    // ���b�V���̕`��
    gsDrawMesh(mesh_);
    glPopMatrix();

    // �X�P���g���̎����v�Z��L���ɂ���
    gsEnable(GS_CALC_SKELETON);
}

void AnimatedMesh::change_motion(GSuint motion, bool loop) {
    for (auto& layer : animation_layers_) {
        layer.change_motion(motion, loop);
        layer.current_motion_time(0.0f);    // �Đ����Ԃ��ꗥ�ɂ���(���C���[�@�\�Ŋ��ɍĐ����̏ꍇ�����邽��)
    }
}

void AnimatedMesh::change_motion(GSuint layer, GSuint motion, bool loop) {
    animation_layers_[layer].change_motion(motion, loop);

    // �Đ����Ԃ𓯊�
    sync_layers_playing_same_motion(layer);
}

void AnimatedMesh::set_layer_indices(GSuint layer_index, const GSuint layer_bones[], GSuint layer_bones_count) {
    for (GSuint i = 0; i < layer_bones_count; ++i) {
        animation_layer_indices_[layer_bones[i]] = layer_index;
    }
}

void AnimatedMesh::transform(const GSmatrix4& matrix) {
    // ���W�ϊ��s����X�V
    transform_ = matrix;
    // �e���C���[���烍�[�J���ϊ��s����擾
    for (GSuint i = 0; i < gsGetSkeletonNumBones(skeleton_); ++i) {
        local_bone_matrices_[i] = animation_layers_[animation_layer_indices_[i]].local_bone_matrix(i);
    }
    // �{�[���̕ϊ��s����v�Z
    gsBindSkeleton(skeleton_);
    gsCalculateSkeleton(NULL, local_bone_matrices_.data(), bone_matrices_.data());
}

float AnimatedMesh::motion_end_time(GSuint layer) const {
    return animation_layers_[layer].motion_end_time();
}

bool AnimatedMesh::is_motion_end(GSuint layer) const {
    return animation_layers_[layer].is_motion_end();
}

GSmatrix4 AnimatedMesh::bone_matrices(int bone_no) const {
    return bone_matrices_[bone_no] * transform_;
}

GSmatrix4 AnimatedMesh::bone_local_matrices(int bone_no) const {
    return bone_matrices_[bone_no];
}

float AnimatedMesh::current_motion_time(GSuint layer) const {
    return animation_layers_[layer].current_motion_time();
}

void AnimatedMesh::current_motion_time(float time, GSuint layer) {
    animation_layers_[layer].current_motion_time(time);

    // �Đ����Ԃ𓯊�
    sync_layers_playing_same_motion(layer);
}

void AnimatedMesh::apply_root_motion(GStransform& transform) {
    // �A�j���[�V�����N���X��root_motion���Ăяo��
    if (!animation_layers_.empty()) {
        animation_layers_[0].root_motion(transform);
    }
}

void AnimatedMesh::add_animation_event(GSuint motion, GSfloat time, function<void()> callback) {
    // ���C���[�Ȃ���ԂŕK���Đ������0�Ԃɓo�^
    animation_layers_[0].add_event(motion, time, callback);
}

void AnimatedMesh::add_animation_event(GSuint layer, GSuint motion, GSfloat time, function<void()> callback) {
    animation_layers_[layer].add_event(motion, time, callback);
}

void AnimatedMesh::sync_layers_playing_same_motion(GSuint check_layer) {
    // ���̃��C���[���������[�V�������Đ����Ă��邩����
    vector<GSuint> layers;
    if (is_multiple_layers_playing_same_motion(check_layer, layers)) {
        // ���̃��C���[�̃��[�V�����Đ����Ԃ𓯊�������
        const float time = animation_layers_[check_layer].current_motion_time();
        for (const auto& layer_num : layers) {
            animation_layers_[layer_num].current_motion_time(time);
        }
    }
}

bool AnimatedMesh::is_multiple_layers_playing_same_motion(GSuint check_layer, vector<GSuint>& layers) const {
    const GSuint current = animation_layers_[check_layer].motion_num();
    
    for (GSuint i = 0; i < (GSuint)animation_layers_.size(); ++i) {
        if (i == check_layer) continue;
        // �������[�V�����Ȃ�o�^����
        if (animation_layers_[i].motion_num() == current) layers.push_back(i);
    }
    // �o�^����0�Ȃ瑶�݂��Ȃ�
    return layers.size() > 0;
}

AnimatedMesh::Animation::Animation(GSuint animation, GSuint motion, bool loop) :
    animation_{ animation },
    motion_{ motion },
    motion_timer_{ 0.0f },
    prev_timer_{ 0.0f },
    motion_loop_{ loop },
    prev_motion_{ motion },
    prev_motion_timer_{ 0.0f },
    lerp_timer_{ 0.0f } {
}

void AnimatedMesh::Animation::update(float delta_time) {
    // �O��̃��[�V�����^�C�}��ۑ�
    prev_timer_ = motion_timer_;
    // �A�j���[�V�����^�C�}�̍X�V
    motion_timer_ += delta_time;
    if (motion_loop_) {
        // ���[�V�����^�C�}�����[�v������
        motion_timer_ = fmod(motion_timer_, motion_end_time());
    }
    else {
        // ���[�V�����^�C�}���N�����v����
        motion_timer_ = min(motion_timer_, motion_end_time() - 1.0f);
    }
    // ��Ԓ��^�C�}�̍X�V
    lerp_timer_ = min(lerp_timer_ + delta_time, cLerpTime);

    // ���[�V���������[�v�������ǂ����B
    // ���݂̎��Ԃ�1�t���[���O�̎��Ԃ�菬�����ꍇ�́A���[�v�����Ɣ��f�ł���B
    bool looped = motion_timer_ < prev_timer_;
    // �S�C�x���g���`�F�b�N���A�K�v�ł���΃C�x���g�𔭍s����
    for (const auto& event : events_) {
        // ���݂̃��[�V�������C�x���g�Ώۂ̃��[�V�����łȂ���΁A�������Ȃ�
        if (event->motion_ != motion_) continue;

        if (looped) {
            if (prev_timer_ < event->time_ || event->time_ <= motion_timer_) {
                event->callback_(); // �R�[���o�b�N�������Ăяo��
            }
        }
        else {
            if (prev_timer_ < event->time_ && event->time_ <= motion_timer_) {
                event->callback_(); // �R�[���o�b�N�������Ăяo��
            }
        }
    }
}

void AnimatedMesh::Animation::change_motion(GSuint motion, bool loop) {
    // ���݂Ɠ������[�V�����̏ꍇ�͉������Ȃ�
    if (motion_ == motion) return;
    // ��Ԓ��̑O���́A�O��̃��[�V�������X�V�����Ȃ��悤�ɂ���
    if (lerp_timer_ > (cLerpTime * 0.5f)) {
        // �O��̃��[�V�����ԍ��ƃ��[�V�������Ԃ�ۑ�
        prev_motion_ = motion_;
        prev_motion_timer_ = motion_timer_;
        // ��Ԓ��^�C�}�̏����� 
        lerp_timer_ = 0.0f;
    }
    // ���[�V�����̍X�V
    motion_ = motion;
    // ���[�V�����^�C�}�̏�����
    motion_timer_ = 0.0f;
    // ���[�v�t���O�̐ݒ�
    motion_loop_ = loop;
}

GSuint AnimatedMesh::Animation::motion_num() const {
    return motion_;
}

float AnimatedMesh::Animation::motion_end_time() const {
    return gsGetEndAnimationTime(animation_, motion_);
}

float AnimatedMesh::Animation::current_motion_time() const {
    return motion_timer_;
}

void AnimatedMesh::Animation::current_motion_time(float time) {
    motion_timer_ = time;
}

bool AnimatedMesh::Animation::is_motion_end() const {
    // ���[�v���[�V�����͏I�����Ȃ�
    if (motion_loop_) return false;
    // �I�����Ă��邩�H
    return motion_timer_ >= (motion_end_time() - 1.0f);
}

GSmatrix4 AnimatedMesh::Animation::local_bone_matrix(GSuint bone_no) const {
    GSmatrix4 result;
    // ��ԕt���A�j���[�V�����ϊ��s����v�Z
    gsCalculateBoneAnimationLerp(
        animation_, 
        prev_motion_, 
        prev_motion_timer_,
        animation_, 
        motion_, 
        motion_timer_,
        bone_no,
        lerp_timer_ / cLerpTime,
        &result
    );
    return result;
}

GSuint AnimatedMesh::Animation::bone_count() const {
    return gsGetAnimationNumBones(animation_, motion_);
}

void AnimatedMesh::Animation::root_motion(GStransform& transform) {
    // �ړ��ʂƉ�]�̍������擾����
    GSvector3 velocity;
    GSquaternion rotation;
    gsGetAnimationKeyFrameDelta(
        animation_, 
        motion_, 
        0,
        motion_timer_, 
        prev_timer_,
        &rotation, 
        &velocity
    );

    // ���[�J�����W�n�ŉ�]������
    transform.rotate(rotation);
    // ���s�ړ��ʂɂ̓X�P�[�����O���l��
    GSvector3 scale = transform.localScale();
    // ���[�J�����W�n�ŕ��s�ړ�����
    transform.translate(GSvector3::scale(velocity, scale));
}

void AnimatedMesh::Animation::add_event(GSuint motion, GSfloat time, function<void()> callback) {    
    events_.push_back(std::make_shared<AnimationEvent>(motion, time, callback));
}
