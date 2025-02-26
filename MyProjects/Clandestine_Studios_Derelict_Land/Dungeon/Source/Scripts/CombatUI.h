/******************************************************************************/
/*!
\file		CombatUI.h
\author 	Lim Hui Ching
\par    	email: l.huiching\@digipen.edu, 620004221
\date   	Jan 28, 2023
\brief		CombatUI Script.

	Copyright (C) 2022 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "./Logic/Script.h"
#include "Logic/Logic.h"
#include "CombatComponent.h"
#include "Input/Input.h"
#include "CoreSys/Core.h"
#include "SceneManager/SceneManager.h"

extern bool is_paused;

namespace CombatUI {
	std::string SKey{ "CombatUI" };

	/*!*************************************************************************
	****
	\brief
		Initializes CombatUI script.
	\param[in] GO
		Game Object to apply script on.
	\param[in] dt
		Delta time.
	****************************************************************************
	***/
	void Init(GameObject* GO, float dt) {
		UNREFERENCED_PARAMETER(dt);
		GameObject* Player = Engine->GetGameObject("Player");

		if (Player != nullptr) { return; }
		else {
			LogicComponent* LC = dynamic_cast<LogicComponent*>(GO->GetComponent(ComponentType::Logic));
			//This must always work, otherwise how is this function even being called?
			//Do a simple check anyway for safety, but UDB if it's actually nullptr
			if (LC != nullptr) {
				std::cout << GO->GetName() << " couldn't properly initialise CombatUI script, removing.\n";
				LC->RemoveScriptKey(SKey);
			}
		}
	}

	/*!*************************************************************************
	****
	\brief
		Updates CombatUI.
	\param[in] GO
		Game Object to apply script on.
	\param[in] dt
		Delta time.
	****************************************************************************
	***/
	void Update(GameObject* GO, float dt) {
		UNREFERENCED_PARAMETER(dt);
		UNREFERENCED_PARAMETER(GO);
		std::vector<GameObject*> InventoryVector = {};
		std::vector<Renderer*> RendererVector = {};
		std::vector<GameUI*> GameUIVector = {};
		Engine->GetAllObjectsByPrefix(InventoryVector, "Inventory Slot ");
		GameObject* Player = Engine->GetGameObject("Player");

		Inventory* IC = dynamic_cast<Inventory*>(Player->GetComponent(ComponentType::Inventory));

		for (int i = 0; i < IC->InventorySize; ++i) {
			RendererVector.emplace_back(dynamic_cast<Renderer*>(InventoryVector[i]->GetComponent(ComponentType::Renderer)));
			GameUIVector.emplace_back(dynamic_cast<GameUI*>(InventoryVector[i]->GetComponent(ComponentType::GameUI)));
		}

		GameObject* PHPGO = Engine->GetGameObject("Yu HP Gauge Outline");
		GameObject* PAPGO = Engine->GetGameObject("Yu AP Gauge Outline");

		GameUI* phpgoUI = dynamic_cast<GameUI*>(PHPGO->GetComponent(ComponentType::GameUI));
		GameUI* papgoUI = dynamic_cast<GameUI*>(PAPGO->GetComponent(ComponentType::GameUI));

		GameObject* Block = Engine->GetGameObject("Block");
		GameObject* Flee = Engine->GetGameObject("Flee");

		PlayerCombat* PCB = dynamic_cast<PlayerCombat*>(Player->GetComponent(ComponentType::PlayerCombat));

		for (int i = 0; i < IC->InventorySize; ++i) {
			if (IC->Invent[i].ID) {
				RendererVector[i]->SetVisibility(true);
				RendererVector[i]->SetTexHandle(IC->Invent[i].TexHandle);
				RendererVector[i]->SetUVMin(IC->Invent[i].Tex_UV_Min_COORD);
				RendererVector[i]->SetUVMax(IC->Invent[i].Tex_UV_Max_COORD);
			}
			else { RendererVector[i]->SetVisibility(false); }
		}
		Renderer* BlockR = dynamic_cast<Renderer*>(Block->GetComponent(ComponentType::Renderer));
		Renderer* FleeR = dynamic_cast<Renderer*>(Flee->GetComponent(ComponentType::Renderer));

		std::string PlayerAPText = std::to_string(static_cast<int>(PCB->currentAP)) + "/" + std::to_string(static_cast<int>(PCB->maxAP));
		std::string PlayerHPText = std::to_string(PCB->currentHP) + "/" + std::to_string(PCB->maxHP);

		papgoUI->SetText(PlayerAPText);
		phpgoUI->SetText(PlayerHPText);

		int clicked_pos = static_cast<int>(GLSetup::current_height - Input::GetMouse().y);
		for (int i = 0; i < IC->InventorySize; ++i) {
			if (!IC->Invent[i].ID) {
				RendererVector[i]->SetVisibility(false);
			}
			if (!is_paused) {
				if (PCB->currentAP < IC->Invent[i].WStats.APCost || GameUIVector[i]->MouseHoverClick(Input::GetMouse().x, clicked_pos) || (IC->Invent[i].Type == "Consumable" && PCB->currentAP < 20.f) || (IC->Invent[i].Type != "Consumable" && IC->Invent[i].Type != "Weapon")) {
					RendererVector[i]->RGB_0to1 = { 0.5f,0.5f,0.5f };
				}
				else { RendererVector[i]->RGB_0to1 = { 1.0f,1.0f,1.0f }; }
			}
		}

		if (!is_paused) {
			GameUI* blockUI = dynamic_cast<GameUI*>(Block->GetComponent(ComponentType::GameUI));
			if (PCB->currentAP < 50.f || blockUI->MouseHoverClick(Input::GetMouse().x, clicked_pos)) { BlockR->RGB_0to1 = { 0.5f,0.5f,0.5f }; }
			else { BlockR->RGB_0to1 = { 1.0f,1.0f,1.0f }; }

			GameUI* fleeUI = dynamic_cast<GameUI*>(Flee->GetComponent(ComponentType::GameUI));
			if (PCB->currentAP < 20.f || fleeUI->MouseHoverClick(Input::GetMouse().x, clicked_pos)) { FleeR->RGB_0to1 = { 0.5f,0.5f,0.5f }; }
			else { FleeR->RGB_0to1 = { 1.0f,1.0f,1.0f }; }
		}
	}

	/*!*************************************************************************
	****
	\brief
		Ends CombatUI script.
	\param[in] GO
		Game Object to apply script on.
	\param[in] dt
		Delta time.
	****************************************************************************
	***/
	void End(GameObject* GO, float dt) {
		(void)GO;
		(void)dt;
	}
}