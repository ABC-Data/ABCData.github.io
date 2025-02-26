/******************************************************************************/
/*!
\file		AllScripts.h
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	Nov 1, 2022
\brief		Contains all scripts to be loaded into the game
			Registers all scripts

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

/*

INSTRUCTIONS FOR USE:

1. Include ALL script files in this file
2. Register ALL scripts in LoadLogic() and give them a key (Use their SKey)
3. Include this file in main.cpp
4. Call Engine->AddSystem(LoadLogic()) in main.cpp AFTER Input BEFORE Physics

*/

#pragma once
#include "Logic/Logic.h"
#include "Combat.h"
#include "Camfollow.h"
#include "Mousemovement.h"
#include "Inventory.h"
#include "UIFunctions.h"
#include "CheatCodes.h"
#include "TutorialController.h"
#include "CombatController.h"
#include "Tooltip.h"
#include "EnemyAI.h"
#include "CombatUI.h"
#include "MainMenu.h"
#include "TypewriterEffect.h"
#include "FlashingGameUIText.h"
#include "MainMenuController.h"
#include "BreatheEffect.h"
#include "Defeat.h"
#include "Victory.h"
#include "CombatTutorialDialogue.h"
#include "InventoryZoom.h"
#include "HubController.h"
#include "Crafting.h"
#include "OptionsMenu.h"
#include "Level1Controller.h"
#include "CraftVFX.h"
#include "Cooking.h"
#include "Followmouse.h"
#include "CombatEquipment.h"
#include "Level1Controller2.h"
#include "Level1Controller3.h"
#include "Level1Controller4.h"
#include "Level1Controller5.h"
#include "Level1Controller6.h"
#include "EndingController.h"


Logic* LoadLogic() {
	Logic* LogicSystem = new Logic();

	LogicSystem->RegisterScript(CombatTutorialDialogue::Init, CombatTutorialDialogue::Update, CombatTutorialDialogue::End, CombatTutorialDialogue::SKey);
	LogicSystem->RegisterScript(MainMenuController::Init, MainMenuController::Update, MainMenuController::End, MainMenuController::SKey);
	LogicSystem->RegisterScript(FlasingGameUIText::Init, FlasingGameUIText::Update, FlasingGameUIText::End, FlasingGameUIText::SKey);
	LogicSystem->RegisterScript(TypewriterEffect::Init, TypewriterEffect::Update, TypewriterEffect::End, TypewriterEffect::SKey);
	LogicSystem->RegisterScript(MainMenu::Init, MainMenu::Update, MainMenu::End, MainMenu::SKey);
	LogicSystem->RegisterScript(CombatScript::Init, CombatScript::Update, CombatScript::End, CombatScript::SKey);
	LogicSystem->RegisterScript(Camfollow::Init, Camfollow::Update, Camfollow::End, Camfollow::SKey);
	LogicSystem->RegisterScript(MMovement::Init, MMovement::Update, MMovement::End, MMovement::SKey);
	LogicSystem->RegisterScript(InvScript::Init, InvScript::Update, InvScript::End, InvScript::SKey);
	LogicSystem->RegisterScript(UIFunctions::Init, UIFunctions::Update, UIFunctions::End, UIFunctions::SKey);
	LogicSystem->RegisterScript(CheatCodes::Init, CheatCodes::Update, CheatCodes::End, CheatCodes::SKey);
	LogicSystem->RegisterScript(TutorialController::Init, TutorialController::Update, TutorialController::End, TutorialController::SKey);
	LogicSystem->RegisterScript(CombatController::Init, CombatController::Update, CombatController::End, CombatController::SKey);
	LogicSystem->RegisterScript(Tooltip::Init, Tooltip::Update, Tooltip::End, Tooltip::SKey);
	LogicSystem->RegisterScript(EnemyAI::Init, EnemyAI::Update, EnemyAI::End, EnemyAI::SKey);
	LogicSystem->RegisterScript(CombatUI::Init, CombatUI::Update, CombatUI::End, CombatUI::SKey);
	LogicSystem->RegisterScript(BreatheEffect::Init, BreatheEffect::Update, BreatheEffect::End, BreatheEffect::SKey);
	LogicSystem->RegisterScript(DefeatScript::Init, DefeatScript::Update, DefeatScript::End, DefeatScript::SKey);
	LogicSystem->RegisterScript(VictoryScript::Init, VictoryScript::Update, VictoryScript::End, VictoryScript::SKey);
	LogicSystem->RegisterScript(InventoryZoom::Init, InventoryZoom::Update, InventoryZoom::End, InventoryZoom::SKey);
	LogicSystem->RegisterScript(HubController::Init, HubController::Update, HubController::End, HubController::SKey);
	LogicSystem->RegisterScript(Crafting::Init, Crafting::Update, Crafting::End, Crafting::SKey);
	LogicSystem->RegisterScript(OptionsMenu::Init, OptionsMenu::Update, OptionsMenu::End, OptionsMenu::SKey);
	LogicSystem->RegisterScript(Quests::Init, Quests::Update, Quests::End, Quests::SKey);
	LogicSystem->RegisterScript(Level1Controller::Init, Level1Controller::Update, Level1Controller::End, Level1Controller::SKey);
	LogicSystem->RegisterScript(CraftVFX::Init, CraftVFX::Update, CraftVFX::End, CraftVFX::SKey);
	LogicSystem->RegisterScript(Cooking::Init, Cooking::Update, Cooking::End, Cooking::SKey);
	LogicSystem->RegisterScript(Followmouse::Init, Followmouse::Update, Followmouse::End, Followmouse::SKey);
	LogicSystem->RegisterScript(CombatEquipment::Init, CombatEquipment::Update, CombatEquipment::End, CombatEquipment::SKey);
	LogicSystem->RegisterScript(Level1Controller2::Init, Level1Controller2::Update, Level1Controller2::End, Level1Controller2::SKey);
	LogicSystem->RegisterScript(Level1Controller3::Init, Level1Controller3::Update, Level1Controller3::End, Level1Controller3::SKey);
	LogicSystem->RegisterScript(Level1Controller4::Init, Level1Controller4::Update, Level1Controller4::End, Level1Controller4::SKey);
	LogicSystem->RegisterScript(Level1Controller5::Init, Level1Controller5::Update, Level1Controller5::End, Level1Controller5::SKey);
	LogicSystem->RegisterScript(Level1Controller6::Init, Level1Controller6::Update, Level1Controller6::End, Level1Controller6::SKey);
	LogicSystem->RegisterScript(EndingController::Init, EndingController::Update, EndingController::End, EndingController::SKey);


	return LogicSystem;
}