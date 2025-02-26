/******************************************************************************/
/*!
\file		Inventory.h
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	Nov 15, 2022
\brief		Contains Inventory script

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "Essential.h"
#include "Logic/Logic.h"
#include "Logic/Script.h"
#include "GameObject/GameObject.h"
#include "Input/Input.h"
#include "Graphics/BatchRendering.h"
#include "InventoryComponent.h"
#include "CoreSys/Core.h"
#include "GameUI/GameUI.h"
#include "Graphics/GLFWsetup.h"
#include "Transform/Transform.h"
#include "Renderer/Renderer.h"

extern MouseInput MyMouse;


namespace InvScript {
	InventoryItem DraggedItem;
	GameObject* DraggedItemGO = nullptr;
	GameObject* DiscardConfirmGO = nullptr;
	bool DiscardConfirm = false;
	int RememberedID = -1;

	std::string SKey{ "Inventory" };

	void Init(GameObject* GO, float dt) {
		(void)dt;
		Inventory* IC = dynamic_cast<Inventory*>(GO->GetComponent(ComponentType::Inventory));

		if (IC != nullptr) {
			//All good
			DraggedItemGO = nullptr;
			DraggedItem.ClearItem();
			DiscardConfirm = false;
			RememberedID = -1;
		}
		else {
			std::cout << "Inventory component not found, removing InvScript\n";
			LogicComponent* LC = dynamic_cast<LogicComponent*>(GO->GetComponent(ComponentType::Logic));
			//This must always work, otherwise how is this function even being called?
			//Do a simple check anyway for safety, but UDB if it's actually nullptr
			if (LC != nullptr) {
				LC->RemoveScriptKey(SKey);
			}
		}
	}

	void Update(GameObject* GO, float dt) {
		(void)dt;
		Inventory* IC = GO->GetComponent<Inventory>();
		GameUI* GUI = Engine->GetObjectComponent<GameUI>(Engine->GetGameObject("Bag Icon"));

		std::vector<GameObject*> Icons;
		std::vector<GameObject*> ItemTexts;
		Engine->GetAllObjectsByPrefix(Icons, "InventoryIcon");
		Engine->GetAllObjectsByPrefix(Icons, "EquipmentIcon");
		Engine->GetAllObjectsByPrefix(ItemTexts, "ItemStats");
		int hover_x = Input::GetMouse().x;
		int hover_y_aft_TRS = (GLSetup::current_height - Input::GetMouse().y);
		if (Input::GetKeyTriggered(Key::I)) {
			if (DraggedItem.ID == 0) {
				audio->PlaySfx(30);
				IC->Visible = !IC->Visible;
				LogicComponent* L = Engine->GetObjectComponent<LogicComponent>(GO);
				L->SetEnabled("MMovement", !IC->Visible);
				if (IC->Visible) {
					GO->GetComponent<RigidBody>()->SetVelocity({ 0.0f,0.0f });
					GO->GetComponent<Animate>()->SetTexHandle("Yu_Idle");
					Engine->GetObjectComponent<Renderer>(Engine->GetGameObject("Bag Icon"))->SetUVMin({ 0.f, 4367.f });
					Engine->GetObjectComponent<Renderer>(Engine->GetGameObject("Bag Icon"))->SetUVMax({ 87.f, 4454.f });
				}
				else {
					Engine->GetObjectComponent<Renderer>(Engine->GetGameObject("Bag Icon"))->SetUVMin({ 0.f, 4454.f });
					Engine->GetObjectComponent<Renderer>(Engine->GetGameObject("Bag Icon"))->SetUVMax({ 87.f, 4541.f });
				}
				Renderer* RDR = Engine->GetObjectComponent<Renderer>(Engine->GetGameObject("ScreenTint"));
				RDR->SetVisibility(IC->Visible);
			}
		}
		if ((hover_x >= GUI->AA.x && hover_x <= GUI->BB.x &&
				(hover_y_aft_TRS) >= GUI->AA.y && (hover_y_aft_TRS <= GUI->BB.y))) {
			Engine->GetObjectComponent<Renderer>(Engine->GetGameObject("Bag Icon"))->RGB_0to1 = { 0.5f, 0.5f, 0.5f };

			if (Input::GetMouseTriggeredDepth(MouseButton::Button0, Engine->GetGameObject("Bag Icon"))) {
				if (DraggedItem.ID == 0) {
					audio->PlaySfx(30);
					IC->Visible = !IC->Visible;
					LogicComponent* L = Engine->GetObjectComponent<LogicComponent>(GO);
					L->SetEnabled("MMovement", !IC->Visible);
					if (IC->Visible) {
						GO->GetComponent<RigidBody>()->SetVelocity({ 0.0f,0.0f });
						GO->GetComponent<Animate>()->SetTexHandle("Yu_Idle");
						Engine->GetObjectComponent<Renderer>(Engine->GetGameObject("Bag Icon"))->SetUVMin({ 0.f, 4367.f });
						Engine->GetObjectComponent<Renderer>(Engine->GetGameObject("Bag Icon"))->SetUVMax({ 87.f, 4454.f });
					}
					else {
						Engine->GetObjectComponent<Renderer>(Engine->GetGameObject("Bag Icon"))->SetUVMin({ 0.f, 4454.f });
						Engine->GetObjectComponent<Renderer>(Engine->GetGameObject("Bag Icon"))->SetUVMax({ 87.f, 4541.f });
					}
					Renderer* RDR = Engine->GetObjectComponent<Renderer>(Engine->GetGameObject("ScreenTint"));
					RDR->SetVisibility(IC->Visible);
				}
			}
		}
		else {
			Engine->GetObjectComponent<Renderer>(Engine->GetGameObject("Bag Icon"))->RGB_0to1 = { 1.f, 1.f, 1.f };
		}

		int i = 0;
		for (GameObject* Obj : Icons) {
			Renderer* RD = Obj->GetComponent<Renderer>();
			RD->SetVisibility(IC->Visible);
			if (i < IC->InventorySize && IC->Invent[i].ID != 0) {
				RD->SetTexHandle(IC->Invent[i].TexHandle);
				RD->SetUVMin(IC->Invent[i].Tex_UV_Min_COORD);
				RD->SetUVMax(IC->Invent[i].Tex_UV_Max_COORD);
			}
			else if (i >= IC->InventorySize && IC->Equipment[i - IC->InventorySize].ID != 0) {
				RD->SetTexHandle(IC->Equipment[i - IC->InventorySize].TexHandle);
				RD->SetUVMin(IC->Equipment[i - IC->InventorySize].Tex_UV_Min_COORD);
				RD->SetUVMax(IC->Equipment[i - IC->InventorySize].Tex_UV_Max_COORD);
			}
			else {
				RD->SetTexHandle("GameUI");
				RD->SetUVMin({ 0.f, 2193.f });
				RD->SetUVMax({ 166.f, 2355.f });
			}
			++i;
		}
		for (GameObject* Obj : ItemTexts) {
			GameUI* G = Obj->GetComponent<GameUI>();
			G->Alpha = IC->Visible ? 1.f : 0.f;
			G->SetText("");
			G->SetFontColor(1.f, 1.f, 1.f);
		}
		i = 0;
		for (GameObject* Obj : Icons) {
			GameUI* G = Obj->GetComponent<GameUI>();
			if (Input::GetMouseTriggered(MouseButton::Button0)
				&& G->MouseHoverClick(Input::GetMouse().x, GLSetup::current_height - Input::GetMouse().y)
				&& IC->Visible) {
				if (DraggedItem.ID == 0) {
					// Case: No dragged item
					if (i < IC->InventorySize) {
						if (IC->Invent[i].ID != 0) {
							DraggedItem = i >= IC->InventorySize ? IC->Equipment[i - IC->InventorySize] : IC->Invent[i];
							DraggedItemGO = factory->AddGameObject("./Data/UIInventoryItem.txt");
							RememberedID = i;
							Renderer* GOR = DraggedItemGO->GetComponent<Renderer>();
							GOR->SetTexHandle(DraggedItem.TexHandle);
							GOR->SetUVMin(DraggedItem.Tex_UV_Min_COORD);
							GOR->SetUVMax(DraggedItem.Tex_UV_Max_COORD);
							float new_x = (float)(Input::GetMouse().x / (float)((float)GLSetup::current_width / (float)GLSetup::width));
							float new_y = (float)((GLSetup::current_height - Input::GetMouse().y) / (float)((float)GLSetup::current_height / (float)GLSetup::height));
							DraggedItemGO->GetComponent<Transform>()->Position = { new_x, new_y };
							factory->QueueAdd(DraggedItemGO);
							Renderer* IcoR = Obj->GetComponent<Renderer>();
							IcoR->SetTexHandle("GameUI");
							IcoR->SetUVMin({ 0.f, 2193.f });
							IcoR->SetUVMax({ 166.f, 2355.f });
							i >= IC->InventorySize ? IC->Equipment[i - IC->InventorySize].ClearItem() : IC->Invent[i].ClearItem();
						}
					}
					else {
						if (IC->Equipment[i - IC->InventorySize].ID != 0) {
							DraggedItem = i >= IC->InventorySize ? IC->Equipment[i - IC->InventorySize] : IC->Invent[i];
							DraggedItemGO = factory->AddGameObject("./Data/UIInventoryItem.txt");
							RememberedID = i;
							Renderer* GOR = DraggedItemGO->GetComponent<Renderer>();
							GOR->SetTexHandle(DraggedItem.TexHandle);
							GOR->SetUVMin(DraggedItem.Tex_UV_Min_COORD);
							GOR->SetUVMax(DraggedItem.Tex_UV_Max_COORD);
							float new_x = (float)(Input::GetMouse().x / (float)((float)GLSetup::current_width / (float)GLSetup::width));
							float new_y = (float)((GLSetup::current_height - Input::GetMouse().y) / (float)((float)GLSetup::current_height / (float)GLSetup::height));
							DraggedItemGO->GetComponent<Transform>()->Position = { new_x, new_y };
							factory->QueueAdd(DraggedItemGO);
							Renderer* IcoR = Obj->GetComponent<Renderer>();
							IcoR->SetTexHandle("GameUI");
							IcoR->SetUVMin({ 0.f, 2193.f });
							IcoR->SetUVMax({ 166.f, 2355.f });
							i >= IC->InventorySize ? IC->Equipment[i - IC->InventorySize].ClearItem() : IC->Invent[i].ClearItem();
						}
					}
				}
				else {
					// Case: Dragged item
					if (G->MouseHoverClick(Input::GetMouse().x, GLSetup::current_height - Input::GetMouse().y)) {
						audio->PlaySfx(30);
						if (i >= IC->InventorySize) {
							if (DraggedItem.Type == "Equipment") {
								if (i - (int)IC->InventorySize + 1 == (int)DraggedItem.EStats.Slot) {
									if (IC->Equipment[i - IC->InventorySize].ID != 0) {
										if (RememberedID < IC->InventorySize && RememberedID >= 0) {
											IC->Invent[RememberedID] = IC->Equipment[i - IC->InventorySize];
										}
										else {
											// Theoretically shouldn't get to this
											IC->Equipment[RememberedID - IC->InventorySize] = IC->Equipment[i - IC->InventorySize];
										}
									}
									
									IC->Equipment[i - IC->InventorySize] = DraggedItem;
									DraggedItem.ClearItem();
									factory->QueueDelete(DraggedItemGO);
									RememberedID = -1;
								}
							}
						}
						else {
							if (IC->Invent[i].ID == 0) {
								IC->Invent[i] = DraggedItem;
								DraggedItem.ClearItem();
								factory->QueueDelete(DraggedItemGO);
								RememberedID = -1;
							}
							else if (DraggedItem.Type == "Equipment" && RememberedID >= IC->InventorySize) {
								// Case: Dragged item is equipment which came from an equipment slot
								// Allow swap only if dest is same slot
								if (IC->Invent[i].EStats.Slot == DraggedItem.EStats.Slot) {
									IC->Equipment[RememberedID - IC->InventorySize] = IC->Invent[i];
									IC->Invent[i] = DraggedItem;
									DraggedItem.ClearItem();
									factory->QueueDelete(DraggedItemGO);
									RememberedID = -1;
								}
							}
							else if (RememberedID < IC->InventorySize) {
								// Case: Everything else
								IC->Invent[RememberedID] = IC->Invent[i];
								IC->Invent[i] = DraggedItem;
								DraggedItem.ClearItem();
								factory->QueueDelete(DraggedItemGO);
								RememberedID = -1;
							}
						}
						
					}
				}
			}
			if (G->MouseHoverClick(Input::GetMouse().x, GLSetup::current_height - Input::GetMouse().y)) {
				if (i < IC->InventorySize) {
					if (IC->Invent[i].ID == 0) continue;
					ItemTexts[0]->GetComponent<GameUI>()->SetText(IC->Invent[i].Name);
					ItemTexts[1]->GetComponent<GameUI>()->SetText(IC->Invent[i].Type);
					if (IC->Invent[i].Type == "Weapon") {
						ItemTexts[2]->GetComponent<GameUI>()->SetText(IC->Invent[i].WStats.WeaponType + " weapon");
						ItemTexts[3]->GetComponent<GameUI>()->SetText("Damage: " + std::to_string(static_cast<int>(IC->Invent[i].WStats.Damage)));
						ItemTexts[4]->GetComponent<GameUI>()->SetText("Crit Chance: " + std::to_string(static_cast<int>(IC->Invent[i].WStats.CritChance * 100.f)) + "%");
						ItemTexts[5]->GetComponent<GameUI>()->SetText("Crit Damage Multiplier: " + std::to_string(static_cast<int>(IC->Invent[i].WStats.CritMultiplier * 100.f)) + "%");
						ItemTexts[6]->GetComponent<GameUI>()->SetText("Accuracy: " + std::to_string(static_cast<int>(IC->Invent[i].WStats.Accuracy * 100.f)) + "%");
						ItemTexts[7]->GetComponent<GameUI>()->SetText("AP Cost: " + std::to_string(IC->Invent[i].WStats.APCost));
					}
					else if (IC->Invent[i].Type == "Consumable") {
						ItemTexts[2]->GetComponent<GameUI>()->SetText(ConvertReadable(IC->Invent[i].CStats.Effect));
						if (IC->Invent[i].CStats.EffectValue > 0.f) {
							ItemTexts[3]->GetComponent<GameUI>()->SetText("Amount: " + std::to_string(static_cast<int>(IC->Invent[i].CStats.EffectValue)));
						}
						if (IC->Invent[i].CStats.Duration > 0.f) {
							ItemTexts[4]->GetComponent<GameUI>()->SetText("Duration: " + std::to_string(static_cast<int>(IC->Invent[i].CStats.Duration)));
						}
					}
					else if (IC->Invent[i].Type == "Material") {
						ItemTexts[2]->GetComponent<GameUI>()->SetText("Quantity: " + std::to_string(IC->Invent[i].Quantity));
					}
					else if (IC->Invent[i].Type == "Equipment") {
						ItemTexts[2]->GetComponent<GameUI>()->SetText(ConvertReadable(IC->Invent[i].EStats.Slot) + " Slot");
						ItemTexts[3]->GetComponent<GameUI>()->SetText("Physical Resistance: " + std::to_string(static_cast<int>(IC->Invent[i].EStats.Phys_Resist * 100.f)) + "%");
						ItemTexts[4]->GetComponent<GameUI>()->SetText("Stun Resistance: " + std::to_string(static_cast<int>(IC->Invent[i].EStats.Stun_Resist * 100.f)) + "%");
						ItemTexts[5]->GetComponent<GameUI>()->SetText("Bleed Resistance: " + std::to_string(static_cast<int>(IC->Invent[i].EStats.Bleed_Resist * 100.f)) + "%");
						ItemTexts[6]->GetComponent<GameUI>()->SetText("Set Effect: " + IC->Invent[i].EStats.Set + " (" + std::to_string(IC->CountSetItems(IC->Invent[i].EStats.Set)) + "/4)");
						for (int j = 0; j < 4; ++j) {
							std::string setitemname = MasterObjectList->GetInventoryItem(MasterObjectList->MasterSetEffectList[IC->Invent[i].EStats.Set].SetIDs[j]).Name;
							ItemTexts[(size_t)7 + (size_t)j]->GetComponent<GameUI>()->SetText(setitemname);
							if (IC->Equipment[j].Name == setitemname) {
								ItemTexts[(size_t)7 + (size_t)j]->GetComponent<GameUI>()->SetFontColor(0.f, 1.f, 0.f);
							}
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
						ItemTexts[11]->GetComponent<GameUI>()->SetText(setdesc);
						if (MasterObjectList->MasterSetEffectList[IC->Invent[i].EStats.Set].HP_Bonus_4 > 0) {
							setdesc = "(4) Max HP +";
							setdesc += std::to_string(MasterObjectList->MasterSetEffectList[IC->Invent[i].EStats.Set].HP_Bonus_4);
						}
						else {
							setdesc = "(4) Max AP +";
							setdesc += std::to_string(static_cast<int>(MasterObjectList->MasterSetEffectList[IC->Invent[i].EStats.Set].AP_Bonus_4));
						}
						ItemTexts[12]->GetComponent<GameUI>()->SetText(setdesc);
						if (IC->VerifySetBonus(IC->Invent[i].EStats.Set) >= 1) {
							ItemTexts[11]->GetComponent<GameUI>()->SetFontColor(0.f, 1.f, 0.f);
						}
						if (IC->VerifySetBonus(IC->Invent[i].EStats.Set) == 2) {
							ItemTexts[12]->GetComponent<GameUI>()->SetFontColor(0.f, 1.f, 0.f);
						}
					}
				}
				else {
					int j = i - (int)IC->InventorySize;
					if (IC->Equipment[j].ID == 0) {
						ItemTexts[0]->GetComponent<GameUI>()->SetText("Empty Equipment Slot");
						ItemTexts[2]->GetComponent<GameUI>()->SetText(ConvertReadable((EquipmentSlot)(j + 1)) + " Slot");
					}
					else {
						ItemTexts[0]->GetComponent<GameUI>()->SetText(IC->Equipment[j].Name);
						ItemTexts[1]->GetComponent<GameUI>()->SetText(IC->Equipment[j].Type);
						ItemTexts[2]->GetComponent<GameUI>()->SetText(ConvertReadable(IC->Equipment[j].EStats.Slot) + " Slot");
						ItemTexts[3]->GetComponent<GameUI>()->SetText("Physical Resistance: " + std::to_string(static_cast<int>(IC->Equipment[j].EStats.Phys_Resist * 100.f)) + "%");
						ItemTexts[4]->GetComponent<GameUI>()->SetText("Stun Resistance: " + std::to_string(static_cast<int>(IC->Equipment[j].EStats.Stun_Resist * 100.f)) + "%");
						ItemTexts[5]->GetComponent<GameUI>()->SetText("Bleed Resistance: " + std::to_string(static_cast<int>(IC->Equipment[j].EStats.Bleed_Resist * 100.f)) + "%");
						ItemTexts[6]->GetComponent<GameUI>()->SetText("Set Effect: " + IC->Equipment[j].EStats.Set + " (" + std::to_string(IC->CountSetItems(IC->Equipment[j].EStats.Set)) + "/4)");
						for (int k = 0; k < 4; ++k) {
							std::string setitemname = MasterObjectList->GetInventoryItem(MasterObjectList->MasterSetEffectList[IC->Equipment[j].EStats.Set].SetIDs[k]).Name;
							ItemTexts[(size_t)7 + (size_t)k]->GetComponent<GameUI>()->SetText(setitemname);
							if (IC->Equipment[k].Name == setitemname) {
								ItemTexts[(size_t)7 + (size_t)k]->GetComponent<GameUI>()->SetFontColor(0.f, 1.f, 0.f);
							}
						}
						std::string setdesc = "(2) ";
						if (MasterObjectList->MasterSetEffectList[IC->Equipment[j].EStats.Set].HP_Bonus_2 > 0) {
							setdesc += "Max HP +";
							setdesc += std::to_string(MasterObjectList->MasterSetEffectList[IC->Equipment[j].EStats.Set].HP_Bonus_2);
						}
						else {
							setdesc += "Max AP +";
							setdesc += std::to_string(static_cast<int>(MasterObjectList->MasterSetEffectList[IC->Equipment[j].EStats.Set].AP_Bonus_2));
						}
						ItemTexts[11]->GetComponent<GameUI>()->SetText(setdesc);
						if (MasterObjectList->MasterSetEffectList[IC->Equipment[j].EStats.Set].HP_Bonus_4 > 0) {
							setdesc = "(4) Max HP +";
							setdesc += std::to_string(MasterObjectList->MasterSetEffectList[IC->Equipment[j].EStats.Set].HP_Bonus_4);
						}
						else {
							setdesc = "(4) Max AP +";
							setdesc += std::to_string(static_cast<int>(MasterObjectList->MasterSetEffectList[IC->Equipment[j].EStats.Set].AP_Bonus_4));
						}
						ItemTexts[12]->GetComponent<GameUI>()->SetText(setdesc);
						if (IC->VerifySetBonus(IC->Equipment[j].EStats.Set) >= 1) {
							ItemTexts[11]->GetComponent<GameUI>()->SetFontColor(0.f, 1.f, 0.f);
						}
						if (IC->VerifySetBonus(IC->Equipment[j].EStats.Set) == 2) {
							ItemTexts[12]->GetComponent<GameUI>()->SetFontColor(0.f, 1.f, 0.f);
						}
					}
				}
				
			}
			++i;
		}

		if (DiscardConfirm && Input::GetMouseTriggered(MouseButton::Button0)) {
			factory->QueueDelete(DiscardConfirmGO);
			DiscardConfirm = false;
		}

		if (DraggedItem.ID != 0 && Input::GetMouseTriggered(MouseButton::Button1)) {
			audio->PlaySfx(30);
			if (DiscardConfirm) {
				DraggedItem.ClearItem();
				factory->QueueDelete(DraggedItemGO);
				factory->QueueDelete(DiscardConfirmGO);
				DiscardConfirm = false;
			}
			else {
				DiscardConfirmGO = factory->AddGameObject("./Data/DiscardItemConfirm.txt");
				factory->QueueAdd(DiscardConfirmGO);
				DiscardConfirm = true;
			}
		}
		
	}

	void End(GameObject* GO, float dt) {
		(void)dt;
		(void)GO;
	}
}