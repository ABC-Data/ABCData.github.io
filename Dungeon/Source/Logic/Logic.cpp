/******************************************************************************/
/*!
\file		Logic.cpp
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	Nov 1, 2022
\brief		Logic system and Logic Component

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "Logic.h"
#include "Editor/UI/Editor.h"

/*!*************************************************************************
****
\brief
	Default constructor.
****************************************************************************
***/
Logic::Logic() {}

/*!*************************************************************************
****
\brief
	Initialize the Logic system
****************************************************************************
***/
void Logic::Init() {
	//Initialise targets
	for (GameObject* GO : this->GetLevel()->GetGameObjectArray()) {
		LogicTargets* LT = dynamic_cast<LogicTargets*>(GO->GetComponent(ComponentType::LogicTargets));
		if (LT != nullptr) {
			LT->AssignTargets(this->GetLevel()->GetGameObjectArray());
		}
	}
}

/*!*************************************************************************
****
\brief
	Updates all game objects
\param[in] dt
	delta time
****************************************************************************
***/
void Logic::Update(float dt) {
	
	// Scripts will only be in use if the editor is in playing state.
	if (editor->GetState() != EditorState::EditorPlaying) { return; }

	for (GameObject* GO : this->GetLevel()->GetGameObjectArray()) {
		LogicComponent* LC = dynamic_cast<LogicComponent*>(GO->GetComponent(ComponentType::Logic));
		if (LC != nullptr) {
			for (size_t i = 0; i < LC->GetToInits().size(); ++i) {
				if (LC->GetToInits()[i].second == false) continue;
				if (LogicScripts.contains(LC->GetToInits()[i].first)) {
					LogicScripts[LC->GetToInits()[i].first]->Init(GO, dt);
				}
			}
			LC->GetToInits().clear();

			for (size_t i = 0; i < LC->GetLogicIDs().size(); ++i) {
				if (LC->GetLogicIDs()[i].second == false) continue;
				if (LogicScripts.contains(LC->GetLogicIDs()[i].first)) {
					LogicScripts[LC->GetLogicIDs()[i].first]->Update(GO, dt);
				}
			}

			for (size_t i = 0; i < LC->GetToEnds().size(); ++i) {
				if (LC->GetToEnds()[i].second == false) continue;
				if (LogicScripts.contains(LC->GetToEnds()[i].first)) {
					LogicScripts[LC->GetToEnds()[i].first]->End(GO, dt);
				}
			}
			LC->GetToEnds().clear();
		}
	}
}

/*!*************************************************************************
****
\brief
	Shut down the Logic system
****************************************************************************
***/
void Logic::Shutdown() {
	//Run all end scripts first
	for (GameObject* GO : this->GetLevel()->GetGameObjectArray()) {
		LogicComponent* LC = dynamic_cast<LogicComponent*>(GO->GetComponent(ComponentType::Logic));
		if (LC != nullptr) {
			for (size_t i = 0; i < LC->GetLogicIDs().size(); ++i) {
				LC->RemoveScriptKey(LC->GetLogicIDs()[i].first);
			}
			for (size_t i = 0; i < LC->GetToEnds().size(); ++i) {
				if (LogicScripts.contains(LC->GetToEnds()[i].first)) {
					LogicScripts[LC->GetToEnds()[i].first]->End(GO, 0.f);
				}
			}
		}
	}
	for (auto it = LogicScripts.begin(); it != LogicScripts.end(); ++it) {
		delete it->second;
	}
}

/**************************************************************************/
/*!
  \brief
	Register a script for use
  \param init
	Init function of the script
  \param update
	Update function of the script
  \param end
	End function of the script
  \param key
	What the script will be accessed by
*/
/**************************************************************************/
void Logic::RegisterScript(const ScriptItem& init, const ScriptItem& update, const ScriptItem& end, std::string key) {
	Script* s = new Script(init, update, end);
	LogicScripts[key] = s;
}

/**************************************************************************/
/*!
	\brief
	Deregisters a script
	\param key
	Key of the script
*/
/**************************************************************************/
void Logic::DeregisterScript(std::string key) {
	delete LogicScripts[key];
	LogicScripts.erase(key);
}

/**************************************************************************/
/*!
	\brief
	Adds a script's key to this logic component
	\param key
	Key of the script
*/
/**************************************************************************/
void LogicComponent::AddScriptKey(const std::string key) {
	LogicIDs.push_back(std::make_pair(key, true));
	ToInit.push_back(std::make_pair(key, true));
}

/**************************************************************************/
/*!
	\brief
	Removes a script's key from this logic component
	\param key
	Key of the script
*/
/**************************************************************************/
void LogicComponent::RemoveScriptKey(const std::string& key) {
	bool found = false;
	for (auto it = ToInit.begin(); it != ToInit.end(); ++it) {
		if (strcmp(it->first.c_str(), key.c_str()) == 0) {
			found = true;
			ToInit.erase(it);
			break;
		}
	}
	for (auto it = LogicIDs.begin(); it != LogicIDs.end(); ++it) {
		if (strcmp(it->first.c_str(), key.c_str()) == 0) {
			found = true;
			LogicIDs.erase(it);
			break;
		}
	}
	if (found) {
		ToEnd.push_back(std::make_pair(key, true));
	}
}

/**************************************************************************/
/*!
	\brief
	Enable/disable a script
	\param key
	Key of the script
	\param toggle
	Enabled/disabled
*/
/**************************************************************************/
void LogicComponent::SetEnabled(const std::string& key, bool toggle)
{
	for (auto it = ToInit.begin(); it != ToInit.end(); ++it) {
		if (strcmp(it->first.c_str(), key.c_str()) == 0) {
			it->second = toggle;
		}
	}
	for (auto it = LogicIDs.begin(); it != LogicIDs.end(); ++it) {
		if (strcmp(it->first.c_str(), key.c_str()) == 0) {
			it->second = toggle;
		}
	}
	for (auto it = ToEnd.begin(); it != ToEnd.end(); ++it) {
		if (strcmp(it->first.c_str(), key.c_str()) == 0) {
			it->second = toggle;
		}
	}
}

bool LogicComponent::GetEnabled(const std::string& key)
{
	bool check = false;
	for (auto it = ToInit.begin(); it != ToInit.end(); ++it) {
		if (strcmp(it->first.c_str(), key.c_str()) == 0) {
			check = it->second;
		}
	}
	for (auto it = LogicIDs.begin(); it != LogicIDs.end(); ++it) {
		if (strcmp(it->first.c_str(), key.c_str()) == 0) {
			check = it->second;
		}
	}
	for (auto it = ToEnd.begin(); it != ToEnd.end(); ++it) {
		if (strcmp(it->first.c_str(), key.c_str()) == 0) {
			check = it->second;
		}
	}
	return check;
}

/**************************************************************************/
/*!
	\brief
	Serialize to file
	\param fp
	Filepath
*/
/**************************************************************************/
void LogicComponent::Serialize(FILE* fp)
{
	fprintf(fp, "Logic\n");
	fprintf(fp, "Scripts: ");
	for (int k = 0; k < GetLogicIDs().size(); ++k) {
		fprintf(fp, "%s", GetLogicIDs()[k].first.c_str());
		if (k != (int)(GetLogicIDs().size() - 1)) {
			fprintf(fp, ",");
		}
		else {
			fprintf(fp, "\n");
		}
	}
}

/**************************************************************************/
/*!
	\brief
	Deserialize from file
	\param fp
	Filepath
*/
/**************************************************************************/
void LogicComponent::Deserialize(FILE* fp)
{
	int discard = fscanf(fp, "Scripts: ");
	while (true) {
		char buffer[100]{ 0 };
		int success = fscanf(fp, "%[^,\n]", &buffer);
		if (!success) break;
		AddScriptKey(buffer);
		success = fscanf(fp, ",");
	}
	discard = fscanf(fp, "\n");
}

/**************************************************************************/
/*!
	\brief
	Copy data from other component
	\param Target
	Target to get data from
*/
/**************************************************************************/
void LogicComponent::CopyData(Component* Target)
{
	LogicComponent* LC = dynamic_cast<LogicComponent*>(Target);

	if (LC != nullptr)
	{
		this->ToInit = LC->ToInit;
		this->LogicIDs = LC->LogicIDs;
		this->ToEnd = LC->ToEnd;
	}
}
