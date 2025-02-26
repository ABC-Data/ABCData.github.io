/******************************************************************************/
/*!
\file		System.h
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	Sep 14, 2022
\brief		Base System header

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#ifndef SYSTEM_H
#define SYSTEM_H
#include "Essential.h"
#include "GameObject/GameObject.h"
#include "Level/Level.h"
//Systems
//Game Systems (Graphics, Physics, etc)
//Each system has its own behaviour
//This behaviour is given to Components

//Virtual class as each system has their own implementations

class System {
public:
	System() = default;

	virtual ~System() = default;

	virtual void Init() {};

	virtual void Shutdown() {};

	virtual void Update(float dt) = 0;
	//Private data members and additional functions are defined by each system
	//The base class does not have such features
	virtual void AdditionalUpdate(float dt) { (void)dt; };

	void SetLevel(Level* lvl) { SystemLevel = lvl; };

	Level* GetLevel() { return SystemLevel; };
private:
	Level* SystemLevel = nullptr;
};
#endif