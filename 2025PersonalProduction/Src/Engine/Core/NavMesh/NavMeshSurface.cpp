#include "Engine/Core/NavMesh/NavMeshSurface.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <utility> 
#include <algorithm>
#include <queue>
#include <unordered_map> 
#include <cmath>

#ifdef _DEBUG
#include <imgui/imgui.h>
#include "Engine/Utils/DebugMarker.h"
#endif

NavMeshSurface::NavMeshSurface(const string& mesh_path) {
	load(mesh_path);
}

NavMeshSurface::~NavMeshSurface() {
	clear();
}

void NavMeshSurface::update(float delta_time) {
#ifdef _DEBUG
    ImGui::Begin("Field Window");

    ImGui::Separator();
    ImGui::Text("NavMesh Surface Option");

    if (ImGui::Button("Toggle Draw NavMesh")) draw_navmesh_ = !draw_navmesh_;

    ImGui::End();
#endif
}

void NavMeshSurface::draw() const {
#ifdef _DEBUG
    if (draw_navmesh_) {
        // ���C�e�B���O�𖳌��ɂ���
        glDisable(GL_LIGHTING);
        glColor4f(0.0f, 1.0f, 1.0f, 0.2f);
        // ���_�z��I�u�W�F�N�g�̃o�C���h
        glBindVertexArray(vertex_array_);
        // �|���S���̕`��
        glDrawElements(GL_TRIANGLES, index_count_, GL_UNSIGNED_INT, nullptr);
        // ���_�z��I�u�W�F�N�g�̃o�C���h����
        glBindVertexArray(0);
        // ���C�e�B���O��L���ɂ���
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_LIGHTING);
    }
#endif
}

void NavMeshSurface::load(const string& mesh_path) {
    // ���b�V���f�[�^�̓ǂݍ���
    ifstream file{ mesh_path };
    // �t�@�C���I�[�v���ɐ����������H
    if (!file) return;
    
    clear();

    vector<Vertex> vertices;
    vector<GLuint> indices;
    string line;

    // ���_
    int vertex_count = 0;
    getline(file, line);
    vertex_count = stoi(line);
    vertices.reserve(vertex_count);
    for (int i = 0; i < vertex_count; ++i) {
        getline(file, line);
        stringstream ss(line);
        string value;
        Vertex v;
        getline(ss, value, ','); v.position.x = stof(value);
        getline(ss, value, ','); v.position.y = stof(value);
        getline(ss, value, ','); v.position.z = stof(value);
        vertices.push_back(v);
    }

    // �C���f�b�N�X
    int index_count = 0;
    getline(file, line);
    index_count = stoi(line);
    indices.reserve(index_count);
    for (int i = 0; i < index_count; i += 3) {
        getline(file, line);
        stringstream ss(line);
        string value;
        GLuint i1, i2, i3;
        getline(ss, value, ','); i1 = stoul(value);
        getline(ss, value, ','); i2 = stoul(value);
        getline(ss, value, ','); i3 = stoul(value);
        indices.push_back(i1);
        indices.push_back(i2);
        indices.push_back(i3);
    }

    vector<Vertex> final_vertices;
    vector<GLuint> final_indices;

    // ���_�}�[�W�������Ăяo��
    merging_duplicate_vertices(vertices, indices, final_vertices, final_indices);

    // ���_�����擾
    index_count_ = (GLsizei)final_indices.size();
    // �f�[�^��ێ�
    path_vertices_ = final_vertices;
    // ���_�o�b�t�@�̍쐬
    vertices_ = create_buffer(GL_ARRAY_BUFFER, sizeof(Vertex) * final_vertices.size(), final_vertices.data());
    // �C���f�b�N�X�o�b�t�@�̍쐬
    indices_ = create_buffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * final_indices.size(), final_indices.data());
    // ���_�z��I�u�W�F�N�g�̍쐬
    vertex_array_ = create_vertex_array();
    // �O���t���쐬
    build_graph(final_vertices, final_indices);
}

void NavMeshSurface::clear() {
    // ���_�z��I�u�W�F�N�g�̍폜
    glDeleteVertexArrays(1, &vertex_array_);
    // ���_�o�b�t�@�I�u�W�F�N�g�̍폜
    glDeleteBuffers(1, &vertices_);
    // �C���f�b�N�X�o�b�t�@�I�u�W�F�N�g�̍폜
    glDeleteBuffers(1, &indices_);

    index_count_ = 0;
    indices_ = 0;
    vertices_ = 0;
    vertex_array_ = 0;

    graph_.clear();
    path_vertices_.clear();
}

vector<int> NavMeshSurface::find_path(const GSvector3& start, const GSvector3& end) const {
    // �i�r���b�V��������
    if (graph_.empty()) return vector<int>{};

    // �X�^�[�g�ƃS�[���̃|���S��������
    int start_id = find_nearest_polygon(start);
    int end_id = find_nearest_polygon(end);
    if (start_id == ERROR_NODE_ID || end_id == ERROR_NODE_ID) return vector<int>{};

    // Astar�f�[�^�\��
    priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int>>> open_list;
    unordered_map<int, bool> closed_list; // �]���ς݃m�[�h
    unordered_map<int, int> parent_umap;
    unordered_map<int, float> goal_cost;

    auto heuristic = [&](int from_id, int to_id) {
        const auto& p1 = graph_[from_id].center;
        const auto& p2 = graph_[to_id].center;
        return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
    };

    // ������
    for (const auto& poly : graph_) {
        goal_cost[poly.id] = numeric_limits<float>::max();
        closed_list[poly.id] = false;
    }
    goal_cost[start_id] = 0;
    parent_umap[start_id] = ERROR_NODE_ID;
    open_list.push({ heuristic(start_id, end_id), start_id });

    // Astar�T��
    while (!open_list.empty()) {
        int current_id = open_list.top().second;
        open_list.pop();

        // �T���ς݂̓X�L�b�v
        if (closed_list[current_id]) continue;
        closed_list[current_id] = true;

        // �S�[������
        if (current_id == end_id) {
            vector<int> path;
            while (current_id != ERROR_NODE_ID) {
                path.push_back(current_id);
                current_id = parent_umap[current_id];
            }
            reverse(path.begin(), path.end());
            
            // �T���I��
            return path;    
        }

        // �אڃm�[�h��T��
        for (int neighbor_id : graph_[current_id].neighbors_id) {
            // �T���ς݂̓X�L�b�v
            if (closed_list[neighbor_id]) continue;

            float tentative_goal_cost = goal_cost[current_id] + heuristic(current_id, neighbor_id); // �R�X�g = ����
            if (tentative_goal_cost < goal_cost[neighbor_id]) {
                parent_umap[neighbor_id] = current_id;
                goal_cost[neighbor_id] = tentative_goal_cost;
                float f_cost = tentative_goal_cost + heuristic(neighbor_id, end_id);
                open_list.push({ f_cost, neighbor_id });
            }
        }
    }

    // �T�����s
    return vector<int>{};
}

vector<GSvector3> NavMeshSurface::create_line_path(const vector<int>& path) const {
    if (path.empty()) return vector<GSvector3>{};

    vector<GSvector3> final_path;
    for (int poly_id : path) {
        final_path.push_back(graph_[poly_id].center);
    }
    return final_path;
}

vector<GSvector3> NavMeshSurface::create_smooth_path(const vector<int>& path, const GSvector3& start, const GSvector3& end) const {
    // �m�[�h�������Ȃ�P���Ȓ����ړ��ŕԋp
    if (path.size() <= 1) return { start, end };

    vector<GSvector3> final_path;

    // �ŏ��̃E�F�C�|�C���g�Ƃ��ăX�^�[�g�n�_��ǉ�
    final_path.push_back(start);

    // TODO ���炩�Ȍo�H�ɕϊ����鏈��
    vector<GSvector3> tmp = create_line_path(path);
    final_path.insert(final_path.end(), tmp.begin(), tmp.end());

    // �Ō�̃E�F�C�|�C���g�Ƃ��ăS�[���n�_��ǉ�
    final_path.push_back(end);

    return final_path;
}

GLuint NavMeshSurface::create_buffer(GLenum target, GLuint size, const GLvoid* data) {
    if (size == 0) return 0;
    GLuint buffer = 0;
    glGenBuffers(1, &buffer);
    glBindBuffer(target, buffer);
    glBufferData(target, size, data, GL_STATIC_DRAW);
    glBindBuffer(target, 0);
    return buffer;
}

GLuint NavMeshSurface::create_vertex_array() {
    // ���_�z��I�u�W�F�N�g�̍쐬
    GLuint vertex_array = 0;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    // VBO�ƒ��_�����̐ݒ�
    glBindBuffer(GL_ARRAY_BUFFER, vertices_);
    glEnableVertexAttribArray(0); // layout (location = 0) in vec3 aPos;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    // EBO��VAO�ɕR�t��
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_);

    // �o�C���h������
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return vertex_array;
}

void NavMeshSurface::build_graph(const vector<Vertex>& vertices, const vector<GLuint>& indices) {
    graph_.clear();

    int polygon_count = indices.size() / 3; // �O�p�`�O��
    graph_.reserve(polygon_count);

    // �m�[�h���쐬
    for (int i = 0; i < polygon_count; ++i) {
        NavNode poly;
        poly.id = i;
        poly.indices[0] = indices[i * 3 + 0];
        poly.indices[1] = indices[i * 3 + 1];
        poly.indices[2] = indices[i * 3 + 2];

        // �|���S���̒��S�_���v�Z
        const GSvector3& v0 = vertices[poly.indices[0]].position;
        const GSvector3& v1 = vertices[poly.indices[1]].position;
        const GSvector3& v2 = vertices[poly.indices[2]].position;
        poly.center = calc_polygon_center(v0, v1, v2);

        graph_.push_back(poly);
    }

    // �L�[(�ӗp�̒��_) �l(�ӂ����L����m�[�h��ID)
    map<pair<GLuint, GLuint>, vector<int>> edge;

    // �ӂ̗אڌv�Z
    for (int i = 0; i < polygon_count; ++i) {
        const NavNode& poly = graph_[i];
        GLuint p_indices[] = { poly.indices[0], poly.indices[1], poly.indices[2] };

        // �|���S����3�̕ӂ��}�b�v�ɓo�^
        for (int j = 0; j < 3; ++j) {
            GLuint v1 = p_indices[j];
            GLuint v2 = p_indices[(j + 1) % 3];
            // �ӂ̒��_�C���f�b�N�X���\�[�g���āA(1,2)��(2,1)�������L�[�ɂȂ�悤�ɂ���
            pair<GLuint, GLuint> e = { min(v1, v2), max(v1, v2) };
            edge[e].push_back(i);
        }
    }

    for (const auto& pair : edge) {
        const vector<int>& polygons = pair.second;
        // 1�̕ӂ�2�̃|���S�������L���Ă���ꍇ�A�����͗אڂ��Ă���
        if (polygons.size() == 2) {
            int poly_id1 = polygons[0];
            int poly_id2 = polygons[1];
            graph_[poly_id1].neighbors_id.push_back(poly_id2);
            graph_[poly_id2].neighbors_id.push_back(poly_id1);
        }
    }
}

GSvector3 NavMeshSurface::calc_polygon_center(const GSvector3& v0, const GSvector3& v1, const GSvector3& v2) const {
    GSvector3 center;
    center.x = (v0.x + v1.x + v2.x) / 3.0f;
    center.y = (v0.y + v1.y + v2.y) / 3.0f;
    center.z = (v0.z + v1.z + v2.z) / 3.0f;
    return center;
}

int NavMeshSurface::find_nearest_polygon(const GSvector3& position) const {
    int id = ERROR_NODE_ID;
    float min_distance_sq = numeric_limits<float>::max();

    for (const auto& poly : graph_) {
        float dx = poly.center.x - position.x;
        float dy = poly.center.y - position.y;
        float dz = poly.center.z - position.z;
        float distance_sq = dx * dx + dy * dy + dz * dz;

        if (distance_sq < min_distance_sq) {
            min_distance_sq = distance_sq;
            id = poly.id;
        }
    }
    return id;
}

void NavMeshSurface::merging_duplicate_vertices(const vector<Vertex>& original_vertices, const vector<GLuint>& original_indices, vector<Vertex>& out_vertices, vector<GLuint>& out_indices) {
    // �L�[: ���_���W, �l: �V�������j�[�N�Ȓ��_�C���f�b�N�X
    map<GSvector3, GLuint, CompareGSvector3> unique_vertices_map;

    out_vertices.clear();
    out_indices.clear();
    out_indices.reserve(original_indices.size());

    for (const GLuint original_index : original_indices) {
        const GSvector3& pos = original_vertices[original_index].position;

        // ���̍��W�����߂Č����������ǂ������`�F�b�N
        if (unique_vertices_map.find(pos) == unique_vertices_map.end()) {
            // ���߂Č��������ꍇ�A�V�������j�[�N�Ȓ��_�Ƃ��ă��X�g�ɒǉ�
            out_vertices.push_back({ pos });
            // �V�����C���f�b�N�X���擾(���݂̃��X�g�̃T�C�Y - 1)
            GLuint new_index = (GLuint)(out_vertices.size() - 1);
            // �}�b�v�ɋL�^
            unique_vertices_map[pos] = new_index;
            // �ǉ�
            out_indices.push_back(new_index);
        }
        else {
            // ���łɌ������Ă���Ȃ�A�}�b�v����V�����C���f�b�N�X���擾���ă��X�g�ɒǉ�
            out_indices.push_back(unique_vertices_map[pos]);
        }
    }
}
