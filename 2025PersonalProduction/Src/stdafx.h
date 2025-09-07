// -----------------------------------------------------------------------------------------
//  File          : stdafx.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/08
//  Updated       : 2025/07/08
//  Description   : 頻繁に更新しないクラスかつ使用頻度の高いヘッダーファイルを
//					事前にコンパイルするために定義するためのクラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef STDAFX_H_
#define STDAFX_H_

// c++
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <iterator>
#include <algorithm>
#include <memory>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <functional>

// gslib
#include <gslib.h>
#include <GSgame.h>
#include <GSeffect.h>
#include <GSmovie.h>

// imgui
#include <imgui/imgui.h>

// lib
#include "Lib/json.hpp"
#include "Lib/imgui_stdlib.h"

// my program
#include "Engine/Core/Actor/Actor.h"
#include "Engine/Core/Actor/ActorManager.h"
#include "Engine/Core/Actor/FieldActor/FieldActor.h"
#include "Engine/Core/Actor/FieldActor/FieldActorManager.h"
#include "Engine/Core/Actor/Pawn/Pawn.h"
#include "Engine/Core/Actor/Pawn/PawnManager.h"
#include "Engine/Core/Actor/Pawn/Character/Character.h"
#include "Engine/Core/Actor/Pawn/Character/CharacterManager.h"

#include "Engine/Core/Assets/AssetsManager.h"

#include "Engine/Core/Camera/Camera.h"
#include "Engine/Core/Camera/CameraManager.h"

#include "Engine/Core/Collision/AttackCollider.h"
#include "Engine/Core/Collision/AttackColliderPool.h"
#include "Engine/Core/Collision/BoundingSphere.h"

#include "Engine/Core/Field/Field.h"

//#include "Engine/Core/Input/Input.h"

#include "Engine/Core/NavMesh/NavMeshAgent.h"
//#include "Engine/Core/NavMesh/NavMeshDebugger.h"
#include "Engine/Core/NavMesh/NavMeshSurface.h"

#include "Engine/Core/Scene/IScene.h"
#include "Engine/Core/Scene/NullScene.h"
#include "Engine/Core/Scene/SceneManager.h"

#include "Engine/Core/Screen/Screen.h"

//#include "Engine/Core/Setting/Setting.h"

#include "Engine/Core/StateMachine/IState.h"
#include "Engine/Core/StateMachine/NullState.h"
#include "Engine/Core/StateMachine/StateMachine.h"

#include "Engine/Core/Timeline/Editor/CameraTimelineEditor.h"
#include "Engine/Core/Timeline/Editor/TimelineEditor.h"

#include "Engine/Core/Timeline/Parameters/CameraTimeline.h"

#include "Engine/Core/Timeline/ITimeline.h"
#include "Engine/Core/Timeline/TimelineManager.h"
#include "Engine/Core/Timeline/TimelineEditorManager.h"

#include "Engine/Core/Tween/ColorUnit.h"
#include "Engine/Core/Tween/Tween.h"
#include "Engine/Core/Tween/TweenUnit.h"
#include "Engine/Core/Tween/ValueUnit.h"
#include "Engine/Core/Tween/Vector2Unit.h"
#include "Engine/Core/Tween/Vector3Unit.h"

#include "Engine/Core/World/IWorld.h"
#include "Engine/Core/World/Light.h"
#include "Engine/Core/World/World.h"

#include "Engine/Graphics/AnimatedMesh/AnimatedMesh.h"
#include "Engine/Graphics/Canvas/Canvas.h"
#include "Engine/Graphics/PostEffect/PostEffect.h"

#include "Engine/Utils/Calc.h"
#include "Engine/Utils/Check.h"
#include "Engine/Utils/DebugMarker.h"
#include "Engine/Utils/Folder.h"
#include "Engine/Utils/Line.h"
#include "Engine/Utils/ObjectPool.h"
#include "Engine/Utils/OpenBrowser.h"
#include "Engine/Utils/Ray.h"

#endif
