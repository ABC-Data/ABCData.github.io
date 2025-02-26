/******************************************************************************/
/*!
\file		CombatEquipment.h
\author 	Lim Hui Ching
\par    	email: l.huiching\@digipen.edu, 620004221
\date   	Jan 28, 2023
\brief		CombatEquipment Script.

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

namespace CombatEquipment {
	std::string SKey{ "CombatEquipment" };

	/*!*************************************************************************
	****
	\brief
		Initializes CombatEquipment script.
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
				std::cout << GO->GetName() << " couldn't properly initialise CombatEquipment script, removing.\n";
				LC->RemoveScriptKey(SKey);
			}
		}
	}

	/*!*************************************************************************
	****
	\brief
		Updates CombatEquipment.
	\param[in] GO
		Game Object to apply script on.
	\param[in] dt
		Delta time.
	****************************************************************************
	***/
	void Update(GameObject* GO, float dt) {
		UNREFERENCED_PARAMETER(dt);
		UNREFERENCED_PARAMETER(GO);
		GameObject* Player = Engine->GetGameObject("Player");
		PlayerCombat* PCB = dynamic_cast<PlayerCombat*>(Player->GetComponent(ComponentType::PlayerCombat));

		if (!is_paused && PCB->InCombat) {
			std::vector<GameObject*> EquipmentVector = {};
			Engine->GetAllObjectsByPrefix(EquipmentVector, "Equipment Slot ");
			std::vector<GameUI*> GameUIVector = {};
			std::vector<Renderer*> RendererVector = {};

			for (int i = 0; i < 4; ++i) {
				GameUIVector.emplace_back(dynamic_cast<GameUI*>(EquipmentVector[i]->GetComponent(ComponentType::GameUI)));
				RendererVector.emplace_back(dynamic_cast<Renderer*>(EquipmentVector[i]->GetComponent(ComponentType::Renderer)));
			}

			GameObject* Profile = Engine->GetGameObject("Yu Profile");
			GameUI* PGUI = dynamic_cast<GameUI*>(Profile->GetComponent(ComponentType::GameUI));
			Inventory* IC = dynamic_cast<Inventory*>(Player->GetComponent(ComponentType::Inventory));

			GameObject* Background = Engine->GetGameObject("Equipment Background");
			Renderer* BackgroundR = dynamic_cast<Renderer*>(Background->GetComponent(ComponentType::Renderer));

			if (PGUI->MouseHoverClick(Input::GetMouse().x, (GLSetup::current_height - Input::GetMouse().y)) && Input::GetMouseTriggered(MouseButton::Button0))
			{
				audio->PlaySfx(30); // General_Button_Clicked_SFX.mp3
				if (BackgroundR->GetVisibility() == false) {
					for (int i = 0; i < 4; ++i) {
						if (IC->Equipment[i].ID) {
							RendererVector[i]->SetVisibility(true);
							RendererVector[i]->SetTexHandle(IC->Equipment[i].TexHandle);
							RendererVector[i]->SetUVMin(IC->Equipment[i].Tex_UV_Min_COORD);
							RendererVector[i]->SetUVMax(IC->Equipment[i].Tex_UV_Max_COORD);
						}
						else {
							RendererVector[i]->SetVisibility(false);
						}
					}
					BackgroundR->SetVisibility(true);
				}
				else {
					for (Renderer* x : RendererVector) {
						x->SetVisibility(false);
					}
					BackgroundR->SetVisibility(false);
				}
			}
		}
	}

	/*!*************************************************************************
	****
	\brief
		Ends CombatEquipment script.
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