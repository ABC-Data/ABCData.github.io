/******************************************************************************/
/*!
\file		HubController.h
\author 	Brandon Hsu, Louis Mineo @ Linn Min Htoo
\par    	email: b.hsu\@digipen.edu , 620007421, louismineo.l@digipen.edu
\date		Mar 3, 2023
\brief		Controller Script for Hub Level

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
#include "GameUI/GameUI.h"
#include "Graphics/GLFWsetup.h"
#include "Crafting.h"
#include "Cooking.h"

extern bool is_paused;
extern MouseInput MyMouse;
extern bool keystateLM;
extern bool prevloop_keystateLM;
bool craftingcooking_open, questui_open;

namespace Quests {
	std::string current_quest;
	std::string next_scn_filepath;
	bool selected_changed;
	int mouse_x, mouse_y_aft_TRS;
	bool can_switch_scene;
	std::map<std::string, std::string> QuestName_TO_QuestDesc;
	std::map<std::string, std::string> QuestName_TO_ScnFilePath;
}

namespace HubController {
	std::string SKey{ "HubController" };
	int dialogue_counter = 0;
	int max_tut_dialogue = 2;
	bool hit_wall = false;
	int event_counter = 0;
	bool can_move = false;
	bool fired_message = false;
	Vec2 stored_cam = { 0.f, 0.f };
	bool changed_level = false;
	std::string levelpath;


	/**************************************************************************/
	/*!
		\brief
			This function either displays or hides the hub's QuestUI Gameobjects

		\param t_f
			True or False, true to show, false to hide
	*/
	/**************************************************************************/
	inline void SetQuestsGOs(bool t_f)
	{
		//background
		dynamic_cast<Renderer*>(Engine->GetGameObject("QuestUI Main")->GetComponent(ComponentType::Renderer))->SetVisibility(t_f);

		//item 1
		dynamic_cast<Renderer*>(Engine->GetGameObject("QuestUI Item1")->GetComponent(ComponentType::Renderer))->SetVisibility(t_f);
		dynamic_cast<GameUI*>(Engine->GetGameObject("QuestUI Item1")->GetComponent(ComponentType::GameUI))->SetFontVisibility(t_f);

		//item 2
		dynamic_cast<Renderer*>(Engine->GetGameObject("QuestUI Item2")->GetComponent(ComponentType::Renderer))->SetVisibility(t_f);
		dynamic_cast<GameUI*>(Engine->GetGameObject("QuestUI Item2")->GetComponent(ComponentType::GameUI))->SetFontVisibility(t_f);

		//item 3
		//dynamic_cast<Renderer*>(Engine->GetGameObject("QuestUI Item3")->GetComponent(ComponentType::Renderer))->SetVisibility(t_f);
		//dynamic_cast<GameUI*>(Engine->GetGameObject("QuestUI Item3")->GetComponent(ComponentType::GameUI))->SetFontVisibility(t_f);

		//QuestUI Desc
		dynamic_cast<GameUI*>(Engine->GetGameObject("QuestUI Desc")->GetComponent(ComponentType::GameUI))->SetFontVisibility(t_f);
		dynamic_cast<GameUI*>(Engine->GetGameObject("QuestUI Desc")->GetComponent(ComponentType::GameUI))->SetText("DESCRIPTION:");

		//QuestUI Req
		dynamic_cast<GameUI*>(Engine->GetGameObject("QuestUI Req")->GetComponent(ComponentType::GameUI))->SetFontVisibility(t_f);
		dynamic_cast<GameUI*>(Engine->GetGameObject("QuestUI Req")->GetComponent(ComponentType::GameUI))->SetText("REQUIREMENTS:");

		//QuestUI PierItems
		if(t_f == false)
		dynamic_cast<Renderer*>(Engine->GetGameObject("QuestUI PierItems")->GetComponent(ComponentType::Renderer))->SetVisibility(false);

		//QuestUI StartBtn
		dynamic_cast<Renderer*>(Engine->GetGameObject("QuestUI StartBtn")->GetComponent(ComponentType::Renderer))->SetVisibility(t_f);
		dynamic_cast<GameUI*>(Engine->GetGameObject("QuestUI StartBtn")->GetComponent(ComponentType::GameUI))->SetFontVisibility(t_f);

		//QuestUI 
		if (t_f)
		{
			//move the close btn to the top righ
			Vec2 pos = Engine->GetGameObject("QuestUI Main")->GetComponent<Transform>()->Position;
			Vec2 sca = Engine->GetGameObject("QuestUI Main")->GetComponent<Transform>()->Scale;
			Vec2 dim = Engine->GetGameObject("QuestUI Main")->GetComponent<Transform>()->Dimensions;
			pos = { 0.5f * dim.x * (sca.x / 100.f) + pos.x  , 0.5f * dim.y * (sca.y / 100.f) + pos.y };
			Engine->GetGameObject("CraftCookQuest CloseBtn")->GetComponent<Transform>()->SetPosition(pos);
		}

		Engine->GetGameObject("CraftCookQuest CloseBtn")->GetComponent<Renderer>()->SetVisibility(t_f);
		// make the bag and pause button visible
		Engine->GetGameObject("Bag Icon")->GetComponent<Renderer>()->SetVisibility(!t_f);
		Engine->GetGameObject("Play/Pause Button")->GetComponent<Renderer>()->SetVisibility(!t_f);

		Quests::current_quest = "";
		Quests::next_scn_filepath = "";
		can_move = !t_f;
	}
	/**************************************************************************/
	/*!
		\brief
			This function either displays or hides the hub's dialogue box
			and dialogue name.

		\param t_f
			True or False, true to show, false to hide
	*/
	/**************************************************************************/
	inline void ToggleDialogueBox(bool toggle) {
		dynamic_cast<Renderer*>(Engine->GetGameObject("DialogueBox")->GetComponent(ComponentType::Renderer))->SetVisibility(toggle);
		dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBox")->GetComponent(ComponentType::GameUI))->Alpha = (toggle ? 1.f : 0.f);
		dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBox")->GetComponent(ComponentType::GameUI))->SetFontVisibility(toggle);
		dynamic_cast<Renderer*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::Renderer))->SetVisibility(toggle);
		dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::GameUI))->Alpha = (toggle ? 1.f : 0.f);
		dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::GameUI))->SetFontVisibility(toggle);
	}

	/**************************************************************************/
	/*!
		\brief
			This function either displays or hides the hub's  CookingCraftingUI
			LEFT and RIGHT menus, and its corresponding buttons

		\param t_f
			True or False, true to show, false to hide
	*/
	/**************************************************************************/
	inline void MakeINVISIBLECookCraftUI()
	{
		dynamic_cast<Renderer*>(Engine->GetGameObject("CookingCraftingUILeft")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
		dynamic_cast<GameUI*>(Engine->GetGameObject("CookingCraftingUILeft")->GetComponent(ComponentType::GameUI))->SetFontVisibility(false);

		dynamic_cast<Renderer*>(Engine->GetGameObject("CookingCraftingUIRight")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
		dynamic_cast<GameUI*>(Engine->GetGameObject("CookingCraftingUIRight")->GetComponent(ComponentType::GameUI))->SetFontVisibility(false);


		std::string top_items = "CraftCook TopItem ";
		int top_max_items = 5;

		for (int i = 1; i <= top_max_items; i++)
		{
			std::string GO = top_items + std::to_string(i);
			dynamic_cast<Renderer*>(Engine->GetGameObject(GO)->GetComponent(ComponentType::Renderer))->SetVisibility(false);
			dynamic_cast<GameUI*>(Engine->GetGameObject(GO)->GetComponent(ComponentType::GameUI))->SetFontVisibility(false);
		}


		std::string bot_items = "CraftCook BtmItem ";
		int bot_max_items = 8;

		for (int i = 1; i <= bot_max_items; i++)
		{
			std::string GO = bot_items + std::to_string(i);
			dynamic_cast<Renderer*>(Engine->GetGameObject(GO)->GetComponent(ComponentType::Renderer))->SetVisibility(false);
			dynamic_cast<GameUI*>(Engine->GetGameObject(GO)->GetComponent(ComponentType::GameUI))->SetFontVisibility(false);
		}

		std::string food_items = "Cooked Food ";
		int food_max_items = 3;

		for (int i = 1; i <= food_max_items; i++)
		{
			std::string GO = food_items + std::to_string(i);
			dynamic_cast<Renderer*>(Engine->GetGameObject(GO)->GetComponent(ComponentType::Renderer))->SetVisibility(false);
			dynamic_cast<GameUI*>(Engine->GetGameObject(GO)->GetComponent(ComponentType::GameUI))->SetFontVisibility(false);
		}


		// right UI

		dynamic_cast<Renderer*>(Engine->GetGameObject("CraftCook_Selected Name")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
		dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook_Selected Name")->GetComponent(ComponentType::GameUI))->SetFontVisibility(false);

		dynamic_cast<Renderer*>(Engine->GetGameObject("CraftCook_Selected Icon")->GetComponent(ComponentType::Renderer))->SetVisibility(false);

		dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook_Selected Desc")->GetComponent(ComponentType::GameUI))->Text.clear();
		dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook_Selected Desc")->GetComponent(ComponentType::GameUI))->SetFontVisibility(false);
		dynamic_cast<Renderer*>(Engine->GetGameObject("CraftCook_Selected Desc")->GetComponent(ComponentType::Renderer))->SetVisibility(false);

		dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook_Selected Req")->GetComponent(ComponentType::GameUI))->Text.clear();
		dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook_Selected Req")->GetComponent(ComponentType::GameUI))->SetFontVisibility(false);
		dynamic_cast<Renderer*>(Engine->GetGameObject("CraftCook_Selected Req")->GetComponent(ComponentType::Renderer))->SetVisibility(false);

		dynamic_cast<Renderer*>(Engine->GetGameObject("CraftCook_Selected Status")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
		dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook_Selected Status")->GetComponent(ComponentType::GameUI))->SetFontVisibility(false);

		dynamic_cast<Renderer*>(Engine->GetGameObject("CraftCook_Selected CraftBtn")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
		dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook_Selected CraftBtn")->GetComponent(ComponentType::GameUI))->SetFontVisibility(false);

		dynamic_cast<Renderer*>(Engine->GetGameObject("CraftCook Confirmation")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
		dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook Confirmation")->GetComponent(ComponentType::GameUI))->SetFontVisibility(false);
		dynamic_cast<Renderer*>(Engine->GetGameObject("CraftCook Confirmation Yes")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
		dynamic_cast<Renderer*>(Engine->GetGameObject("CraftCook Confirmation No")->GetComponent(ComponentType::Renderer))->SetVisibility(false);

		Engine->GetGameObject("Cooking MinigameBGFade")->GetComponent<Renderer>()->SetVisibility(false);
		Engine->GetGameObject("CraftCookQuest CloseBtn")->GetComponent<Renderer>()->SetVisibility(false);

		// make the bag and pause button visible
		Engine->GetGameObject("Bag Icon")->GetComponent<Renderer>()->SetVisibility(true);
		Engine->GetGameObject("Play/Pause Button")->GetComponent<Renderer>()->SetVisibility(true);


		//null out crafting
		Crafting::current_selected_craftable = "";
		Crafting::material_qnt.clear();

		//null out the cooking
		Cooking::current_selected_cookable = "";
		Cooking::material_qnt.clear();
		can_move = true;
	}
	/**************************************************************************/
	/*!
		\brief
			This function shows the hub's CraftingUI
			LEFT and RIGHT menus, and its corresponding buttons

	*/
	/**************************************************************************/
	inline void MakeCraftingVisible()
	{
		dynamic_cast<Renderer*>(Engine->GetGameObject("CookingCraftingUILeft")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
		dynamic_cast<GameUI*>(Engine->GetGameObject("CookingCraftingUILeft")->GetComponent(ComponentType::GameUI))->SetFontVisibility(true);
		dynamic_cast<GameUI*>(Engine->GetGameObject("CookingCraftingUILeft")->GetComponent(ComponentType::GameUI))->SetText("Weapons\nArmour");

		dynamic_cast<Renderer*>(Engine->GetGameObject("CookingCraftingUIRight")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
		dynamic_cast<GameUI*>(Engine->GetGameObject("CookingCraftingUIRight")->GetComponent(ComponentType::GameUI))->SetFontVisibility(true);


		std::string top_items = "CraftCook TopItem ";
		int top_max_items = 5;

		for (int i = 1; i <= top_max_items; i++)
		{
			std::string GO = top_items + std::to_string(i);
			dynamic_cast<Renderer*>(Engine->GetGameObject(GO)->GetComponent(ComponentType::Renderer))->SetVisibility(true);
			dynamic_cast<GameUI*>(Engine->GetGameObject(GO)->GetComponent(ComponentType::GameUI))->SetFontVisibility(true);
		}


		std::string bot_items = "CraftCook BtmItem ";
		int bot_max_items = 8;

		for (int i = 1; i <= bot_max_items; i++)
		{
			std::string GO = bot_items + std::to_string(i);
			dynamic_cast<Renderer*>(Engine->GetGameObject(GO)->GetComponent(ComponentType::Renderer))->SetVisibility(true);
			dynamic_cast<GameUI*>(Engine->GetGameObject(GO)->GetComponent(ComponentType::GameUI))->SetFontVisibility(true);
		}
		can_move = false;

		// show the close btn
		Engine->GetGameObject("CraftCookQuest CloseBtn")->GetComponent<Renderer>()->SetVisibility(true);
		//move the close btn to the top righ
		Vec2 pos = Engine->GetGameObject("CookingCraftingUIRight")->GetComponent<Transform>()->Position;
		Vec2 sca = Engine->GetGameObject("CookingCraftingUIRight")->GetComponent<Transform>()->Scale;
		Vec2 dim = Engine->GetGameObject("CookingCraftingUIRight")->GetComponent<Transform>()->Dimensions;
		pos = { 0.5f * dim.x * (sca.x / 100.f) + pos.x  , 0.5f * dim.y * (sca.y / 100.f) + pos.y };
		Engine->GetGameObject("CraftCookQuest CloseBtn")->GetComponent<Renderer>()->SetVisibility(true);
		Engine->GetGameObject("CraftCookQuest CloseBtn")->GetComponent<Transform>()->SetPosition(pos);

		// make the bag and pause button invisible
		Engine->GetGameObject("Bag Icon")->GetComponent<Renderer>()->SetVisibility(false);
		Engine->GetGameObject("Play/Pause Button")->GetComponent<Renderer>()->SetVisibility(false);
	}
	/**************************************************************************/
	/*!
		\brief
			This function shows the hub's Cooking UI
			LEFT and RIGHT menus, and its corresponding buttons

	*/
	/**************************************************************************/
	inline void MakeCookingVisible()
	{
		dynamic_cast<Renderer*>(Engine->GetGameObject("CookingCraftingUILeft")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
		dynamic_cast<GameUI*>(Engine->GetGameObject("CookingCraftingUILeft")->GetComponent(ComponentType::GameUI))->SetFontVisibility(true);
		dynamic_cast<GameUI*>(Engine->GetGameObject("CookingCraftingUILeft")->GetComponent(ComponentType::GameUI))->SetText("Food");

		dynamic_cast<Renderer*>(Engine->GetGameObject("CookingCraftingUIRight")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
		dynamic_cast<GameUI*>(Engine->GetGameObject("CookingCraftingUIRight")->GetComponent(ComponentType::GameUI))->SetFontVisibility(true);


		Engine->GetGameObject("CraftCookQuest CloseBtn")->GetComponent<Renderer>()->SetVisibility(true);

		std::string top_items = "Cooked Food ";
		int top_max_items = 3;

		for (int i = 1; i <= top_max_items; i++)
		{
			std::string GO = top_items + std::to_string(i);
			dynamic_cast<Renderer*>(Engine->GetGameObject(GO)->GetComponent(ComponentType::Renderer))->SetVisibility(true);
			dynamic_cast<GameUI*>(Engine->GetGameObject(GO)->GetComponent(ComponentType::GameUI))->SetFontVisibility(true);
		}

		can_move = false;

		// show the close btn
		Engine->GetGameObject("CraftCookQuest CloseBtn")->GetComponent<Renderer>()->SetVisibility(true);
		//move the close btn to the top righ
		Vec2 pos = Engine->GetGameObject("CookingCraftingUIRight")->GetComponent<Transform>()->Position;
		Vec2 sca = Engine->GetGameObject("CookingCraftingUIRight")->GetComponent<Transform>()->Scale;
		Vec2 dim = Engine->GetGameObject("CookingCraftingUIRight")->GetComponent<Transform>()->Dimensions;
		pos = { 0.5f * dim.x * (sca.x / 100.f) + pos.x  , 0.5f * dim.y * (sca.y / 100.f) + pos.y };
		Engine->GetGameObject("CraftCookQuest CloseBtn")->GetComponent<Renderer>()->SetVisibility(true);
		Engine->GetGameObject("CraftCookQuest CloseBtn")->GetComponent<Transform>()->SetPosition(pos);

		// make the bag and pause button invisible
		Engine->GetGameObject("Bag Icon")->GetComponent<Renderer>()->SetVisibility(false);
		Engine->GetGameObject("Play/Pause Button")->GetComponent<Renderer>()->SetVisibility(false);
	}
	/**************************************************************************/
	/*!
		\brief
			This is the init function of the HubController Script.
		\param GO
			The Game Object to be parsed into the init function
		\param dt
			delta_time
	*/
	/**************************************************************************/
	void Init(GameObject* GO, float dt) {
		(void)dt;
		Controller* CT = dynamic_cast<Controller*>(GO->GetComponent(ComponentType::Controller));
		GameObject* Player = Engine->GetObjectByPrefix("Player");

		audio->StopMusic();
		audio->StopSfx();
		audio->PlayMusic(3);
		dialogue_counter = 0;
		event_counter = 0;
		levelpath = "";
		changed_level = false;
		MasterObjectList->ClearController();
		if (CT != nullptr) {
			if (MasterObjectList->StoredHubController != nullptr) {
				MasterObjectList->GetHubController(CT);
				Inventory* IC = dynamic_cast<Inventory*>(Player->GetComponent(ComponentType::Inventory));
				MasterObjectList->ImportInventory(IC);
			}

			if (MasterObjectList->StoredPlayerCombat) { MasterObjectList->StoredPlayerCombat->currentHP = MasterObjectList->StoredPlayerCombat->maxHP; }

			Animate* FadeIn = dynamic_cast<Animate*>(Engine->GetGameObject("FadeIn")->GetComponent(ComponentType::Animate));
			FadeIn->SetVisibility(false);
			FadeIn->SetIsPlaying(false);
			MasterObjectList->MasterAssets.GetSprite(FadeIn->GetTexHandle()).idx = 0;
			Animate* FadeOut = dynamic_cast<Animate*>(Engine->GetGameObject("FadeOut")->GetComponent(ComponentType::Animate));
			FadeOut->SetVisibility(true);
			FadeOut->SetIsPlaying(true);
			MasterObjectList->MasterAssets.GetSprite(FadeOut->GetTexHandle()).idx = 0;
			//Disable player movement until fade animation plays
			Player->GetComponent<LogicComponent>()->SetEnabled("MMovement", false);
			if (CT->LevelFlags[0]) {
				Player->GetComponent<Transform>()->Position.x = Engine->GetObjectByPrefix("QuestBoard")->GetComponent<Transform>()->Position.x;
				Engine->GetObjectByPrefix("Camera")->GetComponent<Transform>()->Position.x = Player->GetComponent<Transform>()->Position.x;
			}
			dynamic_cast<Renderer*>(Engine->GetGameObject("Sir")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
		}

		craftingcooking_open = false; questui_open = false;
		//disable crafting and cooking scripts
		//Engine->GetGameObject("CookingCraftingUILeft")->GetComponent<LogicComponent>()->SetEnabled("Cooking", false);
		//Engine->GetGameObject("CookingCraftingUILeft")->GetComponent<LogicComponent>()->SetEnabled("Crafting", false);
		Engine->GetGameObject("CookingCraftingUIRight")->GetComponent<LogicComponent>()->SetEnabled("Cooking", false);
		Engine->GetGameObject("CookingCraftingUIRight")->GetComponent<LogicComponent>()->SetEnabled("Crafting", false);
		Engine->GetGameObject("CraftCookQuest CloseBtn")->GetComponent<LogicComponent>()->SetEnabled("Cooking", false);
		Engine->GetGameObject("CraftCookQuest CloseBtn")->GetComponent<LogicComponent>()->SetEnabled("Crafting", false);
		Engine->GetGameObject("CraftCook Confirmation")->GetComponent<LogicComponent>()->SetEnabled("Cooking", false);
		Engine->GetGameObject("CraftCook Confirmation")->GetComponent<LogicComponent>()->SetEnabled("Crafting", false);
		Engine->GetGameObject("Cooking MinigameBGFade")->GetComponent<LogicComponent>()->SetEnabled("Cooking", false);



	}
	/**************************************************************************/
	/*!
		\brief
			This is the update function of the HubController Script.
		\param GO
			The Game Object to be parsed into the init function
		\param dt
			delta_time
	*/
	/**************************************************************************/
	void Update(GameObject* GO, float dt) {
		PlayerAnimation::SmokeAnimation();
		Controller* CT = dynamic_cast<Controller*>(GO->GetComponent(ComponentType::Controller));
		Animate* FadeIn = dynamic_cast<Animate*>(Engine->GetGameObject("FadeIn")->GetComponent(ComponentType::Animate));
		Animate* FadeOut = dynamic_cast<Animate*>(Engine->GetGameObject("FadeOut")->GetComponent(ComponentType::Animate));
		GameObject* P = Engine->GetObjectByPrefix("Player");
		//Collision* PCol = P->GetComponent<Collision>();
		Transform* PT = P->GetComponent<Transform>();
		Animate* PAnim = P->GetComponent<Animate>();
		Transform* CamT = Engine->GetGameObject("Camera")->GetComponent<Transform>();

		if (FadeOut->GetVisibility() && MasterObjectList->MasterAssets.GetSprite(FadeOut->GetTexHandle()).idx == MasterObjectList->MasterAssets.GetSprite(FadeOut->GetTexHandle()).frames_per_row - 1) {
			FadeOut->SetVisibility(false);
			if (CT->LevelFlags[0]) {
				can_move = true;
			}
		}

		if (!FadeOut->GetVisibility()) {
			if (CT->LevelFlags[0] == false) {
				switch (event_counter) {
				case 0:
				{
					//Yu walks into scene
					//Stops at -2000
					PAnim->SetTexHandle("Yu_Run");
					P->GetComponent<RigidBody>()->MoveTo(PT->Position, { -2000.f, PT->Position.y }, dt);
					P->GetComponent<LogicComponent>()->SetEnabled("Camfollow", false);
					P->GetComponent<LogicComponent>()->SetEnabled("Inventory", false);
					if (Engine->GetObjectInProximityByPrefix({ -2000.f, PT->Position.y }, "Player", 10.f)) {
						PAnim->SetTexHandle("Yu_Idle");
						P->GetComponent<RigidBody>()->SetVelocity({ 0.f, 0.f });
						event_counter++;
					}
					break;
				}
				case 1:
				{
					ToggleDialogueBox(true);
					dynamic_cast<Renderer*>(Engine->GetGameObject("Sir")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
					dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::GameUI))->SetText("Sir");
					if (!fired_message) {
						TypewriterEffect::SetNewText("Ah, you reached. Sorry for the unexpected detour.");
						fired_message = true;
					}
					if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
						event_counter++;
						fired_message = false;
					}
					break;
				}
				case 2:
				{
					dynamic_cast<Renderer*>(Engine->GetGameObject("Sir")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
					dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::GameUI))->SetText("Sir");
					if (!fired_message) {
						TypewriterEffect::SetNewText("Our base got attacked while you were gone and we had to relocate.");
						fired_message = true;
					}
					if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
						event_counter++;
						fired_message = false;
					}
					break;
				}
				case 3:
				{
					dynamic_cast<Renderer*>(Engine->GetGameObject("Sir")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
					dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::GameUI))->SetText("Sir");
					if (!fired_message) {
						TypewriterEffect::SetNewText("So we had to move here and setup a new base.");
						fired_message = true;
					}
					if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
						event_counter++;
						fired_message = false;
					}
					break;
				}
				case 4:
				{
					dynamic_cast<Renderer*>(Engine->GetGameObject("Sir")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
					dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxName")->GetComponent(ComponentType::GameUI))->SetText("Sir");
					if (!fired_message) {
						TypewriterEffect::SetNewText("Come, I show you around.");
						fired_message = true;
					}
					if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
						event_counter++;
						fired_message = false;
						ToggleDialogueBox(false);
						stored_cam = CamT->Position;
						dynamic_cast<Renderer*>(Engine->GetGameObject("Sir")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
					}
					break;
				}
				case 5:
				{
					//Pan to cooking
					Vec2 camtarget = { Engine->GetGameObject("Cooking")->GetComponent<Transform>()->Position.x, 0.f };
					Vec2 distance = camtarget - stored_cam;
					distance = distance * (dt / 2.f);
					CamT->Position += distance;
					if (Vec2Length(camtarget - CamT->Position) < 10.f) {
						event_counter++;
					}
					break;
				}
				case 6:
				{
					Engine->GetGameObject("Cooking")->GetComponent<LogicComponent>()->SetEnabled("BreatheEffect", true);
					dynamic_cast<Renderer*>(Engine->GetGameObject("Sir")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
					ToggleDialogueBox(true);
					if (!fired_message) {
						TypewriterEffect::SetNewText("Over here is the cooking station. You can cook food here.");
						fired_message = true;
					}
					if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
						event_counter++;
						fired_message = false;
						Engine->GetGameObject("Cooking")->GetComponent<LogicComponent>()->SetEnabled("BreatheEffect", false);
						Engine->GetGameObject("Cooking")->GetComponent<Renderer>()->Alpha_0to1 = 1.f;
						dynamic_cast<Renderer*>(Engine->GetGameObject("Sir")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
						ToggleDialogueBox(false);
						stored_cam = CamT->Position;
					}
					break;
				}
				case 7:
				{
					//Pan to quest board
					Vec2 camtarget = { Engine->GetGameObject("QuestBoard")->GetComponent<Transform>()->Position.x, 0.f };
					dynamic_cast<Renderer*>(Engine->GetGameObject("Sir")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
					Vec2 distance = camtarget - stored_cam;
					distance = distance * (dt);
					CamT->Position += distance;
					if (Vec2Length(camtarget - CamT->Position) < 10.f) {
						event_counter++;
					}
					break;
				}
				case 8:
				{
					Engine->GetGameObject("QuestBoard")->GetComponent<LogicComponent>()->SetEnabled("BreatheEffect", true);
					dynamic_cast<Renderer*>(Engine->GetGameObject("Sir")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
					ToggleDialogueBox(true);
					if (!fired_message) {
						TypewriterEffect::SetNewText("This is the Quest Board. You can go on missions from here.");
						fired_message = true;
					}
					if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
						event_counter++;
						fired_message = false;
						Engine->GetGameObject("QuestBoard")->GetComponent<LogicComponent>()->SetEnabled("BreatheEffect", false);
						Engine->GetGameObject("QuestBoard")->GetComponent<Renderer>()->Alpha_0to1 = 1.f;
						dynamic_cast<Renderer*>(Engine->GetGameObject("Sir")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
						ToggleDialogueBox(false);
						stored_cam = CamT->Position;
					}
					break;
				}
				case 9:
				{
					//Pan to crafting station
					Vec2 camtarget = { Engine->GetGameObject("CraftingStation")->GetComponent<Transform>()->Position.x, 0.f };
					Vec2 distance = camtarget - stored_cam;
					distance = distance * (dt / 2.f);
					CamT->Position += distance;
					if (Vec2Length(camtarget - CamT->Position) < 10.f) {
						event_counter++;
					}
					break;
				}
				case 10:
				{
					Engine->GetGameObject("CraftingStation")->GetComponent<LogicComponent>()->SetEnabled("BreatheEffect", true);
					dynamic_cast<Renderer*>(Engine->GetGameObject("Sir")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
					ToggleDialogueBox(true);
					if (!fired_message) {
						TypewriterEffect::SetNewText("This is the Crafting Station. Craft new equipment here.");
						fired_message = true;
					}
					if (TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
						event_counter++;
						fired_message = false;
						Engine->GetGameObject("CraftingStation")->GetComponent<LogicComponent>()->SetEnabled("BreatheEffect", false);
						Engine->GetGameObject("CraftingStation")->GetComponent<Renderer>()->Alpha_0to1 = 1.f;
						stored_cam = CamT->Position;
						dynamic_cast<Renderer*>(Engine->GetGameObject("Sir")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
					}
					break;
				}
				case 11:
				{
					dynamic_cast<Renderer*>(Engine->GetGameObject("Sir")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
					//Pan to back to Player
					Vec2 camtarget = { -1800.f, 0.f };
					Vec2 distance = camtarget - stored_cam;
					distance = distance * (dt / 3.f);
					if (Vec2Length(camtarget - CamT->Position) >= 30.f) {
						CamT->Position += distance;
					}
					if (!fired_message) {
						TypewriterEffect::SetNewText("That's all for now. Feel free to own time own target explore.");
						fired_message = true;
					}
					if (Vec2Length(camtarget - CamT->Position) < 30.f && TypewriterEffect::done && Input::GetKeyTriggered(Key::E)) {
						event_counter++;
						fired_message = false;
						ToggleDialogueBox(false);
						dynamic_cast<Renderer*>(Engine->GetGameObject("Sir")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
						P->GetComponent<LogicComponent>()->SetEnabled("MMovement", true);
						P->GetComponent<LogicComponent>()->SetEnabled("Camfollow", true);
						P->GetComponent<LogicComponent>()->SetEnabled("Inventory", true);
						CT->LevelFlags[0] = true;
						can_move = true;
						stored_cam = { 0.f, 0.f };
					}
					break;
				}
				}
			}
			else {
				//let the crafting station breathe if within 250 units proximity
				if (Engine->GetObjectInProximityByPrefix(Engine->GetGameObject("CraftingStation")->GetComponent<Transform>()->Position, "Player", 250.f))
				{
					Engine->GetGameObject("CraftingStation")->GetComponent<LogicComponent>()->SetEnabled("BreatheEffect", true);

					if (Input::GetKeyTriggered(Key::E))
					{
						craftingcooking_open = !craftingcooking_open;
						//std::cout << "Spawn the Crafting and Cooking UI" << std::endl;
						if (craftingcooking_open)
						{
							MakeCraftingVisible();
							Engine->GetGameObject("CookingCraftingUILeft")->GetComponent<LogicComponent>()->SetEnabled("Crafting", true);
							Engine->GetGameObject("CookingCraftingUIRight")->GetComponent<LogicComponent>()->SetEnabled("Crafting", true);
							Engine->GetGameObject("CraftCookQuest CloseBtn")->GetComponent<LogicComponent>()->SetEnabled("Crafting", true);
							Engine->GetGameObject("CraftCook Confirmation")->GetComponent<LogicComponent>()->SetEnabled("Crafting", true);
						}
						else
						{
							MakeINVISIBLECookCraftUI();
							//Engine->GetGameObject("CookingCraftingUILeft")->GetComponent<LogicComponent>()->SetEnabled("Crafting", false);
							Engine->GetGameObject("CookingCraftingUIRight")->GetComponent<LogicComponent>()->SetEnabled("Crafting", false);
							Engine->GetGameObject("CraftCookQuest CloseBtn")->GetComponent<LogicComponent>()->SetEnabled("Crafting", false);
							Engine->GetGameObject("CraftCook Confirmation")->GetComponent<LogicComponent>()->SetEnabled("Crafting", false);
							//P->GetComponent<LogicComponent>()->SetEnabled("MMovement", true);
						}
					}
					if (!dynamic_cast<Renderer*>(Engine->GetGameObject("CookingCraftingUILeft")->GetComponent(ComponentType::Renderer))->GetVisibility()) {
						can_move = true; //Additional check to enable movement
					}
				}
				else
				{
					//MakeINVISIBLECookCraftUI();
					Engine->GetGameObject("CraftingStation")->GetComponent<LogicComponent>()->SetEnabled("BreatheEffect", false);
					Engine->GetGameObject("CraftingStation")->GetComponent<Renderer>()->Alpha_0to1 = 1.f;
				}

				//let the questboard breathe if within 300 units proximity
				if (Engine->GetObjectInProximityByPrefix(Engine->GetGameObject("QuestBoard")->GetComponent<Transform>()->Position, "Player", 300.f))
				{
					Engine->GetGameObject("QuestBoard")->GetComponent<LogicComponent>()->SetEnabled("BreatheEffect", true);

					if (Input::GetKeyTriggered(Key::E))
					{
						questui_open = !questui_open;
						//std::cout << "Spawn the Crafting and Cooking UI" << std::endl;
						if (questui_open)
						{
							SetQuestsGOs(true);
						}
						else
						{
							SetQuestsGOs(false);
							//P->GetComponent<LogicComponent>()->SetEnabled("MMovement", true);
						}
					}
					if (!dynamic_cast<Renderer*>(Engine->GetGameObject("QuestUI Main")->GetComponent(ComponentType::Renderer))->GetVisibility()) {
						can_move = true; //Additional check to enable movement
					}
				}
				else
				{
					//MakeINVISIBLECookCraftUI();
					Engine->GetGameObject("QuestBoard")->GetComponent<LogicComponent>()->SetEnabled("BreatheEffect", false);
					Engine->GetGameObject("QuestBoard")->GetComponent<Renderer>()->Alpha_0to1 = 1.f;
				}

				//let the cooking stove breathe if within 150 units proximity
				if (Engine->GetObjectInProximityByPrefix(Engine->GetGameObject("Cooking")->GetComponent<Transform>()->Position, "Player", 150.f))
				{
					Engine->GetGameObject("Cooking")->GetComponent<LogicComponent>()->SetEnabled("BreatheEffect", true);

					if (Input::GetKeyTriggered(Key::E))
					{
						craftingcooking_open = !craftingcooking_open;
						//std::cout << "Spawn the Crafting and Cooking UI" << std::endl;
						if (craftingcooking_open)
						{

							Engine->GetGameObject("CookingCraftingUILeft")->GetComponent<LogicComponent>()->SetEnabled("Cooking", true);
							Engine->GetGameObject("CookingCraftingUIRight")->GetComponent<LogicComponent>()->SetEnabled("Cooking", true);
							Engine->GetGameObject("CraftCookQuest CloseBtn")->GetComponent<LogicComponent>()->SetEnabled("Cooking", true);
							Engine->GetGameObject("CraftCook Confirmation")->GetComponent<LogicComponent>()->SetEnabled("Cooking", true);
							MakeCookingVisible();
						}
						else
						{
							MakeINVISIBLECookCraftUI();
							//Engine->GetGameObject("CookingCraftingUILeft")->GetComponent<LogicComponent>()->SetEnabled("Cooking", false);
							Engine->GetGameObject("CookingCraftingUIRight")->GetComponent<LogicComponent>()->SetEnabled("Cooking", false);
							Engine->GetGameObject("CraftCookQuest CloseBtn")->GetComponent<LogicComponent>()->SetEnabled("Cooking", false);
							Engine->GetGameObject("CraftCook Confirmation")->GetComponent<LogicComponent>()->SetEnabled("Cooking", false);

							//P->GetComponent<LogicComponent>()->SetEnabled("MMovement", true);
						}
					}
					if (!dynamic_cast<Renderer*>(Engine->GetGameObject("CookingCraftingUILeft")->GetComponent(ComponentType::Renderer))->GetVisibility()) {
						can_move = true; //Additional check to enable movement
					}
				}
				else
				{
					//MakeINVISIBLECookCraftUI();
					Engine->GetGameObject("Cooking")->GetComponent<LogicComponent>()->SetEnabled("BreatheEffect", false);
					Engine->GetGameObject("Cooking")->GetComponent<Renderer>()->Alpha_0to1 = 1.f;
				}

				if (changed_level) {
					FadeIn->SetVisibility(true);
					FadeIn->SetIsPlaying(true);
					can_move = false;

					if (MasterObjectList->MasterAssets.GetSprite(FadeIn->GetTexHandle()).idx == MasterObjectList->MasterAssets.GetSprite(FadeIn->GetTexHandle()).frames_per_row - 1) {
						MasterObjectList->SetHubController(CT);
						MasterObjectList->ExportInventory(P->GetComponent<Inventory>());
						GO->GetComponent<LogicComponent>()->RemoveScriptKey(SKey);
					}
				}

				P->GetComponent<LogicComponent>()->SetEnabled("MMovement", can_move);
				P->GetComponent<LogicComponent>()->SetEnabled("Inventory", can_move);
			}
		}
	}
	/**************************************************************************/
	/*!
		\brief
			This is the end function of the HubController Script.
		\param GO
			The Game Object to be parsed into the init function
		\param dt
			delta_time
	*/
	/**************************************************************************/
	void End(GameObject* GO, float dt) {
		(void)dt;
		(void)GO;
		if (changed_level) {
			SceneManager::StoreLevel(levelpath.c_str());
			levelpath = "";
			changed_level = false;
		}
	}
}

namespace Quests
{
	std::string SKey{ "Quests" };
	bool can_go = false;


	/**************************************************************************/
	/*!
		\brief
			This function inserts the string pairs into the questname and
			quest description map
	*/
	/**************************************************************************/
	inline void InitQuestNameTODescMap()
	{
		QuestName_TO_QuestDesc.insert(std::pair<std::string, std::string >("Area 1", "A desolated street with old shop houses.\nBe careful of any monsters lurking\naround the area."));
		QuestName_TO_QuestDesc.insert(std::pair<std::string, std::string >("Pier", "A pier full of broken down ships. Perhaps with\n enough materials one of them can be\nrepaired..."));
	}

	/**************************************************************************/
	/*!
		\brief
			This function inserts the string pairs into the questname and
			scenefilepath map
	*/
	/**************************************************************************/
	inline void InitQuestNameTOSCNFilepath()
	{
		QuestName_TO_ScnFilePath.insert(std::pair<std::string, std::string >("Area 1", "./Scene/FirstLevel_Map1.scn"));
		QuestName_TO_ScnFilePath.insert(std::pair<std::string, std::string >("Pier", "./Scene/Ending.scn"));
	}

	/**************************************************************************/
	/*!
		\brief
			This function updates the QuestUI_Main by showing its changed
			description and filepath for the next
		\param GO
			The GO to check if it has been pressed on
		\param dt
			For dt calculations if needed
	*/
	/**************************************************************************/
	inline void Update_QuestUI_Main(GameObject* GO, float dt)
	{
		UNREFERENCED_PARAMETER(GO);
		UNREFERENCED_PARAMETER(dt);

		Renderer* self_rdr = dynamic_cast<Renderer*>(GO->GetComponent(ComponentType::Renderer));
		// if the frame is not active or visible , skip
		if (self_rdr->GetVisibility() == false) return;

		GameUI* self_GUI = dynamic_cast<GameUI*>(GO->GetComponent(ComponentType::GameUI));

		//only if the mouse is within the ui, then we go into the update loop
		if ((mouse_x > self_GUI->AA.x && mouse_x < self_GUI->BB.x)
			&& (mouse_y_aft_TRS > self_GUI->AA.y && mouse_y_aft_TRS < self_GUI->BB.y))
		{
			for (int i = 1; i <= 2; i++)
			{
				std::string string = "QuestUI Item";
				std::string goname = string + std::to_string(i);

				if (Input::GetMouseTriggeredDepth(MouseButton::Button0, Engine->GetGameObject(goname)))
				{

					selected_changed = true;

					//reset the current quest
					if (current_quest != (dynamic_cast<GameUI*>(Engine->GetGameObject(goname)->GetComponent(ComponentType::GameUI))->GetText()))
					{
						current_quest = dynamic_cast<GameUI*>(Engine->GetGameObject(goname)->GetComponent(ComponentType::GameUI))->GetText();
					}

					//change the desc
					dynamic_cast<GameUI*>(Engine->GetGameObject("QuestUI Desc")->GetComponent(ComponentType::GameUI))->SetText("DESCRIPTION:\n" + QuestName_TO_QuestDesc[current_quest]);

					//change the filepath of the button
					if (next_scn_filepath != QuestName_TO_ScnFilePath[current_quest])
					{
						next_scn_filepath = QuestName_TO_ScnFilePath[current_quest];
					}

					//change the req
					can_go = false;
					if (current_quest == "Area 1")
					{
						dynamic_cast<GameUI*>(Engine->GetGameObject("QuestUI Req")->GetComponent(ComponentType::GameUI))->SetText("REQUIREMENTS:\n None");
						Engine->GetGameObject("QuestUI Req")->GetComponent<Renderer>()->SetVisibility(false);
						Engine->GetGameObject("QuestUI PierItems")->GetComponent<Renderer>()->SetVisibility(false);
						can_go = true;
					}
					else
					{
						//1 camp pass 4 gasoline 30 metal pieces 10 used plastic

						int itemsID[4] = {27,28,33,29};
						int itemsREQqty[4] = {30,10,1,4};
						int itemsINVqty[4] = {0};

						
						std::string final_line;

						for (int j = 0; j < 4; ++j)
						{
							std::string line = MasterObjectList->GetInventoryItem(itemsID[j]).Name + ": ";
							for (auto x : Engine->GetObjectByPrefix("Player")->GetComponent<Inventory>()->Invent)
							{
								if (x.ID == itemsID[j])
								{
									//line += (std::to_string(x.Quantity) + " out of " + std::to_string(itemsREQqty[i]));
									itemsINVqty[j] = x.Quantity;
									can_go = (x.Quantity >= itemsREQqty[j]) ? true : false;
									break;
								}
							}
							final_line += line + std::to_string(itemsINVqty[j]) + " out of " + std::to_string(itemsREQqty[j]) + "\n";
						}
						dynamic_cast<GameUI*>(Engine->GetGameObject("QuestUI Req")->GetComponent(ComponentType::GameUI))->SetText("REQUIREMENTS:\n"+ final_line);
						Engine->GetGameObject("QuestUI PierItems")->GetComponent<Renderer>()->SetVisibility(true);


					}

					if (can_go)
					{
						Engine->GetGameObject("QuestUI StartBtn")->GetComponent<Renderer>()->Alpha_0to1 = 1.f;
						Engine->GetGameObject("QuestUI StartBtn")->GetComponent<GameUI>()->Alpha = 1.f;
					}
					else
					{
						Engine->GetGameObject("QuestUI StartBtn")->GetComponent<Renderer>()->Alpha_0to1 = 0.5f;
						Engine->GetGameObject("QuestUI StartBtn")->GetComponent<GameUI>()->Alpha = 0.5f;
					}
					
					audio->PlaySfx(30);
				}
			}
		}

	}


	/**************************************************************************/
	/*!
		\brief
			This function changes the scene to the next area.
		\param GO
			The GO to check if it has been pressed on
		\param dt
			For dt calculations if needed
	*/
	/**************************************************************************/
	inline void Update_QuestUI_StartBtn(GameObject* GO, float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		if (next_scn_filepath == "") return;
		GameUI* start_btn_GUI = GO->GetComponent<GameUI>();

		if (mouse_x > start_btn_GUI->AA.x &&
			mouse_x < start_btn_GUI->BB.x &&
			mouse_y_aft_TRS > start_btn_GUI->AA.y&&
			mouse_y_aft_TRS < start_btn_GUI->BB.y
			&&
			Input::GetMouseTriggeredDepth(MouseButton::Button0, GO))
		{
			if (can_go)
			{
				std::cout << "Switching Scenes!" << std::endl;
				audio->PlaySfx(30);
				HubController::changed_level = true;
				HubController::levelpath = next_scn_filepath;
			}
			
		}
		/*
		if (Input::GetMouseTriggeredDepth(MouseButton::Button0, GO))
		{
			std::cout << next_scn_filepath << std::endl;
			std::cout << "Switching Scenes!" << std::endl;
			audio->PlaySfx(30);
			HubController::changed_level = true;
			HubController::levelpath = next_scn_filepath;
		}
		*/
	}
	/**************************************************************************/
	/*!
		\brief
			This is the update function of the quest's close btn
		\param GO
			The GO to check if it has been pressed on
		\param dt
			For dt calculations if needed
	*/
	/**************************************************************************/
	inline void Update_QuestUI_CloseBtn(GameObject* GO, float dt)
	{
		(void)dt;
		(void)GO;
		if (Input::GetMouseTriggeredDepth(MouseButton::Button0, Engine->GetGameObject("CraftCookQuest CloseBtn")))
		{
			//hide all UIS
			HubController::SetQuestsGOs(false);
			questui_open = false;
		}
	}

	/**************************************************************************/
	/*!
		\brief
			Initializer for the TypewriterEffect script. It is called for every
			GO that has this script attached to it.
		\param GO
			The specific Gameobject who is the calling and is affected by
			this script function
	*/
	/**************************************************************************/
	void Init(GameObject* GO, float dt)
	{
		UNREFERENCED_PARAMETER(GO);
		UNREFERENCED_PARAMETER(dt);

		selected_changed = false;
		can_switch_scene = false;
		current_quest = "";
		next_scn_filepath = "";

		InitQuestNameTODescMap();
		InitQuestNameTOSCNFilepath();
	}
	/**************************************************************************/
	/*!
		\brief
			Update for the TypewriterEffect script. It is called for every
			GO that has this script attached to it.
		\param GO
			The specific Gameobject who is the calling and is affected by
			this script function
	*/
	/**************************************************************************/
	void Update(GameObject* GO, float dt)
	{
		UNREFERENCED_PARAMETER(GO);
		UNREFERENCED_PARAMETER(dt);

		mouse_x = Input::mouseObj.x;
		mouse_y_aft_TRS = GLSetup::current_height - Input::mouseObj.y;

		if (GO->GetName() == "QuestUI Main") Update_QuestUI_Main(GO, dt);
		if (GO->GetName() == "QuestUI StartBtn") Update_QuestUI_StartBtn(GO, dt);
		if (GO->GetName() == "CraftCookQuest CloseBtn") Update_QuestUI_CloseBtn(GO, dt);

	}
	/**************************************************************************/
	/*!
		\brief
			End for the TypewriterEffect script. It is called for every
			GO that has this script attached to it.
		\param GO
			The specific Gameobject who is the calling and is affected by
			this script function
	*/
	/**************************************************************************/
	void End(GameObject* GO, float dt)
	{
		UNREFERENCED_PARAMETER(GO);
		UNREFERENCED_PARAMETER(dt);
	}
}