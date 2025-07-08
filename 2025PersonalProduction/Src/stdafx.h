// -----------------------------------------------------------------------------------------
//  File          : stdafx.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/08
//  Updated       : 2025/07/08
//  Description   : �p�ɂɍX�V���Ȃ��N���X���g�p�p�x�̍����w�b�_�[�t�@�C����
//					���O�ɃR���p�C�����邽�߂ɒ�`���邽�߂̃N���X
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
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

// my program
#include "Engine/Core/Scene/SceneManager.h"
#include "Engine/Core/Scene/IScene.h"
#include "Engine/Core/Scene/NullScene.h"
#include "Engine/Core/StateMachine/StateMachine.h"
#include "Engine/Core/StateMachine/IState.h"
#include "Engine/Core/StateMachine/NullState.h"
#include "Engine/Graphics/AnimatedMesh/AnimatedMesh.h"
#include "Engine/Utils/ObjectPool.h"

#endif
