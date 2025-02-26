/*!*************************************************************************
\file   CheatCodes.h
\author Anderson Phua (65%)
\author Lim Hui Ching (10%)
\author Brandon Hsu (25%)
\email: a.phua\@digipen.edu
\email: l.huiching\@digipen.edu
\email: b.hsu\@digipen.edu
\date   26-11-22
\brief	Script that contains all cheat codes and functions to execute them.

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*****************************************************************************/
#pragma once
#include "Logic/Logic.h"
#include "Logic/Script.h"
#include "Essential.h"
#include "Scripts/Combat.h"
#include "Scripts/InventoryComponent.h"
#include "Input/Input.h"
#include "Editor/UI/Editor.h"
#include "CoreSys/MasterObjects.h"
#include "SceneManager/SceneManager.h"

/*
	This script, intended to be placed on the player, allows the player to
	change the pace of the demo via the cheat codes.
*/

namespace CheatCodes
{
	std::string SKey{ "CheatCodes" };


	/**************************************************************************/
	/*!
	* \brief
	*	Processes the cheat codes.
	* \param GO
	* The game object that this script is attached to.
	*/
	/**************************************************************************/
	void EvaluateCheatCode(GameObject*& GO)
	{
		(void)GO;
	}

	void Init(GameObject* GO, float dt)
	{
		(void)GO; (void)dt;
	}

	void Update(GameObject* GO, float dt)
	{
		(void)GO; (void)dt;
		if (GO->HasComponent(ComponentType::RigidBody)) {
			//Toggle speed
			if (Input::GetKeyTriggered(Key::M)) {
				float* im = &GO->GetComponent<RigidBody>()->InverseMass;
				*im = (*im < 60.f) ? 60.f : 30.f;
			}
		}
		if (GO->HasComponent(ComponentType::Inventory)) {
			//Spawn godsword
			if (Input::GetKeyTriggered(Key::G)) {
				GO->GetComponent<Inventory>()->AddItem(InventoryItem(14));
			}
			if (Input::GetKeyTriggered(Key::Z))
			{
				GO->GetComponent<Inventory>()->AddItem(InventoryItem(27)); // 27 is the material metal pieces
				std::cout << InventoryItem(27).Name <<  " added" << std::endl;
				
			}
			if (Input::GetKeyTriggered(Key::X))
			{
				GO->GetComponent<Inventory>()->AddItem(InventoryItem(31)); // 27 is the material metal pieces
				std::cout << InventoryItem(31).Name << " added" << std::endl;

			}
			if (Input::GetKeyTriggered(Key::C))
			{
				GO->GetComponent<Inventory>()->AddItem(InventoryItem(32)); // 27 is the material metal pieces
				std::cout << InventoryItem(32).Name << " added" << std::endl;

			}
			if (Input::GetKeyTriggered(Key::V))
			{
				GO->GetComponent<Inventory>()->AddItem(InventoryItem(37)); // 27 is the material metal pieces
				std::cout << InventoryItem(37).Name << " added" << std::endl;

			}
			if (Input::GetKeyTriggered(Key::R)) {
				MasterObjectList->ExportInventory(GO->GetComponent<Inventory>());
				SceneManager::StoreLevel("./Scene/Hub.scn");
			}
			if (Input::GetKeyTriggered(Key::P)) {
				for (int i = 0; i < 30; ++i) {
					GO->GetComponent<Inventory>()->AddItem(InventoryItem(27));
				}
				for (int i = 0; i < 10; ++i) {
					GO->GetComponent<Inventory>()->AddItem(InventoryItem(28));
				}
				for (int i = 0; i < 4; ++i) {
					GO->GetComponent<Inventory>()->AddItem(InventoryItem(29));
				}
				GO->GetComponent<Inventory>()->AddItem(InventoryItem(33));
			}
		}
		if (GO->HasComponent(ComponentType::PlayerCombat)) {
			//Regen
			if (Input::GetKeyTriggered(Key::H)) {
				PlayerCombat* PC = GO->GetComponent<PlayerCombat>();
				PC->currentHP = PC->maxHP;
				PC->currentAP = PC->maxAP;
			}
			if (Input::GetKeyTriggered(Key::L)) {
				PlayerCombat* P = GO->GetComponent<PlayerCombat>();
				P->currentHP = 1;
			}
		}
	}

	void End(GameObject* GO, float dt)
	{
		(void)GO; (void)dt;
	}
}