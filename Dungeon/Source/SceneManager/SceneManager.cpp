/*!*************************************************************************
\file	SceneManager.cpp
\author Anderson Phua
\email: a.phua@digipen.edu
\date	31-01-23
\brief	SceneManager contains the interface to the SceneManager system.
		This system is used to manage the scenes in the game.

		Copyright (C) 2023 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
*****************************************************************************/
#include "SceneManager/SceneManager.h"

/*!*************************************************************************
* \brief
* 	Load a level into the game.
* \param level
* 	Level to load.
**************************************************************************/
void SceneManager::LoadLevel(Level* level)
{
	if (level)
	{
		Engine->SetLevel(level);
		for (size_t i = 0; i < editor->GetSystems()->size(); ++i) { editor->GetSystems()->at(i)->SetLevel(level); }
		editor->GetPropertyEditor().DeselectEntity(); editor->GetHierarchy().RefreshHierarchy();
		editor->SetState(editor->GetState()); editor->Log("Loaded scene: " + level->GetId());
		
		//factory->ResetLastGOID();

		LogicTargets* LT;
		// reload logic targets or else error
		for (GameObject* GO : level->GetGameObjectArray())
		{
			LT = dynamic_cast<LogicTargets*>(GO->GetComponent(ComponentType::LogicTargets));
			if (LT != nullptr) { LT->AssignTargets(level->GetGameObjectArray()); }
		}
		audio->LoadAudioFiles("./Data/AudioData.txt");

		editor->GetGameWindow().transformHistory.clear();
		editor->GetGameWindow().transformHistoryIndex = 0;
	}
	else { editor->Log("Error loading scene: " + level->GetId(), Logger::LogType::Error); }
}

/*!*************************************************************************
* \brief
* 	Load a level into the game.
* \param levelName
* 	Name of the level to load.
**************************************************************************/
void SceneManager::LoadLevel(const char* levelName) { Level* nLevel = FilePtr::SerializeLevel(levelName); LoadLevel(nLevel); }

/*!*************************************************************************
* \brief
* 	Store a level into the game for loading when appropriate. Intended to
* 	be used to allow frames to finish before loading a new level.
* \param levelName
* 	Name of the level to load.
**************************************************************************/
void SceneManager::StoreLevel(const char* levelName)
{
	Level* nLevel = FilePtr::SerializeLevel(levelName);
	Engine->SetNextLevel(nLevel);
}
