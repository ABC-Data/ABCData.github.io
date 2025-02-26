/******************************************************************************/
/*!
\file		InventoryComponent.cpp
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	Nov 15, 2022
\brief		Contains Inventory Component required for Inventory script

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "InventoryComponent.h"


WeaponStats::WeaponStats() :
	WeaponType{ std::string("Melee") }, WeaponStatus{ std::string("None") }, Damage{ 0.f }, CritChance{ 0.f }, CritMultiplier{ 0.f }, Accuracy{ 0.f }, APCost{ 0 }{}

WeaponStats::WeaponStats(std::string weptype, std::string wepstatus, float dmg, float cc, float cm, float acc, int apc) :
	WeaponType{ weptype }, WeaponStatus{ wepstatus }, Damage{ dmg }, CritChance{ cc }, CritMultiplier{ cm }, Accuracy{ acc }, APCost{ apc }{}

ConsumableStats::ConsumableStats(EffectMods effect, float value, float duration) :
	Effect{ effect }, EffectValue{ value }, Duration{ duration }{}

EquipmentStats::EquipmentStats() :
	Slot{ EquipmentSlot::INVALID }, Phys_Resist{ 0.f }, Stun_Resist{ 0.f }, Bleed_Resist{ 0.f }, Set{ std::string("NULL") } {}

EquipmentStats::EquipmentStats(EquipmentSlot slot, float pr, float sr, float br, std::string set) :
	Slot{ slot }, Phys_Resist{ pr }, Stun_Resist{ sr }, Bleed_Resist{ br }, Set{ set } {}

SetEffect::SetEffect() :
	Name{ std::string("NULL") }, SetIDs{ 0 }, HP_Bonus_2{ 0 }, AP_Bonus_2{ 0.f }, HP_Bonus_4{ 0 }, AP_Bonus_4{ 0.f }{}

SetEffect::SetEffect(std::string name, int id1, int id2, int id3, int id4, int hpbonus2, float apbonus2, int hpbonus4, float apbonus4) :
	Name{ name }, SetIDs{ {} }, HP_Bonus_2{ hpbonus2 }, AP_Bonus_2{ apbonus2 }, HP_Bonus_4{ hpbonus4 }, AP_Bonus_4{ apbonus4 }{
	SetIDs[0] = id1;
	SetIDs[1] = id2;
	SetIDs[2] = id3;
	SetIDs[3] = id4;
}

InventoryItem::InventoryItem() : ID{ 0 }, TexHandle{ "-" }, Tex_UV_Min_COORD{ 0.f, 0.f }, Tex_UV_Max_COORD{ 0.f,0.f }, TtTexHandle{ "-" }, TtTex_UV_Min_COORD{ 0.f, 0.f }, TtTex_UV_Max_COORD{ 0.f,0.f }, TooltipDimensions{ 0.f,0.f }, Weight{ 0.f }, Name{ "NULL" }, Type{ "NULL" }, Lore{ "NULL" }, Quantity{ 1 }, WStats{ WeaponStats() }, CStats{ ConsumableStats() }, EStats{ EquipmentStats() }{}

InventoryItem::InventoryItem(int id) : ID{ id }, TexHandle{ "-" }, Tex_UV_Min_COORD{ 0.f, 0.f }, Tex_UV_Max_COORD{ 0.f,0.f }, TtTexHandle{ "-" }, TtTex_UV_Min_COORD{ 0.f, 0.f }, TtTex_UV_Max_COORD{ 0.f,0.f }, TooltipDimensions{ 0.f,0.f }, Weight{ 0.f }, Name{ "NULL" }, Type{ "NULL" }, Lore{ "NULL" }, Quantity{ 1 }, WStats{ WeaponStats() }, CStats{ ConsumableStats() }, EStats{ EquipmentStats() }{
	*this = MasterObjectList->GetInventoryItem(id);
}


void InventoryItem::ClearItem() {
	ID = 0;
	TexHandle = "-";
	Tex_UV_Min_COORD = { 0.f, 0.f };
	Tex_UV_Max_COORD = { 0.f, 0.f };
	TtTexHandle = "-";
	TtTex_UV_Min_COORD = { 0.f, 0.f };
	TtTex_UV_Max_COORD = { 0.f, 0.f };
	TooltipDimensions = { 0.f, 0.f };
	Weight = 0.f;
	Name = "NULL";
	Type = "NULL";
	Lore = "NULL";
	Quantity = 0;
	WStats = WeaponStats();
	CStats = ConsumableStats();
	EStats = EquipmentStats();
}

void InventoryItem::SetItem(int id, float weight, std::string name, std::string type, std::string lore, int qty,
	WeaponStats&& ws, ConsumableStats&& cs, EquipmentStats&& es) {
	ID = id;
	Weight = weight;
	Name = name;
	Type = type;
	Lore = lore;
	Quantity = qty;
	WStats = ws;
	CStats = cs;
	EStats = es;
}

void InventoryItem::SetWeapon(int id, float weight, std::string name, std::string type, std::string lore, WeaponStats&& ws)
{
	ID = id;
	Weight = weight;
	Name = name;
	Type = type;
	Lore = lore;
	Quantity = 1;
	WStats = ws;
	CStats = ConsumableStats();
	EStats = EquipmentStats();
}

void InventoryItem::SetConsumable(int id, float weight, std::string name, std::string type, std::string lore, ConsumableStats&& cs)
{
	ID = id;
	Weight = weight;
	Name = name;
	Type = type;
	Lore = lore;
	Quantity = 1;
	WStats = WeaponStats();
	CStats = cs;
	EStats = EquipmentStats();
}

void InventoryItem::SetEquipment(int id, float weight, std::string name, std::string type, std::string lore, EquipmentStats&& es)
{
	ID = id;
	Weight = weight;
	Name = name;
	Type = type;
	Lore = lore;
	Quantity = 1;
	WStats = WeaponStats();
	CStats = ConsumableStats();
	EStats = es;
}

void InventoryItem::SetMaterial(int id, float weight, std::string name, std::string type, std::string lore, int qty)
{
	ID = id;
	Weight = weight;
	Name = name;
	Type = type;
	Lore = lore;
	Quantity = qty;
	WStats = WeaponStats();
	CStats = ConsumableStats();
	EStats = EquipmentStats();
}

void InventoryItem::SetTexture(std::string handle, Vec2 uv_min, Vec2 uv_max)
{
	TexHandle = handle;
	Tex_UV_Min_COORD = uv_min;
	Tex_UV_Max_COORD = uv_max;
}

void InventoryItem::SetTooltipTexture(std::string tthandle, Vec2 ttuv_min, Vec2 ttuv_max, Vec2 ttdimensions)
{
	TtTexHandle = tthandle;
	TtTex_UV_Min_COORD = ttuv_min;
	TtTex_UV_Max_COORD = ttuv_max;
	TooltipDimensions = ttdimensions;
}

Inventory::Inventory() : InventorySize{ 10 }, Visible{ false } {
	SetType(ComponentType::Inventory);
	for (size_t i = 0; i < INV_SIZE_MAX; ++i) {
		Invent.emplace_back(InventoryItem());
	}
	for (size_t i = 0; i < 4; ++i) {
		Equipment.emplace_back(InventoryItem());
	}
}

void Inventory::AddItem(InventoryItem Item, size_t index) {
	Invent.insert(Invent.cbegin() + index, Item);
	Invent.erase(Invent.cbegin() + index + 1);
}

void Inventory::SetEquipment(InventoryItem Item, size_t index) {
	if (Item.Type != "Equipment" && Item.ID != 0) return;
	Equipment.insert(Equipment.cbegin() + index, Item);
	Equipment.erase(Equipment.cbegin() + index + 1);
}

void Inventory::AddItem(InventoryItem Item)
{
	size_t i = 0;
	if (Item.Type == "Material") {
		for (; i < InventorySize; ++i) {
			if (Invent[i].ID == Item.ID) {
				Invent[i].Quantity += Item.Quantity;
				return;
			}
		}
	}
	i = 0;
	for (; i < InventorySize; ++i) {
		if (Invent[i].ID == 0) break;
	}
	if (i < InventorySize) {
		AddItem(Item, i);
	}
	else {
		std::cout << "Inventory full!\n";
	}
}

void Inventory::RemoveItem(size_t index, int amount) {
	if (Invent[index].Type == "Material") {
		Invent[index].Quantity -= amount;
		if (Invent[index].Quantity < 1) {
			Invent.insert(Invent.cbegin() + index, InventoryItem());
			Invent.erase(std::cbegin(Invent) + index + 1);
			return;
		}
	}
	else {
		Invent.insert(Invent.cbegin() + index, InventoryItem());
		Invent.erase(std::cbegin(Invent) + index + 1);
	}
}

void Inventory::RemoveEquipment(EquipmentSlot slot) {
	switch (slot) {
	case (EquipmentSlot::HEAD):
		Equipment[0].ClearItem();
		break;
	case (EquipmentSlot::BODY):
		Equipment[1].ClearItem();
		break;
	case (EquipmentSlot::LEGS):
		Equipment[2].ClearItem();
		break;
	case(EquipmentSlot::GLOVES):
		Equipment[3].ClearItem();
		break;
	default:
		break;
	}
}

void Inventory::RemoveEquipment(size_t index) {
	if (index > 3) return;
	Equipment[index].ClearItem();
}

void Inventory::IncreaseSize(size_t amount) {
	InventorySize += amount;
	if (InventorySize > INV_SIZE_MAX) InventorySize = INV_SIZE_MAX;
}

bool Inventory::GetFullInventory() {
	size_t i = 0;
	for (; i < InventorySize; ++i) {
		if (Invent[i].ID == 0) break;
	}
	if (i < InventorySize) return false;
	else return true;
}

int Inventory::VerifySetBonus(std::string Setname)
{
	int counter = 0;
	for (int i = 0; i < 4; ++i) {
		if (Equipment[i].ID == MasterObjectList->MasterSetEffectList[Setname].SetIDs[i]) {
			++counter;
		}
	}
	if (counter < 2) return 0;
	else if (counter < 4) return 1;
	else return 2;
}

int Inventory::CountSetItems(std::string Setname)
{
	int counter = 0;
	for (int i = 0; i < 4; ++i) {
		if (Equipment[i].ID == MasterObjectList->MasterSetEffectList[Setname].SetIDs[i]) {
			++counter;
		}
	}
	return counter;
}

void Inventory::Serialize(FILE* fp) {
	fprintf(fp, "Inventory\n");
	fprintf(fp, "Inventory: %u ", (unsigned int)InventorySize);
	for (size_t i = 0; i < INV_SIZE_MAX; ++i) {
		fprintf(fp, "{%d}", Invent[i].ID);
	}
	fprintf(fp, "\n");
	fprintf(fp, "Equipment: ");
	for (size_t i = 0; i < 4; ++i) {
		fprintf(fp, "{%d}", Equipment[i].ID);
	}
	fprintf(fp, "\n");
}

void Inventory::Deserialize(FILE* fp) {
	unsigned int invsize{ 0 };
	int discard = fscanf(fp, "Inventory: %u ", &invsize);
	if (discard == 0) {
		std::cout << "Failed to load Inventory size!\n";
		return;
	}
	else {
		InventorySize = (size_t)invsize;
	}
	int id{ 0 };
	for (size_t i = 0; i < INV_SIZE_MAX; ++i) {
		int success = fscanf(fp, "{%d}", &id);
		if (success != 1) {
			std::cout << "Failed to load Inventory items!\n";
			return;
		}
		else {
			AddItem(InventoryItem(id), i);
			std::cout << "Slot " << i + 1 << ": " << Invent[i].Name << " | " << Invent[i].Lore << std::endl;
		}
	}
	discard = fscanf(fp, "\nEquipment: ");
	for (size_t i = 0; i < 4; ++i) {
		int success = fscanf(fp, "{%d}", &id);
		if (success != 1) {
			std::cout << "Failed to load Inventory items!\n";
			return;
		}
		else {
			SetEquipment(InventoryItem(id), i);
			std::cout << "Slot " << i + 1 << ": " << Equipment[i].Name << " | " << Equipment[i].Lore << std::endl;
		}
	}
	discard = fscanf(fp, "\n");
	//Discard additional items past the inventory size. No cheating!
	for (size_t i = InventorySize; i < INV_SIZE_MAX; ++i) {
		AddItem(InventoryItem(), i);
	}
}

void Inventory::CopyData(Component* Target) {
	Inventory* IC = dynamic_cast<Inventory*>(Target);

	if (IC != nullptr)
	{
		this->Invent = IC->Invent;
		this->Equipment = IC->Equipment;
		this->InventorySize = IC->InventorySize;
		this->Visible = IC->Visible;
	}
}

std::string EquipmentSlotString(EquipmentSlot slot)
{
	if (slot == EquipmentSlot::HEAD) return "HEAD"; 
	if (slot ==  EquipmentSlot::BODY) return "BODY";
	if (slot == EquipmentSlot::LEGS) return "LEGS";
	if (slot == EquipmentSlot::GLOVES) return "GLOVES";
	return "";
}

EffectMods ConvertEffectMods(std::string mod)
{
	if (mod == "HEAL_INSTANT") return EffectMods::HEAL_INSTANT;
	if (mod == "HP_REGEN") return EffectMods::HP_REGEN;
	if (mod == "AP_REGEN") return EffectMods::AP_REGEN;
	if (mod == "HEAL_STUN") return EffectMods::HEAL_STUN;
	if (mod == "HEAL_BLEED") return EffectMods::HEAL_BLEED;
	if (mod == "STATUS_IMMUNE") return EffectMods::STATUS_IMMUNE;
	if (mod == "DAMAGE_BOOST") return EffectMods::DAMAGE_BOOST;
	if (mod == "DAMAGE_REDUCTION_INSTANT") return EffectMods::DAMAGE_REDUCTION_INSTANT;
	if (mod == "DAMAGE_REDUCTION_DURATION") return EffectMods::DAMAGE_REDUCTION_DURATION;
	if (mod == "EVASION") return EffectMods::EVASION;
	if (mod == "CRIT_CHANCE_BOOST") return EffectMods::CRIT_CHANCE_BOOST;
	if (mod == "CRIT_MULTIPLIER_BOOST") return EffectMods::CRIT_MULTIPLIER_BOOST;
	return EffectMods::INVALID;
}

std::string ConvertReadable(EffectMods effect)
{
	std::string result;
	switch (effect) {
	case(EffectMods::HEAL_INSTANT):
		result = "Instant Heal";
		break;
	case(EffectMods::HP_REGEN):
		result = "Health Regeneration";
		break;
	case(EffectMods::AP_REGEN):
		result = "AP Regen Boost";
		break;
	case(EffectMods::HEAL_STUN):
		result = "Cures Stun";
		break;
	case(EffectMods::HEAL_BLEED):
		result = "Cures Bleeding";
		break;
	case(EffectMods::STATUS_IMMUNE):
		result = "Status Immunity";
		break;
	case(EffectMods::DAMAGE_BOOST):
		result = "Damage Boost";
		break;
	case(EffectMods::DAMAGE_REDUCTION_INSTANT):
		result = "Instant Damage Reduction";
		break;
	case(EffectMods::DAMAGE_REDUCTION_DURATION):
		result = "Damage Reduction";
		break;
	case(EffectMods::EVASION):
		result = "Increases Evasion";
		break;
	case(EffectMods::CRIT_CHANCE_BOOST):
		result = "Increases Crit Chance";
		break;
	case(EffectMods::CRIT_MULTIPLIER_BOOST):
		result = "Increases Crit Multiplier";
		break;
	default:
		result = "Unknown";
		break;
	}
	return result;
}

EquipmentSlot ConvertEquipmentSlot(std::string mod)
{
	if (mod == "HEAD") return EquipmentSlot::HEAD;
	if (mod == "BODY") return EquipmentSlot::BODY;
	if (mod == "LEGS") return EquipmentSlot::LEGS;
	if (mod == "GLOVES") return EquipmentSlot::GLOVES;
	return EquipmentSlot::INVALID;
}

std::string ConvertReadable(EquipmentSlot slot)
{
	std::string result;
	switch (slot) {
	case (EquipmentSlot::HEAD):
		result = "Head";
		break;
	case (EquipmentSlot::BODY):
		result = "Body";
		break;
	case (EquipmentSlot::LEGS):
		result = "Leg";
		break;
	case (EquipmentSlot::GLOVES):
		result = "Glove";
		break;
	default:
		result = "Unknown";
		break;
	}
	return result;
}