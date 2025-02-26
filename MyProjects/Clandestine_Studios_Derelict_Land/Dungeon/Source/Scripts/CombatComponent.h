/******************************************************************************/
/*!
\file		CombatComponent.h
\author 	Lim Hui Ching
\par    	email: l.huiching\@digipen.edu, 620004221
\date   	Nov 16, 2022
\brief		Combat component.

	Copyright (C) 2022 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "Essential.h"
#include "GameObject/GameObject.h"
#include "Audio/Audio.h"

class PlayerCombat : public Component {
public:
	/*!*************************************************************************
	****
	\brief
		PlayerCombat Constructor
	****************************************************************************
	***/
	PlayerCombat();

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
	float CalculateScale(float maxscale, float percentage, float max);

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
	float CalculateScaleTransform(float OldScale, float NewScale);
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
	bool  CheckIfHit(float weaponHitChance);

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
	bool CheckIfCrit(float weaponCritChance);

	/*!*************************************************************************
	****
	\brief
		Resets Player Combat component when retry after defeat.
	****************************************************************************
	***/
	void ResetPlayerCombat(int HP);
	// 6 + 7 + 7 = 20
	int currentHP, maxHP, TotalDamageDealt, AudioCounter; // 6
	bool blocking, playerAttacking, ShockRES, bleedRES, FleeSuccess, InCombat; // 7
	float physicalRES, FleeChance, BlockDamageReduction, currentAP, maxAP, recoverySpd, ATKDelayTimerLimit; // 7
	float stunChance, bleedChance;
	std::string TooltipHandle;
	Vec2 TooltipMin, TooltipMax, TooltipDimensions;

	/*!*************************************************************************
	****
	\brief
		For serialization and deserialization of scene file
	****************************************************************************
	***/
	void SetPlayerCombat(float RS, int cHP, int mHP, float PR, float FC, float BDR, float cAP, float mAP, float ADTL);

	/*!*************************************************************************
	****
	\brief
		Serializes combat component.
	\param[in] fp
		Pointer to file.
	****************************************************************************
	***/
	virtual	void Serialize(FILE* fp) override;

	/*!*************************************************************************
	****
	\brief
		Deserializes combat component.
	\param[in] fp
		Pointer to file.
	****************************************************************************
	***/
	virtual void Deserialize(FILE* fp) override;

	/*!*************************************************************************
	****
	\brief
		Copies data of combat component(to be used for clone function).
	\param[in] Target
		Pointer to Component to copy from.
	****************************************************************************
	***/
	virtual void CopyData(Component* Target) override;
};
