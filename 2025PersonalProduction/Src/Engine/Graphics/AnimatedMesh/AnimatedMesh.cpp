#include "Engine/Graphics/AnimatedMesh/AnimatedMesh.h"
#include <algorithm>
#include <cmath>
#include <GStransform.h>

// 補間フレーム数
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
    // アニメーションレイヤーを更新
    for (auto& layer : animation_layers_) {
        layer.update(delta_time);
    }
}

void AnimatedMesh::draw() const {
    // スケルトンの自動計算を無効にする
    gsDisable(GS_CALC_SKELETON);
    // スケルトンのバインド
    gsBindSkeleton(skeleton_);
    // ボーンのワールド変換行列を設定
    gsSetMatrixSkeleton(bone_matrices_.data());

    glPushMatrix();
    // 座標変換行列を設定する
    glMultMatrixf(transform_);
    // メッシュの描画
    gsDrawMesh(mesh_);
    glPopMatrix();

    // スケルトンの自動計算を有効にする
    gsEnable(GS_CALC_SKELETON);
}

void AnimatedMesh::change_motion(GSuint motion, bool loop) {
    for (auto& layer : animation_layers_) {
        layer.change_motion(motion, loop);
        layer.current_motion_time(0.0f);    // 再生時間を一律にする(レイヤー機能で既に再生中の場合があるため)
    }
}

void AnimatedMesh::change_motion(GSuint layer, GSuint motion, bool loop) {
    animation_layers_[layer].change_motion(motion, loop);

    // 再生時間を同期
    sync_layers_playing_same_motion(layer);
}

void AnimatedMesh::set_layer_indices(GSuint layer_index, const GSuint layer_bones[], GSuint layer_bones_count) {
    for (GSuint i = 0; i < layer_bones_count; ++i) {
        animation_layer_indices_[layer_bones[i]] = layer_index;
    }
}

void AnimatedMesh::transform(const GSmatrix4& matrix) {
    // 座標変換行列を更新
    transform_ = matrix;
    // 各レイヤーからローカル変換行列を取得
    for (GSuint i = 0; i < gsGetSkeletonNumBones(skeleton_); ++i) {
        local_bone_matrices_[i] = animation_layers_[animation_layer_indices_[i]].local_bone_matrix(i);
    }
    // ボーンの変換行列を計算
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

    // 再生時間を同期
    sync_layers_playing_same_motion(layer);
}

void AnimatedMesh::apply_root_motion(GStransform& transform) {
    // アニメーションクラスのroot_motionを呼び出す
    if (!animation_layers_.empty()) {
        animation_layers_[0].root_motion(transform);
    }
}

void AnimatedMesh::add_animation_event(GSuint motion, GSfloat time, function<void()> callback) {
    // レイヤーなし状態で必ず再生される0番に登録
    animation_layers_[0].add_event(motion, time, callback);
}

void AnimatedMesh::add_animation_event(GSuint layer, GSuint motion, GSfloat time, function<void()> callback) {
    animation_layers_[layer].add_event(motion, time, callback);
}

void AnimatedMesh::sync_layers_playing_same_motion(GSuint check_layer) {
    // 他のレイヤーが同じモーションを再生しているか判定
    vector<GSuint> layers;
    if (is_multiple_layers_playing_same_motion(check_layer, layers)) {
        // 他のレイヤーのモーション再生時間を同期させる
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
        // 同じモーションなら登録する
        if (animation_layers_[i].motion_num() == current) layers.push_back(i);
    }
    // 登録数が0なら存在しない
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
    // 前回のモーションタイマを保存
    prev_timer_ = motion_timer_;
    // アニメーションタイマの更新
    motion_timer_ += delta_time;
    if (motion_loop_) {
        // モーションタイマをループさせる
        motion_timer_ = fmod(motion_timer_, motion_end_time());
    }
    else {
        // モーションタイマをクランプする
        motion_timer_ = min(motion_timer_, motion_end_time() - 1.0f);
    }
    // 補間中タイマの更新
    lerp_timer_ = min(lerp_timer_ + delta_time, cLerpTime);

    // モーションがループしたかどうか。
    // 現在の時間が1フレーム前の時間より小さい場合は、ループしたと判断できる。
    bool looped = motion_timer_ < prev_timer_;
    // 全イベントをチェックし、必要であればイベントを発行する
    for (const auto& event : events_) {
        // 現在のモーションがイベント対象のモーションでなければ、何もしない
        if (event->motion_ != motion_) continue;

        if (looped) {
            if (prev_timer_ < event->time_ || event->time_ <= motion_timer_) {
                event->callback_(); // コールバック処理を呼び出す
            }
        }
        else {
            if (prev_timer_ < event->time_ && event->time_ <= motion_timer_) {
                event->callback_(); // コールバック処理を呼び出す
            }
        }
    }
}

void AnimatedMesh::Animation::change_motion(GSuint motion, bool loop) {
    // 現在と同じモーションの場合は何もしない
    if (motion_ == motion) return;
    // 補間中の前半は、前回のモーションを更新をしないようにする
    if (lerp_timer_ > (cLerpTime * 0.5f)) {
        // 前回のモーション番号とモーション時間を保存
        prev_motion_ = motion_;
        prev_motion_timer_ = motion_timer_;
        // 補間中タイマの初期化 
        lerp_timer_ = 0.0f;
    }
    // モーションの更新
    motion_ = motion;
    // モーションタイマの初期化
    motion_timer_ = 0.0f;
    // ループフラグの設定
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
    // ループモーションは終了しない
    if (motion_loop_) return false;
    // 終了しているか？
    return motion_timer_ >= (motion_end_time() - 1.0f);
}

GSmatrix4 AnimatedMesh::Animation::local_bone_matrix(GSuint bone_no) const {
    GSmatrix4 result;
    // 補間付きアニメーション変換行列を計算
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
    // 移動量と回転の差分を取得する
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

    // ローカル座標系で回転させる
    transform.rotate(rotation);
    // 平行移動量にはスケーリングを考慮
    GSvector3 scale = transform.localScale();
    // ローカル座標系で平行移動する
    transform.translate(GSvector3::scale(velocity, scale));
}

void AnimatedMesh::Animation::add_event(GSuint motion, GSfloat time, function<void()> callback) {    
    events_.push_back(std::make_shared<AnimationEvent>(motion, time, callback));
}
