// -----------------------------------------------------------------------------------------
//  File          : ObjectPool.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/08
//  Updated       : 2025/07/08
//  Description   : オブジェクトプールのテンプレート
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
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
    ///プールから未使用のオブジェクトを取得
    ///</summary>
    T* get() {
        // プール内が空なら新規生成
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
    /// プールに使用済みのオブジェクトを返却
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
