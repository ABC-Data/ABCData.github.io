/******************************************************************************/
/*!
\file		InventoryComponent.h
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	Nov 15, 2022
\brief		Contains Inventory Component required for Inventory script

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#ifndef INV_COMP_H
#define INV_COMP_H
#include "Essential.h"
#include "GameObject/GameObject.h"
#include "CoreSys/MasterObjects.h"

#define INV_SIZE_MAX (size_t)12

enum class EffectMods {
	INVALID,
	HEAL_INSTANT,
	HP_REGEN,
	AP_REGEN,
	HEAL_STUN,
	HEAL_BLEED,
	STATUS_IMMUNE,
	DAMAGE_BOOST,
	DAMAGE_REDUCTION_INSTANT,
	DAMAGE_REDUCTION_DURATION,
	EVASION,
	CRIT_CHANCE_BOOST,
	CRIT_MULTIPLIER_BOOST
};

enum class EquipmentSlot {
	INVALID,
	HEAD,
	BODY,
	LEGS,
	GLOVES
};

std::string EquipmentSlotString(EquipmentSlot slot);

EffectMods ConvertEffectMods(std::string mod);

std::string ConvertReadable(EffectMods effect);

EquipmentSlot ConvertEquipmentSlot(std::string mod);

std::string ConvertReadable(EquipmentSlot slot);

class MasterObjects;

class WeaponStats {
public:
	WeaponStats();
	WeaponStats(std::string weptype, std::string wepstatus, float dmg, float cc, float cm, float acc, int apc);

	std::string WeaponType;
	std::string WeaponStatus;
	float Damage;
	float CritChance;
	float CritMultiplier;
	float Accuracy;
	int APCost;
};

class ConsumableStats {
public:
	ConsumableStats() = default;
	ConsumableStats(EffectMods effect, float value, float Duration);

	EffectMods Effect;
	float EffectValue;
	float Duration;
};

class EquipmentStats {
public:
	EquipmentStats();
	EquipmentStats(EquipmentSlot slot, float pr, float sr, float br, std::string set);
	
	EquipmentSlot Slot;
	float Phys_Resist;
	float Stun_Resist;
	float Bleed_Resist;
	std::string Set;
};

class SetEffect {
public:
	SetEffect();
	SetEffect(std::string name, int id1, int id2, int id3, int id4, int hpbonus2, float apbonus2, int hpbonus4, float apbonus4);

	std::string Name;
	int SetIDs[4];
	int HP_Bonus_2;
	float AP_Bonus_2;
	int HP_Bonus_4;
	float AP_Bonus_4;
};

class InventoryItem {
public:
	//ID 0 is always "Nothing"
	InventoryItem();
	InventoryItem(int id);
	~InventoryItem() = default;

	/*!*************************************************************************
	****
	\brief
		Zeroes out this item
	****************************************************************************
	***/
	void ClearItem();

	/*!*************************************************************************
	****
	\brief
		Sets this item's data
	\param[in] id
		Item id
	\param[in] name
		Item name
	\param[in] description
		Item description
	****************************************************************************
	***/
	void SetItem(int id, float weight, std::string name, std::string type, std::string lore, int qty,
		WeaponStats&& ws, ConsumableStats&& cs, EquipmentStats&& es);

	void SetWeapon(int id, float weight, std::string name, std::string type, std::string lore,
		WeaponStats&& ws);

	void SetConsumable(int id, float weight, std::string name, std::string type, std::string lore,
		ConsumableStats&& cs);

	void SetEquipment(int id, float weight, std::string name, std::string type, std::string lore,
		EquipmentStats&& es);

	void SetMaterial(int id, float weight, std::string name, std::string type, std::string lore, int qty);

	void SetTexture(std::string handle, Vec2 uv_min, Vec2 uv_max);

	void SetTooltipTexture(std::string tthandle, Vec2 ttuv_min, Vec2 ttuv_max, Vec2 ttdimensions);

	int ID;
	std::string TexHandle;
	Vec2 Tex_UV_Min_COORD;
	Vec2 Tex_UV_Max_COORD;
	std::string TtTexHandle;
	Vec2 TtTex_UV_Min_COORD;
	Vec2 TtTex_UV_Max_COORD;
	Vec2 TooltipDimensions;
	float Weight;
	std::string Name;
	std::string Type;
	std::string Lore;
	int Quantity;
	WeaponStats WStats;
	ConsumableStats CStats;
	EquipmentStats EStats;
};


class Inventory : public Component {
public:
	/*!*************************************************************************
	****
	\brief
		Default constructor
	****************************************************************************
	***/
	Inventory();

	/*!*************************************************************************
	****
	\brief
		Adds an item to inventory at index
	\param[in] Item
		Item object
	\param[in] index = 0
		Index to put item
	****************************************************************************
	***/
	void AddItem(InventoryItem Item, size_t index);

	/*!*************************************************************************
	****
	\brief
		Adds an item to equipment at index
	\param[in] Item
		Item object
	\param[in] index
		Index to put item
	****************************************************************************
	***/
	void SetEquipment(InventoryItem Item, size_t index);

	/*!*************************************************************************
	****
	\brief
		Adds an item to inventory if enough space (Game Function)
	\param[in] Item
		Item object
	****************************************************************************
	***/
	void AddItem(InventoryItem Item);

	/*!*************************************************************************
	****
	\brief
		Removes an item from inventory at index
	\param[in] index
		Index to remove
	****************************************************************************
	***/
	void RemoveItem(size_t index, int amount = 1);

	/*!*************************************************************************
	****
	\brief
		Removes an item from a slot
	\param[in] slot
		Slot to remove
	****************************************************************************
	***/
	void RemoveEquipment(EquipmentSlot slot);

	/*!*************************************************************************
	****
	\brief
		Removes an item from a slot
	\param[in] index
		Slot to remove
	****************************************************************************
	***/
	void RemoveEquipment(size_t index);

	/*!*************************************************************************
	****
	\brief
		Increases inventory size by an amount
	\param[in] amount = 1
		Increase by this much
	****************************************************************************
	***/
	void IncreaseSize(size_t amount = 1);

	/*!*************************************************************************
	****
	\brief
		Gets if inventory is full
	\return
		true if inventory is full
	****************************************************************************
	***/
	bool GetFullInventory();

	/*!*************************************************************************
	****
	\brief
		Returns set bonuses from equipment
	\param Setname
		Name of set to verify
	\return
		Tier of set bonus
	****************************************************************************
	***/
	int VerifySetBonus(std::string Setname);

	/*!*************************************************************************
	****
	\brief
		Returns count of items in set in equipment
	\param Setname
		Name of set to count
	\return
		Pieces worn
	****************************************************************************
	***/
	int CountSetItems(std::string Setname);

	/*!*************************************************************************
	****
	\brief
		Serialize to file
	\param[in] fp
		Filepath
	****************************************************************************
	***/
	virtual void Serialize(FILE* fp) override;

	/*!*************************************************************************
	****
	\brief
		Deserialize from file
	\param[in] fp
		Filepath
	****************************************************************************
	***/
	virtual void Deserialize(FILE* fp) override;

	/*!*************************************************************************
	****
	\brief
		Copy other component's data
	\param[in] Target
		Copy from
	****************************************************************************
	***/
	virtual void CopyData(Component* Target) override;

	std::vector<InventoryItem> Invent;
	std::vector<InventoryItem> Equipment;
	size_t InventorySize;
	bool Visible;
};

#endif