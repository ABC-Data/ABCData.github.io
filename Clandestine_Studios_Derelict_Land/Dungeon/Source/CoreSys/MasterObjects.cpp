/******************************************************************************/
/*!
\file		MasterObjects.cpp
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	Jan 10, 2023
\brief		Master Object List implementation

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "MasterObjects.h"

MasterObjects::MasterObjects() : TempInventory{ nullptr }, TempPlayerPosition{ nullptr }, StoredController{ nullptr }, StoredPlayerCombat{ nullptr } {
	StoredHubController = new Controller("Hub", 10);
	for (int i = 0; i < StoredHubController->LevelFlagsSize; ++i) {
		StoredHubController->LevelFlags.emplace_back(false);
	}
}

MasterObjects::~MasterObjects()
{
	if (TempInventory != nullptr) {
		delete TempInventory;
		TempInventory = nullptr;
	}
	if (TempPlayerPosition != nullptr) {
		delete TempPlayerPosition;
		TempPlayerPosition = nullptr;
	}
	if (StoredController != nullptr) {
		delete StoredController;
		StoredController = nullptr;
	}
	if (StoredHubController != nullptr) {
		delete StoredHubController;
		StoredHubController = nullptr;
	}
	if (StoredPlayerCombat != nullptr) {
		delete StoredPlayerCombat;
		StoredPlayerCombat = nullptr;
	}
}

void MasterObjects::Init()
{
	MasterInventoryItemList.emplace_back(InventoryItem());
	LoadInventoryItems("./Data/MasterInventory.txt");
	LoadSetEffects("./Data/MasterSetEffect.txt");

	std::sort(MasterInventoryItemList.begin(), MasterInventoryItemList.end(), [](InventoryItem& a, InventoryItem& b) { return a.ID < b.ID; });
}

void MasterObjects::LoadInventoryItems(std::string filepath)
{
	std::ifstream ifs(filepath);
	if (!ifs) throw std::runtime_error("Couldn't open Master Inventory file!");

	std::string Itempath;
	while (std::getline(ifs, Itempath)) {
		FILE* fp = fopen(Itempath.c_str(), "r");
		if (!fp) throw std::runtime_error("Couldn't open an Inventory item file!");

		InventoryItem InvItem;
		int success = 1;
		int id = 0;
		float weight = 0.f;
		char name[64]{ 0 };
		char type[64]{ 0 };
		char lore[256]{ 0 };
		char handle[64]{ 0 };
		Vec2 uvmin{ 0.f, 0.f };
		Vec2 uvmax{ 0.f, 0.f };
		char tooltiphandle[64]{ 0 };
		Vec2 tooltipuvmin{ 0.f, 0.f };
		Vec2 tooltipuvmax{ 0.f, 0.f };
		Vec2 tooltipDimensions{ 0.f, 0.f };


		success *= fscanf(fp, "ID: %d\n", &id);
		success *= fscanf(fp, "Texture Handle: %[^\n]\n", &handle);
		success *= fscanf(fp, "Texture UV MIN: %f, %f\n", &uvmin.x, &uvmin.y);
		success *= fscanf(fp, "Texture UV MAX: %f, %f\n", &uvmax.x, &uvmax.y);
		success *= fscanf(fp, "Tooltip Texture Handle: %[^\n]\n", &tooltiphandle);
		success *= fscanf(fp, "Tooltip Texture UV MIN: %f, %f\n", &tooltipuvmin.x, &tooltipuvmin.y);
		success *= fscanf(fp, "Tooltip Texture UV MAX: %f, %f\n", &tooltipuvmax.x, &tooltipuvmax.y);
		success *= fscanf(fp, "Tooltip Dimensions: %f, %f\n", &tooltipDimensions.x, &tooltipDimensions.y);
		success *= fscanf(fp, "Weight: %f\n", &weight);
		success *= fscanf(fp, "Name: %[^\n]\n", &name);
		success *= fscanf(fp, "Type: %[^\n]\n", &type);
		success *= fscanf(fp, "Lore: %[^;];\n", &lore);

		if (success) {
			InvItem.SetTexture(std::string(handle), uvmin, uvmax);
			InvItem.SetTooltipTexture(std::string(tooltiphandle), tooltipuvmin, tooltipuvmax, tooltipDimensions);
			if (strncmp(type, "Weapon", 6) == 0) {
				char weptype[32]{ 0 };
				char wepstatus[32]{ 0 };
				float dmg = 0.f;
				float cc = 0.f;
				float cm = 0.f;
				float acc = 0.f;
				int apc = 0;
				success *= fscanf(fp, "Weapon Type: %[^\n]\n", &weptype);
				success *= fscanf(fp, "Status Effect: %[^\n]\n", &wepstatus);
				success *= fscanf(fp, "Damage: %f\n", &dmg);
				success *= fscanf(fp, "Crit Chance: %f\n", &cc);
				success *= fscanf(fp, "Crit Damage: %f\n", &cm);
				success *= fscanf(fp, "Accuracy: %f\n", &acc);
				success *= fscanf(fp, "AP Cost: %d", &apc);
				if (success) {
					WeaponStats ws(std::string(weptype), std::string(wepstatus), dmg, cc, cm, acc, apc);
					InvItem.SetWeapon(id, weight, std::string(name), std::string(type), std::string(lore), std::move(ws));
					MasterInventoryItemList.emplace_back(InvItem);
				}
			}
			else if (strncmp(type, "Consumable", 10) == 0) {
				char effectmod[32]{ 0 };
				float val = 0.f;
				float duration = 0.f;
				success *= fscanf(fp, "Effect: %[^\n]\n", &effectmod);
				success *= fscanf(fp, "Value: %f\n", &val);
				success *= fscanf(fp, "Duration: %f\n", &duration);
				if (success) {
					ConsumableStats cs(ConvertEffectMods(effectmod), val, duration);
					InvItem.SetConsumable(id, weight, std::string(name), std::string(type), std::string(lore), std::move(cs));
					MasterInventoryItemList.emplace_back(InvItem);
				}
			}
			else if (strncmp(type, "Material", 8) == 0) {
				//Do nothing, it's done
				InvItem.SetMaterial(id, weight, std::string(name), std::string(type), std::string(lore), 1);
				MasterInventoryItemList.emplace_back(InvItem);
			}
			else if (strncmp(type, "Equipment", 9) == 0) {
				char equipslot[16]{ 0 };
				char set[64]{ 0 };
				float pr = 0.f;
				float sr = 0.f;
				float br = 0.f;
				success *= fscanf(fp, "Slot: %[^\n]\n", &equipslot);
				success *= fscanf(fp, "Physical Resistance: %f\n", &pr);
				success *= fscanf(fp, "Stun Resistance: %f\n", &sr);
				success *= fscanf(fp, "Bleed Resistance: %f\n", &br);
				success *= fscanf(fp, "Set: %[^\n]\n", &set);
				if (success) {
					EquipmentStats es(ConvertEquipmentSlot(equipslot), pr, sr, br, std::string(set));
					InvItem.SetEquipment(id, weight, std::string(name), std::string(type), std::string(lore), std::move(es));
					MasterInventoryItemList.emplace_back(InvItem);
				}
			}
		}
		fclose(fp);
		std::cout << "Loaded " << name << std::endl;
	}
	ifs.close();
}

void MasterObjects::LoadSetEffects(std::string filepath) {
	std::ifstream ifs(filepath);
	if (!ifs) throw std::runtime_error("Couldn't open Master Set Effects file!");

	std::string Itempath;
	while (std::getline(ifs, Itempath)) {
		FILE* fp = fopen(Itempath.c_str(), "r");
		if (!fp) throw std::runtime_error("Couldn't open a Set Effect file!");
		int success = 1;
		char name[64]{ 0 };
		int headslot = 0;
		int bodyslot = 0;
		int legslot = 0;
		int gloveslot = 0;
		int hpbonus2 = 0;
		float apbonus2 = 0.f;
		int hpbonus4 = 0;
		float apbonus4 = 0.f;

		success *= fscanf(fp, "Name: %[^\n]\n", &name);
		success *= fscanf(fp, "IDs: %d %d %d %d\n", &headslot, &bodyslot, &legslot, &gloveslot);
		success *= fscanf(fp, "2 Set: %d %f\n", &hpbonus2, &apbonus2);
		success *= fscanf(fp, "4 Set: %d %f\n", &hpbonus4, &apbonus4);
		if (success) {
			SetEffect set(std::string(name), headslot, bodyslot, legslot, gloveslot, hpbonus2, apbonus2, hpbonus4, apbonus4);
			MasterSetEffectList[std::string(name)] = set;
		}
		else {
			std::cout << "Failed to load a Set Effect!\n";
		}
		fclose(fp);
	}
	ifs.close();
}

const InventoryItem MasterObjects::GetInventoryItem(int id)
{
	if (id == 0) return InventoryItem();
	else return MasterInventoryItemList[id];
}

/*!*************************************************************************
****
\brief
	Exports an Inventory to the Temporary Storage

\param inv
	Inventory to export
****************************************************************************
***/
void MasterObjects::ExportInventory(Inventory* inv)
{
	if (TempInventory == nullptr) {
		TempInventory = new Inventory;
	}
	TempInventory->CopyData(inv);
}

/*!*************************************************************************
****
\brief
	Imports the Inventory stored in Temporary Storage, then nulls TempInventory

\param target
	Inventory to import into
****************************************************************************
***/
void MasterObjects::ImportInventory(Inventory* target)
{
	if (TempInventory != nullptr) {
		target->CopyData(TempInventory);
		delete TempInventory;
		TempInventory = nullptr;
	}
}

/*!*************************************************************************
****
\brief
	Exports a Transform to the Temporary Player Position

\param pos
	Transform to export
****************************************************************************
***/
void MasterObjects::ExportPlayerPosition(Transform* pos)
{
	if (TempPlayerPosition == nullptr) {
		TempPlayerPosition = new Transform;
	}
	TempPlayerPosition->CopyData(pos);
}

/*!*************************************************************************
****
\brief
	Imports the Transform stored in Temporary Player Position, then nulls TempPlayerPosition

\param target
	Transform to import into
****************************************************************************
***/
void MasterObjects::ImportPlayerPosition(Transform* target)
{
	if (TempPlayerPosition != nullptr) {
		target->CopyData(TempPlayerPosition);
		delete TempPlayerPosition;
		TempPlayerPosition = nullptr;
	}
}

void MasterObjects::ExportPlayerCombat(PlayerCombat* cont)
{
	if (!StoredPlayerCombat) {
		StoredPlayerCombat = new PlayerCombat;
	}
	StoredPlayerCombat->CopyData(cont);
}

/*!*************************************************************************
****
\brief
	Exports a Controller to the StoredController

\param cont
	Controller to export
****************************************************************************
***/
void MasterObjects::ExportController(Controller* cont)
{
	if (StoredController == nullptr) {
		StoredController = new Controller;
	}
	StoredController->CopyData(cont);
}

/*!*************************************************************************
****
\brief
	Imports the Controller stored in StoredController, then nulls StoredController

\param target
	Controller to import into
****************************************************************************
***/
void MasterObjects::ImportController(Controller* target)
{
	if (StoredController != nullptr) {
		target->CopyData(StoredController);
		delete StoredController;
		StoredController = nullptr;
	}
}

void MasterObjects::ClearController()
{
	if (StoredController != nullptr) {
		delete StoredController;
		StoredController = nullptr;
	}
}

/*!*************************************************************************
****
\brief
	Exports a Controller to the StoredHubController

\param cont
	Controller to export
****************************************************************************
***/
void MasterObjects::SetHubController(Controller* cont)
{
	StoredHubController->CopyData(cont);
}

/*!*************************************************************************
****
\brief
	Imports the Controller stored in StoredHubController

\param target
	Controller to import into
****************************************************************************
***/
void MasterObjects::GetHubController(Controller* target)
{
	target->CopyData(StoredHubController);
}

void MasterObjects::ExportEnemyFilepath(std::string fp)
{
	StoredEnemyFilepath = fp;
}

void MasterObjects::ImportEnemyFilepath(std::string& fp)
{
	fp = StoredEnemyFilepath;
	StoredEnemyFilepath = "";
}

/*!*************************************************************************
****
\brief
	Instance of MasterObjects
\return
	Created MasterObjects
****************************************************************************
***/
MasterObjects* MasterObjects::Instance() { if (!MasterObjectList) { MasterObjectList = new MasterObjects(); } return MasterObjectList; }

/*!*************************************************************************
****
\brief
	Delete static MasterObjects above.
****************************************************************************
***/
void MasterObjects::DeleteStaticMasterObjects() { if (MasterObjectList) { delete MasterObjectList; MasterObjectList = nullptr; } }

int MasterObjects::GetIDbyInventoryName(std::string name)
{
	for (auto& x : MasterInventoryItemList)
	{
		if (x.Name == name)
			return x.ID;
	}
	return NULL;
}