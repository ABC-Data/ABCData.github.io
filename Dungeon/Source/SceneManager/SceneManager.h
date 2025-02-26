/*!*************************************************************************
\file	SceneManager.h
\author Anderson Phua
\email: a.phua@digipen.edu
\date	31-01-23
\brief	SceneManager contains the interface to the SceneManager system.
		This system is used to manage the scenes in the game.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*****************************************************************************/
#pragma once
#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "CoreSys/Core.h"
#include "Editor/UI/Editor.h"

class SceneManager
{
public:
	/*!*************************************************************************
	* \brief
	* 	Load a level into the game.
	* \param level
	* 	Level to load.
	**************************************************************************/
	static void LoadLevel(Level* level);

	/*!*************************************************************************
	* \brief
	* 	Load a level into the game.
	* \param levelName
	* 	Name of the level to load.
	**************************************************************************/
	static void LoadLevel(const char* levelName);

	/*!*************************************************************************
	* \brief
	* 	Store a level into the game for loading when appropriate. Intended to
	* 	be used to allow frames to finish before loading a new level.
	* \param levelName
	* 	Name of the level to load.
	**************************************************************************/
	static void StoreLevel(const char* levelName);
};

#endif // !SCENE_MANAGER_H