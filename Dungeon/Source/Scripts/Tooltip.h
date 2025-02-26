/******************************************************************************/
/*!
\file		Tooltip.h
\author 	Lim Hui Ching
\par    	email: l.huiching\@digipen.edu, 620004221
\date   	Jan 27, 2023
\brief		Tooltip Script.

	Copyright (C) 2022 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "./Logic/Script.h"
#include "Logic/Logic.h"
#include "CombatComponent.h"
#include "Input/Input.h"
#include "CoreSys/Core.h"
#include "SceneManager/SceneManager.h"

extern bool is_paused;

namespace Tooltip {
	std::string SKey{ "Tooltip" };

	/*!*************************************************************************
	****
	\brief
		Initializes Tooltip script.
	\param[in] GO
		Game Object to apply script on.
	\param[in] dt
		Delta time.
	****************************************************************************
	***/
	void Init(GameObject* GO, float dt) {
		UNREFERENCED_PARAMETER(dt);
		if (GO != nullptr) {
			return;
		}
		else {
			LogicComponent* LC = dynamic_cast<LogicComponent*>(GO->GetComponent(ComponentType::Logic));
			//This must always work, otherwise how is this function even being called?
			//Do a simple check anyway for safety, but UDB if it's actually nullptr
			if (LC != nullptr) {
				std::cout << GO->GetName() << " couldn't properly initialise Tooltip script, removing.\n";
				LC->RemoveScriptKey(SKey);
			}
		}
	}

	/*!*************************************************************************
	****
	\brief
		Updates Tooltip.
	\param[in] GO
		Game Object to apply script on.
	\param[in] dt
		Delta time.
	****************************************************************************
	***/
	void Update(GameObject* GO, float dt) {
		UNREFERENCED_PARAMETER(dt);
		UNREFERENCED_PARAMETER(GO);
		std::vector<GameObject*> InventoryVector = {};
		std::vector<GameUI*> GameUIVector = {};
		GameObject* Player = Engine->GetGameObject("Player");
		Engine->GetAllObjectsByPrefix(InventoryVector, "Inventory Slot ");

		Inventory* IC = dynamic_cast<Inventory*>(Player->GetComponent(ComponentType::Inventory));
		for (int i = 0; i < IC->InventorySize; ++i) {
			GameUIVector.emplace_back(dynamic_cast<GameUI*>(InventoryVector[i]->GetComponent(ComponentType::GameUI)));
		}

		Transform* tT = dynamic_cast<Transform*>(GO->GetComponent(ComponentType::Transform));
		Renderer* tR = dynamic_cast<Renderer*>(GO->GetComponent(ComponentType::Renderer));
		GameUI* tUI = dynamic_cast<GameUI*>(GO->GetComponent(ComponentType::GameUI));

		PlayerCombat* PCB = dynamic_cast<PlayerCombat*>(Player->GetComponent(ComponentType::PlayerCombat));

		GameObject* EBackground = Engine->GetGameObject("Equipment Background");
		GameUI* BgGUI = dynamic_cast<GameUI*>(EBackground->GetComponent(ComponentType::GameUI));
		std::vector<GameUI*> EquipmentGameUIVector = {};

		std::vector<GameObject*> EquipmentVector = {};
		Engine->GetAllObjectsByPrefix(EquipmentVector, "Equipment Slot ");
		std::vector<Renderer*> EquipmentRendererVector = {};
		std::vector<Transform*> EquipmentTransformVector = {};

		Transform* BgT = dynamic_cast<Transform*>(EBackground->GetComponent(ComponentType::Transform));
		GameObject* EProfile = Engine->GetGameObject("Enemy Profile");
		GameUI* EGUI = dynamic_cast<GameUI*>(EProfile->GetComponent(ComponentType::GameUI));
		Transform* EProfileT = dynamic_cast<Transform*>(EProfile->GetComponent(ComponentType::Transform));

		for (int i = 0; i < 4; ++i) {
			EquipmentGameUIVector.emplace_back(dynamic_cast<GameUI*>(EquipmentVector[i]->GetComponent(ComponentType::GameUI)));
			EquipmentRendererVector.emplace_back(dynamic_cast<Renderer*>(EquipmentVector[i]->GetComponent(ComponentType::Renderer)));
			EquipmentTransformVector.emplace_back(dynamic_cast<Transform*>(EquipmentVector[i]->GetComponent(ComponentType::Transform)));
		}
		if (!PCB->InCombat) {
			tR->SetVisibility(false);
			tUI->SetText("-");
			for (GameUI* x : EquipmentGameUIVector) { x->SetText("-"); }
		}

		GameObject* Enemy = Engine->GetGameObject("Enemy");
		EnemyCombat* ECB = dynamic_cast<EnemyCombat*>(Enemy->GetComponent(ComponentType::EnemyCombat));

		GameObject* Profile = Engine->GetGameObject("Yu Profile");
		GameUI* PGUI = dynamic_cast<GameUI*>(Profile->GetComponent(ComponentType::GameUI));

		bool playerReveal = false, InventReveal = false, enemyReveal = false, equipReveal = false;
		float scaleX = (float)GLSetup::current_width / (float)GLSetup::width;
		float scaleY = (float)GLSetup::current_height / (float)GLSetup::height;

		if (!is_paused && PCB->InCombat) {
			if (EGUI->MouseHoverClick(Input::GetMouse().x, (GLSetup::current_height - Input::GetMouse().y))) /// Tooltip for Enemy
			{
				tUI->SetSize(1.0f);
				tUI->FontFamily = "Idk";
				tUI->SetText(ECB->Name + "\n\n                    " + std::to_string(static_cast<int>(100 * ECB->physicalRES)) + "\n                    " + std::to_string(static_cast<int>(100 * ECB->ShockRES)) + "\n                    " + std::to_string(static_cast<int>(100 * ECB->bleedRES)));
				tR->SetTexHandle(ECB->TooltipHandle);
				tR->SetUVMin(ECB->TooltipMin);
				tR->SetUVMax(ECB->TooltipMax);
				tT->Dimensions = ECB->TooltipDimensions;
				tUI->x_offset = -150.f;
				tUI->y_offset = 200.f;
				enemyReveal = true;

				tT->Position = { static_cast<float>((Input::GetMouse().x / scaleX + 0.5f * tT->Dimensions.x * tT->Scale.x / 100.0f)),
			(GLSetup::current_height - Input::GetMouse().y) / scaleY + 0.5f * tT->Dimensions.y * tT->Scale.y / 100.0f };

				if (tT->Position.x >= (GLSetup::current_width / scaleX - 0.5f * (tT->Dimensions.x * tT->Scale.x / 100.0f))) { // Clamping of Tooltip
					tT->Position.x = GLSetup::current_width / scaleX - 0.5f * (tT->Dimensions.x * tT->Scale.x / 100.0f);
				}

				if (tT->Position.y >= (GLSetup::current_height / scaleY - 0.5f * (tT->Dimensions.y * tT->Scale.y / 100.0f))) {
					tT->Position.y = GLSetup::current_height / scaleY - 0.5f * (tT->Dimensions.y * tT->Scale.y / 100.0f);
				}
			}
			else { enemyReveal = false; }

			/// Tooltip for Player
			if (PGUI->MouseHoverClick(Input::GetMouse().x, (GLSetup::current_height - Input::GetMouse().y)))
			{
				tUI->SetSize(1.0f);
				tUI->FontFamily = "Idk";
				tUI->SetText(std::to_string(static_cast<int>(100 * PCB->physicalRES)) + "\n" + std::to_string(static_cast<int>(100 * PCB->stunChance)) + "\n" + std::to_string(static_cast<int>(100 * PCB->bleedChance)));
				tR->SetTexHandle(PCB->TooltipHandle);
				tR->SetUVMin(PCB->TooltipMin);
				tR->SetUVMax(PCB->TooltipMax);
				tT->Dimensions = PCB->TooltipDimensions;
				tUI->x_offset = 105.f;
				tUI->y_offset = -15.f;
				playerReveal = true;

				tT->Position = { static_cast<float>((Input::GetMouse().x / scaleX + 0.5f * tT->Dimensions.x * tT->Scale.x / 100.0f)),
			(GLSetup::current_height - Input::GetMouse().y) / scaleY + 0.5f * tT->Dimensions.y * tT->Scale.y / 100.0f };
			}
			else { playerReveal = false; }

			/// Tooltip for Inventory Items
			for (int i = 0; i < IC->InventorySize; ++i) {
				if (GameUIVector[i]->MouseHoverClick(Input::GetMouse().x, (GLSetup::current_height - Input::GetMouse().y))) {
					tT->Dimensions = IC->Invent[i].TooltipDimensions;
					tT->Position = { static_cast<float>((Input::GetMouse().x / scaleX + 0.5f * tT->Dimensions.x * tT->Scale.x / 100.0f)),
					(GLSetup::current_height - Input::GetMouse().y) / scaleY + 0.5f * tT->Dimensions.y * tT->Scale.y / 100.0f };
					tR->SetTexHandle(IC->Invent[i].TtTexHandle);
					tR->SetUVMin(IC->Invent[i].TtTex_UV_Min_COORD);
					tR->SetUVMax(IC->Invent[i].TtTex_UV_Max_COORD);
					InventReveal = true;
					if (IC->Invent[i].Type == "Equipment") {
						tUI->FontFamily = "Dialogue";
						tUI->SetText(IC->Invent[i].EStats.Set + " Set Bonus (" + std::to_string(IC->CountSetItems(IC->Invent[i].EStats.Set)) + "/4)"); // Line 1
						tUI->x_offset = -160.f;
						tUI->y_offset = 50.f;
						tUI->SetSize(0.7f);

						for (int k = 0; k < 4; ++k) {
							std::string setitemname = MasterObjectList->GetInventoryItem(MasterObjectList->MasterSetEffectList[IC->Invent[i].EStats.Set].SetIDs[k]).Name;
							EquipmentGameUIVector[k]->SetText(std::string(k, '\n') + setitemname);
							EquipmentGameUIVector[k]->x_offset = tT->Position.x - EquipmentTransformVector[k]->Position.x - 0.45f * tT->Dimensions.x * tT->Scale.x / 100.0f;
							EquipmentGameUIVector[k]->y_offset = (tT->Position.y - EquipmentTransformVector[k]->Position.y);
							EquipmentGameUIVector[k]->SetFontColor(0.5f, 0.5f, 0.5f);
						}

						std::string setdesc = "(2) ";
						if (MasterObjectList->MasterSetEffectList[IC->Invent[i].EStats.Set].HP_Bonus_2 > 0) {
							setdesc += "Max HP +";
							setdesc += std::to_string(MasterObjectList->MasterSetEffectList[IC->Invent[i].EStats.Set].HP_Bonus_2);
						}
						else {
							setdesc += "Max AP +";
							setdesc += std::to_string(static_cast<int>(MasterObjectList->MasterSetEffectList[IC->Invent[i].EStats.Set].AP_Bonus_2));
						}
						BgGUI->x_offset = tT->Position.x - BgT->Position.x - 0.45f * tT->Dimensions.x * tT->Scale.x / 100.0f;
						BgGUI->y_offset = (tT->Position.y - BgT->Position.y);
						BgGUI->SetText("\n\n\n\n\n" + setdesc);

						if (MasterObjectList->MasterSetEffectList[IC->Invent[i].EStats.Set].HP_Bonus_4 > 0) {
							setdesc = "(4) Max HP +";
							setdesc += std::to_string(MasterObjectList->MasterSetEffectList[IC->Invent[i].EStats.Set].HP_Bonus_4);
						}
						else {
							setdesc = "(4) Max AP +";
							setdesc += std::to_string(static_cast<int>(MasterObjectList->MasterSetEffectList[IC->Invent[i].EStats.Set].AP_Bonus_4));
						}
						EGUI->x_offset = tT->Position.x - EProfileT->Position.x - 0.45f * tT->Dimensions.x * tT->Scale.x / 100.0f;
						EGUI->y_offset = (tT->Position.y - EProfileT->Position.y);
						EGUI->SetText("\n\n\n\n\n\n\n" + setdesc);
						(IC->VerifySetBonus(IC->Invent[i].EStats.Set) >= 1) ? BgGUI->SetFontColor(1.f, 1.f, 1.f) : BgGUI->SetFontColor(0.5f, 0.5f, 0.5f);
						(IC->VerifySetBonus(IC->Invent[i].EStats.Set) == 2) ? EGUI->SetFontColor(1.f, 1.f, 1.f) : EGUI->SetFontColor(0.5f, 0.5f, 0.5f);

						equipReveal = true;
					}
					break;
				}
				else { InventReveal = false; }
			}

			/// Tooltip for equipment
			for (int i = 0; i < 4; ++i) {
				if (EquipmentGameUIVector[i]->MouseHoverClick(Input::GetMouse().x, (GLSetup::current_height - Input::GetMouse().y)) && EquipmentRendererVector[i]->GetVisibility()) {
					tUI->FontFamily = "Dialogue";
					tUI->SetText(IC->Equipment[i].EStats.Set + " Set Bonus (" + std::to_string(IC->CountSetItems(IC->Equipment[i].EStats.Set)) + "/4)"); // Line 1
					tUI->x_offset = -160.f;
					tUI->y_offset = 50.f;
					tUI->SetSize(0.7f);
					tT->Dimensions = IC->Equipment[i].TooltipDimensions;

					tT->Position = { static_cast<float>((Input::GetMouse().x / scaleX + 0.5f * tT->Dimensions.x * tT->Scale.x / 100.0f)),
				(GLSetup::current_height - Input::GetMouse().y) / scaleY + 0.5f * tT->Dimensions.y * tT->Scale.y / 100.0f };

					if (tT->Position.x >= (GLSetup::current_width / scaleX - 0.5f * (tT->Dimensions.x * tT->Scale.x / 100.0f))) { // Clamping of Tooltip
						tT->Position.x = GLSetup::current_width / scaleX - 0.5f * (tT->Dimensions.x * tT->Scale.x / 100.0f);
					}

					if (tT->Position.y >= (GLSetup::current_height / scaleY - 0.5f * (tT->Dimensions.y * tT->Scale.y / 100.0f))) {
						tT->Position.y = GLSetup::current_height / scaleY - 0.5f * (tT->Dimensions.y * tT->Scale.y / 100.0f);
					}

					for (int k = 0; k < 4; ++k) {
						std::string setitemname = MasterObjectList->GetInventoryItem(MasterObjectList->MasterSetEffectList[IC->Equipment[i].EStats.Set].SetIDs[k]).Name;
						EquipmentGameUIVector[k]->SetText(std::string(k, '\n') + setitemname);
						EquipmentGameUIVector[k]->x_offset = tT->Position.x - EquipmentTransformVector[k]->Position.x - 0.45f * tT->Dimensions.x * tT->Scale.x / 100.0f;
						EquipmentGameUIVector[k]->y_offset = (tT->Position.y - EquipmentTransformVector[k]->Position.y);

						(IC->Equipment[k].Name == setitemname) ? EquipmentGameUIVector[k]->SetFontColor(1.f, 1.f, 1.f) : EquipmentGameUIVector[k]->SetFontColor(0.5f, 0.5f, 0.5f);
					}

					std::string setdesc = "(2) ";
					if (MasterObjectList->MasterSetEffectList[IC->Equipment[i].EStats.Set].HP_Bonus_2 > 0) {
						setdesc += "Max HP +";
						setdesc += std::to_string(MasterObjectList->MasterSetEffectList[IC->Equipment[i].EStats.Set].HP_Bonus_2);
					}
					else {
						setdesc += "Max AP +";
						setdesc += std::to_string(static_cast<int>(MasterObjectList->MasterSetEffectList[IC->Equipment[i].EStats.Set].AP_Bonus_2));
					}
					BgGUI->x_offset = tT->Position.x - BgT->Position.x - 0.45f * tT->Dimensions.x * tT->Scale.x / 100.0f;
					BgGUI->y_offset = (tT->Position.y - BgT->Position.y);
					BgGUI->SetText("\n\n\n\n\n" + setdesc);

					if (MasterObjectList->MasterSetEffectList[IC->Equipment[i].EStats.Set].HP_Bonus_4 > 0) {
						setdesc = "(4) Max HP +";
						setdesc += std::to_string(MasterObjectList->MasterSetEffectList[IC->Equipment[i].EStats.Set].HP_Bonus_4);
					}
					else {
						setdesc = "(4) Max AP +";
						setdesc += std::to_string(static_cast<int>(MasterObjectList->MasterSetEffectList[IC->Equipment[i].EStats.Set].AP_Bonus_4));
					}
					EGUI->x_offset = tT->Position.x - EProfileT->Position.x - 0.45f * tT->Dimensions.x * tT->Scale.x / 100.0f;
					EGUI->y_offset = (tT->Position.y - EProfileT->Position.y);
					EGUI->SetText("\n\n\n\n\n\n\n" + setdesc);
					(IC->VerifySetBonus(IC->Equipment[i].EStats.Set) >= 1) ? BgGUI->SetFontColor(1.f, 1.f, 1.f) : BgGUI->SetFontColor(0.5f, 0.5f, 0.5f);
					(IC->VerifySetBonus(IC->Equipment[i].EStats.Set) == 2) ? EGUI->SetFontColor(1.f, 1.f, 1.f) : EGUI->SetFontColor(0.5f, 0.5f, 0.5f);

					tR->SetTexHandle(IC->Equipment[i].TtTexHandle);
					tR->SetUVMin(IC->Equipment[i].TtTex_UV_Min_COORD);
					tR->SetUVMax(IC->Equipment[i].TtTex_UV_Max_COORD);
					equipReveal = true;

					break;
				}
			}

			(InventReveal || playerReveal || enemyReveal || equipReveal) ? tR->SetVisibility(true) : tR->SetVisibility(false);
			if (enemyReveal == false && playerReveal == false && equipReveal == false) { tUI->SetText("-"); }
			if (equipReveal == false) {
				for (GameUI* x : EquipmentGameUIVector) { x->SetText("-"); }
				EGUI->SetText("-");
				BgGUI->SetText("-");
			}
			else {
				for (GameUI* x : EquipmentGameUIVector) { x->SetFontVisibility(true); }
			}
		}
		else {
			for (GameUI* x : EquipmentGameUIVector) { x->SetText("-"); }
			EGUI->SetText("-");
			BgGUI->SetText("-");
		}
	}

	/*!*************************************************************************
	****
	\brief
		Ends Tooltip script.
	\param[in] GO
		Game Object to apply script on.
	\param[in] dt
		Delta time.
	****************************************************************************
	***/
	void End(GameObject* GO, float dt) {
		(void)GO;
		(void)dt;
	}
}