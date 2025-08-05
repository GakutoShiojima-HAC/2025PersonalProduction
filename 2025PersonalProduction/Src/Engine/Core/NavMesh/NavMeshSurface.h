#ifndef NAV_MESH_SURFACE_H_
#define NAV_MESH_SURFACE_H_

#include <string>
#include <vector>
#include <gslib.h>
#include <map>

using namespace std;

// �G���[�m�[�h
const int ERROR_NODE_ID{ -1 };

class NavMeshSurface {
public:
	NavMeshSurface(const string& mesh_path);

	~NavMeshSurface();

public:
	// �o�H�T���O���t�̃m�[�h
	struct NavNode {
		NavNode() : 
			id{ 0 },
			indices{ 0, 0, 0 }
		{}

		int id;						// �|���S���̃C���f�b�N�X
		GLuint indices[3];			// �\�����钸�_�̃C���f�b�N�X
		GSvector3 center;			// �|���S���̒��S���W
		vector<int> neighbors_id;	// �אڂ���|���S����ID���X�g
	};

private:
	// ���_�f�[�^�\����
	struct Vertex {
		GSvector3 position;
	};

	// GSvector3��map�̃L�[�Ƃ��Ďg����悤�ɂ���
	struct CompareGSvector3 {
		bool operator()(const GSvector3& a, const GSvector3& b) const {
			if (a.x != b.x) return a.x < b.x;
			if (a.y != b.y) return a.y < b.y;
			return a.z < b.z;
		}
	};

public:
	/// <summary>
	/// �i�r���b�V�����X�V����
	/// </summary>
	void update(float delta_time);

	/// <summary>
	/// �i�r���b�V����`�悷��
	/// </summary>
	void draw() const;

	/// <summary>
	/// �i�r���b�V����ǂݍ���
	/// </summary>
	/// <param name="mesh_path">= �t�@�C���p�X</param>
	void load(const string& mesh_path);

	/// <summary>
	/// �j��
	/// </summary>
	void clear();

	/// <summary>
	/// �o�H�T��
	/// </summary>
	/// <param name="start">= �J�n�n�_</param>
	/// <param name="end">= �I���n�_</param>
	/// <returns>�o�H</returns>
	vector<int> find_path(const GSvector3& start, const GSvector3& end) const;

	/// <summary>
	/// �����I�Ȍo�H�𐶐�
	/// </summary>
	/// <param name="path">= �o�H�T������</param>
	/// <param name="start">= �J�n�n�_</param>
	/// <param name="end">= �I���n�_</param>
	/// <returns>�����I�ɂ����o�H</returns>
	vector<GSvector3> create_line_path(const vector<int>& path) const;

	/// <summary>
	/// ���炩�Ȍo�H�𐶐�
	/// </summary>
	/// <param name="path">= �o�H�T������</param>
	/// <param name="start">= �J�n�n�_</param>
	/// <param name="end">= �I���n�_</param>
	/// <returns>���炩�ɂ����o�H</returns>
	vector<GSvector3> create_smooth_path(const vector<int>& path, const GSvector3& start, const GSvector3& end) const;

private:
	/// <summary>
	/// �o�b�t�@�I�u�W�F�N�g�̍쐬
	/// </summary>
	/// <param name="target">= </param>
	/// <param name="size">= </param>
	/// <param name="data">= </param>
	/// <returns>�o�b�t�@�I�u�W�F�N�g�ԍ�</returns>
	GLuint create_buffer(GLenum target, GLuint size, const GLvoid* data);

	/// <summary>
	/// ���_�z��I�u�W�F�N�g�̍쐬
	/// </summary>
	/// <returns>���_�z��I�u�W�F�N�g�ԍ�</returns>
	GLuint create_vertex_array();

	/// <summary>
	/// �o�H�T���O���t�𐶐�����
	/// </summary>
	void build_graph(const vector<Vertex>& vertices, const vector<GLuint>& indices);

	/// <summary>
	/// �|���S���̒��S�����߂�
	/// </summary>
	/// <param name="v0">= ���_</param>
	/// <param name="v1">= ���_</param>
	/// <param name="v2">= ���_</param>
	/// <returns>���S</returns>
	GSvector3 calc_polygon_center(const GSvector3& v0, const GSvector3& v1, const GSvector3& v2) const;

	/// <summary>
	/// ���W����ł��߂��|���S��������
	/// </summary>
	/// <param name="position">= ���W</param>
	/// <returns>�|���S��</returns>
	int find_nearest_polygon(const GSvector3& position) const;

	// �d���d���̃}�[�W
	void merging_duplicate_vertices(
		const vector<Vertex>& original_vertices, 
		const vector<GLuint>& original_indices,
		vector<Vertex>& out_vertices,
		vector<GLuint>& out_indices
	);

private:
	// �`�悷�钸�_��
	GLuint index_count_{ 0 };
	// �C���f�b�N�X�o�b�t�@
	GLuint indices_{ 0 };
	// ���_�o�b�t�@
	GLuint vertices_{ 0 };
	// ���_�z��I�u�W�F�N�g
	GLuint vertex_array_{ 0 };

	// �o�H�T���p�O���t�f�[�^
	vector<NavNode> graph_;
	// ���炩�Ȍo�H�T���p�f�[�^
	vector<Vertex> path_vertices_;

#ifdef _DEBUG
	// �i�r���b�V����`�悷�邩�ǂ���
	bool draw_navmesh_{ false };
	// �i�r���b�V���̃|���S���̒��_��`�悷�邩�ǂ���
	bool draw_navmesh_vertex_{ false };
	// �i�r���b�V���̃|���S���̒��S�_��`�悷�邩�ǂ���
	bool draw_navmesh_center_{ false };
#endif

public:
	// �R�s�[�֎~
	NavMeshSurface(const NavMeshSurface& other) = delete;
	NavMeshSurface& operator = (const NavMeshSurface& other) = delete;

};

#endif
