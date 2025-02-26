/******************************************************************************/
/*!
\file		MasterObjects.h
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	Jan 10, 2023
\brief		Master Object list

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#ifndef MASTEROBJECTS_H
#define MASTEROBJECTS_H

#include "Essential.h"
#include "GameObject/GameObject.h"
#include "Scripts/InventoryComponent.h"
#include "Scripts/CombatComponent.h"

#include "Transform/Transform.h"
#include "Level/ControllerComponent.h"
#include "Graphics/AssetManager.h"
#include "Deserializer/Deserializer.h"

class InventoryItem;
class Inventory;
class SetEffect;

class MasterObjects {
public:
	//Default Constructor
	MasterObjects();

	//Default Destructor. Might be customised in the future
	~MasterObjects();

	/*!*************************************************************************
	****
	\brief
		Loads all objects into the Master List.
	****************************************************************************
	***/
	void Init();

	/*!*************************************************************************
	****
	\brief
		Loads Inventory items into the Master List.

	\param filepath
		File with Item list to open.
	****************************************************************************
	***/
	void LoadInventoryItems(std::string filepath);

	/*!*************************************************************************
	****
	\brief
		Loads Set Effects into the Master List.

	\param filepath
		File with Set Effect list to open.
	****************************************************************************
	***/
	void LoadSetEffects(std::string filepath);

	/*!*************************************************************************
	****
	\brief
		Gets an inventory item by ID

	\param id
		Item ID to get (starts at 1)
	****************************************************************************
	***/
	const InventoryItem GetInventoryItem(int id);

	int GetIDbyInventoryName(std::string name);

	/*!*************************************************************************
	****
	\brief
		Exports an Inventory to the Temporary Storage

	\param inv
		Inventory to export
	****************************************************************************
	***/
	void ExportInventory(Inventory* inv);

	/*!*************************************************************************
	****
	\brief
		Imports the Inventory stored in Temporary Storage

	\param target
		Inventory to import into
	****************************************************************************
	***/
	void ImportInventory(Inventory* target);

	/*!*************************************************************************
	****
	\brief
		Exports a Transform to the Temporary Player Position

	\param pos
		Transform to export
	****************************************************************************
	***/
	void ExportPlayerPosition(Transform* pos);

	/*!*************************************************************************
	****
	\brief
		Imports the Transform stored in Temporary Player Position

	\param target
		Transform to import into
	****************************************************************************
	***/
	void ImportPlayerPosition(Transform* target);

	/*!*************************************************************************
	****
	\brief
		Exports a PlayerCombat to StoredPlayerCombat

	\param cont
		PlayerCombat to export
	****************************************************************************
	***/
	void ExportPlayerCombat(PlayerCombat* cont);

	/*!*************************************************************************
	****
	\brief
		Exports a Controller to the StoredController

	\param cont
		Controller to export
	****************************************************************************
	***/
	void ExportController(Controller* cont);

	/*!*************************************************************************
	****
	\brief
		Imports the Controller stored in StoredController

	\param target
		Controller to import into
	****************************************************************************
	***/
	void ImportController(Controller* target);

	/*!*************************************************************************
	****
	\brief
		Clears the Stored Controller
	****************************************************************************
	***/
	void ClearController();

	/*!*************************************************************************
	****
	\brief
		Exports a Controller to the StoredHubController

	\param cont
		Controller to export
	****************************************************************************
	***/
	void SetHubController(Controller* cont);

	/*!*************************************************************************
	****
	\brief
		Imports the Controller stored in StoredHubController

	\param target
		Controller to import into
	****************************************************************************
	***/
	void GetHubController(Controller* target);

	/*!*************************************************************************
	****
	\brief
		Exports a filepath to the StoredEnemyFileath

	\param fp
		Filepath to export
	****************************************************************************
	***/
	void ExportEnemyFilepath(std::string fp);

	/*!*************************************************************************
	****
	\brief
		Imports a filepath from the StoredEnemyFileath

	\param fp
		Filepath to import to
	****************************************************************************
	***/
	void ImportEnemyFilepath(std::string& fp);

	/*!*************************************************************************
	****
	\brief
		Instance of MasterObjects
	\return
		Created MasterObjects
	****************************************************************************
	***/
	static MasterObjects* Instance();
	/*!*************************************************************************
	****
	\brief
		Delete static MasterObjects above.
	****************************************************************************
	***/
	static void DeleteStaticMasterObjects();


	//Data Members

	//Master Lists
	std::vector<InventoryItem> MasterInventoryItemList;
	std::map<std::string, SetEffect> MasterSetEffectList;

	//Temporary Storage - For persistent data across scenes
	PlayerCombat* StoredPlayerCombat;
	Inventory* TempInventory;
	Transform* TempPlayerPosition;
	Controller* StoredController;
	Controller* StoredHubController;
	std::string StoredEnemyFilepath;
	Assets MasterAssets;
};

static MasterObjects* MasterObjectList = MasterObjects::Instance();

#endif