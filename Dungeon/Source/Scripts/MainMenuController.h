/******************************************************************************/
/*!
\file		MainMenuController.h
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	Feb 1, 2023
\brief		Controller script for main menu level

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
#include "SceneManager/SceneManager.h"
#include "Audio/Audio.h"


namespace MainMenuController {
	
	std::string SKey{ "MainMenuController" };
	int playonce{};
	float light1_itensity_save, light2_itensity_save;
	float light1_delta, light2_delta;


	void Init(GameObject* GO, float dt)
	{
		UNREFERENCED_PARAMETER(GO);
		UNREFERENCED_PARAMETER(dt);
		audio->StopMusic();
		audio->StopSfx();
		audio->PlayMusic(2);
		if (MasterObjectList->StoredController != nullptr) {
			delete MasterObjectList->StoredController;
			MasterObjectList->StoredController = nullptr;
		}
		if (MasterObjectList->TempInventory != nullptr) {
			delete MasterObjectList->TempInventory;
			MasterObjectList->TempInventory = nullptr;
		}
		if (MasterObjectList->TempPlayerPosition != nullptr) {
			delete MasterObjectList->TempPlayerPosition;
			MasterObjectList->TempPlayerPosition = nullptr;
		}
		if (MasterObjectList->StoredHubController != nullptr) {
			delete MasterObjectList->StoredHubController;
			MasterObjectList->StoredHubController = new Controller("Hub", 10);
			for (int i = 0; i < MasterObjectList->StoredHubController->LevelFlagsSize; ++i) {
				MasterObjectList->StoredHubController->LevelFlags.emplace_back(false);
			}
		}

		Animate* TeamLogoa = dynamic_cast<Animate*>(Engine->GetGameObject("TeamLogo")->GetComponent(ComponentType::Animate));
		TeamLogoa->SetVisibility(false);
		TeamLogoa->SetIsPlaying(false);

		Animate* GameLogoa = dynamic_cast<Animate*>(Engine->GetGameObject("GameLogo")->GetComponent(ComponentType::Animate));
		GameLogoa->SetVisibility(false);
		GameLogoa->SetIsPlaying(false);

		Animate* MenuFadea = dynamic_cast<Animate*>(Engine->GetGameObject("MenuFade")->GetComponent(ComponentType::Animate));
		MenuFadea->SetVisibility(false);
		MenuFadea->SetIsPlaying(false);

		GameObject* text = Engine->GetGameObject("Title");
		GameUI* ui_text = dynamic_cast<GameUI*>(text->GetComponent(ComponentType::GameUI));
		Renderer* btn = dynamic_cast<Renderer*>(text->GetComponent(ComponentType::Renderer));

		Renderer* BGr = dynamic_cast<Renderer*>(Engine->GetGameObject("Background")->GetComponent(ComponentType::Renderer));
		Renderer* Titler = dynamic_cast<Renderer*>(Engine->GetGameObject("GameTitle")->GetComponent(ComponentType::Renderer));
		
		BGr->SetVisibility(false);
			
		Titler->SetVisibility(false);
			
		ui_text->SetFontVisibility(false);
		btn->SetVisibility(false);

		text = Engine->GetGameObject("Start Game Btn");
		ui_text = dynamic_cast<GameUI*>(text->GetComponent(ComponentType::GameUI));
		btn = dynamic_cast<Renderer*>(text->GetComponent(ComponentType::Renderer));
		ui_text->SetFontVisibility(false);
		btn->SetVisibility(false);

		text = Engine->GetGameObject("Quit Game Btn");
		ui_text = dynamic_cast<GameUI*>(text->GetComponent(ComponentType::GameUI));
		btn = dynamic_cast<Renderer*>(text->GetComponent(ComponentType::Renderer));
		ui_text->SetFontVisibility(false);
		btn->SetVisibility(false);

		text = Engine->GetGameObject("Options Btn");
		ui_text = dynamic_cast<GameUI*>(text->GetComponent(ComponentType::GameUI));
		btn = dynamic_cast<Renderer*>(text->GetComponent(ComponentType::Renderer));
		ui_text->SetFontVisibility(false);
		btn->SetVisibility(false);

		++playonce;

		light1_itensity_save = Engine->GetGameObject("Light1")->GetComponent<Lighting>()->Light.LightIntensity;
		light2_itensity_save = Engine->GetGameObject("Light2")->GetComponent<Lighting>()->Light.LightIntensity;

		light1_delta = light1_itensity_save / 60.f;
		light2_delta = light2_itensity_save / 60.f;

		Engine->GetGameObject("Light1")->GetComponent<Lighting>()->Light.LightIntensity = 0.f;;
		Engine->GetGameObject("Light2")->GetComponent<Lighting>()->Light.LightIntensity = 0.f;;

	}


	void Update(GameObject* GO, float dt) 
	{
		UNREFERENCED_PARAMETER(GO);
		UNREFERENCED_PARAMETER(dt);
		if (playonce > 1) {
			GameObject* text = Engine->GetGameObject("Title");
			GameUI* ui_text = dynamic_cast<GameUI*>(text->GetComponent(ComponentType::GameUI));
			Renderer* btn = dynamic_cast<Renderer*>(text->GetComponent(ComponentType::Renderer));

			Renderer* BGr = dynamic_cast<Renderer*>(Engine->GetGameObject("Background")->GetComponent(ComponentType::Renderer));
			Renderer* Titler = dynamic_cast<Renderer*>(Engine->GetGameObject("GameTitle")->GetComponent(ComponentType::Renderer));

			BGr->SetVisibility(true);

			Titler->SetVisibility(true);

			ui_text->SetFontVisibility(true);
			btn->SetVisibility(true);

			text = Engine->GetGameObject("Start Game Btn");
			ui_text = dynamic_cast<GameUI*>(text->GetComponent(ComponentType::GameUI));
			btn = dynamic_cast<Renderer*>(text->GetComponent(ComponentType::Renderer));
			ui_text->SetFontVisibility(true);
			btn->SetVisibility(true);

			text = Engine->GetGameObject("Quit Game Btn");
			ui_text = dynamic_cast<GameUI*>(text->GetComponent(ComponentType::GameUI));
			btn = dynamic_cast<Renderer*>(text->GetComponent(ComponentType::Renderer));
			ui_text->SetFontVisibility(true);
			btn->SetVisibility(true);

			text = Engine->GetGameObject("Options Btn");
			ui_text = dynamic_cast<GameUI*>(text->GetComponent(ComponentType::GameUI));
			btn = dynamic_cast<Renderer*>(text->GetComponent(ComponentType::Renderer));
			ui_text->SetFontVisibility(true);
			btn->SetVisibility(true);

			playonce = 1;

			

		}

		if (Engine->GetGameObject("GameTitle")->GetComponent<Renderer>()->GetVisibility())
		{
			if (Engine->GetGameObject("Light1")->GetComponent<Lighting>()->Light.LightIntensity < light1_itensity_save)
			{
				Engine->GetGameObject("Light1")->GetComponent<Lighting>()->Light.LightIntensity += light1_delta;
			}

			if (Engine->GetGameObject("Light1")->GetComponent<Lighting>()->Light.LightIntensity > light1_itensity_save)
			{
				Engine->GetGameObject("Light1")->GetComponent<Lighting>()->Light.LightIntensity = light1_itensity_save;
			}

			if (Engine->GetGameObject("Light2")->GetComponent<Lighting>()->Light.LightIntensity < light2_itensity_save)
			{
				Engine->GetGameObject("Light2")->GetComponent<Lighting>()->Light.LightIntensity += light2_delta;
			}

			if (Engine->GetGameObject("Light2")->GetComponent<Lighting>()->Light.LightIntensity > light2_itensity_save)
			{
				Engine->GetGameObject("Light2")->GetComponent<Lighting>()->Light.LightIntensity = light2_itensity_save;
			}
		}

		//std::cout << "Light1: " << Engine->GetGameObject("Light1")->GetComponent<Lighting>()->Light.LightIntensity << "\nLight2: " << Engine->GetGameObject("Light2")->GetComponent<Lighting>()->Light.LightIntensity
		//	<< std::endl;
	}

	

	void End(GameObject* GO, float dt) 
	{
		UNREFERENCED_PARAMETER(GO);
		UNREFERENCED_PARAMETER(dt);
	}
}