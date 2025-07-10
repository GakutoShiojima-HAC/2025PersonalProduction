//  File        : World.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/06/25
//  Updated     : 2025/07/10
//  Description : ���[���h�Ǘ��N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef WORLD_H_
#define WORLD_H_

#include "Engine/Core/World/IWorld.h"
#include <gslib.h>
#include "Engine/Core/Actor/ActorManager.h"
#include "Engine/Core/Camera/CameraManager.h"

class World : public IWorld {
public:
	World() = default;

	virtual ~World();

public:
	virtual void update(float delta_time);

	virtual void draw() const;

	virtual void clear();

public:
	/// <summary>
	/// �V���h�E�}�b�v�̕`��p�̊֐�
	/// </summary>
	static void shadow_map_callback(void* param, const GSmatrix4* view, const GSmatrix4* projection);

public:
	/// <summary>
	/// �t�B�[���h�̒ǉ�
	/// </summary>
	/// <param name="field">= �t�B�[���h</param>
	void add_field(Field* field);

	/// <summary>
	/// ���C�g�̒ǉ�
	/// </summary>
	/// <param name="light">= ���C�g</param>
	void add_light(Light* light);

	/// <summary>
	/// �A�N�^�[�̒ǉ�
	/// </summary>
	/// <param name="actor">�A�N�^�[</param>
	void add_actor(Actor* actor);

public:
	Field* get_field() override;

	Light* get_light() override;
	
	void add_camera(Camera* camera) override;;

	Camera* get_camera() const override;

	Camera* find_camera(const CameraTag tag) const override;

	void camera_transition(Camera* to, float time = 0) override;

	void camera_transition(Camera* from, Camera* to, float time = 0) override;

	Actor* find_actor(const string& name) const override;

	vector<Actor*> find_actor_with_tag(const ActorTag tag) const override;
	
	int count_actor() const override;

	int count_actor_with_tag(const ActorTag tag) const override;

protected:
	// �t�B�[���h
	Field* field_{ nullptr };
	// ���C�g
	Light* light_{ nullptr };
	// �A�N�^�[�}�l�[�W���[
	ActorManager actor_;
	// �J�����}�l�[�W���[
	CameraManager camera_;
};

#endif
