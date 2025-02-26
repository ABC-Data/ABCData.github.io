/******************************************************************************/
/*!
\file		Victory.h
\author 	Lim Hui Ching
\par    	email: l.huiching\@digipen.edu, 620004221
\date   	2 Feb, 2023
\brief		Script for Win state (Enemy HP <= 0)

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "Essential.h"
#include "Logic/Logic.h"
#include "./Logic/Script.h"
#include "Input/Input.h"
#include "CoreSys/Core.h"


namespace VictoryScript {
	std::string SKey{ "Victory" };
	int ContinueClickCounter = 0;

	/*!*************************************************************************
	****
	\brief
		Initializes Victory script.
	\param[in] GO
		Game Object to apply script on.
	\param[in] dt
		Delta time.
	****************************************************************************
	***/
	void Init(GameObject* GO, float dt) {
		(void)dt;
		ContinueClickCounter = 0;
		GameObject* Player = Engine->GetGameObject("Player");
		PlayerCombat* PCB = dynamic_cast<PlayerCombat*>(Player->GetComponent(ComponentType::PlayerCombat));

		if (PCB != nullptr) {
			//All good
		}
		else {
			std::cout << "PlayerCombat component not found, removing VictoryScript\n";
			LogicComponent* LC = dynamic_cast<LogicComponent*>(GO->GetComponent(ComponentType::Logic));
			//This must always work, otherwise how is this function even being called?
			//Do a simple check anyway for safety, but UDB if it's actually nullptr
			if (LC != nullptr) {
				LC->RemoveScriptKey(SKey);
			}
		}
	}

	/*!*************************************************************************
	****
	\brief
		Updates Victory script.
	\param[in] GO
		Game Object to apply script on.
	\param[in] dt
		Delta time.
	****************************************************************************
	***/
	void Update(GameObject* GO, float dt) {
		(void)dt;
		GameObject* Player = Engine->GetGameObject("Player");
		PlayerCombat* PCB = dynamic_cast<PlayerCombat*>(Player->GetComponent(ComponentType::PlayerCombat));

		GameObject* Enemy = Engine->GetGameObject("Enemy");
		EnemyCombat* ECB = dynamic_cast<EnemyCombat*>(Enemy->GetComponent(ComponentType::EnemyCombat));
		Animate* EA = dynamic_cast<Animate*>(Enemy->GetComponent(ComponentType::Animate));

		// Step 1: Stop other actions
		if (!PCB->InCombat && ECB->currentHP <= 0) {
			dynamic_cast<LogicComponent*>(Engine->GetGameObject("Inventory Slot 1")->GetComponent(ComponentType::Logic))->SetEnabled("CombatUI", false);

			GameObject* Continue = Engine->GetGameObject("Continue");
			Renderer* ContinueR = dynamic_cast<Renderer*>(Continue->GetComponent(ComponentType::Renderer));
			if ((MasterObjectList->MasterAssets.GetSprite(EA->GetTexHandle()).idx == MasterObjectList->MasterAssets.GetSprite(EA->GetTexHandle()).frames_per_row - 1) ||
				ContinueR->GetVisibility() == true) {
				// Step 2: Calculate total damage recieved (damage dealt by enemy) under defeat text
				// and total damage dealt (under victory text)
				// Step 3: Add inventory items gained
				Inventory* IC = dynamic_cast<Inventory*>(Player->GetComponent(ComponentType::Inventory));

				// Step 4: Make Victory pop-up, items gained, total damage recieved and total damage dealt text and Continue button with text appear
				GameObject* Defeat = Engine->GetGameObject("Defeat");
				GameUI* DefeatUI = dynamic_cast<GameUI*>(Defeat->GetComponent(ComponentType::GameUI));

				GameObject* Gain1 = Engine->GetGameObject("Gained Item 1");
				GameObject* Gain2 = Engine->GetGameObject("Gained Item 2");
				GameObject* Gain3 = Engine->GetGameObject("Gained Item 3");
				GameObject* Tooltip = Engine->GetGameObject("Tooltip");
				Transform* tT = dynamic_cast<Transform*>(Tooltip->GetComponent(ComponentType::Transform));
				Renderer* tR = dynamic_cast<Renderer*>(Tooltip->GetComponent(ComponentType::Renderer));
				std::vector <GameObject*> VictoryObjectsVector = { GO, Continue, Gain1, Gain2, Gain3 };
				std::vector <Renderer*> VictoryRendererVector = {};
				std::vector <GameUI*> VictoryGameUIVector = {};
				for (GameObject* VictoryObject : VictoryObjectsVector) {
					VictoryRendererVector.emplace_back(dynamic_cast<Renderer*>(VictoryObject->GetComponent(ComponentType::Renderer)));
					VictoryGameUIVector.emplace_back(dynamic_cast<GameUI*>(VictoryObject->GetComponent(ComponentType::GameUI)));
				}
				for (int i = 0; i < 2; ++i) { VictoryRendererVector[i]->SetVisibility(true); } // for GO and Continue

				for (int i = 2, j = 0; i < (2 + (sizeof(ECB->ItemDrop) / sizeof(ECB->ItemDrop[0]))); ++i, ++j) { // for gained items only
					if (ECB->ItemDrop[j]) { VictoryRendererVector[i]->SetVisibility(true); }
					else { VictoryRendererVector[i]->SetVisibility(false); }
					VictoryRendererVector[i]->SetTexHandle(InventoryItem(ECB->ItemDrop[j]).TexHandle);
					VictoryRendererVector[i]->SetUVMin(InventoryItem(ECB->ItemDrop[j]).Tex_UV_Min_COORD);
					VictoryRendererVector[i]->SetUVMax(InventoryItem(ECB->ItemDrop[j]).Tex_UV_Max_COORD);

					///// Tooltip
					if (VictoryGameUIVector[i]->MouseHoverClick(Input::GetMouse().x, (GLSetup::current_height - Input::GetMouse().y)) && (VictoryRendererVector[i]->GetVisibility() == true)) {
						tR->SetTexHandle(InventoryItem(ECB->ItemDrop[j]).TtTexHandle);
						tR->SetUVMin(InventoryItem(ECB->ItemDrop[j]).TtTex_UV_Min_COORD);
						tR->SetUVMax(InventoryItem(ECB->ItemDrop[j]).TtTex_UV_Max_COORD);
						tT->Dimensions = InventoryItem(ECB->ItemDrop[j]).TooltipDimensions;
						tR->SetVisibility(true);
						float scaleX = (float)GLSetup::current_width / (float)GLSetup::width;
						float scaleY = (float)GLSetup::current_height / (float)GLSetup::height;

						tT->Position = { static_cast<float>((Input::GetMouse().x / scaleX + 0.5f * tT->Dimensions.x * tT->Scale.x / 100.0f)),
					(GLSetup::current_height - Input::GetMouse().y) / scaleY + 0.5f * tT->Dimensions.y * tT->Scale.y / 100.0f };
						break;
					}
					else { tR->SetVisibility(false); }
				}
				VictoryGameUIVector[1]->SetText("Continue");
				if (PCB->TotalDamageDealt > ECB->maxHP) { PCB->TotalDamageDealt = ECB->maxHP; }
				VictoryGameUIVector[0]->SetText(std::to_string(PCB->TotalDamageDealt));
				DefeatUI->SetText(std::to_string(ECB->TotalDamageDealt));

				// Step 5: Make scene switch when click continue
				if (VictoryGameUIVector[1]->MouseHoverClick(Input::GetMouse().x, GLSetup::current_height - Input::GetMouse().y) &&
					Input::GetMouseTriggered(MouseButton::Button0)) { // Clicking continue
					if (!ContinueClickCounter) {
						++ContinueClickCounter;
						for (int ItemToAdd : ECB->ItemDrop) { if (ItemToAdd) { IC->AddItem(InventoryItem(ItemToAdd)); } }
					}
					GameObject* EHPGO = Engine->GetGameObject("Enemy HP Gauge Outline");
					GameUI* ehpgoUI = dynamic_cast<GameUI*>(EHPGO->GetComponent(ComponentType::GameUI));
					GameObject* PHPGO = Engine->GetGameObject("Yu HP Gauge Outline");
					GameObject* PAPGO = Engine->GetGameObject("Yu AP Gauge Outline");
					GameUI* phpgoUI = dynamic_cast<GameUI*>(PHPGO->GetComponent(ComponentType::GameUI));
					GameUI* papgoUI = dynamic_cast<GameUI*>(PAPGO->GetComponent(ComponentType::GameUI));

					papgoUI->Alpha = 0.f;
					phpgoUI->Alpha = 0.f;
					ehpgoUI->Alpha = 0.f;
					VictoryGameUIVector[0]->Alpha = 0.f;
					DefeatUI->Alpha = 0.f;

					for (int i = 0; i < VictoryRendererVector.size(); ++i) { VictoryGameUIVector[i]->SetText("-"); }

					PCB->FleeSuccess = true;
				}
			}
		}
	}

	/*!*************************************************************************
	****
	\brief
		Ends Victory script.
	\param[in] GO
		Game Object to apply script on.
	\param[in] dt
		Delta time.
	****************************************************************************
	***/
	void End(GameObject* GO, float dt) {
		(void)dt;
		(void)GO;
	}
}