#pragma once
/************************************************************************ /
/*!
\file   CombatTutorialDialogue.h
\author Louis Mineo @ Linn Min Htoo
\par    email: louismineo.l@digipen.edu
\date   February 2nd, 2023
\brief	Function definitions of script, CombatTutorialDialogue, which guides the 
		player within the combat scene.

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "Essential.h"
#include "Logic/Script.h"
#include "Logic/Logic.h"
#include "Input/Input.h"
#include "Transform/Transform.h"
#include "GameUI/GameUI.h"
#include "Graphics/GLFWsetup.h"

extern bool is_paused;
extern MouseInput MyMouse;
extern bool keystateLM;
extern bool prevloop_keystateLM;

namespace CombatTutorialDialogue
{
	std::string SKey{ "CombatTutorialDialogue" };

	std::vector <std::string> tutorial_lines
		=
	{ "Are you ready for combat?",
	"The HP bar represents how much damage you can take\nbefore you pass out.",
	"The AP bar represents how much energy you have.\nEvery action consumes AP.",
	"You have the choice of Blocking the enemy's attack\nor Fleeing to escape this enemy.",
	"Or you can be brave, and utilise your AP and inventory\nweapons to attack the enemy.",
	"Your inventory allows you to pull different weapons\nfrom your bag to attack the enemy...",
	"Use consumables to heal yourself, or eat food to gain\nvarious buffs.",
	"That is all you will need. Good luck soldier, and godspeed."};

	int current_line;
	bool done;
	float total_dt;

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
		//disable combat scripts, so that it renders but doesnt go into combat yet
		//LogicComponent* LC = reinterpret_cast<LogicComponent*>(GO->GetComponent(ComponentType::Logic));
		
		dynamic_cast<LogicComponent*>(Engine->GetGameObject("Player")->GetComponent(ComponentType::Logic))->SetEnabled("Combat", false);//LC->SetEnabled("CombatUI", false);
		dynamic_cast<LogicComponent*>(Engine->GetGameObject("Player")->GetComponent(ComponentType::Logic))->SetEnabled("CombatComponent", false);

		dynamic_cast<LogicComponent*>(Engine->GetGameObject("Enemy")->GetComponent(ComponentType::Logic))->SetEnabled("EnemyAI", false);

		dynamic_cast<LogicComponent*>(Engine->GetGameObject("Inventory Slot 1")->GetComponent(ComponentType::Logic))->SetEnabled("CombatUI", false);

		dynamic_cast<LogicComponent*>(Engine->GetGameObject("Defeat")->GetComponent(ComponentType::Logic))->SetEnabled("Defeat", false);

		dynamic_cast<LogicComponent*>(Engine->GetGameObject("Victory")->GetComponent(ComponentType::Logic))->SetEnabled("Victory", false);

		dynamic_cast<LogicComponent*>(Engine->GetGameObject("Tooltip")->GetComponent(ComponentType::Logic))->SetEnabled("Tooltip", false);

		dynamic_cast<LogicComponent*>(Engine->GetGameObject("Equipment Slot 1")->GetComponent(ComponentType::Logic))->SetEnabled("CombatEquipment", false);
		
		dynamic_cast<Renderer*>(Engine->GetGameObject("Tooltip")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
		dynamic_cast<GameUI*>(Engine->GetGameObject("Tooltip")->GetComponent(ComponentType::GameUI))->SetText("-");

		current_line = 0;

		TypewriterEffect::Init(GO,dt);

		done = false;
		TypewriterEffect::done = false;
		

		total_dt = 0.f;
		//printed_text = saved_text.substr(0, last_idx);
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
		/*
		* { "Are you ready for combat?",
	"The HP bar represents how many hits you can take before you pass out", 1
	"The AP bar represents how much energy you have. Every action consumes AP", 2
	"You have the choice of Blocking the enemy's attack or Fleeing to escape this enemy.", 3
	"Or you can be brave, and utilise your AP and inventory weapons to attack the enemy.", 4
	"Your inventory allows you to pull different weapons from your bag, to attack enemy...", 5
	"use consumables to heal yourself, or eat food to gain various buffs.", 6
	"That is all you will need. Good luck soldier, and godspeed."}; 7
		*/
		if (Engine->GetObjectByPrefix("FadeOut")->GetComponent<Animate>()->GetVisibility()) {
			return;
		}
		total_dt += dt;
		
		//LogicComponent* LC = reinterpret_cast<LogicComponent*>(GO->GetComponent(ComponentType::Logic));
		if (!done)
		{
			GameUI* GUI = reinterpret_cast<GameUI*>(GO->GetComponent(ComponentType::GameUI));
			GUI->Alpha = 1.f; GUI->SetFontVisibility(true);
			Renderer* RDR = reinterpret_cast<Renderer*>(GO->GetComponent(ComponentType::Renderer));
			RDR->SetVisibility(true);

			float mx = abs(sinf(total_dt * 2));

			switch (current_line)
			{
				case 1:
					// highlight the HP bar
					dynamic_cast<Renderer*>(Engine->GetGameObject("Yu HP")->GetComponent(ComponentType::Renderer))->RGB_0to1 = {mx,mx ,mx };
					break;
				case 2:
					dynamic_cast<Renderer*>(Engine->GetGameObject("Yu HP")->GetComponent(ComponentType::Renderer))->RGB_0to1 = { 1.f,1.f ,1.f };
					// highlight the AP bar
					dynamic_cast<Renderer*>(Engine->GetGameObject("Action Points")->GetComponent(ComponentType::Renderer))->RGB_0to1 = { mx,mx ,mx };
					break;
				case 3:
					dynamic_cast<Renderer*>(Engine->GetGameObject("Action Points")->GetComponent(ComponentType::Renderer))->RGB_0to1 = { 1.f,1.f ,1.f };
					//highlight blocking and fleeing buttons
					dynamic_cast<Renderer*>(Engine->GetGameObject("Block")->GetComponent(ComponentType::Renderer))->RGB_0to1 = { mx,mx ,mx };
					dynamic_cast<Renderer*>(Engine->GetGameObject("Flee")->GetComponent(ComponentType::Renderer))->RGB_0to1 = { mx,mx ,mx };
					break;
				case 4:
					dynamic_cast<Renderer*>(Engine->GetGameObject("Block")->GetComponent(ComponentType::Renderer))->RGB_0to1 = { 1.f,1.f ,1.f };
					dynamic_cast<Renderer*>(Engine->GetGameObject("Flee")->GetComponent(ComponentType::Renderer))->RGB_0to1 = { 1.f,1.f ,1.f };
					//highlight kitchen knife
					dynamic_cast<Renderer*>(Engine->GetGameObject("Inventory Slot 1")->GetComponent(ComponentType::Renderer))->RGB_0to1 = { mx,mx ,mx }; 
					break;
				case 5:
					dynamic_cast<Renderer*>(Engine->GetGameObject("Block")->GetComponent(ComponentType::Renderer))->RGB_0to1 = { 1.f,1.f ,1.f };
					dynamic_cast<Renderer*>(Engine->GetGameObject("Inventory Slot 1")->GetComponent(ComponentType::Renderer))->RGB_0to1 = { 1.f,1.f ,1.f };
					break;
				case 6:
					// highlight consumable
					dynamic_cast<Renderer*>(Engine->GetGameObject("Inventory Slot 1")->GetComponent(ComponentType::Renderer))->RGB_0to1 = { mx,mx ,mx };
					break;
				case 7:
					dynamic_cast<Renderer*>(Engine->GetGameObject("Inventory Slot 1")->GetComponent(ComponentType::Renderer))->RGB_0to1 = { 1.f,1.f ,1.f };
					break;
			}

			// INPUT HANDLING
			
			// means that the text has finished rendering and player has pressed E
			if (TypewriterEffect::done)
			{
				// show the press E DialogueBoxNext
				dynamic_cast<LogicComponent*>(Engine->GetGameObject("DialogueBoxNext")->GetComponent(ComponentType::Logic))->SetEnabled("FlasingGameUIText", true);
				dynamic_cast<Renderer*>(Engine->GetGameObject("DialogueBoxNext")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
				dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxNext")->GetComponent(ComponentType::GameUI))->SetFontVisibility(true);

				//LC->SetEnabled("FlasingGameUIText", true); // enable the script so that the font can flash

				if (Input::GetKeyTriggered(Key::E))
				{

					// disable DialogueBoxNext's as invisible FlasingGameUIText script
					/*
					dynamic_cast<LogicComponent*>(Engine->GetGameObject("DialogueBoxNext")->GetComponent(ComponentType::Logic))->SetEnabled("FlasingGameUIText", false);
					// set DialogueBoxNext as invisible and font alpha to 0
					dynamic_cast<Renderer*>(Engine->GetGameObject("DialogueBoxNext")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
					dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxNext")->GetComponent(ComponentType::GameUI))->SetFontVisibility(false);//dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxNext")->GetComponent(ComponentType::GameUI))->Alpha = 0.f;
					*/

					// increase the line

					if (current_line < tutorial_lines.size() - 1)
					{
						current_line++;

						// re-init the typewriterscript
						TypewriterEffect::SetNewText(tutorial_lines[current_line]);
						/*
						dynamic_cast<GameUI*>(GO->GetComponent(ComponentType::GameUI))->Text.clear();	//clear current text
						TypewriterEffect::last_idx = 0; // set the last_dix to  
						TypewriterEffect::saved_text = tutorial_lines[current_line]; //change the saved text to new string
						TypewriterEffect::total_dt = 0.f; //reset total_dt
						TypewriterEffect::done = false; // set the done to false
						*/
						
					}
					else
					{
						//set the child press E box as invis, font invis and script disabled
						dynamic_cast<LogicComponent*>(Engine->GetGameObject("DialogueBoxNext")->GetComponent(ComponentType::Logic))->SetEnabled("FlasingGameUIText", false);
						// set DialogueBoxNext as invisible and font alpha to 0
						dynamic_cast<Renderer*>(Engine->GetGameObject("DialogueBoxNext")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
						dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxNext")->GetComponent(ComponentType::GameUI))->SetFontVisibility(false);//dynamic_cast<GameUI*>(Engine->GetGameObject("DialogueBoxNext")->GetComponent(ComponentType::GameUI))->Alpha = 0.f;

						//set the GO dialogue box as invis, font invis and script disabled
						dynamic_cast<LogicComponent*>(GO->GetComponent(ComponentType::Logic))->SetEnabled("CombatTutorialDialogue", false);
						dynamic_cast<Renderer*>(GO->GetComponent(ComponentType::Renderer))->SetVisibility(false);
						dynamic_cast<GameUI*>(GO->GetComponent(ComponentType::GameUI))->Alpha = 0.f;



						dynamic_cast<LogicComponent*>(Engine->GetGameObject("Player")->GetComponent(ComponentType::Logic))->SetEnabled("Combat", true);//LC->SetEnabled("CombatUI", false);
						dynamic_cast<LogicComponent*>(Engine->GetGameObject("Player")->GetComponent(ComponentType::Logic))->SetEnabled("CombatComponent", true);

						dynamic_cast<LogicComponent*>(Engine->GetGameObject("Enemy")->GetComponent(ComponentType::Logic))->SetEnabled("EnemyAI", true);

						dynamic_cast<LogicComponent*>(Engine->GetGameObject("Inventory Slot 1")->GetComponent(ComponentType::Logic))->SetEnabled("CombatUI", true);

						dynamic_cast<LogicComponent*>(Engine->GetGameObject("Defeat")->GetComponent(ComponentType::Logic))->SetEnabled("Defeat", true);

						dynamic_cast<LogicComponent*>(Engine->GetGameObject("Victory")->GetComponent(ComponentType::Logic))->SetEnabled("Victory", true);

						dynamic_cast<LogicComponent*>(Engine->GetGameObject("Tooltip")->GetComponent(ComponentType::Logic))->SetEnabled("Tooltip", true);

						dynamic_cast<LogicComponent*>(Engine->GetGameObject("Equipment Slot 1")->GetComponent(ComponentType::Logic))->SetEnabled("CombatEquipment", true);

						done = true;
					}

				}

			}
			/*
			else if (TypewriterEffect::done == false)
			{
				if (Input::GetKeyTriggered(Key::E))
				{
					// finish the text
					GUI->SetText(TypewriterEffect::saved_text);

					TypewriterEffect::last_idx = TypewriterEffect::saved_text.length() - 1 ; // set the last_dix to  
					TypewriterEffect::total_dt = 0.f; //reset total_dt
					TypewriterEffect::done = true; // set the done to false

				}
			}
			*/


		}
	
		
	}
	/**************************************************************************/
	/*!
		\brief
			End for the UIFunctions script. It is called for every
			GO that has this script attached to it.
		\param GO
			The specific Gameobject who is the calling and is affected by
			this script function
	*/
	/**************************************************************************/
	void End(GameObject* GO, float dt)
	{
		(void)dt;
		(void)GO;
	}
}




