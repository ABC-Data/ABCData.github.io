/******************************************************************************/
/*!
\file		Script.h
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	Nov 1, 2022
\brief		Script base class for integration into Logic System

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "Essential.h"
#include "GameObject/GameObject.h"

typedef void(*ScriptItem)(GameObject*, float);

class Script
{
public:
	Script(const ScriptItem& init, const ScriptItem& update, const ScriptItem& end) : Init{ init }, Update{ update }, End{ end } {}

	ScriptItem Init;
	ScriptItem Update;
	ScriptItem End;
};