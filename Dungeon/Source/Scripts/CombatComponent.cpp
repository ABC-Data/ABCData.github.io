/******************************************************************************/
/*!
\file		CombatComponent.cpp
\author 	Lim Hui Ching
\par    	email: l.huiching\@digipen.edu, 620004221
\date   	Jan 16, 2023
\brief		Player component source file.

	Copyright (C) 2022 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "CombatComponent.h"

/*!*************************************************************************
****
\brief
	PlayerCombat Constructor
****************************************************************************
***/
PlayerCombat::PlayerCombat() : physicalRES{ 0.1f }, ShockRES{ false }, bleedRES{ false }, recoverySpd{ 0.08f }, FleeChance{ 0.4f },
currentAP{ 100.f }, currentHP{ 100 }, maxAP{ 100.f }, maxHP{ 100 }, blocking{ false }, playerAttacking{ false }, BlockDamageReduction{ 0.5f },
FleeSuccess{ false }, InCombat{ true }, TotalDamageDealt{ 0 }, AudioCounter{ 0 }, ATKDelayTimerLimit{ 0.f }, TooltipDimensions{ 0.f,0.f },
TooltipMax{ 0.f,0.f }, TooltipMin{ 0.f,0.f }, TooltipHandle{ "" }, stunChance{ 0.5f }, bleedChance{ 0.5f }{	SetType(ComponentType::PlayerCombat); }

/*!*************************************************************************
****
\brief
	Resets Player Combat component when retry after defeat.
****************************************************************************
***/
void PlayerCombat::ResetPlayerCombat(int HP) {
	this->BlockDamageReduction = 0.5f;
	this->blocking = false;
	this->currentAP = this->maxAP;
	this->currentHP = HP;
	this->FleeChance = 0.4f;
	this->FleeSuccess = false;
	this->InCombat = true;
	this->playerAttacking = false;
	this->ShockRES = false;
	this->TotalDamageDealt = 0;
	this->AudioCounter = 0;
}

/*!*************************************************************************
****
\brief
	Calculates new scale of the AP bar.
\param[in] maxscale
	Maximum scale of the AP bar.
\param[in] percentage
	Current AP percentage.
\return
	New Scale.
****************************************************************************
***/
float PlayerCombat::CalculateScale(float maxscale, float percentage, float max) {
	if (percentage > max) {
		percentage = max;
	}
	return static_cast<float>(maxscale / max * percentage);
}

/*!*************************************************************************
****
\brief
	Calculates position increment of the AP bar.
\param[in] OldScale
	Current scale of the AP bar.
\param[in] NewScale
	New Scale.
\return
	Position increment of the AP bar.
****************************************************************************
***/
float PlayerCombat::CalculateScaleTransform(float OldScale, float NewScale) {
	float ScaleDiff = OldScale - NewScale;
	float STdivideBy = 200.0f;

	return -(ScaleDiff / STdivideBy);
}

/*!*************************************************************************
****
\brief
	Checks if weapon hits enemy.
\param[in] weaponHitChance
	Hit Chance of chosen weapon.
\return
	Result of check.
****************************************************************************
***/
bool PlayerCombat::CheckIfHit(float weaponHitChance)
{
	float randomValue = static_cast<float> (rand() % 100 + 1.0f);
	randomValue /= 100.f;

	if (randomValue <= weaponHitChance)
	{
		playerAttacking = true;
		return true;
	}
	else { audio->PlaySfx(10); } // Miss - Both Attack and Getting Hit.mp3
	return false;
}

/*!*************************************************************************
****
\brief
	Checks if weapon hit on enemy is critical.
\param[in] weaponCritChance
	Critical Hit Chance of chosen weapon.
\return
	Result of check.
****************************************************************************
***/
bool PlayerCombat::CheckIfCrit(float weaponCritChance)
{
	float randomValue = static_cast<float> (rand() % 100 + 1.0f);
	randomValue /= 100.f;

	if (randomValue <= weaponCritChance) { return true; }
	else { return false; }
}

/*!*************************************************************************
****
\brief
	For serialization and deserialization of scene file
****************************************************************************
***/
void PlayerCombat::SetPlayerCombat(float RS, int cHP, int mHP, float PR, float FC, float BDR, float cAP, float mAP, float ADTL) {
	recoverySpd = RS;
	currentAP = cAP;
	maxAP = mAP;
	currentHP = cHP;
	maxHP = mHP;
	physicalRES = PR;
	FleeChance = FC;
	BlockDamageReduction = BDR;
	ATKDelayTimerLimit = ADTL;
}

/*!*************************************************************************
****
\brief
	Serializes combat component.
\param[in] fp
	Pointer to file.
****************************************************************************
***/
void PlayerCombat::Serialize(FILE* fp) {
	fprintf(fp, "PlayerCombat\n");
	fprintf(fp, "Tooltip Texture Handle: %s\n", TooltipHandle.c_str());
	fprintf(fp, "Tooltip Texture UV MIN: %f, %f\n", TooltipMin.x, TooltipMin.y);
	fprintf(fp, "Tooltip Texture UV MAX: %f, %f\n", TooltipMax.x, TooltipMax.y);
	fprintf(fp, "Tooltip Dimensions: %f, %f\n", TooltipDimensions.x, TooltipDimensions.y);
	fprintf(fp, "Current, Max HP: %d, %d\n", currentHP, maxHP);
	fprintf(fp, "Current, Max AP: %f, %f\n", currentAP, maxAP);
	fprintf(fp, "Recovery Speed: %f\n", recoverySpd);
	fprintf(fp, "Physical Resistance: %f\n", physicalRES);
	fprintf(fp, "Flee, Stun, Bleed Chance: %f, %f, %f\n", FleeChance, stunChance, bleedChance);
	fprintf(fp, "Block Damage Reduction: %f\n", BlockDamageReduction);
	fprintf(fp, "Player Attack Delay Timer Limit: %f\n", ATKDelayTimerLimit);
}

/*!*************************************************************************
	****
	\brief
		Deserializes combat component.
	\param[in] fp
		Pointer to file.
	****************************************************************************
	***/
void PlayerCombat::Deserialize(FILE* fp) {
	char TTTH[50]{ 0 };
	Vec2 TTMin, TTMax, TTDimensions;
	int cHP, mHP;
	float RS, PR, FC, BDR, cAP, mAP, ADTL, BC, SC;
	int success = 1;

	success *= fscanf(fp, "Tooltip Texture Handle: %s\n", &TTTH);
	success *= fscanf(fp, "Tooltip Texture UV MIN: %f, %f\n", &TTMin.x, &TTMin.y);
	success *= fscanf(fp, "Tooltip Texture UV MAX: %f, %f\n", &TTMax.x, &TTMax.y);
	success *= fscanf(fp, "Tooltip Dimensions: %f, %f\n", &TTDimensions.x, &TTDimensions.y);

	success *= fscanf(fp, "Current, Max HP: %i, %i\n", &cHP, &mHP);
	success *= fscanf(fp, "Current, Max AP: %f, %f\n", &cAP, &mAP);
	success *= fscanf(fp, "Recovery Speed: %f\n", &RS);
	success *= fscanf(fp, "Physical Resistance: %f\n", &PR);
	success *= fscanf(fp, "Flee, Stun, Bleed Chance: %f, %f, %f\n", &FC, &SC, &BC);
	success *= fscanf(fp, "Block Damage Reduction: %f\n", &BDR);
	success *= fscanf(fp, "Player Attack Delay Timer Limit: %f\n", &ADTL);

	if (!success) {
		std::cout << "Serialize PlayerCombat Failed!";
		SetPlayerCombat(0.f, 0, 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f);
		TooltipDimensions = { 0.f,0.f };
		TooltipMax = { 0.f,0.f };
		TooltipMin = { 0.f,0.f };
		TooltipHandle = "fail";
		stunChance = 0.f;
		bleedChance = 0.f;
		return;
	}
	SetPlayerCombat(RS, cHP, mHP, PR, FC, BDR, cAP, mAP, ADTL);
	TooltipDimensions = TTDimensions;
	TooltipHandle = TTTH;
	TooltipMin = TTMin;
	TooltipMax = TTMax;
	stunChance = SC;
	bleedChance = BC;
}

/*!*************************************************************************
****
\brief
	Copies data of combat component(to be used for clone function).
\param[in] Target
	Pointer to Component to copy from.
****************************************************************************
***/
void PlayerCombat::CopyData(Component* Target) {
	PlayerCombat* P = dynamic_cast<PlayerCombat*>(Target);
	if (P != nullptr)
	{
		this->stunChance = P->stunChance;
		this->bleedChance = P->bleedChance;

		this->TooltipDimensions = P->TooltipDimensions;
		this->TooltipHandle = P->TooltipHandle;
		this->TooltipMin = P->TooltipMin;
		this->TooltipMax = P->TooltipMax;

		// Integer variables
		this->currentHP = P->currentHP;							// 3
		this->maxHP = P->maxHP;									// 4
		this->TotalDamageDealt = P->TotalDamageDealt;			// 5
		this->AudioCounter = P->AudioCounter;					// 6

		// Bools
		this->blocking = P->blocking;							// 7
		this->playerAttacking = P->playerAttacking;				// 8
		this->ShockRES = P->ShockRES;							// 9
		this->bleedRES = P->bleedRES;							// 10
		this->FleeSuccess = P->FleeSuccess;						// 11
		this->InCombat = P->InCombat;							// 12

		// Floats
		this->physicalRES = P->physicalRES;						// 14
		this->FleeChance = P->FleeChance;						// 15
		this->BlockDamageReduction = P->BlockDamageReduction;	// 16
		this->currentAP = P->currentAP;							// 17
		this->maxAP = P->maxAP;									// 18
		this->recoverySpd = P->recoverySpd;						// 19
		this->ATKDelayTimerLimit = P->ATKDelayTimerLimit;		// 20
	}
}