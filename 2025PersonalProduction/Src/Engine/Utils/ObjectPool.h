// -----------------------------------------------------------------------------------------
//  File          : ObjectPool.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/08
//  Updated       : 2025/07/08
//  Description   : �I�u�W�F�N�g�v�[���̃e���v���[�g
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef OBJECT_POOL_H_
#define OBJECT_POOL_H_

#include <stack>
#include <memory>

using namespace std;

template <typename T>
class ObjectPool {
public:
    ObjectPool(size_t init_size = 10) {
        for (size_t i = 0; i < init_size; ++i) pool_.push(new T());
    }

    ~ObjectPool() {
        while (!pool_.empty()) {
            delete pool_.top();
            pool_.pop();
        }
    }

public: 
    /// <summary>
    ///�v�[�����疢�g�p�̃I�u�W�F�N�g���擾
    ///</summary>
    T* get() {
        // �v�[��������Ȃ�V�K����
        if (pool_.empty()) {
            return new T();
        }
        else {
            T* obj = pool_.top();
            pool_.pop();
            return obj;
        }
    }

    /// <summary>
    /// �v�[���Ɏg�p�ς݂̃I�u�W�F�N�g��ԋp
    /// </summary>
    void release(T* obj) {
        if (obj) {
            pool_.push(obj);
        }
    }

private:
	stack<T*> pool_;

};

#endif
