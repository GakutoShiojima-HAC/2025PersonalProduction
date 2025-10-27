#ifndef ACTOR_GENERATOR_H_
#define ACTOR_GENERATOR_H_

#include <unordered_map>
#include <string>
#include "Engine/Core/Actor/IActorGenerator.h"

class ActorGenerator {
public:
    ActorGenerator() = default;

    ~ActorGenerator();

public:
    /// <summary>
    /// �����ɕK�v�ȃf�[�^��ǂݍ���
    /// </summary>
    /// <param name="world">= ������̃��[���h</param>
    void load(World* world);

    /// <summary>
    /// ��������
    /// </summary>
    /// <param name="json_file">= �������@��������Ă���json�t�@�C��</param>
    void generate(const std::string& json_file);

    /// <summary>
    /// ��������
    /// </summary>
    /// <param name="actor_key">= ���ʃL�[</param>
    /// <param name="position">= ���W</param>
    /// <param name="lookat">= �����_</param>
    /// <param name="hp">= �̗�</param>
    /// <param name="damage">= �U����</param>
    void generate(const std::string& actor_key, const GSvector3& position, const GSvector3& lookat, int hp = 1, int damage = 1);

    /// <summary>
    /// �����Ŏg�����f�[�^�̔j��
    /// </summary>
    void clear();

private:
    // �����p�f�[�^
    std::unordered_map<std::string, IActorGenerator*> data_;

public:
    // �R�s�[�֎~
    ActorGenerator(const ActorGenerator& other) = delete;
    ActorGenerator& operator = (const ActorGenerator& other) = delete;

};

#endif
