#pragma once
/************************************************************************ /
/*!
\file   Crafting.h
\author Louis Mineo @ Linn Min Htoo
\par    email: louismineo.l@digipen.edu
\date   March 4th, 2023
\brief	Function definitions of the script, Crafting, which aids in the UI
		manipulation of the crafting game mechanic.

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
/*
	Note for my fellow programmers: 
	Thankfully, the crafting ui is contained within a "frame" which is the left
	and right crraftcook UI, so the button presses within the update loop are tracked
	within the "frame" only. 
	If outside, the update loop should not do anything.
	-Louis
*/

#include "Essential.h"
#include "Logic/Script.h"
#include "Logic/Logic.h"
#include "Input/Input.h"
#include "Transform/Transform.h"
#include "GameUI/GameUI.h"
#include "Graphics/GLFWsetup.h"
#include "CoreSys/MasterObjects.h"
#include "HubController.h"

extern bool is_paused;
extern MouseInput MyMouse;
extern bool keystateLM;
extern bool prevloop_keystateLM;
extern bool craftingcooking_open, questui_open;

namespace Crafting
{
	std::string SKey{ "Crafting" };
	std::string current_selected_craftable;
	bool selected_changed;
	bool can_craft;
	bool inventory_full;
	int mouse_x, mouse_y_aft_TRS;
	std::map<std::string, std::string> GO_TO_ItemName;
	std::map<std::string, std::string> ItemName_TO_Req;
	std::map<std::string, std::string> MaterialID_TO_MaterialName;

	std::vector<std::pair<std::string, int>> material_qnt; // used for crafting and etc 
	// CLEAR when ui is closed or confirm yes, or change item
	
	/**************************************************************************/
	/*!
		\brief
			This function fetch the quantity of the specific item if it exists
			in the inventory
		\param item_id
			The id of the item to search for
		\return 
			The quantity of the specific item, in the inventory.
	*/
	/**************************************************************************/
	inline int GetItemQntInInventory(int item_id)
	{
		int total = 0;

		for (auto x : Engine->GetObjectByPrefix("Player")->GetComponent<Inventory>()->Invent)
		{
			if (x.ID == item_id)
			{
				if (x.Type == "Material") {
					total += x.Quantity;
				}
				else {
					total++;
				}
			}
		}

		return total;
	}

	/**************************************************************************/
	/*!
		\brief
			This function removes the quantity of the specific item if it exists
			in the inventory
	*/
	/**************************************************************************/
	inline void RemoveCraftingMaterialsInInventory()
	{
		for (auto x : material_qnt)
		{
			int count = x.second;

			while (count != 0)
			{
				// get the index of the item in the array
				Inventory* inv_obj = Engine->GetObjectByPrefix("Player")->GetComponent<Inventory>();
				size_t index;
				for (index = inv_obj->InventorySize - 1; index >= 0; index--)
				{
					if (inv_obj->Invent[index].Name == x.first) break;
				}
				// remove item via id
				if (inv_obj->Invent[index].Type == "Material") {
					//This function assumes you have the required items already
					inv_obj->Invent[index].Quantity -= count;
					count = 0;
					if (inv_obj->Invent[index].Quantity < 1) {
						inv_obj->RemoveItem(index);
					}
				}
				else {
					inv_obj->RemoveItem(index);
					count--;
				}
			}
		}
	}

	/**************************************************************************/
	/*!
		\brief
			This function will make all cooking and crafting GameUI Objects
			invisible and unable to be used
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

		Crafting::current_selected_craftable = "";

		Engine->GetGameObject("CraftCookQuest CloseBtn")->GetComponent<Renderer>()->SetVisibility(false);

		// make the bag and pause button visible
		Engine->GetGameObject("Bag Icon")->GetComponent<Renderer>()->SetVisibility(true);
		Engine->GetGameObject("Play/Pause Button")->GetComponent<Renderer>()->SetVisibility(true);
	}

	/**************************************************************************/
	/*!
		\brief
			This function will compile the info of the item and display
			it in a nice viewable format
		\param item
			The specific item to get info from
	*/
	/**************************************************************************/
	inline std::string CompileItemDesc(InventoryItem item)
	{
		std::string total_str = "DESCRIPTION:\n";

		if (item.Type == "Weapon")
		{

			total_str += ("Lore: " + item.Lore + "\n");
			total_str += ("Weapon Type: " + item.WStats.WeaponType + "\n");
			total_str += ("Status Effect: " + item.WStats.WeaponStatus + "\n");
			total_str += ("Damage: " + std::to_string(static_cast<int>(item.WStats.Damage)) + "\n");
			total_str += ("Crit Chance: " + std::to_string(static_cast<int>(item.WStats.CritChance * 100.f)) + "\n");
			total_str += ("Accuracy: " + std::to_string(static_cast<int>(item.WStats.Accuracy * 100.f)) + "\n");
			total_str += ("AP Cost: " + std::to_string(item.WStats.APCost));
		}
		else if (item.Type == "Equipment")
		{
			total_str += ("Lore: " + item.Lore + "\n");
			total_str += ("Slot: " + ConvertReadable(item.EStats.Slot) + "\n");
			total_str += ("Physical Resistance: " + std::to_string(static_cast<int>(item.EStats.Phys_Resist * 100.f)) + "%\n");
			total_str += ("Stun Resistance: " + std::to_string(static_cast<int>(item.EStats.Stun_Resist * 100.f)) + "%\n");
			total_str += ("Bleed Resistance: " + std::to_string(static_cast<int>(item.EStats.Bleed_Resist * 100.f)) + "%\n");
		}

		return total_str;
	}

	/**************************************************************************/
	/*!
		\brief
			This function will compile the crafting requirement info of the 
			item and display it in a nice viewable format
		\param item
			The specific item to get info from
	*/
	/**************************************************************************/
	inline std::string CompileItemReq(InventoryItem item)
	{
		std::string req_str = ItemName_TO_Req[item.Name];
		//splitting the requirements
		std::vector<std::string> materials;
		char* token = strtok(const_cast<char*>(req_str.c_str()), "_");
		while (token != nullptr)
		{
			materials.push_back(std::string(token));
			token = strtok(nullptr, "_");
		}


		
		// split them into a pair, material name and qty needed
		for (auto& str : materials)
		{

			std::string mat_name = MaterialID_TO_MaterialName[strtok(const_cast<char*>(str.c_str()),"x")];
			int qty = atoi(strtok(NULL,"X"));

			material_qnt.push_back(std::make_pair(mat_name,qty ));
		}
		
		
		std::string total_str = "MATERIAL REQUIREMENTS:\n";
		
		can_craft = true; // set as true first
		// find if the appropriate material is in the inventory.
		for (auto& pair : material_qnt)
		{
			//std::cout << pair.first << " --- " << pair.second << std::endl;
			//find the material in the bag and get its qty
			int qty = GetItemQntInInventory(MasterObjectList->GetIDbyInventoryName(pair.first));//10;// 0 for now

			total_str += std::string(pair.first + ":   " + std::to_string(qty) + " out of " + std::to_string(pair.second) + "\n");
		
			if (qty < pair.second) can_craft = false; // means 
		}

		return total_str;
	}

	/**************************************************************************/
	/*!
		\brief
			This is the update loop separately, and specifically just
			for the left crafting UI
		\param GO
			The specific Gameobject who is the calling and is affected by
			this script function
	*/
	/**************************************************************************/
	inline void Update_CraftingLeftUI(GameObject* GO, float dt)
	{
		UNREFERENCED_PARAMETER(GO);
		UNREFERENCED_PARAMETER(dt);

		Renderer* self_rdr = dynamic_cast<Renderer*>(GO->GetComponent(ComponentType::Renderer));
		// if the button is not active or visible , skip
		if (self_rdr->GetVisibility() == false) return;

		GameUI* self_GUI = dynamic_cast<GameUI*>(GO->GetComponent(ComponentType::GameUI));

		//only if the mouse is within the ui, then we go into the update loop
		if ((mouse_x > self_GUI->AA.x && mouse_x < self_GUI->BB.x)
			&& (mouse_y_aft_TRS > self_GUI->AA.y && mouse_y_aft_TRS < self_GUI->BB.y))
		{
			int total = 14; // currently theres only 14 items

			for (int i = 1; i <= total; i++)
			{
				int index = (i > 6) ? i - 6 : i;
				std::string string = (i > 6) ? "CraftCook BtmItem " : "CraftCook TopItem ";
				std::string goname = string + std::to_string(index);
				if (//dynamic_cast<Renderer*>(Engine->GetGameObject(goname)->GetComponent(ComponentType::Renderer))->GetVisibility() &&
					Input::GetMouseTriggeredDepth(MouseButton::Button0, Engine->GetGameObject(goname)))
				{
					Crafting::material_qnt.clear();
					//std::cout << goname << " pressed" << std::endl;

					if (current_selected_craftable != GO_TO_ItemName[goname])
					{
						selected_changed = true;
						can_craft = false;
						current_selected_craftable = GO_TO_ItemName[goname];
						
						//set selected ID to active
						dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook_Selected Name")->GetComponent(ComponentType::GameUI))->SetFontVisibility(true);
						//set Icon to active
						dynamic_cast<Renderer*>(Engine->GetGameObject("CraftCook_Selected Icon")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
						//set selected info to active
						dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook_Selected Desc")->GetComponent(ComponentType::GameUI))->SetFontVisibility(true);
						//set selected requirements to active
						dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook_Selected Req")->GetComponent(ComponentType::GameUI))->SetFontVisibility(true);
						//set selected status to active
						dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook_Selected Status")->GetComponent(ComponentType::GameUI))->SetFontVisibility(true);
						//set selected craft btn to active
						dynamic_cast<Renderer*>(Engine->GetGameObject("CraftCook_Selected CraftBtn")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
						dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook_Selected CraftBtn")->GetComponent(ComponentType::GameUI))->SetFontVisibility(true);
						dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook_Selected CraftBtn")->GetComponent(ComponentType::GameUI))->SetText("Craft!");

						

						audio->PlaySfx(30);


					}
					break;
				}


			}


		}
	}

	/**************************************************************************/
	/*!
		\brief
			This is the update loop separately, and specifically just 
			for the right crafting UI
		\param GO
			The specific Gameobject who is the calling and is affected by
			this script function
	*/
	/**************************************************************************/
	inline void Update_CraftingRightUI(GameObject* GO, float dt)
	{
		UNREFERENCED_PARAMETER(GO);
		UNREFERENCED_PARAMETER(dt);

		if (selected_changed)
		{
			InventoryItem selected_item = MasterObjectList->GetInventoryItem( MasterObjectList->GetIDbyInventoryName(current_selected_craftable));


			//set the weapon ID as the selected ID
			dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook_Selected Name")->GetComponent(ComponentType::GameUI))->SetText(current_selected_craftable);

			//change the texture on the icon GO "CraftCook_Selected Icon"
			dynamic_cast<Renderer*>(Engine->GetGameObject("CraftCook_Selected Icon")->GetComponent(ComponentType::Renderer))
				->SetTexHandle(selected_item.TexHandle);
			dynamic_cast<Renderer*>(Engine->GetGameObject("CraftCook_Selected Icon")->GetComponent(ComponentType::Renderer))
				->SetUVMin(selected_item.Tex_UV_Min_COORD);
			dynamic_cast<Renderer*>(Engine->GetGameObject("CraftCook_Selected Icon")->GetComponent(ComponentType::Renderer))
				->SetUVMax(selected_item.Tex_UV_Max_COORD);

			//change the selected info
			dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook_Selected Desc")->GetComponent(ComponentType::GameUI))->SetText(CompileItemDesc(selected_item));

			
			//change selected requirements
			dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook_Selected Req")->GetComponent(ComponentType::GameUI))->SetText(CompileItemReq(selected_item));

			inventory_full = Engine->GetObjectByPrefix("Player")->GetComponent<Inventory>()->GetFullInventory();
			if (inventory_full) {
				//Check if a slot would be freed if you crafted
				for (auto x : material_qnt) {
					//int count = x.second;
					Inventory* inv = Engine->GetObjectByPrefix("Player")->GetComponent<Inventory>();
					for (auto y : inv->Invent) {
						if (y.Name == x.first) {
							if (y.Quantity == x.second) {
								inventory_full = false;
								break;
							}
						}
					}
					if (!inventory_full) break;
				}
			}
			//change selected status
			std::string status = (can_craft ? "   You can craft this item" : "    Not Enough Materials");
			if (can_craft)
			{
				//make the button white
				dynamic_cast<Renderer*>(Engine->GetGameObject("CraftCook_Selected CraftBtn")->GetComponent(ComponentType::Renderer))->Alpha_0to1 = 1.f;
				dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook_Selected CraftBtn")->GetComponent(ComponentType::GameUI))->Alpha = 1.f;

			}
			else
			{
				//make the button grey
				dynamic_cast<Renderer*>(Engine->GetGameObject("CraftCook_Selected CraftBtn")->GetComponent(ComponentType::Renderer))->Alpha_0to1 = 0.5f;
				dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook_Selected CraftBtn")->GetComponent(ComponentType::GameUI))->Alpha = 0.5f;
			}

			if (can_craft && inventory_full) {
				status = "          Inventory full!";
				dynamic_cast<Renderer*>(Engine->GetGameObject("CraftCook_Selected CraftBtn")->GetComponent(ComponentType::Renderer))->Alpha_0to1 = 0.5f;
				dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook_Selected CraftBtn")->GetComponent(ComponentType::GameUI))->Alpha = 0.5f;
			}
			Vec3 clr = (can_craft && !inventory_full ? Vec3{0.f,1.0f,0.f} : Vec3{ 1.f,0.0f,0.f });
			dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook_Selected Status")->GetComponent(ComponentType::GameUI))->SetText(status);
			dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook_Selected Status")->GetComponent(ComponentType::GameUI))->SetFontColor(clr.x, clr.y, clr.z);

			
			

			selected_changed = false;
		}

		

		GameUI* CraftBtn_GUI = dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook_Selected CraftBtn")->GetComponent(ComponentType::GameUI));

		//only if the mouse is within the ui, then we go into the update loop
		if ((mouse_x > CraftBtn_GUI->AA.x && mouse_x < CraftBtn_GUI->BB.x)
			&& (mouse_y_aft_TRS > CraftBtn_GUI->AA.y && mouse_y_aft_TRS < CraftBtn_GUI->BB.y))
		{
			if (Input::GetMouseTriggeredDepth(MouseButton::Button0, Engine->GetGameObject("CraftCook_Selected CraftBtn")))
			{
				std::cout << "Craft Btn Pressed" << std::endl;
				if (can_craft && !inventory_full)
				{
					audio->PlaySfx(30);

					(Engine->GetGameObject("CraftCook Confirmation")->GetComponent<LogicComponent>()->SetEnabled("Crafting", true));
					(Engine->GetGameObject("CraftCook Confirmation")->GetComponent<LogicComponent>()->SetEnabled("Cooking", false));

					// activate the confirmation
					dynamic_cast<Renderer*>(Engine->GetGameObject("CraftCook Confirmation")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
					dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook Confirmation")->GetComponent(ComponentType::GameUI))->SetText("Confirm Craft?");
					dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook Confirmation")->GetComponent(ComponentType::GameUI))->SetFontVisibility(true);
					dynamic_cast<Renderer*>(Engine->GetGameObject("CraftCook Confirmation Yes")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
					dynamic_cast<Renderer*>(Engine->GetGameObject("CraftCook Confirmation No")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
				}
			}
		}
		
	}

	/**************************************************************************/
	/*!
		\brief
			This is the update loop separately, and specifically just
			for the confirmation UI after clicking on the craft button
		\param GO
			The specific Gameobject who is the calling and is affected by
			this script function
	*/
	/**************************************************************************/
	inline void Update_CraftingConfirmation(GameObject* GO, float dt)
	{
		UNREFERENCED_PARAMETER(dt);
		
		if (dynamic_cast<Renderer*>(GO->GetComponent(ComponentType::Renderer))->GetVisibility()
			&& dynamic_cast<Renderer*>(Engine->GetGameObject("CraftCook Confirmation Yes")->GetComponent(ComponentType::Renderer))->GetVisibility()
			&& dynamic_cast<Renderer*>(Engine->GetGameObject("CraftCook Confirmation No")->GetComponent(ComponentType::Renderer))->GetVisibility())
		{
			GameUI* self_GUI = dynamic_cast<GameUI*>(GO->GetComponent(ComponentType::GameUI));

			if ((mouse_x > self_GUI->AA.x && mouse_x < self_GUI->BB.x)
				&& (mouse_y_aft_TRS > self_GUI->AA.y && mouse_y_aft_TRS < self_GUI->BB.y))
			{
				if (Input::GetMouseTriggeredDepth(MouseButton::Button0, Engine->GetGameObject("CraftCook Confirmation Yes")))
				{
					std::cout << "CRAFT SUCCESS!" << std::endl;

					audio->PlaySfx(27);
					// play the sfx for crafting success audio, id 29

					
					// remove the crafting materials from the inventory
					RemoveCraftingMaterialsInInventory();
					
					// add the crafted item into the inventry
					Engine->GetObjectByPrefix("Player")->GetComponent<Inventory>()->AddItem(MasterObjectList->GetIDbyInventoryName(current_selected_craftable));

					GameObject* CraftingVFX = factory->AddGameObject("./Data/CraftingResult.txt");
					CraftingVFX->GetComponent<Renderer>()->SetTexHandle(MasterObjectList->GetInventoryItem(MasterObjectList->GetIDbyInventoryName(current_selected_craftable)).TexHandle);
					CraftingVFX->GetComponent<Renderer>()->SetUVMin(MasterObjectList->GetInventoryItem(MasterObjectList->GetIDbyInventoryName(current_selected_craftable)).Tex_UV_Min_COORD);
					CraftingVFX->GetComponent<Renderer>()->SetUVMax(MasterObjectList->GetInventoryItem(MasterObjectList->GetIDbyInventoryName(current_selected_craftable)).Tex_UV_Max_COORD);
					factory->QueueAdd(CraftingVFX);

					Engine->GetGameObject("CraftCookQuest CloseBtn")->GetComponent<LogicComponent>()->SetEnabled("Crafting", false);
					Engine->GetGameObject("CookingCraftingUIRight")->GetComponent<LogicComponent>()->SetEnabled("Crafting", false);
					Engine->GetGameObject("CraftCook Confirmation")->GetComponent<LogicComponent>()->SetEnabled("Crafting", false);
					craftingcooking_open = false;
					material_qnt.clear();
					MakeINVISIBLECookCraftUI();

				}

				if (Input::GetMouseTriggeredDepth(MouseButton::Button0, Engine->GetGameObject("CraftCook Confirmation No")))
				{
					audio->PlaySfx(30);
					dynamic_cast<Renderer*>(Engine->GetGameObject("CraftCook Confirmation")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
					dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook Confirmation")->GetComponent(ComponentType::GameUI))->SetFontVisibility(false);
					dynamic_cast<Renderer*>(Engine->GetGameObject("CraftCook Confirmation Yes")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
					dynamic_cast<Renderer*>(Engine->GetGameObject("CraftCook Confirmation No")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
					//disable scripts
					Engine->GetGameObject("CraftCook Confirmation")->GetComponent<LogicComponent>()->SetEnabled("Crafting", false);
					

				}
			}
		}
	}
	/**************************************************************************/
	/*!
		\brief
			This function is the update loop for the close btn on the crafting
			UI when open
		\param GO
			The specific Gameobject who is the calling and is affected by
			this script function
		\param dt
			delta time
	*/
	/**************************************************************************/
	inline void Update_CloseUI(GameObject* GO, float dt)
	{
		(void)GO;
		(void)dt;
		if (Input::GetMouseTriggeredDepth(MouseButton::Button0, Engine->GetGameObject("CraftCookQuest CloseBtn")))
		{
			MakeINVISIBLECookCraftUI();
			craftingcooking_open = false;
			Engine->GetGameObject("CookingCraftingUILeft")->GetComponent<LogicComponent>()->SetEnabled("Crafting", false);
			Engine->GetGameObject("CookingCraftingUIRight")->GetComponent<LogicComponent>()->SetEnabled("Crafting", false);
			Engine->GetGameObject("CraftCookQuest CloseBtn")->GetComponent<LogicComponent>()->SetEnabled("Crafting", false);
			Engine->GetGameObject("CraftCook Confirmation")->GetComponent<LogicComponent>()->SetEnabled("Crafting", false);
		}
	}

	/**************************************************************************/
	/*!
		\brief
			To populate the maps with GO name to Item Name
			(Temporary implementation while materials are being added into the
			master object list)
	*/
	/**************************************************************************/
	inline void MapGONameToItemName()
	{
		GO_TO_ItemName.insert(std::pair<std::string, std::string >("CraftCook TopItem 1", "Makeshift Flamethrower"));
		GO_TO_ItemName.insert(std::pair<std::string, std::string >("CraftCook TopItem 2", "Police Taser"));
		GO_TO_ItemName.insert(std::pair<std::string, std::string >("CraftCook TopItem 3", "Homemade Spear"));
		GO_TO_ItemName.insert(std::pair<std::string, std::string >("CraftCook TopItem 4", "SAR21"));
		GO_TO_ItemName.insert(std::pair<std::string, std::string >("CraftCook TopItem 5", "Parang"));

		GO_TO_ItemName.insert(std::pair<std::string, std::string >("CraftCook BtmItem 1", "Durian Helmet"));
		GO_TO_ItemName.insert(std::pair<std::string, std::string >("CraftCook BtmItem 2", "Padded Jacket"));
		GO_TO_ItemName.insert(std::pair<std::string, std::string >("CraftCook BtmItem 3", "Running Shoes"));
		GO_TO_ItemName.insert(std::pair<std::string, std::string >("CraftCook BtmItem 4", "Cut-Resistant Gloves"));
		GO_TO_ItemName.insert(std::pair<std::string, std::string >("CraftCook BtmItem 5", "Army Helmet"));
		GO_TO_ItemName.insert(std::pair<std::string, std::string >("CraftCook BtmItem 6", "ILBV"));
		GO_TO_ItemName.insert(std::pair<std::string, std::string >("CraftCook BtmItem 7", "Combat Boots"));
		GO_TO_ItemName.insert(std::pair<std::string, std::string >("CraftCook BtmItem 8", "Tactical Leather Combat Gloves"));
	}


	/**************************************************************************/
	/*!
		\brief
			To populate the maps with Item Names to crafting material requirements
			(Temporary implementation while materials are being added into the
			master object list)
	*/
	/**************************************************************************/
	inline void MapItemNameToCraftRequirements()
	{
		
		ItemName_TO_Req.insert(std::pair<std::string, std::string >("Parang","MAT001x3"));
		ItemName_TO_Req.insert(std::pair<std::string, std::string >("Makeshift Flamethrower","MAT001x3"));
		ItemName_TO_Req.insert(std::pair<std::string, std::string >("Police Taser",  "MAT001x4_MAT002x3"));
		ItemName_TO_Req.insert(std::pair<std::string, std::string >("Homemade Spear", "MAT001x4_MAT002x3"));
		ItemName_TO_Req.insert(std::pair<std::string, std::string >("SAR21", "MAT001x5_MAT002x3_MAT003x1"));
		

		
		ItemName_TO_Req.insert(std::pair<std::string, std::string >("Durian Helmet", "MAT001x3_MAT002x3"));
		ItemName_TO_Req.insert(std::pair<std::string, std::string >("Padded Jacket", "MAT001x2_MAT002x4"));
		ItemName_TO_Req.insert(std::pair<std::string, std::string >("Running Shoes", "MAT001x1_MAT002x2"));
		ItemName_TO_Req.insert(std::pair<std::string, std::string >("Cut-Resistant Gloves", "MAT001x3_MAT002x3"));

		ItemName_TO_Req.insert(std::pair<std::string, std::string >("Army Helmet", "MAT001x6_MAT004x4_MAT008x1"));
		ItemName_TO_Req.insert(std::pair<std::string, std::string >("ILBV", "MAT001x5_MAT004x5_MAT008x1"));
		ItemName_TO_Req.insert(std::pair<std::string, std::string >("Combat Boots", "MAT002x5_MAT004x5_MAT008x1"));
		ItemName_TO_Req.insert(std::pair<std::string, std::string >("Tactical Leather Combat Gloves", "MAT002x6_MAT004x4_MAT008x1"));
		
	}


	/**************************************************************************/
	/*!
		\brief
			To populate the maps with Material IDs to the Material Names
			(Temporary implementation while materials are being added into the 
			master object list)
	*/
	/**************************************************************************/
	inline void MapMATIDToMatName()
	{
		MaterialID_TO_MaterialName.insert(std::pair<std::string, std::string >("MAT001","Metal Pieces"));
		MaterialID_TO_MaterialName.insert(std::pair<std::string, std::string >("MAT002", "Used Plastic"));
		MaterialID_TO_MaterialName.insert(std::pair<std::string, std::string >("MAT003", "Gasoline"));
		MaterialID_TO_MaterialName.insert(std::pair<std::string, std::string >("MAT004", "Hardened Bones"));
		MaterialID_TO_MaterialName.insert(std::pair<std::string, std::string >("MAT005", "Yeast"));
		MaterialID_TO_MaterialName.insert(std::pair<std::string, std::string >("MAT006", "Irridated Water"));
		MaterialID_TO_MaterialName.insert(std::pair<std::string, std::string >("MAT007", "Porkchop"));
		MaterialID_TO_MaterialName.insert(std::pair<std::string, std::string >("MAT008", "Camp Pass"));

	}



	/**************************************************************************/
	/*!
		\brief
			To initialize the Textures of the Crafting UI when opening
	*/
	/**************************************************************************/
	inline void InitGOTex()
	{
		for (auto& x : GO_TO_ItemName)
		{			
			InventoryItem _item = MasterObjectList->GetInventoryItem(MasterObjectList->GetIDbyInventoryName(x.second));

			Renderer* rdr = dynamic_cast<Renderer*>(Engine->GetGameObject(x.first)->GetComponent(ComponentType::Renderer));
			if(rdr != nullptr)
			// change the texhandle
			rdr->SetTexHandle(_item.TexHandle);
			// change the tex min
			rdr->SetUVMin(_item.Tex_UV_Min_COORD);
			// change the tex max
			rdr->SetUVMax(_item.Tex_UV_Max_COORD);
		}
	}

	/**************************************************************************/
	/*!
		\brief
			Initializer for the Crafting script. It is called for every
			GO that has this script attached to it. As of this implementation,
			it is only the CraftCook left UI and CraftCook right UI.
		\param GO
			The specific Gameobject who is the calling and is affected by
			this script function
		\param dt
			delta_time
	*/
	/**************************************************************************/
	void Init(GameObject* GO, float dt)
	{
		mouse_x = Input::mouseObj.x;
		mouse_y_aft_TRS = GLSetup::current_height - Input::mouseObj.y;


		UNREFERENCED_PARAMETER(GO);
		UNREFERENCED_PARAMETER(dt);
		selected_changed = false;
		inventory_full = false;
		
		MapGONameToItemName();
		MapItemNameToCraftRequirements();
		MapMATIDToMatName();
		InitGOTex();
	}
	/**************************************************************************/
	/*!
		\brief
			Update for the Crafting script. It is called for the crafting left
			and crafting right UI GOs
		\param GO
			The specific Gameobject who is the calling and is affected by
			this script function
		\param dt
			delta_time
	*/
	/**************************************************************************/
	void Update(GameObject* GO, float dt)
	{
		mouse_x = Input::mouseObj.x;
		mouse_y_aft_TRS = GLSetup::current_height - Input::mouseObj.y;

		//std::cout << "Crafting::update running for " << GO->GetName() << std::endl;
		if (GO->GetName() == "CookingCraftingUILeft") Update_CraftingLeftUI(GO, dt);
		else if (GO->GetName() == "CookingCraftingUIRight") Update_CraftingRightUI(GO, dt);
		else if (GO->GetName() == "CraftCook Confirmation") Update_CraftingConfirmation(GO, dt);
		else if (GO->GetName() == "CraftCookQuest CloseBtn") Update_CloseUI(GO, dt);
	}
	/**************************************************************************/
	/*!
		\brief
			End for the Crafting script. It is called for the crafting 
			left and crafting right UI GOs
		\param GO
			The specific Gameobject who is the calling and is affected by
			this script function
		\param dt
			delta_time
	*/
	/**************************************************************************/
	void End(GameObject* GO, float dt)
	{
		UNREFERENCED_PARAMETER(GO);
		UNREFERENCED_PARAMETER(dt);
	}

}