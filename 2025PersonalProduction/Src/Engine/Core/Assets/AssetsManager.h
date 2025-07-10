// -----------------------------------------------------------------------------------------
//  File          : AssetsManager.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/11
//  Updated       : 2025/07/11
//  Description   : �A�Z�b�g�̃��[�h�E�N���A���Ǘ�����N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef ASSETS_MANAGER_H_
#define ASSETS_MANAGER_H_

#include <string>
#include <vector>
#include <list>
#include <utility>

typedef	unsigned int GSuint;
using namespace std;

// �ǂݍ��ރA�Z�b�g
struct LoadAssets {
	string name{ "" };	// ���ʖ�
	vector<pair<GSuint, string>> mesh;
	vector<pair<GSuint, string>> skinmesh;
	vector<pair<GSuint, string>> texture;
	vector<pair<GSuint, string>> octree;
	vector<pair<GSuint, string>> bgm;
	vector<pair<GSuint, string>> se;
	vector<pair<GSuint, string>> effect;
};

// �ǂݍ��񂾃A�Z�b�g
struct LoadedAssets {
	string name{ "" };	// ���ʖ�
	vector<GSuint> mesh;
	vector<GSuint> skinmesh;
	vector<GSuint> texture;
	vector<GSuint> octree;
	vector<GSuint> bgm;
	vector<GSuint> se;
	vector<GSuint> effect;
};

class AssetsManager {
private:
	// �R���X�g���N�^�i�O������̃C���X�^���X���֎~�j
	AssetsManager() = default;

public:
	~AssetsManager();

public:
	/// <summary>
	/// �C���X�^���X���擾���邽�߂�static�����o�֐�
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static AssetsManager& get_instance();

public:
	/// <summary>
	/// �Ǘ����̃A�Z�b�g��S�Ĕj�����A�Ǘ����������
	/// </summary>
	void clear();

	/// <summary>
	/// �A�Z�b�g�����[�h
	/// </summary>
	/// <param name="data">= �f�[�^</param>
	/// <returns>�f�[�^</returns>
	LoadedAssets* load_assets(LoadAssets* data);

	/// <summary>
	/// �A�Z�b�g��񓯊��Ń��[�h
	/// </summary>
	/// <param name="data">= �f�[�^</param>
	void load_assets_async(LoadAssets* data);

public:
	/// <summary>
	/// �w�肵���A�Z�b�g��j��
	/// </summary>
	/// <param name="name">= ���ʖ�</param>
	/// <returns>�j��������������^</returns>
	bool delete_assets(const string& assets_name);

	/// <summary>
	/// �w�肵���A�Z�b�g��j��
	/// </summary>
	/// <param name="data">= �f�[�^</param>
	/// <returns>�j��������������^</returns>
	bool delete_assets(LoadedAssets* data);

public:
	/// <summary>
	/// �񓯊��ŃA�Z�b�g��ǂݍ���ł��邩
	/// </summary>
	/// <returns>���Ă�����^</returns>
	bool is_async_load() const;

	/// <summary>
	/// �ǂݍ��񂾃A�Z�b�g�� �ǂݍ��ݏI����0�Ƀ��Z�b�g
	/// </summary>
	/// <returns>��</returns>
	int loaded_count() const;

	/// <summary>
	/// ���ʖ�����ǂݍ��ݍς݃A�Z�b�g�\���̂�����
	/// </summary>
	/// <param name="name">= ���ʖ�</param>
	/// <returns>�\����</returns>
	LoadedAssets* find(const string& assets_name);

private:
	/// <summary>
	/// �񓯊��ŃA�Z�b�g�ǂݍ��݂��J�n
	/// </summary>
	/// <param name="data">�f�[�^</param>
	void async_load_asset(LoadAssets* data);
	
	/// <summary>
	/// �A�Z�b�g��j��
	/// </summary>
	/// <param name="data">= �j������A�Z�b�g�f�[�^</param>
	void delete_asset(LoadedAssets* data);

	/// <summary>
	/// �w�肵���ǂݍ��ݍς݃A�Z�b�g���Ǘ��������������
	/// </summary>
	/// <param name="data">= �f�[�^</param>
	void remove_loaded_asset(LoadedAssets* data);

	/// <summary>
	/// �w�肵���ǂݍ��ݒ��A�Z�b�g���Ǘ��������������
	/// </summary>
	/// <param name="data">= �f�[�^</param>
	void remove_loading_asset(LoadAssets* data);

	/// <summary>
	/// �������\�[�XID�𑼃A�Z�b�g�ł��g�p���Ă��邩
	/// </summary>
	/// <param name="id">= �m�F������ID</param>
	/// <param name="other">= ���탊�\�[�X</param>
	/// <returns>�g�p���Ă�����^</returns>
	bool is_same_id_for_other_asset(const GSuint id, const vector<GSuint>& other) const;

private:
	// �ǂݍ��񂾃A�Z�b�g
	list<LoadedAssets*> assets_;
	// �񓯊��œǂݍ��ݒ��̃A�Z�b�g
	list<LoadAssets*> loading_assets_;

	// �ǂݍ��񂾃A�Z�b�g��
	int loaded_assets_count_{ 0 };

public:
	// �R�s�[�֎~
	AssetsManager(const AssetsManager& other) = delete;
	AssetsManager& operator = (const AssetsManager& other) = delete;

};

#endif
