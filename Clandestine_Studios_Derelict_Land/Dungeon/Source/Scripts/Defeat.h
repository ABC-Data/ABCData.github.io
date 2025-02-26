/******************************************************************************/
/*!
\file		Defeat.h
\author 	Lim Hui Ching
\par    	email: l.huiching\@digipen.edu, 620004221
\date   	2 Feb, 2023
\brief		Script for Lose state (Player HP <= 0)

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


namespace DefeatScript {
	std::string SKey{ "Defeat" };
	float InitialShock = 0.f, InitialBleed = 0.f, InitialPhys = 0.f;

	/*!*************************************************************************
	****
	\brief
		Initializes Defeat script.
	\param[in] GO
		Game Object to apply script on.
	\param[in] dt
		Delta time.
	****************************************************************************
	***/
	void Init(GameObject* GO, float dt) {
		(void)dt;
		GameObject* Player = Engine->GetGameObject("Player");
		PlayerCombat* PCB = dynamic_cast<PlayerCombat*>(Player->GetComponent(ComponentType::PlayerCombat));
		GameObject* Enemy = Engine->GetGameObject("Enemy");
		EnemyCombat* ECB = dynamic_cast<EnemyCombat*>(Enemy->GetComponent(ComponentType::EnemyCombat));
		if (PCB != nullptr) {
			InitialBleed = ECB->bleedRES;
			InitialShock = ECB->ShockRES;
			InitialPhys = ECB->physicalRES;
			//All good
		}
		else {
			std::cout << "PlayerCombat component not found, removing DefeatScript\n";
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
		Updates Defeat.
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
		Animate* Player_Anim = dynamic_cast<Animate*>(Player->GetComponent(ComponentType::Animate));
		GameObject* EHP = Engine->GetGameObject("Enemy HP");
		Transform* ehpT = dynamic_cast<Transform*>(EHP->GetComponent(ComponentType::Transform));

		// Step 1: Stop other actions
		if (!PCB->InCombat && PCB->currentHP <= 0) {
			ehpT->Position.x = CombatScript::InitialEHPPos;
			ehpT->Scale.x = EnemyAI::EnemyHPMaxScale;
			dynamic_cast<LogicComponent*>(Engine->GetGameObject("Inventory Slot 1")->GetComponent(ComponentType::Logic))->SetEnabled("CombatUI", false);

			// Step 2: Make Defeat, Retry and Quit appear with text
			GameObject* Retry = Engine->GetGameObject("Retry");
			GameObject* Quit = Engine->GetGameObject("Quit");
			std::vector <GameObject*> DefeatObjectsVector = { GO, Retry, Quit };
			std::vector <Renderer*> DefeatRendererVector = {};
			std::vector <GameUI*> DefeatGameUIVector = {};
			for (GameObject* DefeatObject : DefeatObjectsVector) {
				DefeatRendererVector.emplace_back(dynamic_cast<Renderer*>(DefeatObject->GetComponent(ComponentType::Renderer)));
				DefeatGameUIVector.emplace_back(dynamic_cast<GameUI*>(DefeatObject->GetComponent(ComponentType::GameUI)));
			}
			for (Renderer* DefeatRenderer : DefeatRendererVector) { DefeatRenderer->SetVisibility(true); }
			DefeatGameUIVector[1]->SetText("Retry");
			DefeatGameUIVector[2]->SetText("Quit");

			// Step 3: Make clicking retry go back to combat scene and make the 3 game objects disappear
			if (DefeatGameUIVector[1]->MouseHoverClick(Input::GetMouse().x, GLSetup::current_height - Input::GetMouse().y) &&
				Input::GetMouseTriggered(MouseButton::Button0)) { // Clicking retry
				audio->PlaySfx(30); // General_Button_Clicked_SFX.mp3
				MasterObjectList->StoredPlayerCombat ? PCB->ResetPlayerCombat(MasterObjectList->StoredPlayerCombat->currentHP) : PCB->ResetPlayerCombat(PCB->maxHP);

				GameObject* Enemy = Engine->GetGameObject("Enemy");
				EnemyCombat* ECB = dynamic_cast<EnemyCombat*>(Enemy->GetComponent(ComponentType::EnemyCombat));
				ECB->ResetEnemyCombat(InitialPhys, InitialShock, InitialBleed);
				for (Renderer* DefeatRenderer : DefeatRendererVector) { DefeatRenderer->SetVisibility(false); }
				for (int i = 1; i < DefeatGameUIVector.size(); ++i) { DefeatGameUIVector[i]->SetText(""); }

				MasterObjectList->MasterAssets.GetSprite("Yu_Death").idx = 0;
				Player_Anim->SetTexHandle("Yu_Idle");
				Player_Anim->SetVisibility(true);
				Player_Anim->SetIsPlaying(true);
				PCB->InCombat = true;
			}

			// Step 4: Make clicking Quit go to Main Menu scene
			if (DefeatGameUIVector[2]->MouseHoverClick(Input::GetMouse().x, GLSetup::current_height - Input::GetMouse().y) &&
				Input::GetMouseTriggered(MouseButton::Button0)) { // Clicking quit
				audio->PlaySfx(30); // General_Button_Clicked_SFX.mp3
				CombatScript::ToMainMenu = true;
			}
		}
		else {
			dynamic_cast<LogicComponent*>(Engine->GetGameObject("Inventory Slot 1")->GetComponent(ComponentType::Logic))->SetEnabled("CombatUI", true);
		}
	}

	/*!*************************************************************************
	****
	\brief
		Ends Defeat script.
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