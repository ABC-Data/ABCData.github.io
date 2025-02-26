/******************************************************************************/
/*!
\file		Level1Controller6.h
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date		Jan 20, 2023
\brief		Controller Script for Level 1 area 6

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
#include "Scripts/TypewriterEffect.h"

namespace Level1Controller6 {
	std::string SKey{ "Level1Controller6" };
	float aggro_timer = 0.f;
	float aux_timer = 0.f;
	bool can_move = false;
	bool triggered_combat = false;

	/*
	WARP FLAGS:
	LEFT ARROW (AREA 4) - 3
	*/

	void Init(GameObject* GO, float dt) {
		(void)dt;
		GameObject* P = Engine->GetObjectByPrefix("Player");
		Controller* CT = GO->GetComponent<Controller>();
		if (MasterObjectList->TempInventory) {
			MasterObjectList->ImportInventory(P->GetComponent<Inventory>());
		}
		audio->StopMusic();
		audio->StopSfx();
		audio->PlayMusic(0);
		if (CT != nullptr) {
			if (MasterObjectList->StoredController != nullptr) {
				if (MasterObjectList->StoredController->LevelTag == "Level1_6") {
					MasterObjectList->ImportController(CT);
					MasterObjectList->ImportPlayerPosition(P->GetComponent<Transform>());
					Engine->GetObjectComponent<Transform>(Engine->GetObjectByPrefix("Camera"))->SetPosition(P->GetComponent<Transform>()->Position);
				}
				else if (MasterObjectList->StoredController->LevelTag == "Level1_4" || MasterObjectList->StoredController->LevelTag == "Level1_5") {
					//Set player position to correct "side"
					MasterObjectList->ClearController();
					P->GetComponent<Transform>()->Position.x = Engine->GetGameObject("ArrowLeft")->GetComponent<Transform>()->Position.x + 100.f;
					Engine->GetObjectComponent<Transform>(Engine->GetObjectByPrefix("Camera"))->SetPosition(P->GetComponent<Transform>()->Position);
				}
				else {
					//Unknown place or hub?
					MasterObjectList->ClearController();
				}
			}
			else {
				CT->LevelFlags.clear();
				for (int i = 0; i < CT->LevelFlagsSize; ++i) {
					CT->LevelFlags.push_back(false);
				}
			}
		}
		if (Engine->GetObjectComponent<Transform>(Engine->GetObjectByPrefix("Camera"))->Position.x < Engine->GetLevel()->GetLevelMin()) {
			Engine->GetObjectComponent<Transform>(Engine->GetObjectByPrefix("Camera"))->Position.x = Engine->GetLevel()->GetLevelMin();
		}
		else if (Engine->GetObjectComponent<Transform>(Engine->GetObjectByPrefix("Camera"))->Position.x > Engine->GetLevel()->GetLevelMax()) {
			Engine->GetObjectComponent<Transform>(Engine->GetObjectByPrefix("Camera"))->Position.x = Engine->GetLevel()->GetLevelMax();
		}
		can_move = false;
		aggro_timer = 0.f;
		aux_timer = 0.f;
		triggered_combat = false;
		Animate* FadeIn = dynamic_cast<Animate*>(Engine->GetGameObject("FadeIn")->GetComponent(ComponentType::Animate));
		FadeIn->SetVisibility(false);
		FadeIn->SetIsPlaying(false);
		MasterObjectList->MasterAssets.GetSprite(FadeIn->GetTexHandle()).idx = 0;
		Animate* FadeOut = dynamic_cast<Animate*>(Engine->GetGameObject("FadeOut")->GetComponent(ComponentType::Animate));
		FadeOut->SetVisibility(true);
		FadeOut->SetIsPlaying(true);
		MasterObjectList->MasterAssets.GetSprite(FadeOut->GetTexHandle()).idx = 0;
		P->GetComponent<LogicComponent>()->SetEnabled("MMovement", can_move);
		P->GetComponent<LogicComponent>()->SetEnabled("Inventory", can_move);
	}

	void Update(GameObject* GO, float dt) {
		Animate* FadeIn = dynamic_cast<Animate*>(Engine->GetGameObject("FadeIn")->GetComponent(ComponentType::Animate));
		Animate* FadeOut = dynamic_cast<Animate*>(Engine->GetGameObject("FadeOut")->GetComponent(ComponentType::Animate));
		Controller* CT = GO->GetComponent<Controller>();
		GameObject* P = Engine->GetObjectByPrefix("Player");
		if (FadeOut->GetVisibility() && MasterObjectList->MasterAssets.GetSprite(FadeOut->GetTexHandle()).idx == MasterObjectList->MasterAssets.GetSprite(FadeOut->GetTexHandle()).frames_per_row - 1) {
			// DO ONCE: Remove fadeout and allow movement
			FadeOut->SetVisibility(false);
			can_move = true;
		}

		if (!FadeOut->GetVisibility()) {
			std::vector<GameObject*> car_vec;
			Engine->GetAllObjectsByPrefix(car_vec, "Car");
			std::vector<GameObject*> arrow_vec;
			Engine->GetAllObjectsByPrefix(arrow_vec, "Arrow");
			int arrow_counter = 0;
			int car_counter = 0;
			for (GameObject* Car : car_vec) {
				if (!CT->LevelFlags[(size_t)car_counter] && Engine->GetObjectInProximityByPrefix(Car->GetComponent<Transform>()->Position, "Player", 500.f)) {
					Car->GetComponent<LogicComponent>()->SetEnabled("BreatheEffect", true);
					if (Input::CheckKeyTriggered(Key::E)) {
						CT->LevelFlags[(size_t)car_counter] = true;
						Inventory* IC = P->GetComponent<Inventory>();
						Transform* CarT = Car->GetComponent<Transform>();
						audio->PlaySfx(31);
						for (size_t i = 0; i < 3; ++i) {
							int item = rand() % 100;
							int item_idx = 0;
							int qty = 1;
							if (item < 40) {
								item_idx = 13;
							}
							else {
								item_idx = 27;
								qty = (rand() % 3) + 1;
							}

							for (size_t j = 0; j < (size_t)qty; ++j) {
								IC->AddItem(InventoryItem(item_idx));
									
							}
							GameObject* ItemVFX = factory->AddGameObject("./Data/WorldInventoryItem.txt");
							ItemVFX->SetName(ItemVFX->GetName() + std::to_string(i));
							ItemVFX->GetComponent<Transform>()->Position = { CarT->Position.x + (((int)i - 1) * 240.f), CarT->Position.y + 94.f };
							ItemVFX->GetComponent<Renderer>()->SetTexHandle(MasterObjectList->GetInventoryItem(item_idx).TexHandle);
							ItemVFX->GetComponent<Renderer>()->SetUVMin(MasterObjectList->GetInventoryItem(item_idx).Tex_UV_Min_COORD);
							ItemVFX->GetComponent<Renderer>()->SetUVMax(MasterObjectList->GetInventoryItem(item_idx).Tex_UV_Max_COORD);
							factory->QueueAdd(ItemVFX);
						}
					}
				}
				else {
					Car->GetComponent<LogicComponent>()->SetEnabled("BreatheEffect", false);
					Car->GetComponent<Renderer>()->Alpha_0to1 = 1.f;
				}
				++car_counter;
			}

			for (GameObject* Arrow : arrow_vec) {
				if (!CT->LevelFlags[(size_t)arrow_counter + 3] && Engine->GetObjectInProximityByPrefix(Arrow->GetComponent<Transform>()->Position, "Player", 300.f)) {
					Arrow->GetComponent<LogicComponent>()->SetEnabled("BreatheEffect", true);
					if (Input::CheckKeyTriggered(Key::E)) {
						CT->LevelFlags[(size_t)arrow_counter + 3] = true;
					}
				}
				else {
					Arrow->GetComponent<LogicComponent>()->SetEnabled("BreatheEffect", false);
					Arrow->GetComponent<Renderer>()->Alpha_0to1 = 1.f;
				}
				++arrow_counter;
			}

			if (can_move && Vec2SquareLength(P->GetComponent<RigidBody>()->Velocity) > 0.f) {
				aux_timer += dt;
			}
			if (aux_timer >= 0.5f) {
				aggro_timer += 0.5f;
				aux_timer = 0.f;
				if (aggro_timer > 5.f) {
					int monster_roll = rand() % 100;
					int hit = static_cast<int>(min((2.f * aggro_timer) - 8.f, 8.f));
					if (monster_roll <= hit) {
						triggered_combat = true;
						aggro_timer = 0.f;
					}
				}
			}



			if (triggered_combat || CT->LevelFlags[3] || CT->LevelFlags[4] || CT->LevelFlags[5]) {

				if (CT->LevelFlags[3] || CT->LevelFlags[4] || CT->LevelFlags[5]) {
					//Warping takes priority over combat
					triggered_combat = false;
				}
				FadeIn->SetVisibility(true);
				FadeIn->SetIsPlaying(true);

				if (MasterObjectList->MasterAssets.GetSprite(FadeIn->GetTexHandle()).idx == MasterObjectList->MasterAssets.GetSprite(FadeIn->GetTexHandle()).frames_per_row - 1) {
					if (triggered_combat) {
						audio->PlaySfx(16);
						MasterObjectList->ExportPlayerPosition(P->GetComponent<Transform>());
					}
					MasterObjectList->ExportInventory(P->GetComponent<Inventory>());
					GO->GetComponent<LogicComponent>()->RemoveScriptKey(SKey);
					MasterObjectList->ExportController(CT);
				}
			}
			P->GetComponent<LogicComponent>()->SetEnabled("MMovement", can_move);
			P->GetComponent<LogicComponent>()->SetEnabled("Inventory", can_move);
		}
	}

	void End(GameObject* GO, float dt) {
		(void)dt;
		Controller* CT = GO->GetComponent<Controller>();
		if (triggered_combat) {
			MasterObjectList->ExportEnemyFilepath("./Data/Enemies/4.txt");
			SceneManager::StoreLevel("./Scene/Combat.scn");
			return;
		}

		if (CT->LevelFlags[3]) {
			SceneManager::StoreLevel("./Scene/FirstLevel_Map4.scn");
		}
		else {
			//Invalid teleport!
			SceneManager::StoreLevel("./Scene/Hub.scn");
		}
	}
}