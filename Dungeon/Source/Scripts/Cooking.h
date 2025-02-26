/************************************************************************ /
/*!
\file   Cooking.h
\author Louis Mineo @ Linn Min Htoo
\par    email: louismineo.l@digipen.edu
\date   March 4th, 2023
\brief	Function definitions of the script, Cooking, which aids in the UI
		manipulation cooking minigame mechanic.

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/


#pragma once
#include "Essential.h"
#include "Logic/Script.h"
#include "Logic/Logic.h"
#include "Input/Input.h"
#include "Transform/Transform.h"
#include "GameUI/GameUI.h"
#include "Graphics/GLFWsetup.h"
#include "CoreSys/MasterObjects.h"
#include "HubController.h"

extern bool craftingcooking_open;

namespace Cooking
{
	std::string SKey{ "Cooking" };

	int mouse_x, mouse_y_aft_TRS;
	std::string current_selected_cookable;
	bool selected_changed;
	bool inventory_full;
	bool can_cook;
	float slider_time;
	std::map <std::string, std::string> GO_TO_ItemName;
	std::map<std::string, std::string> ItemName_TO_Req;
	std::map<std::string, std::string> MaterialID_TO_MaterialName;
	std::vector<std::pair<std::string, int>> material_qnt;

	std::map <std::string, int> ItemName_TO_PointerSpeed;

	/**************************************************************************/
	/*!
		\brief
			This function helps to split the input string by ":" as existent
			in the lore string of the cooking item, and get the lore string
		\param input_txt
			lore string of the cooking item
		\return
			the lore string
	*/
	/**************************************************************************/
	inline std::string GetLorestringfromLore(std::string input_txt)
	{
		std::string ret;

		size_t id = input_txt.find_first_of(":");
		ret = input_txt.substr(0,id);

		return ret;
	}
	/**************************************************************************/
	/*!
		\brief
			This function helps to split the input string by ":" as existent
			in the lore string of the cooking item, and get the effect text
			string
		\param input_txt
			lore string of the cooking item
		\return
			the effect text string
	*/
	/**************************************************************************/
	inline std::string GetEffectStringfromLore(std::string input_txt)
	{
		std::string ret;

		size_t id = input_txt.find_first_of(":");
		size_t id2 = input_txt.find_first_of(";");
		ret = input_txt.substr(id+1,id2);

		return ret;
	}

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
	inline void RemoveCookingMaterialsInInventory()
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

			std::string mat_name = MaterialID_TO_MaterialName[strtok(const_cast<char*>(str.c_str()), "x")];
			int qty = atoi(strtok(NULL, "X"));

			material_qnt.push_back(std::make_pair(mat_name, qty));
		}

		

		std::string total_str = "MATERIAL REQUIREMENTS:\n";

		can_cook = true; // set as true first
		// find if the appropriate material is in the inventory.
		for (auto& pair : material_qnt)
		{
			//std::cout << pair.first << " --- " << pair.second << std::endl;
			//find the material in the bag and get its qty
			int qty = GetItemQntInInventory(MasterObjectList->GetIDbyInventoryName(pair.first));//10;// 0 for now

			total_str += std::string(pair.first + ":   " + std::to_string(qty) + " out of " + std::to_string(pair.second) + "\n");

			if (qty < pair.second) can_cook = false; // means 
		}

		return total_str;
	}
	/**************************************************************************/
	/*!
		\brief
			This function will hide all the cooking and crafting ui gameobjects
			as it returns to the hub space
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

		Crafting::current_selected_craftable = "";
		Crafting::material_qnt.clear();

		Cooking::current_selected_cookable = "";
		Cooking::material_qnt.clear();
	}
	/**************************************************************************/
	/*!
		\brief
			This function initialises the data need for cooking specific 
			food items
	*/
	/**************************************************************************/
	inline void InitGOsToItem()
	{
		GO_TO_ItemName.insert(std::make_pair("Cooked Food 1", "Irridated Bread"));
		GO_TO_ItemName.insert(std::make_pair("Cooked Food 2", "Bakolo"));
		GO_TO_ItemName.insert(std::make_pair("Cooked Food 3", "Cooked Porkchop"));

		ItemName_TO_Req.insert(std::pair<std::string, std::string >("Irridated Bread", "MAT005x1_MAT006x2"));
		ItemName_TO_Req.insert(std::pair<std::string, std::string >("Bakolo", "MAT005x2_MAT007x1"));
		ItemName_TO_Req.insert(std::pair<std::string, std::string >("Cooked Porkchop", "MAT006x1_MAT007x2"));
		

		MaterialID_TO_MaterialName.insert(std::pair<std::string, std::string >("MAT001", "Metal Pieces"));
		MaterialID_TO_MaterialName.insert(std::pair<std::string, std::string >("MAT002", "Used Plastic"));
		MaterialID_TO_MaterialName.insert(std::pair<std::string, std::string >("MAT003", "Gasoline"));
		MaterialID_TO_MaterialName.insert(std::pair<std::string, std::string >("MAT004", "Hardened Bones"));
		MaterialID_TO_MaterialName.insert(std::pair<std::string, std::string >("MAT005", "Yeast"));
		MaterialID_TO_MaterialName.insert(std::pair<std::string, std::string >("MAT006", "Irridated Water"));
		MaterialID_TO_MaterialName.insert(std::pair<std::string, std::string >("MAT007", "Porkchop"));
		MaterialID_TO_MaterialName.insert(std::pair<std::string, std::string >("MAT008", "Camp Pass"));

		/*
		ItemName_TO_PointerSpeed.insert(std::make_pair("Irridated Bread",3));
		ItemName_TO_PointerSpeed.insert(std::make_pair("Bakolo",10));
		ItemName_TO_PointerSpeed.insert(std::make_pair("Cooked Porkchop",7));
		*/
	}
	/**************************************************************************/
	/*!
		\brief
			This function creates the cookable item description string from
			the cookable inventory item itself
		\param item
			The cookable item
		\return
			the description string
	*/
	/**************************************************************************/
	inline std::string make_cookable_desc(InventoryItem item)
	{
		std::string return_str;
		return_str = "DESCRIPTION:\n";
		return_str += "Lore: " + GetLorestringfromLore(item.Lore) +"\n";
		//std::cout << GetEffectStringfromLore(item.Lore) << std::endl;
		std::cout << return_str << std::endl;
		return_str += "Effect: " + GetEffectStringfromLore(item.Lore) ;
		std::cout << return_str << std::endl;
		return return_str;
		/*
		if (ConvertReadable(item.CStats.Effect) == "Damage Boost")
		{
			//return_str += "Effect: " + ConvertReadable(item.CStats.Effect)  +" by " + std::to_string(static_cast<int>(item.CStats.EffectValue)) + " points for " + std::to_string(static_cast<int>(item.CStats.Duration));
			
		}
		else if (ConvertReadable(item.CStats.Effect) == "Instant Heal")
		{
			return_str += "Effect: " + ConvertReadable(item.CStats.Effect) + " by " + std::to_string(static_cast<int>(item.CStats.EffectValue)) + " HP";
		}
		return return_str;
		*/
		

	}
	/**************************************************************************/
	/*!
		\brief
			This function creates the cookable item description string from
			the cookable inventory item itself
		\param item
			The cookable item
		\return
			the description string
	*/
	/**************************************************************************/
	inline void Update_CookingLeftUI(GameObject* GO, float dt)
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
			int total = 3; // currently theres only 14 items

			for (int i = 1; i <= total; i++)
			{
				std::string string = "Cooked Food ";
				std::string goname = string + std::to_string(i);
				if (Input::GetMouseTriggeredDepth(MouseButton::Button0, Engine->GetGameObject(goname)))
				{
					Cooking::material_qnt.clear();
					if(current_selected_cookable != GO_TO_ItemName[goname])
					{
						selected_changed = true;
						current_selected_cookable = GO_TO_ItemName[goname];

						//set selected ID to active
						Engine->GetGameObject("CraftCook_Selected Name")->GetComponent<GameUI>()->SetFontVisibility(true);
						//set Icon to active
						Engine->GetGameObject("CraftCook_Selected Icon")->GetComponent<Renderer>()->SetVisibility(true);
						//set selected info to active
						Engine->GetGameObject("CraftCook_Selected Desc")->GetComponent<GameUI>()->SetFontVisibility(true);

						Engine->GetGameObject("CraftCook_Selected Req")->GetComponent<GameUI>()->SetFontVisibility(true);
						
						Engine->GetGameObject("CraftCook_Selected Status")->GetComponent<Renderer>()->SetVisibility(true);
						Engine->GetGameObject("CraftCook_Selected Status")->GetComponent<GameUI>()->SetFontVisibility(true);

						Engine->GetGameObject("CraftCook_Selected CraftBtn")->GetComponent<Renderer>()->SetVisibility(true);
						Engine->GetGameObject("CraftCook_Selected CraftBtn")->GetComponent<GameUI>()->SetFontVisibility(true);
						Engine->GetGameObject("CraftCook_Selected CraftBtn")->GetComponent<GameUI>()->SetText(" Cook");
					}
				}
			}
		}

	}
	/**************************************************************************/
	/*!
		\brief
			this the update function of the GameObject for the updating of the 
			right menu ui for cooking
		\param GO
			The specific Gameobject who is the calling and is affected by
			this script function
		\param dt
			delta_time
	*/
	/**************************************************************************/
	inline void Update_CookingRightUI(GameObject* GO, float dt)
	{
		UNREFERENCED_PARAMETER(GO);
		UNREFERENCED_PARAMETER(dt);

		if (selected_changed)
		{
			InventoryItem selected_item = MasterObjectList->GetInventoryItem(MasterObjectList->GetIDbyInventoryName(current_selected_cookable));


			//set the weapon ID as the selected ID
			Engine->GetGameObject("CraftCook_Selected Name")->GetComponent<GameUI>()->SetText(selected_item.Name);

			//change the texture on the icon GO "CraftCook_Selected Icon"
			Engine->GetGameObject("CraftCook_Selected Icon")->GetComponent<Renderer>()
				->SetTexHandle(selected_item.TexHandle);
			Engine->GetGameObject("CraftCook_Selected Icon")->GetComponent<Renderer>()
				->SetUVMin(selected_item.Tex_UV_Min_COORD);
			Engine->GetGameObject("CraftCook_Selected Icon")->GetComponent<Renderer>()
				->SetUVMax(selected_item.Tex_UV_Max_COORD);

			//change the selected info	
			(Engine->GetGameObject("CraftCook_Selected Desc")->GetComponent<GameUI>())->SetText(make_cookable_desc(selected_item));

			//change the selected info
			(Engine->GetGameObject("CraftCook_Selected Req")->GetComponent<GameUI>())->SetText(CompileItemReq(selected_item));

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
			std::string status = (can_cook ? "   You can cook this item" : "    Not Enough Materials");
			if (can_cook)
			{
				//make the button white
				Engine->GetGameObject("CraftCook_Selected CraftBtn")->GetComponent<Renderer>()->Alpha_0to1 = 1.f;
				Engine->GetGameObject("CraftCook_Selected CraftBtn")->GetComponent<GameUI>()->Alpha = 1.f;

			}
			else
			{
				//make the button grey
				Engine->GetGameObject("CraftCook_Selected CraftBtn")->GetComponent<Renderer>()->Alpha_0to1 = 0.5f;
				Engine->GetGameObject("CraftCook_Selected CraftBtn")->GetComponent<GameUI>()->Alpha = 0.5f;
			}

			if (can_cook && inventory_full) {
				status = "          Inventory full!";
				(Engine->GetGameObject("CraftCook_Selected CraftBtn")->GetComponent<Renderer>())->Alpha_0to1 = 0.5f;
				(Engine->GetGameObject("CraftCook_Selected CraftBtn")->GetComponent<GameUI>())->Alpha = 0.5f;
			}
			Vec3 clr = (can_cook && !inventory_full ? Vec3{ 0.f,1.0f,0.f } : Vec3{ 1.f,0.0f,0.f });
			(Engine->GetGameObject("CraftCook_Selected Status")->GetComponent<GameUI>())->SetText(status);
			(Engine->GetGameObject("CraftCook_Selected Status")->GetComponent<GameUI>())->SetFontColor(clr.x, clr.y, clr.z);
			
			selected_changed = false;

		}



		GameUI* CraftBtn_GUI = dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook_Selected CraftBtn")->GetComponent(ComponentType::GameUI));

		//only if the mouse is within the ui, then we go into the update loop
		if ((mouse_x > CraftBtn_GUI->AA.x && mouse_x < CraftBtn_GUI->BB.x)
			&& (mouse_y_aft_TRS > CraftBtn_GUI->AA.y && mouse_y_aft_TRS < CraftBtn_GUI->BB.y))
		{
			if (Input::GetMouseTriggeredDepth(MouseButton::Button0, Engine->GetGameObject("CraftCook_Selected CraftBtn")))
			{
				std::cout << "Cook Btn Pressed" << std::endl;
				if (can_cook && !inventory_full)
				{
					audio->PlaySfx(30);
					
					//disable the script for confirmation for the crafting
					(Engine->GetGameObject("CraftCook Confirmation")->GetComponent<LogicComponent>()->SetEnabled("Cooking", true));
					(Engine->GetGameObject("CraftCook Confirmation")->GetComponent<LogicComponent>()->SetEnabled("Crafting",false));
					

					// activate the confirmation
					dynamic_cast<Renderer*>(Engine->GetGameObject("CraftCook Confirmation")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
					dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook Confirmation")->GetComponent(ComponentType::GameUI))->SetFontVisibility(true);
					dynamic_cast<GameUI*>(Engine->GetGameObject("CraftCook Confirmation")->GetComponent(ComponentType::GameUI))->SetText("Confirm Cook?");

					dynamic_cast<Renderer*>(Engine->GetGameObject("CraftCook Confirmation Yes")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
					dynamic_cast<Renderer*>(Engine->GetGameObject("CraftCook Confirmation No")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
					
				}
			}
		}

		

	}
	/**************************************************************************/
	/*!
		\brief
			this the update function of the GameObject for the closure of the
			cooking ui menus and its corresponding game objects
		\param GO
			The specific Gameobject who is the calling and is affected by
			this script function
		\param dt
			delta_time
	*/
	/**************************************************************************/
	inline void Update_CloseUI(GameObject* GO, float dt)
	{
		(void)GO;
		(void)dt;
		if (Input::GetMouseTriggeredDepth(MouseButton::Button0, Engine->GetGameObject("CraftCookQuest CloseBtn")))
		{
			std::cout << "Close button pressed" << std::endl;
			MakeINVISIBLECookCraftUI();
			craftingcooking_open = false;
			Engine->GetGameObject("CookingCraftingUILeft")->GetComponent<LogicComponent>()->SetEnabled("Cooking", false);
			Engine->GetGameObject("CookingCraftingUIRight")->GetComponent<LogicComponent>()->SetEnabled("Cooking", false);
			Engine->GetGameObject("CraftCookQuest CloseBtn")->GetComponent<LogicComponent>()->SetEnabled("Cooking", false);
			Engine->GetGameObject("CraftCook Confirmation")->GetComponent<LogicComponent>()->SetEnabled("Cooking", false);
		}
	}

	inline bool CheckCookingMinigameWin()
	{
		Transform* Collision_TRFCOMP = static_cast<Transform*>(Engine->GetGameObject("Cooking MinigameSuccessCollisionBox")->GetComponent(ComponentType::Transform));

		float collision_x_max = 0.5f * Collision_TRFCOMP->Dimensions.x * (Collision_TRFCOMP->Scale.x / 100.f) + Collision_TRFCOMP->Position.x;
		float collision_x_min = -0.5f * Collision_TRFCOMP->Dimensions.x * (Collision_TRFCOMP->Scale.x / 100.f) + Collision_TRFCOMP->Position.x;
	
		float pointer_x_pos = Engine->GetGameObject("Cooking MinigamePointer")->GetComponent<Transform>()->Position.x;

		if (collision_x_min < pointer_x_pos && pointer_x_pos < collision_x_max)
		{
			return true;
		}
		else
		{
			return false;
		}
	
	}

	/**************************************************************************/
	/*!
		\brief
			this the update function for cofmration of cooking 
		\param GO
			The specific Gameobject who is the calling and is affected by
			this script function
		\param dt
			delta_time
	*/
	/**************************************************************************/
	inline void Update_Confirmation(GameObject* GO, float dt)
	{
		(void)dt;
		if (GO->GetComponent<Renderer>()->GetVisibility() == false) return;

		if (Input::GetMouseTriggeredDepth(MouseButton::Button0, Engine->GetGameObject("CraftCook Confirmation Yes")))
		{
			// remove the crafting materials from the inventory
			RemoveCookingMaterialsInInventory();

			Engine->GetGameObject("Cooking MinigameBGFade")->GetComponent<LogicComponent>()->SetEnabled("Cooking", true);

			//hide confirmation
			Engine->GetGameObject("CraftCook Confirmation No")->GetComponent<Renderer>()->SetVisibility(false);
			Engine->GetGameObject("CraftCook Confirmation Yes")->GetComponent<Renderer>()->SetVisibility(false);

			Engine->GetGameObject("CraftCook Confirmation")->GetComponent<Renderer>()->SetVisibility(false);
			Engine->GetGameObject("CraftCook Confirmation")->GetComponent<GameUI>()->SetFontVisibility(false);

			//activate cooking
			Engine->GetGameObject("Cooking MinigameBGFade")->GetComponent<Renderer>()->SetVisibility(true);
			Engine->GetGameObject("Cooking MinigameIcon")->GetComponent<Renderer>()->SetVisibility(true);
			
			Engine->GetGameObject("Cooking MinigameIcon")->GetComponent<Renderer>()
				->SetTexHandle(Engine->GetGameObject("CraftCook_Selected Icon")->GetComponent<Renderer>()->GetTexHandle());
			Engine->GetGameObject("Cooking MinigameIcon")->GetComponent<Renderer>()
				->SetUVMin(Engine->GetGameObject("CraftCook_Selected Icon")->GetComponent<Renderer>()->GetUVMin());
			Engine->GetGameObject("Cooking MinigameIcon")->GetComponent<Renderer>()
				->SetUVMax(Engine->GetGameObject("CraftCook_Selected Icon")->GetComponent<Renderer>()->GetUVMax());

			Engine->GetGameObject("Cooking MinigameCookBtn")->GetComponent<Renderer>()->SetVisibility(true);
			Engine->GetGameObject("Cooking MinigameCookBtn")->GetComponent<GameUI>()->SetFontVisibility(true);

			Engine->GetGameObject("Cooking MinigameSliderBar")->GetComponent<Renderer>()->SetVisibility(true);

			Engine->GetGameObject("Cooking MinigamePointer")->GetComponent<Renderer>()->SetVisibility(true);
				

		}

		if (Input::GetMouseTriggeredDepth(MouseButton::Button0, Engine->GetGameObject("CraftCook Confirmation No")))
		{
			//hide confirmation
			Engine->GetGameObject("CraftCook Confirmation")->GetComponent<Renderer>()->SetVisibility(false);
			Engine->GetGameObject("CraftCook Confirmation")->GetComponent<GameUI>()->SetFontVisibility(false);

			Engine->GetGameObject("CraftCook Confirmation No")->GetComponent<Renderer>()->SetVisibility(false);
			Engine->GetGameObject("CraftCook Confirmation Yes")->GetComponent<Renderer>()->SetVisibility(false);
			
			Engine->GetGameObject("Quit Game Confirmation")->GetComponent<Renderer>()->SetVisibility(false);
			Engine->GetGameObject("Quit Game Confirmation")->GetComponent<GameUI>()->SetFontVisibility(false);
		}

	}
	/**************************************************************************/
	/*!
		\brief
			this the update function for the cooking minigame
		\param GO
			The specific Gameobject who is the calling and is affected by
			this script function
		\param dt
			delta_time
	*/
	/**************************************************************************/
	inline void Update_CookingMinigame(GameObject* GO, float dt)
	{
		UNREFERENCED_PARAMETER(GO);
		UNREFERENCED_PARAMETER(dt);

		if (GO->GetComponent<Renderer>()->GetVisibility() == false) return;

		//add to slider time
		slider_time += dt;
		
		Transform* Slider_TRFCOMP = static_cast<Transform*>(Engine->GetGameObject("Cooking MinigameSliderBar")->GetComponent(ComponentType::Transform));

		float slider_x_max = 0.5f * Slider_TRFCOMP->Dimensions.x * (Slider_TRFCOMP->Scale.x / 100.f) + Slider_TRFCOMP->Position.x;
		float slider_x_min = -0.5f * Slider_TRFCOMP->Dimensions.x * (Slider_TRFCOMP->Scale.x / 100.f) + Slider_TRFCOMP->Position.x;
		float slider_length = slider_x_max - slider_x_min;
		Engine->GetGameObject("Cooking MinigamePointer")->GetComponent<Transform>()->Position.x
			= sin(slider_time) * (slider_length / 2.f) + (Slider_TRFCOMP->Position.x);



		if (Input::GetMouseTriggeredDepth(MouseButton::Button0, Engine->GetGameObject("Cooking MinigameCookBtn")))
		{
			std::cout << "Mini game cook btn pressed" << std::endl;
			audio->PlaySfx(30); // in progress
			//audio->PlaySfx(24); // in progress
			//RemoveCookingMaterialsInInventory();

			if (CheckCookingMinigameWin())
			{

				std::cout << "COOK SUCCESS" << std::endl;
				audio->PlaySfx(25); // 
				//add cooked item into inventory
				Engine->GetObjectByPrefix("Player")->GetComponent<Inventory>()->AddItem(MasterObjectList->GetIDbyInventoryName(current_selected_cookable));

				GameObject* CraftingVFX = factory->AddGameObject("./Data/CraftingResult.txt");
				CraftingVFX->GetComponent<Renderer>()->SetTexHandle(MasterObjectList->GetInventoryItem(MasterObjectList->GetIDbyInventoryName(current_selected_cookable)).TexHandle);
				CraftingVFX->GetComponent<Renderer>()->SetUVMin(MasterObjectList->GetInventoryItem(MasterObjectList->GetIDbyInventoryName(current_selected_cookable)).Tex_UV_Min_COORD);
				CraftingVFX->GetComponent<Renderer>()->SetUVMax(MasterObjectList->GetInventoryItem(MasterObjectList->GetIDbyInventoryName(current_selected_cookable)).Tex_UV_Max_COORD);
				factory->QueueAdd(CraftingVFX);

				Engine->GetGameObject("Cooking MinigameBGFade")->GetComponent<LogicComponent>()->SetEnabled("Cooking", false);
				Engine->GetGameObject("CookingCraftingUIRight")->GetComponent<LogicComponent>()->SetEnabled("Cooking", false);
				Engine->GetGameObject("CraftCook Confirmation")->GetComponent<LogicComponent>()->SetEnabled("Cooking", false);
				Engine->GetGameObject("CraftCookQuest CloseBtn")->GetComponent<LogicComponent>()->SetEnabled("Cooking", false);
				
				craftingcooking_open = false;
				MakeINVISIBLECookCraftUI();
			}
			else
			{
				std::cout << "COOK FAILURE" << std::endl;
				audio->PlaySfx(23);
				craftingcooking_open = false;
				material_qnt.clear();
				MakeINVISIBLECookCraftUI();
				
			}



			//decative cooking minigame
			Engine->GetGameObject("Cooking MinigameBGFade")->GetComponent<Renderer>()->SetVisibility(false);
			Engine->GetGameObject("Cooking MinigameIcon")->GetComponent<Renderer>()->SetVisibility(false);

			Engine->GetGameObject("Cooking MinigameCookBtn")->GetComponent<Renderer>()->SetVisibility(false);
			Engine->GetGameObject("Cooking MinigameCookBtn")->GetComponent<GameUI>()->SetFontVisibility(false);

			Engine->GetGameObject("Cooking MinigameSliderBar")->GetComponent<Renderer>()->SetVisibility(false);

			Engine->GetGameObject("Cooking MinigamePointer")->GetComponent<Renderer>()->SetVisibility(false);

			
			Engine->GetGameObject("Cooking MinigameBGFade")->GetComponent<LogicComponent>()->SetEnabled("Cooking", false);
			Engine->GetGameObject("CookingCraftingUIRight")->GetComponent<LogicComponent>()->SetEnabled("Cooking", false);
			Engine->GetGameObject("CraftCook Confirmation")->GetComponent<LogicComponent>()->SetEnabled("Cooking", false);
			Engine->GetGameObject("CraftCookQuest CloseBtn")->GetComponent<LogicComponent>()->SetEnabled("Cooking", false);

			slider_time = 0.f;

			//Engine->GetObjectByPrefix("Player")->GetComponent<Inventory>()->AddItem(MasterObjectList->GetIDbyInventoryName(current_selected_cookable));
		}
	}
	/**************************************************************************/
	/*!
		\brief
			Init for the cooking script.
		\param GO
			The specific Gameobject who is the calling and is affected by
			this script function
		\param dt
			delta_time
	*/
	/**************************************************************************/
	void Init(GameObject* GO, float dt)
	{
		UNREFERENCED_PARAMETER(GO);
		UNREFERENCED_PARAMETER(dt);
		current_selected_cookable = "";
		selected_changed = false;
		InitGOsToItem();
		slider_time = 0.f;
	}
	/**************************************************************************/
	/*!
		\brief
			Update functions for the cooking script.
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
		
		//std::cout << "Cooking::update running for " << GO->GetName() << std::endl;
		if (GO->GetName() == "CookingCraftingUILeft") Update_CookingLeftUI(GO, dt);
		if (GO->GetName() == "CookingCraftingUIRight") Update_CookingRightUI(GO, dt);
		if (GO->GetName() == "CraftCook Confirmation") Update_Confirmation(GO, dt);
		if (GO->GetName() == "Cooking MinigameBGFade") Update_CookingMinigame(GO, dt);
		if (GO->GetName() == "CraftCookQuest CloseBtn") Update_CloseUI(GO, dt);
	}
	/**************************************************************************/
	/*!
		\brief
			End functions for the cooking script.
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
