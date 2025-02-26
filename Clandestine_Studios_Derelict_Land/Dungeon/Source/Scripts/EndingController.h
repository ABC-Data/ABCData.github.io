/******************************************************************************/
/*!
\file		EndingController.h
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date		Jan 20, 2023
\brief		Controller Script for Ending scene

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

namespace EndingController {
	std::string SKey{ "EndingController" };
	int dialogue_counter = 0;
	int max_tut_dialogue = 2;
	bool hit_wall = false;
	int tutorial_event_counter = 0;
	bool can_move = false;
	bool fired_message = false;
	int event_counter_2 = 0;

	void Init(GameObject* GO, float dt) {
		(void)dt;
		Controller* CT = dynamic_cast<Controller*>(GO->GetComponent(ComponentType::Controller));
		audio->StopMusic();
		audio->StopSfx();
		audio->PlayMusic(0);
		dialogue_counter = 0;
		tutorial_event_counter = 0;
		event_counter_2 = 0;
		hit_wall = false;
		can_move = false;
		fired_message = false;
		if (CT != nullptr) {
			CT->LevelFlags.clear();
			for (int i = 0; i < CT->LevelFlagsSize; ++i) {
				CT->LevelFlags.push_back(false);
			}
			Engine->GetObjectByPrefix("Player")->GetComponent<LogicComponent>()->SetEnabled("Inventory", false);

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
			Engine->GetObjectByPrefix("Bag Icon")->GetComponent<Renderer>()->SetVisibility(false);
			Engine->GetObjectByPrefix("Bag Icon")->SetActive(false);
			Engine->GetObjectByPrefix("Play/")->GetComponent<Renderer>()->SetVisibility(false);
			Engine->GetObjectByPrefix("Play/")->SetActive(false);
		}
	}

	void Update(GameObject* GO, float dt) {
		Controller* CT = dynamic_cast<Controller*>(GO->GetComponent(ComponentType::Controller));
		//Animate* FadeIn = dynamic_cast<Animate*>(Engine->GetGameObject("FadeIn")->GetComponent(ComponentType::Animate));
		Animate* FadeOut = dynamic_cast<Animate*>(Engine->GetGameObject("FadeOut")->GetComponent(ComponentType::Animate));
		GameObject* P = Engine->GetObjectByPrefix("Player");
		GameObject* E = Engine->GetObjectByPrefix("Enemy");
		Collision* PCol = P->GetComponent<Collision>();
		Collision* ECol = E->GetComponent<Collision>();
		Transform* PT = P->GetComponent<Transform>();
		Transform* ET = E->GetComponent<Transform>();
		Animate* PAnim = P->GetComponent<Animate>();
		//Transform* CamT = Engine->GetObjectByPrefix("Camera")->GetComponent<Transform>();

		if (FadeOut->GetVisibility() && MasterObjectList->MasterAssets.GetSprite(FadeOut->GetTexHandle()).idx == MasterObjectList->MasterAssets.GetSprite(FadeOut->GetTexHandle()).frames_per_row - 1) {
			FadeOut->SetVisibility(false);
			if (CT->LevelFlags[0]) {
				can_move = true;
			}
		}

		if (!FadeOut->GetVisibility()) {
			PlayerAnimation::SmokeAnimation();
			if (!CT->LevelFlags[0]) {
				//Yu walks into scene
				//Stops at 8500
				PAnim->SetTexHandle("Yu_Run");
				P->GetComponent<RigidBody>()->MoveTo(PT->Position, { 8300.f, 410.f }, dt);
				P->GetComponent<LogicComponent>()->SetEnabled("Inventory", false);
				if (Engine->GetObjectInProximityByPrefix({ 8300.f, 410.f }, "Player", 10.f)) {
					PAnim->SetTexHandle("Yu_Idle");
					P->GetComponent<RigidBody>()->SetVelocity({ 0.f, 0.f });
					CT->LevelFlags[0] = true;
					can_move = true;
				}
			}
			else if (CT->LevelFlags[1] && !CT->LevelFlags[2]) {
			switch (tutorial_event_counter) {
			case 0:
			{
				dynamic_cast<Renderer*>(Engine->GetGameObject("Sir")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
				dynamic_cast<Renderer*>(Engine->GetGameObject("DialogueBox")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
				dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBox")->GetComponent(ComponentType::GameUI))->Alpha = 1.f;
				dynamic_cast<Renderer*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
				dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::GameUI))->Alpha = 1.f;
				dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::GameUI))->SetText("Sir");
				if (!fired_message) {
					TypewriterEffect::SetNewText("So. This is it...");
					fired_message = true;
				}
				if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
					tutorial_event_counter++;
					fired_message = false;
				}
				break;
			}
			case 1:
			{
				dynamic_cast<Renderer*>(Engine->GetGameObject("Sir")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
				dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::GameUI))->SetText("Sir");
				if (!fired_message) {
					TypewriterEffect::SetNewText("We can finally leave this forsaken place...");
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
				dynamic_cast<Renderer*>(Engine->GetGameObject("Sir")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
				dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::GameUI))->SetText("Yu");
				if (!fired_message) {
					TypewriterEffect::SetNewText("Ya man... Think Australia will take us in? Heh.");
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
				dynamic_cast<Renderer*>(Engine->GetGameObject("Sir")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
				dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::GameUI))->SetText("Sir");
				if (!fired_message) {
					TypewriterEffect::SetNewText("Hahaha... I have no doubt they would.");
					fired_message = true;
				}
				if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
					tutorial_event_counter++;
					fired_message = false;
					dynamic_cast<Renderer*>(Engine->GetGameObject("Sir")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
					dynamic_cast<Renderer*>(Engine->GetGameObject("DialogueBox")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
					dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBox")->GetComponent(ComponentType::GameUI))->Alpha = 0.f;
					dynamic_cast<Renderer*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
					dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::GameUI))->Alpha = 0.f;
					Engine->GetObjectByPrefix("Enemy")->GetComponent<Transform>()->Position.x = 0.f;
					CT->LevelFlags[2] = true;
				}
				break;
			}
			}
			}
			else if (CT->LevelFlags[2] && CT->LevelFlags[1]) {
				switch (event_counter_2) {
				case 0:
				{
					P->GetComponent<LogicComponent>()->SetEnabled("Camfollow", false);
					can_move = false;
					PAnim->SetTexHandle("Yu_Run");
					P->GetComponent<RigidBody>()->MoveTo(PT->Position, { 10500.f, 410.f }, dt);
					Engine->GetObjectByPrefix("ScreenTint")->GetComponent<Renderer>()->SetVisibility(true);
					Engine->GetObjectByPrefix("ScreenTint")->GetComponent<Renderer>()->Alpha_0to1 += dt / 2.f;
					if (Engine->GetObjectByPrefix("ScreenTint")->GetComponent<Renderer>()->Alpha_0to1 > 1.f) {
						Engine->GetObjectByPrefix("ScreenTint")->GetComponent<Renderer>()->Alpha_0to1 = 1.f;
					}
					if (Engine->GetObjectInProximityByPrefix({ 10500.f, 410.f }, "Player", 10.f)) {
						event_counter_2++;
					}
					break;
				}
				case 1:
				{
					dynamic_cast<Renderer*>(Engine->GetGameObject("DialogueBox")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
					dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBox")->GetComponent(ComponentType::GameUI))->Alpha = 1.f;
					if (!fired_message) {
						TypewriterEffect::SetNewText("And so, the last group of survivors in Singapore left the island.");
						fired_message = true;
					}
					if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
						event_counter_2++;
						fired_message = false;
					}
					break;
				}
				case 2:
				{
					if (!fired_message) {
						TypewriterEffect::SetNewText("Leaving behind a wasteland, they set sail for safer ground.");
						fired_message = true;
					}
					if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
						event_counter_2++;
						fired_message = false;
					}
					break;
				}
				case 3:
				{
					if (!fired_message) {
						TypewriterEffect::SetNewText("Who knows what might be in store for them in the future?");
						fired_message = true;
					}
					if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
						event_counter_2++;
						fired_message = false;
					}
					break;
				}
				case 4:
				{
					if (!fired_message) {
						TypewriterEffect::SetNewText("DERELICT LAND");
						fired_message = true;
					}
					if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
						event_counter_2++;
						fired_message = false;
					}
					break;
				}
				case 5:
				{
					if (!fired_message) {
						TypewriterEffect::SetNewText("CLANDESTINE STUDIOS");
						fired_message = true;
					}
					if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
						event_counter_2++;
						fired_message = false;
					}
					break;
				}
				case 6:
				{
					if (!fired_message) {
						TypewriterEffect::SetNewText("DigiPen Institute of Technology Singapore, CSD2400/2450");
						fired_message = true;
					}
					if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
						event_counter_2++;
						fired_message = false;
					}
					break;
				}
				case 7:
				{
					if (!fired_message) {
						TypewriterEffect::SetNewText("Lim Hui Ching");
						fired_message = true;
					}
					if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
						event_counter_2++;
						fired_message = false;
					}
					break;
				}
				case 8:
				{
					if (!fired_message) {
						TypewriterEffect::SetNewText("Brandon Hsu");
						fired_message = true;
					}
					if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
						event_counter_2++;
						fired_message = false;
					}
					break;
				}
				case 9:
				{
					if (!fired_message) {
						TypewriterEffect::SetNewText("Louis Mineo");
						fired_message = true;
					}
					if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
						event_counter_2++;
						fired_message = false;
					}
					break;
				}
				case 10:
				{
					if (!fired_message) {
						TypewriterEffect::SetNewText("Warren Ang");
						fired_message = true;
					}
					if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
						event_counter_2++;
						fired_message = false;
					}
					break;
				}
				case 11:
				{
					if (!fired_message) {
						TypewriterEffect::SetNewText("Anderson Phua");
						fired_message = true;
					}
					if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
						event_counter_2++;
						fired_message = false;
					}
					break;
				}
				case 12:
				{
					if (!fired_message) {
						TypewriterEffect::SetNewText("Kaiser Tan");
						fired_message = true;
					}
					if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
						event_counter_2++;
						fired_message = false;
					}
					break;
				}
				case 13:
				{
					if (!fired_message) {
						TypewriterEffect::SetNewText("Reyes Tan");
						fired_message = true;
					}
					if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
						event_counter_2++;
						fired_message = false;
					}
					break;
				}
				case 14:
				{
					if (!fired_message) {
						TypewriterEffect::SetNewText("Sofian Zahid");
						fired_message = true;
					}
					if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
						event_counter_2++;
						fired_message = false;
					}
					break;
				}
				case 15:
				{
					if (!fired_message) {
						TypewriterEffect::SetNewText("Thank you for playing!");
						fired_message = true;
					}
					if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
						event_counter_2++;
						fired_message = false;
						dynamic_cast<Renderer*>(Engine->GetGameObject("DialogueBox")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
						dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBox")->GetComponent(ComponentType::GameUI))->Alpha = 0.f;
						GO->GetComponent<LogicComponent>()->RemoveScriptKey(SKey);
					}
					break;
				}
				}

			}
			else {
				PCol->SetBoundingBox(PT->Position, PT->Scale, PT->Dimensions);
				ECol->SetBoundingBox(ET->Position, ET->Scale, ET->Dimensions);

				if (!hit_wall && CollisionIntersection_RectRect(PCol->BoundingBox, PCol->Vel, ECol->BoundingBox, ECol->Vel, dt)) {
					can_move = false;
					PAnim->SetTexHandle("Yu_Idle");
					hit_wall = true;
					CT->LevelFlags[1] = true;
				}

				P->GetComponent<LogicComponent>()->SetEnabled("MMovement", can_move);
			}
			
		}
		

	}

	void End(GameObject* GO, float dt) {
		(void)dt;
		(void)GO;
		SceneManager::StoreLevel("./Scene/MainMenu.scn");
	}
}