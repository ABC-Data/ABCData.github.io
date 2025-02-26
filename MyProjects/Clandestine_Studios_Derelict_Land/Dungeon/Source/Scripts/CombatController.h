/******************************************************************************/
/*!
\file		CombatController.h
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date		Jan 20, 2023
\brief		Controller Script for Combat Scene

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "Logic/Script.h"
#include "Logic/Logic.h"
#include "Transform/Transform.h"
#include "CoreSys/Core.h"
#include "CoreSys/MasterObjects.h"
#include "Input/Input.h"
#include "SceneManager/SceneManager.h"
#include "Audio/Audio.h"
#include "EnemyAI.h"
#include "CombatTutorialDialogue.h"

namespace CombatController {
	std::string SKey{ "CombatController" };
	bool ToMainMenu = false;

	void Init(GameObject* GO, float dt) {
		(void)dt;
		audio->StopMusic();
		audio->StopSfx();
		audio->PlayMusic(4); // CombatBGM
		ToMainMenu = false;
		Controller* CT = dynamic_cast<Controller*>(GO->GetComponent(ComponentType::Controller));
		if (CT != nullptr) {
			//Came from another level
			GameObject* Player = Engine->GetObjectByPrefix("Player");
			Inventory* IC = dynamic_cast<Inventory*>(Player->GetComponent(ComponentType::Inventory));
			PlayerCombat* PCB = dynamic_cast<PlayerCombat*>(Player->GetComponent(ComponentType::PlayerCombat));

			if (MasterObjectList->StoredPlayerCombat) {
				PCB->currentHP = MasterObjectList->StoredPlayerCombat->currentHP;
			}
			MasterObjectList->ImportInventory(IC);
		}
		if (MasterObjectList->StoredController->LevelTag != "Tutorial") {
			Engine->GetGameObject("DialogueBox")->GetComponent<LogicComponent>()->SetEnabled("CombatTutorialDialogue", false);
			Engine->GetGameObject("DialogueBox")->GetComponent<LogicComponent>()->SetEnabled("TypewriterEffect", false);

			Engine->GetGameObject("DialogueBox")->GetComponent<Renderer>()->SetVisibility(false);
			Engine->GetGameObject("DialogueBox")->GetComponent<GameUI>()->SetFontVisibility(false);
			Engine->GetGameObject("DialogueBoxNext")->GetComponent<Renderer>()->SetVisibility(false);
			Engine->GetGameObject("DialogueBoxNext")->GetComponent<GameUI>()->SetFontVisibility(false);

			Engine->GetObjectByPrefix("Player")->GetComponent<LogicComponent>()->SetEnabled("Combat", true);
			Engine->GetObjectByPrefix("Player")->GetComponent<LogicComponent>()->SetEnabled("CombatComponent", true);

			Engine->GetObjectByPrefix("Enemy")->GetComponent<LogicComponent>()->SetEnabled("EnemyAI", true);

			Engine->GetObjectByPrefix("Inventory Slot 1")->GetComponent<LogicComponent>()->SetEnabled("CombatUI", true);

			Engine->GetObjectByPrefix("Defeat")->GetComponent<LogicComponent>()->SetEnabled("Defeat", true);

			Engine->GetObjectByPrefix("Victory")->GetComponent<LogicComponent>()->SetEnabled("Victory", true);

			Engine->GetObjectByPrefix("Tooltip")->GetComponent<LogicComponent>()->SetEnabled("Tooltip", true);
		}
		Animate* FadeIn = dynamic_cast<Animate*>(Engine->GetGameObject("FadeIn")->GetComponent(ComponentType::Animate));
		FadeIn->SetVisibility(false);
		FadeIn->SetIsPlaying(false);
		MasterObjectList->MasterAssets.GetSprite(FadeIn->GetTexHandle()).idx = 0;
		Animate* FadeOut = dynamic_cast<Animate*>(Engine->GetGameObject("FadeOut")->GetComponent(ComponentType::Animate));
		FadeOut->SetVisibility(true);
		FadeOut->SetIsPlaying(true);
		MasterObjectList->MasterAssets.GetSprite(FadeOut->GetTexHandle()).idx = 0;

		if (MasterObjectList->StoredEnemyFilepath.size() > 2) {
			MasterObjectList->ImportEnemyFilepath(EnemyAI::enemyfp);
		}
	}

	void Update(GameObject* GO, float dt) {
		(void)dt;
		Animate* FadeOut = dynamic_cast<Animate*>(Engine->GetGameObject("FadeOut")->GetComponent(ComponentType::Animate));
		Animate* FadeIn = dynamic_cast<Animate*>(Engine->GetGameObject("FadeIn")->GetComponent(ComponentType::Animate));

		Animate* VFXa = dynamic_cast<Animate*>(Engine->GetGameObject("PlayerCritEffect")->GetComponent(ComponentType::Animate));
		PlayerAnimation::CheckStopAnimation(VFXa);
		Animate* VFX = dynamic_cast<Animate*>(Engine->GetGameObject("VFX")->GetComponent(ComponentType::Animate));
		PlayerAnimation::CheckStopAnimation(VFX);

		if (!PlayerAnimation::PlayAnimation(FadeOut)) {
			GameObject* Player = Engine->GetObjectByPrefix("Player");
			PlayerCombat* PCB = dynamic_cast<PlayerCombat*>(Player->GetComponent(ComponentType::PlayerCombat));
			ToMainMenu = CombatScript::ToMainMenu;

			if (PCB->FleeSuccess || CombatScript::ToMainMenu) {
				if (!FadeIn->GetIsPlaying())
					FadeIn->SetIsPlaying(true);
				if (!FadeIn->GetVisibility())
					FadeIn->SetVisibility(true);

				if (MasterObjectList->MasterAssets.GetSprite(FadeIn->GetTexHandle()).idx == MasterObjectList->MasterAssets.GetSprite(FadeIn->GetTexHandle()).frames_per_row - 1) {
					FadeIn->SetIsPlaying(false);
					PCB->FleeSuccess = false;
					CombatScript::ToMainMenu = false;
					Inventory* IC = dynamic_cast<Inventory*>(Player->GetComponent(ComponentType::Inventory));
					MasterObjectList->ExportInventory(IC);
					if (MasterObjectList->StoredController->LevelTag != "Tutorial") {
						MasterObjectList->ExportPlayerCombat(PCB);
					}
					LogicComponent* LC = dynamic_cast<LogicComponent*>(GO->GetComponent(ComponentType::Logic));
					LC->RemoveScriptKey(SKey);
				}
			}
		}
	}

	void End(GameObject* GO, float dt) {
		(void)dt;
		(void)GO;
		std::cout << "Switching scenes\n";
		audio->StopMusic();
		audio->StopSfx();
		if (ToMainMenu) { 
			SceneManager::StoreLevel("./Scene/Hub.scn");
			for (int i = 0; i < MasterObjectList->TempInventory->InventorySize; ++i) {
				if (MasterObjectList->TempInventory->Invent[i].Type == "Material") {
					MasterObjectList->TempInventory->Invent[i].Quantity /= 2;
					if (MasterObjectList->TempInventory->Invent[i].Quantity <= 0) {
						MasterObjectList->TempInventory->Invent[i].ClearItem();
					}
				}
				else if (MasterObjectList->TempInventory->Invent[i].Type == "Weapon" || MasterObjectList->TempInventory->Invent[i].Type == "Consumable"){
					MasterObjectList->TempInventory->Invent[i].ClearItem();
				}
			}
			MasterObjectList->TempInventory->AddItem(InventoryItem(1));
		}
		else {
			audio->PlaySfx(19); // Flee_Pass.mp3
			if (MasterObjectList->StoredController->LevelTag == "Tutorial") {
				SceneManager::StoreLevel("./Scene/TutorialLevel.scn");
			}
			else if (MasterObjectList->StoredController->LevelTag == "Level1_1"){
				SceneManager::StoreLevel("./Scene/FirstLevel_Map1.scn");
			}
			else if (MasterObjectList->StoredController->LevelTag == "Level1_2") {
				SceneManager::StoreLevel("./Scene/FirstLevel_Map2.scn");
			}
			else if (MasterObjectList->StoredController->LevelTag == "Level1_3") {
				SceneManager::StoreLevel("./Scene/FirstLevel_Map3.scn");
			}
			else if (MasterObjectList->StoredController->LevelTag == "Level1_4") {
				SceneManager::StoreLevel("./Scene/FirstLevel_Map4.scn");
			}
			else if (MasterObjectList->StoredController->LevelTag == "Level1_5") {
				SceneManager::StoreLevel("./Scene/FirstLevel_Map5.scn");
			}
			else if (MasterObjectList->StoredController->LevelTag == "Level1_6") {
				SceneManager::StoreLevel("./Scene/FirstLevel_Map6.scn");
			}
		}
	}
}