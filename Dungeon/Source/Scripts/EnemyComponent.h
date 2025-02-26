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

class EnemyCombat : public Component {
public:
	/*!*************************************************************************
	****
	\brief
		Constructor for EnemyCombat component.
	****************************************************************************
	***/
	EnemyCombat(); // 11 + 4 + 3 + 3 = 21 variables 
	float physicalRES, ShockRES, bleedRES, recoverySpd, Crit_Chance, Crit_Multiplier, EnemyATKTimer, currentAP, maxAP, ATK, Hit_Chance; // 11
	float ATKTimerFreq, TextDisplayDuration, ShakeDuration, ShakeMagnitude; // 4
	int currentHP, maxHP, TotalDamageDealt; // 3
	int ItemDrop[3];
	std::string Name;
	bool blocking;
	std::string TooltipHandle;
	Vec2 TooltipMin, TooltipMax, TooltipDimensions;


	/*!*************************************************************************
	****
		\brief
			Loads enemy data from Enemies folder for different enemies
		\param filepath
			Filepath of .txt file
		\param background
			Background of scene texture handle
		\param Min
			UV Min of scene background texture
		\param Max
			UV Max of scene background texture
	****************************************************************************
	***/
	void LoadEnemy(std::string filepath, std::string& background, Vec2& Min, Vec2& Max, Renderer* floor, Renderer* Profile, Transform* ground, Transform* backgroundPos);

	/*!*************************************************************************
	****
	\brief
		Boar's Special Ability
	\param[in] dt
		Time elapsed since the last frame
	\param[in] timer
		Reference to a timer that keeps track of the cooldown for this ability
	\param[in] pmaxhp
		Maximum health points of the player
	\param[in] pcurrenthp
		Physical resistance of the player
	****************************************************************************
	***/
	void BoarSpecialAbility(float dt, float& timer, int pmaxhp, float pphysres, int& pcurrenthp);

	/*!*************************************************************************
	****
	\brief
		For serialization and deserialization of scene file
	****************************************************************************
	***/
	void SetEnemyCombat(std::string N, float A, int cHP, int  mHP, int ID1, int  ID2, int ID3, float cAP, float  mAP, float  PR, float  RS,
		float  CHC, float  CHM, float  EAT, float HC);

	/*!*************************************************************************
	****
	\brief
		Resets the EnemyCombat component for retry after defeat
	\param phys
		Initial physical Resistance of Enemy
	\param shock
		Initial shock Resistance of Enemy
	\param bleed
		Initial Bleed Resistance of Enemy
	****************************************************************************
	***/
	void ResetEnemyCombat(float phys, float shock, float bleed);

	enum class EnemyAttack {
		Fast,
		Strong,
		StunBleed,
		BuffAttack,
		BuffResistance
	};
	/*!*************************************************************************
	****
	\brief
		"Rolls dice" to check if attack is:
		1) Fast Attack,
		2) Strong Attack, or
		3) Stun
		4) Increase enemy attack damage
		5) Increase enemy physical resistance
	\return
		Result of "dice roll"
	****************************************************************************
	***/
	EnemyAttack GhoulAttack();

	/*!*************************************************************************
	****
	\brief
		Loads background for ghoul in area 1.
	\param[in] background
		Texture handle of background
	\param[in] Min
		UV min value of background texture handle
	\param[in] Max
		UV max value of background texture handle
	\param[in] floor
		Pointer to Renderer component of floor
	\param[in] ground
		Pointer to Transform component of ground
	\param[in] backgroundPos
		Pointer to Transform component of background
	****************************************************************************
	***/
	void LoadBackground(std::string& background, Vec2& Min, Vec2& Max, Renderer* floor, Transform* ground, Transform* backgroundPos);

	/*!*************************************************************************
	****
	\brief
		Check if enemy hit is critical.
	\return
		Result of check.
	****************************************************************************
	***/
	bool CheckIfCrit();

	/*!*************************************************************************
	****
	\brief
		Check if enemy hit is successful.
	\return
		Result of check.
	****************************************************************************
	***/
	bool CheckIfHit();

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
