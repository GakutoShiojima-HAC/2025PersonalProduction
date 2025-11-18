// -----------------------------------------------------------------------------------------
//  File          : PlayReact.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/11/18
//  Updated       : 2025/11/18
//  Description   : 衝突したら再生
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#include "ICinemaBehavior.h"
#include <string>

class PlayReact : public ICinemaBehavior {
public:
    PlayReact(IWorld* world, const std::string& name);

public:
    void react(Actor& other) override;

private:
    std::string trigger_name_{ "" };

};
