/******************************************************************************/
/*!
\file		EnemyComponent.cpp
\author 	Lim Hui Ching
\par    	email: l.huiching\@digipen.edu, 620004221
\date   	Jan 16, 2023
\brief		Enemy component source file.

	Copyright (C) 2022 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "EnemyComponent.h"

/*!*************************************************************************
****
\brief
	Constructor for EnemyCombat component.
****************************************************************************
***/
EnemyCombat::EnemyCombat() :physicalRES{ 0.1f }, ShockRES{ 0.1f }, bleedRES{ 0.1f }, maxAP{ 100.f }, currentAP{ 100.f }, currentHP{ 50 },
maxHP{ 50 }, recoverySpd{ 0.08f }, ATK{ 10.f }, Crit_Chance{ 0.1f }, Crit_Multiplier{ 1.5f }, EnemyATKTimer{ 0.0f }, TotalDamageDealt{ 0 },
ItemDrop{ 0 }, blocking{ false }, Hit_Chance{ 0.8f }, ATKTimerFreq{ 3.5f }, TextDisplayDuration{ 2.0f }, ShakeDuration{ 0.5f }, ShakeMagnitude{ 4.0f },
TooltipDimensions{ 0.f,0.f }, TooltipMax{ 0.f,0.f }, TooltipMin{ 0.f,0.f }, TooltipHandle{ "" }{SetType(ComponentType::EnemyCombat); }

/*!*************************************************************************
****
\brief
	For serialization and deserialization of scene file
****************************************************************************
***/
void EnemyCombat::SetEnemyCombat(std::string N, float A, int cHP, int mHP, int ID1, int ID2, int ID3, float cAP, float mAP, float PR, float RS, float CHC, float CHM, float EAT, float HC)
{
	Name = N;
	ATK = A;
	currentHP = cHP;
	maxHP = mHP;
	ItemDrop[0] = ID1;
	ItemDrop[1] = ID2;
	ItemDrop[2] = ID3;
	currentAP = cAP;
	maxAP = mAP;
	physicalRES = PR;
	recoverySpd = RS;
	Crit_Chance = CHC;
	Crit_Multiplier = CHM;
	EnemyATKTimer = EAT;
	Hit_Chance = HC;
}

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
void EnemyCombat::ResetEnemyCombat(float phys, float shock, float bleed) {
	this->currentAP = this->maxAP;
	this->currentHP = this->maxHP;
	this->EnemyATKTimer = -3.0f;
	this->physicalRES = phys;
	this->ShockRES = shock;
	this->bleedRES = bleed;
	this->TotalDamageDealt = 0;
}

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
EnemyCombat::EnemyAttack EnemyCombat::GhoulAttack()
{
	float randomValue = static_cast<float> (rand() % 100 + 1.0f);
	randomValue /= 100.f;
	if (Name == "Ghoul") {
		if (currentHP >= maxHP / 100 * 75) {
			if (randomValue <= 0.2f) { return EnemyAttack::Fast; }
			else if (randomValue <= 0.7f) { return  EnemyAttack::Strong; }
			else if (randomValue <= 0.8f) { return EnemyAttack::StunBleed; }
			else if (randomValue <= 0.9f) { return EnemyAttack::BuffAttack; }
			else { return EnemyAttack::BuffResistance; }
		}
		else if (currentHP >= maxHP / 100 * 50) {
			if (randomValue <= 0.35f) { return  EnemyAttack::Fast; }
			else if (randomValue <= 0.75f) { return EnemyAttack::Strong; }
			else if (randomValue <= 0.84f) { return EnemyAttack::StunBleed; }
			else if (randomValue <= 0.92f) { return EnemyAttack::BuffAttack; }
			else { return EnemyAttack::BuffResistance; }
		}
		else if (currentHP >= maxHP / 100 * 25) {
			if (randomValue <= 0.40f) { return  EnemyAttack::Fast; }
			else if (randomValue <= 0.75f) { return EnemyAttack::Strong; }
			else if (randomValue <= 0.84f) { return EnemyAttack::StunBleed; }
			else if (randomValue <= 0.91f) { return EnemyAttack::BuffAttack; }
			else { return EnemyAttack::BuffResistance; }
		}
		else {
			if (randomValue <= 0.50f) { return  EnemyAttack::Fast; }
			else if (randomValue <= 0.75f) { return EnemyAttack::Strong; }
			else if (randomValue <= 0.83f) { return EnemyAttack::StunBleed; }
			else if (randomValue <= 0.90f) { return EnemyAttack::BuffAttack; }
			else { return EnemyAttack::BuffResistance; }
		}
	}
	else {
		if (currentHP >= maxHP / 100 * 75) {
			if (randomValue <= 0.45f) { return EnemyAttack::Strong; }
			else if (randomValue <= 0.65f) { return  EnemyAttack::Fast; }
			else if (randomValue <= 0.75f) { return EnemyAttack::StunBleed; }
			else if (randomValue <= 0.85f) { return EnemyAttack::BuffAttack; }
			else { return EnemyAttack::BuffResistance; }
		}
		else if (currentHP >= maxHP / 100 * 50) {
			if (randomValue <= 0.45f) { return  EnemyAttack::Strong; }
			else if (randomValue <= 0.7f) { return EnemyAttack::Fast; }
			else if (randomValue <= 0.8f) { return EnemyAttack::StunBleed; }
			else if (randomValue <= 0.9f) { return EnemyAttack::BuffAttack; }
			else { return EnemyAttack::BuffResistance; }
		}
		else if (currentHP >= maxHP / 100 * 25) {
			if (randomValue <= 0.55f) { return  EnemyAttack::Strong; }
			else if (randomValue <= 0.85f) { return EnemyAttack::Fast; }
			else if (randomValue <= 0.9f) { return EnemyAttack::StunBleed; }
			else if (randomValue <= 0.95f) { return EnemyAttack::BuffAttack; }
			else { return EnemyAttack::BuffResistance; }
		}
		else {
			if (randomValue <= 0.55f) { return  EnemyAttack::Strong; }
			else if (randomValue <= 0.9f) { return EnemyAttack::Fast; }
			else { return EnemyAttack::StunBleed; }
		}
	}
}

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
void EnemyCombat::LoadBackground(std::string& background, Vec2& Min, Vec2& Max, Renderer* floor, Transform* ground, Transform* backgroundPos)
{
	FILE* pFile;
	pFile = fopen("./Data/Enemies/2.txt", "r");
	if (!pFile) throw std::runtime_error("Can't open file");
	if (pFile != NULL)
	{
		int success = 1;

		char FTH[50]{ 0 };
		Vec2 fUVmin, fUVmax, gPos, bgPos;

		char N[50]{ 0 };
		char BTH[50]{ 0 };
		float minx, miny, maxx, maxy;
		success *= fscanf(pFile, "Name: %s\n", &N);

		success *= fscanf(pFile, "Background Texture Handle: %s\n", &BTH);
		success *= fscanf(pFile, "Position: %f, %f\n", &bgPos.x, &bgPos.y);

		success *= fscanf(pFile, "Texture UV MIN: %f, %f\n", &minx, &miny);
		success *= fscanf(pFile, "Texture UV MAX: %f, %f\n", &maxx, &maxy);

		success *= fscanf(pFile, "Floor Texture Handle: %s\n", &FTH);
		success *= fscanf(pFile, "Floor UV MIN: %f, %f\n", &fUVmin.x, &fUVmin.y);
		success *= fscanf(pFile, "Floor UV MAX: %f, %f\n", &fUVmax.x, &fUVmax.y);
		success *= fscanf(pFile, "Floor Position: %f, %f\n", &gPos.x, &gPos.y);

		fclose(pFile);
		if (!success) {
			std::cout << "Load Enemy Background Failed!";
			return;
		}
		background = BTH;
		Min.x = minx;
		Min.y = miny;
		Max.x = maxx;
		Max.y = maxy;
		floor->Tex_UV_Min_COORD = fUVmin;
		floor->Tex_UV_Max_COORD = fUVmax;
		ground->Position = gPos;
		backgroundPos->Position = bgPos;
	}
}

/*!*************************************************************************
****
\brief
	Check if enemy hit is critical.
\return
	Result of check.
****************************************************************************
***/
bool EnemyCombat::CheckIfCrit()
{
	float randomValue = static_cast<float> (rand() % 100 + 1.0f);
	randomValue /= 100.f;
	if (randomValue <= Crit_Chance) { return true; }
	return false;
}

/*!*************************************************************************
****
\brief
	Check if enemy hit is successful.
\return
	Result of check.
****************************************************************************
***/
bool EnemyCombat::CheckIfHit()
{
	float randomValue = static_cast<float> (rand() % 100 + 1.0f);
	randomValue /= 100.f;
	if (randomValue <= Hit_Chance) { return true; }
	return false;
}

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
void EnemyCombat::LoadEnemy(std::string filepath, std::string& background, Vec2& Min, Vec2& Max, Renderer* floor, Renderer* profile, Transform* ground, Transform* backgroundPos) {
	FILE* pFile;
	pFile = fopen(filepath.c_str(), "r");
	if (!pFile) throw std::runtime_error("Can't open file");
	if (pFile != NULL)
	{
		int success = 1;

		char FTH[50]{ 0 };
		Vec2 pUVmin, pUVmax, fUVmin, fUVmax, gPos, bgPos;

		char N[50]{ 0 };
		char BTH[50]{ 0 };
		float A, cAP, mAP, PR, RS, HC, CHC, CHM, EAT, minx, miny, maxx, maxy;
		int cHP, mHP, ID1, ID2, ID3;
		success *= fscanf(pFile, "Name: %s\n", &N);
		success *= fscanf(pFile, "Background Texture Handle: %s\n", &BTH);
		success *= fscanf(pFile, "Position: %f, %f\n", &bgPos.x, &bgPos.y);

		success *= fscanf(pFile, "Texture UV MIN: %f, %f\n", &minx, &miny);
		success *= fscanf(pFile, "Texture UV MAX: %f, %f\n", &maxx, &maxy);

		success *= fscanf(pFile, "Floor Texture Handle: %s\n", &FTH);
		success *= fscanf(pFile, "Floor UV MIN: %f, %f\n", &fUVmin.x, &fUVmin.y);
		success *= fscanf(pFile, "Floor UV MAX: %f, %f\n", &fUVmax.x, &fUVmax.y);
		success *= fscanf(pFile, "Floor Position: %f, %f\n", &gPos.x, &gPos.y);

		success *= fscanf(pFile, "Profile UV MIN: %f, %f\n", &pUVmin.x, &pUVmin.y);
		success *= fscanf(pFile, "Profile UV MAX: %f, %f\n", &pUVmax.x, &pUVmax.y);

		success *= fscanf(pFile, "Attack: %f\n", &A);
		success *= fscanf(pFile, "Current, Max HP: %i, %i\n", &cHP, &mHP);
		success *= fscanf(pFile, "Current, Max AP: %f, %f\n", &cAP, &mAP);
		success *= fscanf(pFile, "Physical Resistance: %f\n", &PR);
		success *= fscanf(pFile, "Recovery Speed: %f\n", &RS);
		success *= fscanf(pFile, "Hit Chance: %f\n", &HC);
		success *= fscanf(pFile, "Critical Hit Chance: %f\n", &CHC);
		success *= fscanf(pFile, "Critical Hit Multiplier: %f\n", &CHM);
		success *= fscanf(pFile, "Enemy Attack Timer: %f\n", &EAT);
		success *= fscanf(pFile, "Item Drop: %i, %i, %i\n", &ID1, &ID2, &ID3);

		fclose(pFile);
		if (!success) {
			std::cout << "Load Enemy Failed!";
			SetEnemyCombat("", 0, 0, 0, 0, 0, 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f);
			return;
		}
		SetEnemyCombat(N, A, cHP, mHP, ID1, ID2, ID3, cAP, mAP, PR, RS, CHC, CHM, EAT, HC);
		background = BTH;
		Min.x = minx;
		Min.y = miny;
		Max.x = maxx;
		Max.y = maxy;
		floor->Tex_UV_Min_COORD = fUVmin;
		floor->Tex_UV_Max_COORD = fUVmax;
		profile->Tex_UV_Min_COORD = pUVmin;
		profile->Tex_UV_Max_COORD = pUVmax;
		ground->Position = gPos;
		backgroundPos->Position = bgPos;
	}
}

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
void EnemyCombat::BoarSpecialAbility(float dt, float& timer, int pmaxhp, float pphysres, int& pcurrenthp)
{
	if (Name == "Boar") {
		timer += dt;
		if (timer >= 240.f) {
			timer = 0.f;
			float calculatedDamage = 0.8f * static_cast<float>(pmaxhp) - 100.f * pphysres;
			(pcurrenthp - calculatedDamage) > 0 ? currentHP = 0 : pcurrenthp = 0;
		}
	}
}

/*!*************************************************************************
****
\brief
	Serializes combat component.
\param[in] fp
	Pointer to file.
****************************************************************************
***/
void EnemyCombat::Serialize(FILE* fp) {
	fprintf(fp, "EnemyCombat\n");
	fprintf(fp, "Name: %s\n", Name.c_str());
	fprintf(fp, "Tooltip Texture Handle: %s\n", TooltipHandle.c_str());
	fprintf(fp, "Tooltip Texture UV MIN: %f, %f\n", TooltipMin.x, TooltipMin.y);
	fprintf(fp, "Tooltip Texture UV MAX: %f, %f\n", TooltipMax.x, TooltipMax.y);
	fprintf(fp, "Tooltip Dimensions: %f, %f\n", TooltipDimensions.x, TooltipDimensions.y);
	fprintf(fp, "Attack: %f\n", ATK);
	fprintf(fp, "Current, Max HP: %d, %d\n", currentHP, maxHP);
	fprintf(fp, "Current, Max AP: %f, %f\n", currentAP, maxAP);
	fprintf(fp, "Physical Resistance: %f\n", physicalRES);
	fprintf(fp, "Recovery Speed: %f\n", recoverySpd);
	fprintf(fp, "Hit Chance: %f\n", Hit_Chance);
	fprintf(fp, "Critical Hit Chance: %f\n", Crit_Chance);
	fprintf(fp, "Critical Hit Multiplier: %f\n", Crit_Multiplier);
	fprintf(fp, "Enemy Attack Timer, Frequency: %f, %f\n", EnemyATKTimer, ATKTimerFreq);
	fprintf(fp, "Item Drop: %d, %d, %d\n", ItemDrop[0], ItemDrop[1], ItemDrop[2]);
	fprintf(fp, "Text Display Duration: %f\n", TextDisplayDuration);
	fprintf(fp, "Shake Magnitude, Duration: %f, %f\n", ShakeMagnitude, ShakeDuration);
}

/*!*************************************************************************
****
\brief
	Deserializes combat component.
\param[in] fp
	Pointer to file.
****************************************************************************
***/
void EnemyCombat::Deserialize(FILE* fp) {
	char TTTH[50]{ 0 };
	Vec2 TTMin, TTMax, TTDimensions;

	char N[50]{ 0 };
	int  cHP = 0, mHP = 0, ID1 = 0, ID2 = 0, ID3 = 0;
	float A, cAP, mAP, PR, RS, CHC, CHM, EAT, HC, TDD, EAF, SM, SD;
	int success = 1;
	success *= fscanf(fp, "Name: %s\n", &N);
	success *= fscanf(fp, "Tooltip Texture Handle: %s\n", &TTTH);
	success *= fscanf(fp, "Tooltip Texture UV MIN: %f, %f\n", &TTMin.x, &TTMin.y);
	success *= fscanf(fp, "Tooltip Texture UV MAX: %f, %f\n", &TTMax.x, &TTMax.y);
	success *= fscanf(fp, "Tooltip Dimensions: %f, %f\n", &TTDimensions.x, &TTDimensions.y);
	success *= fscanf(fp, "Attack: %f\n", &A);
	success *= fscanf(fp, "Current, Max HP: %i, %i\n", &cHP, &mHP);
	success *= fscanf(fp, "Current, Max AP: %f, %f\n", &cAP, &mAP);
	success *= fscanf(fp, "Physical Resistance: %f\n", &PR);
	success *= fscanf(fp, "Recovery Speed: %f\n", &RS);
	success *= fscanf(fp, "Hit Chance: %f\n", &HC);
	success *= fscanf(fp, "Critical Hit Chance: %f\n", &CHC);
	success *= fscanf(fp, "Critical Hit Multiplier: %f\n", &CHM);
	success *= fscanf(fp, "Enemy Attack Timer, Frequency: %f, %f\n", &EAT, &EAF);
	success *= fscanf(fp, "Item Drop: %i, %i, %i\n", &ID1, &ID2, &ID3);
	success *= fscanf(fp, "Text Display Duration: %f\n", &TDD);
	success *= fscanf(fp, "Shake Magnitude, Duration: %f, %f\n", &SM, &SD);

	if (!success) {
		std::cout << "Serialize EnemyCombat Failed!";
		SetEnemyCombat("", 0, 0, 0, 0, 0, 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f);
		TextDisplayDuration = 0.f;
		ATKTimerFreq = 0.f;
		ShakeDuration = 0.f;
		ShakeMagnitude = 0.f;
		TooltipDimensions = { 0.f,0.f };
		TooltipMax = { 0.f,0.f };
		TooltipMin = { 0.f,0.f };
		TooltipHandle = "fail";
		return;
	}
	SetEnemyCombat(N, A, cHP, mHP, ID1, ID2, ID3, cAP, mAP, PR, RS, CHC, CHM, EAT, HC);
	TextDisplayDuration = TDD;
	ATKTimerFreq = EAF;
	ShakeDuration = SD;
	ShakeMagnitude = SM;
	TooltipDimensions = TTDimensions;
	TooltipHandle = TTTH;
	TooltipMin = TTMin;
	TooltipMax = TTMax;
}

/*!*************************************************************************
****
\brief
	Copies data of combat component(to be used for clone function).
\param[in] Target
	Pointer to Component to copy from.
****************************************************************************
***/
void EnemyCombat::CopyData(Component* Target) {
	EnemyCombat* E = dynamic_cast<EnemyCombat*>(Target);
	if (E != nullptr)
	{
		this->TooltipDimensions = E->TooltipDimensions;
		this->TooltipHandle = E->TooltipHandle;
		this->TooltipMax = E->TooltipMax;
		this->TooltipMin = E->TooltipMin;

		this->currentHP = E->currentHP; // Integer variables (6)
		this->maxHP = E->maxHP;
		this->TotalDamageDealt = E->TotalDamageDealt;
		this->ItemDrop[0] = E->ItemDrop[0];
		this->ItemDrop[1] = E->ItemDrop[1];
		this->ItemDrop[2] = E->ItemDrop[2];

		this->blocking = E->blocking; // Bool (1)

		this->Name = E->Name; // String (1)

		this->physicalRES = E->physicalRES; // Floats (13)
		this->ShockRES = E->ShockRES;
		this->bleedRES = E->bleedRES;
		this->recoverySpd = E->recoverySpd;
		this->Crit_Chance = E->Crit_Chance;
		this->Crit_Multiplier = E->Crit_Multiplier;
		this->EnemyATKTimer = E->EnemyATKTimer;
		this->currentAP = E->currentAP;
		this->maxAP = E->maxAP;
		this->ATK = E->ATK;
		this->Hit_Chance = E->Hit_Chance;
		this->ATKTimerFreq = E->ATKTimerFreq;
		this->TextDisplayDuration = E->TextDisplayDuration;
		this->ShakeDuration = E->ShakeDuration;
		this->ShakeMagnitude = E->ShakeMagnitude;
	}
}