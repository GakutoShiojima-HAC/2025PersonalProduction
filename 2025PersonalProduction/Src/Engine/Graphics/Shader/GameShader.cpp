#include "Engine/Graphics/Shader/GameShader.h"
#include <gslib.h>
#include "Engine/Graphics/Shader/Shaders.h"

GameShader& GameShader::get_instance() {
    // static変数のインスタンスは１つ
    // インスタンス化も１回のみ
    static GameShader self;
    return self;
}

void GameShader::load() {
    gsLoadShader(Shader_Instanced, "Resource/Private/Shader/MyInstanced.vert", "Resource/Private/Shader/MyInstanced.frag");
    gsLoadShader(Shader_Mesh, "Resource/Private/Shader/MyMesh.vert", "Resource/Private/Shader/MyMesh.frag");
    gsLoadShader(Shader_SkinMesh, "Resource/Private/Shader/MySkinMesh.vert", "Resource/Private/Shader/MySkinMesh.frag");
    gsLoadShader(Shader_Octree, "Resource/Private/Shader/MyOctree.vert", "Resource/Private/Shader/MyOctree.frag");
}

void GameShader::clear() {
    end();

    // シェーダーの削除
    gsDeleteShader(Shader_Instanced);
    gsDeleteShader(Shader_Mesh);
    gsDeleteShader(Shader_SkinMesh);
    gsDeleteShader(Shader_Octree);
}

void GameShader::start() {
    // デフォルトシェーダーを退避
    default_instacedmesh_ = gsGetBindDefaultInstacedMeshShader();
    default_mesh_ = gsGetBindDefaultMeshShader();
    default_skinmesh_ = gsGetBindDefaultSkinMeshShader();
    default_octree_ = gsGetBindDefaultOctreeShader();

    // シェーダーを有効化
    gsBindDefaultInstacedMeshShader(Shader_Instanced);
    gsBindDefaultMeshShader(Shader_Mesh);
    gsBindDefaultSkinMeshShader(Shader_SkinMesh);
    gsBindDefaultOctreeShader(Shader_Octree);
}

void GameShader::end() {
    // シェーダーを復帰
    gsBindDefaultInstacedMeshShader(default_instacedmesh_);
    gsBindDefaultMeshShader(default_mesh_);
    gsBindDefaultSkinMeshShader(default_skinmesh_);
    gsBindDefaultOctreeShader(default_octree_);
}
