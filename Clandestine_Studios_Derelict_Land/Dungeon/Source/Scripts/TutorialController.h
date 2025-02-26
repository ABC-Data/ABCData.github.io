/******************************************************************************/
/*!
\file		TutorialController.h
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date		Jan 20, 2023
\brief		Controller Script for Tutorial Level

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

namespace TutorialController {
	std::string SKey{ "TutorialController" };
	int dialogue_counter = 0;
	int max_tut_dialogue = 2;
	bool hit_wall = false;
	int tutorial_event_counter = 0;
	bool can_move = false;
	bool fired_message = false;

	void Init(GameObject* GO, float dt) {
		(void)dt;
		Controller* CT = dynamic_cast<Controller*>(GO->GetComponent(ComponentType::Controller));
		audio->StopMusic();
		audio->StopSfx();
		audio->PlayMusic(0);
		dialogue_counter = 0;
		tutorial_event_counter = 0;
		hit_wall = false;
		can_move = false;
		fired_message = false;
		if (CT != nullptr) {
			if (MasterObjectList->StoredController != nullptr) {
				//Came from another level, which came from this level
				if (MasterObjectList->StoredController->LevelTag == "Tutorial") {
					MasterObjectList->ImportController(CT);
					GameObject* Player = Engine->GetObjectByPrefix("Player");
					Inventory* IC = dynamic_cast<Inventory*>(Player->GetComponent(ComponentType::Inventory));
					Transform* T = dynamic_cast<Transform*>(Player->GetComponent(ComponentType::Transform));
					MasterObjectList->ImportInventory(IC);
					MasterObjectList->ImportPlayerPosition(T);
					Engine->GetObjectComponent<Transform>(Engine->GetObjectByPrefix("Camera"))->SetPosition(T->Position);
				}
			}
			else {
				CT->LevelFlags.clear();
				for (int i = 0; i < CT->LevelFlagsSize; ++i) {
					CT->LevelFlags.push_back(false);
				}
			}
			if (CT->LevelFlags[0]) {
				Engine->GetObjectByPrefix("Player")->GetComponent<LogicComponent>()->SetEnabled("Inventory", true);
				if (CT->LevelFlags[2]) {
					Engine->GetObjectByPrefix("Enemy")->GetComponent<Transform>()->SetPosition({ 9100.f, 400.f });
				}
				else if (CT->LevelFlags[1]) {
					Engine->GetObjectByPrefix("Enemy")->GetComponent<Transform>()->SetPosition({ 6000.f, 400.f });
				}
				else {
					Engine->GetObjectByPrefix("Enemy")->GetComponent<Transform>()->SetPosition({ 3000.f, 400.f });
				}
			}
			else {
				Engine->GetObjectByPrefix("Player")->GetComponent<LogicComponent>()->SetEnabled("Inventory", false);
			}

			Animate* FadeIn = dynamic_cast<Animate*>(Engine->GetGameObject("FadeIn")->GetComponent(ComponentType::Animate));
			FadeIn->SetVisibility(false);
			FadeIn->SetIsPlaying(false);			
			MasterObjectList->MasterAssets.GetSprite(FadeIn->GetTexHandle()).idx = 0;
			Animate* FadeOut = dynamic_cast<Animate*>(Engine->GetGameObject("FadeOut")->GetComponent(ComponentType::Animate));
			FadeOut->SetVisibility(true);
			FadeOut->SetIsPlaying(true);
			MasterObjectList->MasterAssets.GetSprite(FadeOut->GetTexHandle()).idx = 0;
			//Disable player movement until fade animation plays
			Engine->GetObjectByPrefix("Player")->GetComponent<LogicComponent>()->SetEnabled("MMovement", false);
		}
	}

	void Update(GameObject* GO, float dt) {
		Controller* CT = dynamic_cast<Controller*>(GO->GetComponent(ComponentType::Controller));
		Animate* FadeIn = dynamic_cast<Animate*>(Engine->GetGameObject("FadeIn")->GetComponent(ComponentType::Animate));
		Animate* FadeOut = dynamic_cast<Animate*>(Engine->GetGameObject("FadeOut")->GetComponent(ComponentType::Animate));
		GameObject* C1 = Engine->GetGameObject("Car1");
		GameObject* C2 = Engine->GetGameObject("Car2");
		GameObject* P = Engine->GetObjectByPrefix("Player");
		GameObject* E = Engine->GetObjectByPrefix("Enemy");
		Collision* PCol = P->GetComponent<Collision>();
		Collision* ECol = E->GetComponent<Collision>();
		Transform* PT = P->GetComponent<Transform>();
		Transform* ET = E->GetComponent<Transform>();
		Animate* PAnim = P->GetComponent<Animate>();
		Transform* CamT = Engine->GetObjectByPrefix("Camera")->GetComponent<Transform>();

		if (FadeOut->GetVisibility() && MasterObjectList->MasterAssets.GetSprite(FadeOut->GetTexHandle()).idx == MasterObjectList->MasterAssets.GetSprite(FadeOut->GetTexHandle()).frames_per_row - 1) {
			FadeOut->SetVisibility(false);
			if (CT->LevelFlags[0]) {
				can_move = true;
			}
		}
		if (!FadeOut->GetVisibility()) {
			PlayerAnimation::SmokeAnimation();
			if (!CT->LevelFlags[0]) {
				switch (tutorial_event_counter) {
				case 0:
				{
					//Yu walks into scene
					//Stops at -1900
					PAnim->SetTexHandle("Yu_Run");
					P->GetComponent<RigidBody>()->MoveTo(PT->Position, { -1900.f, 410.f }, dt);
					P->GetComponent<LogicComponent>()->SetEnabled("Inventory", false);
					if (Engine->GetObjectInProximityByPrefix({ -1900.f, 410.f }, "Player", 10.f)) {
						PAnim->SetTexHandle("Yu_Idle");
						P->GetComponent<RigidBody>()->SetVelocity({ 0.f, 0.f });
						tutorial_event_counter++;
					}
					break;
				}
				case 1:
				{
					//Sir to Yu line 1
					dynamic_cast<Renderer*>(Engine->GetGameObject("Sir")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
					dynamic_cast<Renderer*>(Engine->GetGameObject("DialogueBox")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
					dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBox")->GetComponent(ComponentType::GameUI))->Alpha = 1.f;
					dynamic_cast<Renderer*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
					dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::GameUI))->Alpha = 1.f;
					dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::GameUI))->SetText("Sir");
					Engine->GetGameObject("TutorialLevel_KeyPrompt")->GetComponent<Transform>()->Position = { -808.f, 60.f };
					if (!fired_message) {
						TypewriterEffect::SetNewText("Yu. Do you copy? You reached the pier already?");
						fired_message = true;
					}
					if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
						tutorial_event_counter++;
						fired_message = false;
					}
					break;
				}
				case 2:
				{
					//Yu reply
					dynamic_cast<Renderer*>(Engine->GetGameObject("Sir")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
					dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::GameUI))->SetText("Yu");
					if (!fired_message) {
						TypewriterEffect::SetNewText("Yes Sir. What should I do now?");
						fired_message = true;
					}
					if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
						tutorial_event_counter++;
						fired_message = false;
					}
					break;
				}
				case 3:
				{
					//Sir gives mouse movement instructions
					dynamic_cast<Renderer*>(Engine->GetGameObject("Sir")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
					dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::GameUI))->SetText("Sir");
					if (!fired_message) {
						TypewriterEffect::SetNewText("Get to the end of the pier. Grab what you can on the way.");
						fired_message = true;
					}
					if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
						tutorial_event_counter++;
						fired_message = false;
					}
					break;
				}
				case 4:
				{
					//Sir gives mouse movement instructions
					dynamic_cast<Renderer*>(Engine->GetGameObject("Sir")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
					dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::GameUI))->SetText("Sir");
					if (!fired_message) {
						TypewriterEffect::SetNewText("I'd say cars are good to search. Use LEFT CLICK to move, E to interact.");
						fired_message = true;
					}
					if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
						tutorial_event_counter++;
						fired_message = false;
					}
					break;
				}
				case 5:
				{
					//Sir gives mouse movement instructions
					dynamic_cast<Renderer*>(Engine->GetGameObject("Sir")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
					dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::GameUI))->SetText("Yu");
					if (!fired_message) {
						TypewriterEffect::SetNewText("Roger Sir.");
						fired_message = true;
					}
					if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
						tutorial_event_counter++;
						fired_message = false;
						can_move = true;
						dynamic_cast<Renderer*>(Engine->GetGameObject("Sir")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
						dynamic_cast<Renderer*>(Engine->GetGameObject("DialogueBox")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
						dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBox")->GetComponent(ComponentType::GameUI))->Alpha = 0.f;
						dynamic_cast<Renderer*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
						dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::GameUI))->Alpha = 0.f;
						Engine->GetGameObject("TutorialLevel_KeyPrompt")->GetComponent<Transform>()->Position = { 1588.f, 356.f };
						P->GetComponent<LogicComponent>()->SetEnabled("Inventory", true);
						TypewriterEffect::SetNewText("GRAHHH!!");
						CT->LevelFlags[0] = true;
					}
					break;
				}
				}
			}
			else {
				//Object 1
				if (!CT->LevelFlags[1] && Engine->GetObjectInProximityByPrefix({ 1648.f , 400.f }, "Player", 500.f)) {
					if (C1 != nullptr) {
						Engine->GetGameObject("TutorialLevel_KeyPrompt")->GetComponent<Renderer>()->SetVisibility(true);
						Engine->GetGameObject("TutorialLevel_KeyPrompt")->GetComponent<GameUI>()->SetFontVisibility(true);
						C1->GetComponent<LogicComponent>()->SetEnabled("BreatheEffect", true);
					}
				}
				else {
					C1->GetComponent<LogicComponent>()->SetEnabled("BreatheEffect", false);
					C1->GetComponent<Renderer>()->Alpha_0to1 = 1.f;
				}
				//Object 2
				if (!CT->LevelFlags[2] && Engine->GetObjectInProximityByPrefix({ 4794.f, 400.f }, "Player", 500.f)) {
					if (C2 != nullptr) {
						Engine->GetGameObject("TutorialLevel_KeyPrompt")->GetComponent<Renderer>()->SetVisibility(true);
						Engine->GetGameObject("TutorialLevel_KeyPrompt")->GetComponent<GameUI>()->SetFontVisibility(true);
						C2->GetComponent<LogicComponent>()->SetEnabled("BreatheEffect", true);
					}
				}
				else {
					C2->GetComponent<LogicComponent>()->SetEnabled("BreatheEffect", false);
					C2->GetComponent<Renderer>()->Alpha_0to1 = 1.f;
				}

				if (Engine->GetObjectInProximityByPrefix({ 1648.f , 400.f }, "Player", 500.f) == nullptr && Engine->GetObjectInProximityByPrefix({ 4794.f, 400.f }, "Player", 500.f) == nullptr
					&& !(CT->LevelFlags[1] && CT->LevelFlags[2])) {
					Engine->GetGameObject("TutorialLevel_KeyPrompt")->GetComponent<Renderer>()->SetVisibility(false);
					Engine->GetGameObject("TutorialLevel_KeyPrompt")->GetComponent<GameUI>()->SetFontVisibility(false);
				}

				PCol->SetBoundingBox(PT->Position, PT->Scale, PT->Dimensions);
				ECol->SetBoundingBox(ET->Position, ET->Scale, ET->Dimensions);

				if (!hit_wall && CollisionIntersection_RectRect(PCol->BoundingBox, PCol->Vel, ECol->BoundingBox, ECol->Vel, dt)) {
					can_move = false;

					dynamic_cast<Renderer*>(Engine->GetGameObject("DialogueBox")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
					dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBox")->GetComponent(ComponentType::GameUI))->Alpha = 1.f;
					dynamic_cast<Renderer*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
					dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::GameUI))->Alpha = 1.f;
					dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::GameUI))->SetText("Yu");
					TypewriterEffect::SetNewText("Hmm... Maybe I should go check that car out...");
					PAnim->SetTexHandle("Yu_Idle");
					hit_wall = true;
				}

				if (Input::CheckKeyTriggered(Key::E)) {
					if (hit_wall && TypewriterEffect::done) {
						dynamic_cast<Renderer*>(Engine->GetGameObject("DialogueBox")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
						dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBox")->GetComponent(ComponentType::GameUI))->Alpha = 0.f;
						dynamic_cast<Renderer*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
						dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::GameUI))->Alpha = 0.f;
						can_move = true;
						if (PT->Dimensions.x > 0.f) {
							PT->Dimensions.x = -PT->Dimensions.x;
						}
						hit_wall = false;
						TypewriterEffect::SetNewText("GRAHHH!!");
					}

					//Object 1
					if (!CT->LevelFlags[1] && Engine->GetObjectInProximityByPrefix({ 1648.f , 400.f }, "Player", 500.f)) {
						//Do stuff
						std::cout << "Triggered Object 1\n";
						GameObject* Player = Engine->GetObjectByPrefix("Player", ComponentType::Inventory);
						if (Player != nullptr) {
							Inventory* IC = dynamic_cast<Inventory*>(Player->GetComponent(ComponentType::Inventory));
							if (IC != nullptr) {
								IC->AddItem(InventoryItem(7));
								audio->PlaySfx(31);
								std::cout << "You got an item!\n";
							}
						}
						GameObject* ItemVFX = factory->AddGameObject("./Data/WorldInventoryItem.txt");
						ItemVFX->GetComponent<Transform>()->Position = { 1648.f, 200.f };
						ItemVFX->GetComponent<Renderer>()->SetTexHandle(MasterObjectList->GetInventoryItem(7).TexHandle);
						ItemVFX->GetComponent<Renderer>()->SetUVMin(MasterObjectList->GetInventoryItem(7).Tex_UV_Min_COORD);
						ItemVFX->GetComponent<Renderer>()->SetUVMax(MasterObjectList->GetInventoryItem(7).Tex_UV_Max_COORD);
						factory->QueueAdd(ItemVFX);

						CT->LevelFlags[1] = true;
						GameObject* Marker1 = Engine->GetGameObject("Car1");
						if (Marker1 != nullptr) {
							Marker1->GetComponent<LogicComponent>()->RemoveScriptKey("BreatheEffect");
						}

						//move the Prompt to the 2nd obj's x postition
						Transform* trf_prompt = dynamic_cast<Transform*>(Engine->GetGameObject("TutorialLevel_KeyPrompt")->GetComponent(ComponentType::Transform));
						trf_prompt->Position.x = dynamic_cast<Transform*>(Engine->GetGameObject("Car2")->GetComponent(ComponentType::Transform))->Position.x;
						Engine->GetObjectByPrefix("Enemy")->GetComponent<Transform>()->SetPosition({ 6000.f, 400.f });


					}
					//Object 2
					if (!CT->LevelFlags[2] && Engine->GetObjectInProximityByPrefix({ 4794.f, 400.f }, "Player", 500.f)) {
						//Do stuff
						std::cout << "Triggered Object 2\n";
						//GameObject* Player = Engine->GetObjectByPrefix("Player");
						bool discard;
						can_move = false;
						PAnim->SetTexHandle("Yu_Idle");
						//SET dialogue box to visible render later
						dynamic_cast<Renderer*>(Engine->GetGameObject("DialogueBox")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
						dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBox")->GetComponent(ComponentType::GameUI))->Alpha = 1.f;
						dynamic_cast<Renderer*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
						dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::GameUI))->Alpha = 1.f;
						Engine->GetGameObject("TutorialLevel_KeyPrompt")->GetComponent<Transform>()->Position = { CamT->Position.x + 592.f, CamT->Position.y - 350.f };
						P->GetComponent<LogicComponent>()->SetEnabled("Inventory", false);
						if (dialogue_counter == 0) {
							dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::GameUI))->SetText("Ghoul");
						}
						if (TypewriterEffect::last_idx < 3) {
							discard = Input::GetKeyTriggered(Key::E);
						}

						if (TypewriterEffect::done == true)
						{
							dialogue_counter++;
							if (CT->LevelFlags[2] == false)
							{
								if (dialogue_counter == 1) {
									dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::GameUI))->Text = "Yu";
									discard = Input::GetKeyTriggered(Key::E);
									TypewriterEffect::SetNewText("It's one of these monsters again. Now's a good time to work out a bit...");
								}
								
							}

							if (dialogue_counter >= max_tut_dialogue) {
								FadeIn->SetVisibility(true);
								FadeIn->SetIsPlaying(true);
							}
						}
					}
				}

				if (MasterObjectList->MasterAssets.GetSprite(FadeIn->GetTexHandle()).idx == MasterObjectList->MasterAssets.GetSprite(FadeIn->GetTexHandle()).frames_per_row - 1
					&& dialogue_counter >= max_tut_dialogue) {
					GameObject* Player = Engine->GetObjectByPrefix("Player");
					Inventory* IC = dynamic_cast<Inventory*>(Player->GetComponent(ComponentType::Inventory));
					Transform* T = dynamic_cast<Transform*>(Player->GetComponent(ComponentType::Transform));
					LogicComponent* LC = dynamic_cast<LogicComponent*>(GO->GetComponent(ComponentType::Logic));

					audio->PlaySfx(16);
					//MOVED DOWN TO CALL AFTER THE TEXT HAS STOPPED PRINTING
					MasterObjectList->ExportInventory(IC);
					MasterObjectList->ExportPlayerPosition(T);
					LC->RemoveScriptKey(SKey);
					CT->LevelFlags[2] = true;
					MasterObjectList->ExportController(CT);
					GameObject* Marker2 = Engine->GetGameObject("Car2");
					if (Marker2 != nullptr) {
						Marker2->GetComponent<LogicComponent>()->RemoveScriptKey("BreatheEffect");
					}
					//set the prompt to not visible & alpha to 0
					dynamic_cast<Renderer*>(Engine->GetGameObject("TutorialLevel_KeyPrompt")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
					dynamic_cast<GameUI*>(Engine->GetGameObject("TutorialLevel_KeyPrompt")->GetComponent(ComponentType::GameUI))->Alpha = 0.f;
				}

				if (!CT->LevelFlags[3] && Engine->GetObjectInProximityByPrefix({ 8900.f, 410.f }, "Player", 300.f)) {
					//GameObject* Player = Engine->GetObjectByPrefix("Player");
					can_move = false;
					PAnim->SetTexHandle("Yu_Idle");
					//SET dialogue box to visible render later
					dynamic_cast<Renderer*>(Engine->GetGameObject("DialogueBox")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
					dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBox")->GetComponent(ComponentType::GameUI))->Alpha = 1.f;
					dynamic_cast<Renderer*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
					dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::GameUI))->Alpha = 1.f;
					dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::GameUI))->SetText("Sir");
					dynamic_cast<Renderer*>(Engine->GetGameObject("Sir")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
					P->GetComponent<LogicComponent>()->SetEnabled("Inventory", false);
					Engine->GetGameObject("TutorialLevel_KeyPrompt")->GetComponent<Transform>()->Position = { CamT->Position.x + 592.f, CamT->Position.y - 350.f };
					if (dialogue_counter == 0) {
						TypewriterEffect::SetNewText("Well done, Yu. Head back to base for next instructions.");
						dialogue_counter++;
					}

					if (Input::CheckKeyTriggered(Key::E) && TypewriterEffect::done == true) {
						FadeIn->SetVisibility(true);
						FadeIn->SetIsPlaying(true);
					}

					if (MasterObjectList->MasterAssets.GetSprite(FadeIn->GetTexHandle()).idx == MasterObjectList->MasterAssets.GetSprite(FadeIn->GetTexHandle()).frames_per_row - 1) {
						//Go to hub
						CT->LevelFlags[3] = true;
						MasterObjectList->ExportInventory(P->GetComponent<Inventory>());
						GO->GetComponent<LogicComponent>()->RemoveScriptKey(SKey);
					}
				}
				P->GetComponent<LogicComponent>()->SetEnabled("MMovement", can_move);
			}
			
		}
		

	}

	void End(GameObject* GO, float dt) {
		(void)dt;
		(void)GO;
		std::cout << "Switching scenes\n";
		if (GO->GetComponent<Controller>()->LevelFlags[3]) {
			SceneManager::StoreLevel("./Scene/Hub.scn");
		}
		else {
			MasterObjectList->ExportEnemyFilepath("./Data/Enemies/1.txt");
			SceneManager::StoreLevel("./Scene/Combat.scn");
		}
	}
}