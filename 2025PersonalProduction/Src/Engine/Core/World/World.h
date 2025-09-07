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
#include "Engine/Core/Actor/Pawn/PawnManager.h"
#include "Engine/Core/Actor/Pawn/Character/CharacterManager.h"
#include "Engine/Core/Camera/CameraManager.h"
#include "Engine/Core/Timeline/TimelineManager.h"
#include "Engine/Core/NavMesh/NavMeshSurface.h"
#include "Engine/Graphics/PostEffect/PostEffect.h"

class AttackColliderPool;

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
	/// �i�r���b�V���̒ǉ�
	/// </summary>
	/// <param name="navmesh">= �i�r���b�V��</param>
	void add_navmesh(NavMeshSurface* navmesh);

	/// <summary>
	/// �A�N�^�[�̒ǉ�
	/// </summary>
	/// <param name="actor">= �A�N�^�[</param>
	void add_actor(Actor* actor);

	/// <summary>
	/// �|�[���̒ǉ�
	/// </summary>
	/// <param name="pawn">= �|�[��</param>
	void add_pawn(Pawn* pawn);

	/// <summary>
	/// �L�����N�^�[�̒ǉ�
	/// </summary>
	/// <param name="character">= �L�����N�^�[</param>
	void add_character(Character* character);

	/// <summary>
	/// �U������v�[���̒ǉ�
	/// </summary>
	/// <param name="pool">= �U������v�[��</param>
	void add_attack_collider_pool(AttackColliderPool* pool);

	/// <summary>
	/// �|�X�g�G�t�F�N�g�ւ̃A�N�Z�X
	/// </summary>
	/// <returns>�Q��</returns>
	PostEffect& posteffect();

public:
	Field* get_field() override;

	Light* get_light() override;
	
	void add_camera(Camera* camera) override;;

	Camera* get_camera() const override;

	Camera* find_camera(const CameraTag tag) const override;

	void camera_transition(Camera* to, float time = 0) override;

	void camera_transition(Camera* from, Camera* to, float time = 0) override;

	NavMeshSurface* navmesh() override;

	Actor* find_actor(const string& name) const override;

	vector<Actor*> find_actor_with_tag(const ActorTag tag) const override;
	
	int count_actor() const override;

	int count_actor_with_tag(const ActorTag tag) const override;

	Pawn* find_pawn(const string& name) const override;

	vector<Pawn*> find_pawn_with_tag(const ActorTag tag) const override;

	Character* find_character(const string& name) const override;

	vector<Character*> find_character_with_tag(const ActorTag tag) const override;

	TimelineManager& timeline() override;

	void generate_attack_collider(float radius, const GSvector3& center, Actor* owner, int damage, float lifespan = 1.0f, float delay = 0.0f) override;

	bool& enable_avoid_posteffct() override;

	void set_avoid_effect_color(const GSvector3& color) override;

protected:
	// �t�B�[���h
	Field* field_{ nullptr };
	// ���C�g
	Light* light_{ nullptr };
	// �i�r���b�V��
	NavMeshSurface* navmesh_{ nullptr };
	// �A�N�^�[�}�l�[�W���[
	ActorManager actor_;
	// �|�[���}�l�[�W���[
	PawnManager pawn_;
	// �L�����N�^�[�}�l�[�W���[
	CharacterManager character_;
	// �J�����}�l�[�W���[
	CameraManager camera_;
	// �^�C�����C���}�l�[�W���[
	TimelineManager timeline_;
	// �U������v�[��
	AttackColliderPool* attack_collider_pool_{ nullptr };
	// �|�X�g�G�t�F�N�g
	PostEffect posteffect_;

};

#endif
