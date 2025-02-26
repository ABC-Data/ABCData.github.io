/******************************************************************************/
/*!
\file		Logic.h
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	Nov 1, 2022
\brief		Logic system and Logic Component

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "Essential.h"
#include "System/System.h"
#include "GameObject/GameObject.h"
#include "LogicTargets.h"
#include "Script.h"

class Logic : public System
{
public:
	/*!*************************************************************************
	****
	\brief
		Default constructor.
	****************************************************************************
	***/
	Logic();
	/*!*************************************************************************
	****
	\brief
		Initialize the Logic system
	****************************************************************************
	***/
	virtual void Init();
	/*!*************************************************************************
	****
	\brief
		Shut down the Logic system
	****************************************************************************
	***/
	virtual void Shutdown();
	/*!*************************************************************************
	****
	\brief
		Updates all game objects
	\param[in] dt
		delta time
	****************************************************************************
	***/
	virtual void Update(float dt);

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
	void RegisterScript(const ScriptItem& init, const ScriptItem& update, const ScriptItem& end, std::string key);

	/**************************************************************************/
	/*!
	  \brief
		Deregisters a script
	  \param key
		Key of the script
	*/
	/**************************************************************************/
	void DeregisterScript(std::string key);

	/**************************************************************************/
	/*!
	  \brief
		Gets LogicScripts data member
	*/
	/**************************************************************************/
	std::map<std::string, Script*>& GetLogicScripts() { return LogicScripts; }
private:
	std::map<std::string, Script*> LogicScripts;
};

class LogicComponent : public Component
{
public:
	LogicComponent() { SetType(ComponentType::Logic); };
	std::vector<std::pair<std::string, bool>>& GetLogicIDs() { return LogicIDs; }
	std::vector<std::pair<std::string, bool>>& GetToInits() { return ToInit; }
	std::vector<std::pair<std::string, bool>>& GetToEnds() { return ToEnd; }

	/**************************************************************************/
	/*!
		\brief
		Adds a script's key to this logic component
		\param key
		Key of the script
	*/
	/**************************************************************************/
	void AddScriptKey(const std::string key);

	/**************************************************************************/
	/*!
		\brief
		Removes a script's key from this logic component
		\param key
		Key of the script
	*/
	/**************************************************************************/
	void RemoveScriptKey(const std::string& key);

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
	void SetEnabled(const std::string& key, bool toggle);

	/**************************************************************************/
	/*!
		\brief
		Get a script's enabled status
		\param key
		Key of the script
		\return
		Enabled/disabled
	*/
	/**************************************************************************/
	bool GetEnabled(const std::string& key);

	/**************************************************************************/
	/*!
		\brief
		Serialize to file
		\param fp
		Filepath
	*/
	/**************************************************************************/
	virtual void Serialize(FILE* fp) override;
	
	/**************************************************************************/
	/*!
		\brief
		Deserialize from file
		\param fp
		Filepath
	*/
	/**************************************************************************/
	virtual void Deserialize(FILE* fp) override;

	/**************************************************************************/
	/*!
		\brief
		Copy data from other component
		\param Target
		Target to get data from
	*/
	/**************************************************************************/
	virtual void CopyData(Component* Target) override;
private:
	std::vector<std::pair<std::string, bool>> LogicIDs;
	std::vector<std::pair<std::string, bool>> ToInit;
	std::vector<std::pair<std::string, bool>> ToEnd;
};