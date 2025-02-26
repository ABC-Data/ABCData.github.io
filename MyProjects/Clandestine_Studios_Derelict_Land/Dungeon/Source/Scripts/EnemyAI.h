/******************************************************************************/
/*!
\file		EnemyAI.h
\author 	Lim Hui Ching
\par    	email: l.huiching\@digipen.edu, 620004221
\date   	Jan 28, 2023
\brief		EnemyAI Script.

	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "./Logic/Script.h"
#include "Logic/Logic.h"
#include "CombatComponent.h"
#include "Input/Input.h"
#include "CoreSys/Core.h"
#include "Scripts/AnimationScripts.h"

extern bool is_paused;

namespace EnemyAI {
	std::string SKey{ "EnemyAI" };
	float DRTextTimer = 0.0f, EnemyHPMaxScale = 0.f;
	bool displayDR = false, Shake = false;
	float ShakeTimer = 0.f, CHTextTimer = 0.0f, BoarTimer = 0.f;
	bool displayCH = false;
	Vec2 baseCamera = { 0.f,0.f };
	std::string enemyfp;

	/*!*************************************************************************
	****
	\brief
		Initializes EnemyAI script.
	\param[in] GO
		Game Object to apply script on.
	\param[in] dt
		Delta time.
	****************************************************************************
	***/
	void Init(GameObject* GO, float dt) {
		UNREFERENCED_PARAMETER(dt);
		GameObject* Background = Engine->GetGameObject("Background");
		Renderer* bgR = dynamic_cast<Renderer*>(Background->GetComponent(ComponentType::Renderer));
		Transform* bgT = dynamic_cast<Transform*>(Background->GetComponent(ComponentType::Transform));

		GameObject* EnemyProfile = Engine->GetGameObject("Enemy Profile");
		Renderer* epR = dynamic_cast<Renderer*>(EnemyProfile->GetComponent(ComponentType::Renderer));

		GameObject* Ground = Engine->GetGameObject("Ground");
		Renderer* gR = dynamic_cast<Renderer*>(Ground->GetComponent(ComponentType::Renderer));
		Transform* gT = dynamic_cast<Transform*>(Ground->GetComponent(ComponentType::Transform));

		GameObject* Player = Engine->GetGameObject("Player");
		PlayerCombat* PCB = dynamic_cast<PlayerCombat*>(Player->GetComponent(ComponentType::PlayerCombat));
		EnemyCombat* ECB = dynamic_cast<EnemyCombat*>(GO->GetComponent(ComponentType::EnemyCombat));

		GameObject* PHP = Engine->GetGameObject("Yu HP");
		GameObject* Camera = Engine->GetGameObject("Camera WITH SPACE");
		Transform* camT = dynamic_cast<Transform*>(Camera->GetComponent(ComponentType::Transform));

		GameObject* EHP = Engine->GetGameObject("Enemy HP");
		Transform* ehpT = dynamic_cast<Transform*>(EHP->GetComponent(ComponentType::Transform));

		GameObject* VFX = Engine->GetGameObject("PlayerCritEffect");
		Animate* VFXa = dynamic_cast<Animate*>(VFX->GetComponent(ComponentType::Animate));
		VFXa->SetVisibility(false);
		VFXa->SetIsPlaying(false);
		MasterObjectList->MasterAssets.GetSprite(VFXa->GetTexHandle()).idx = 0;

		if (PHP != nullptr && PCB != nullptr) {
			ECB->LoadEnemy(enemyfp, bgR->TexHandle, bgR->Tex_UV_Min_COORD, bgR->Tex_UV_Max_COORD, gR, epR, gT, bgT);
			if (MasterObjectList->StoredController->LevelTag != "Tutorial" && ECB->Name == "Ghoul") {
				ECB->LoadBackground(bgR->TexHandle, bgR->Tex_UV_Min_COORD, bgR->Tex_UV_Max_COORD, gR, gT, bgT);
			}
			ECB->EnemyATKTimer = -6.0f;
			EnemyHPMaxScale = ehpT->Scale.x;
			baseCamera = camT->Position;

			return;
		}
		else {
			LogicComponent* LC = dynamic_cast<LogicComponent*>(GO->GetComponent(ComponentType::Logic));
			//This must always work, otherwise how is this function even being called?
			//Do a simple check anyway for safety, but UDB if it's actually nullptr
			if (LC != nullptr) {
				std::cout << GO->GetName() << " couldn't properly initialise EnemyAI script, removing.\n";
				LC->RemoveScriptKey(SKey);
			}
		}
	}

	/*!*************************************************************************
	****
	\brief
		Updates EnemyAI.
	\param[in] GO
		Game Object to apply script on.
	\param[in] dt
		Delta time.
	****************************************************************************
	***/
	void Update(GameObject* GO, float dt) {
		GameObject* eAP = Engine->GetGameObject("Enemy Action Points");
		Transform* eapT = dynamic_cast<Transform*>(eAP->GetComponent(ComponentType::Transform));

		GameObject* eAPGauge = Engine->GetGameObject("Enemy AP Gauge Outline");
		GameUI* eAPGaugeUI = dynamic_cast<GameUI*>(eAPGauge->GetComponent(ComponentType::GameUI));

		GameObject* Camera = Engine->GetGameObject("Camera WITH SPACE");
		Transform* camT = dynamic_cast<Transform*>(Camera->GetComponent(ComponentType::Transform));

		GameObject* Player = Engine->GetGameObject("Player");
		PlayerCombat* PCB = dynamic_cast<PlayerCombat*>(Player->GetComponent(ComponentType::PlayerCombat));
		EnemyCombat* ECB = dynamic_cast<EnemyCombat*>(GO->GetComponent(ComponentType::EnemyCombat));
		Animate* PA = dynamic_cast<Animate*>(Player->GetComponent(ComponentType::Animate));
		Animate* EA = dynamic_cast<Animate*>(GO->GetComponent(ComponentType::Animate));
		if (ShakeTimer > ECB->ShakeDuration) {
			Shake = false;
			ShakeTimer = 0.f;
		}
		GameObject* PHP = Engine->GetGameObject("Yu HP");
		Transform* phpT = dynamic_cast<Transform*>(PHP->GetComponent(ComponentType::Transform));

		GameObject* EHPGO = Engine->GetGameObject("Enemy HP Gauge Outline");
		GameUI* ehpgoUI = dynamic_cast<GameUI*>(EHPGO->GetComponent(ComponentType::GameUI));
		ECB->EnemyATKTimer += dt;
		std::string EnemyHPText = std::to_string(ECB->currentHP) + "/" + std::to_string(ECB->maxHP);
		ehpgoUI->SetText(EnemyHPText);

		GameObject* DRText = Engine->GetGameObject("Damage Recieved Text");
		GameUI* DRTextUI = dynamic_cast<GameUI*>(DRText->GetComponent(ComponentType::GameUI));

		GameObject* CHText = Engine->GetGameObject("UI Text");
		GameUI* chtextUI = dynamic_cast<GameUI*>(CHText->GetComponent(ComponentType::GameUI));

		GameObject* VFX = Engine->GetGameObject("PlayerCritEffect");
		Animate* VFXa = dynamic_cast<Animate*>(VFX->GetComponent(ComponentType::Animate));
		PlayerAnimation::CheckStopAnimation(VFXa);

		if (displayCH) { CHTextTimer += dt; }
		if (CHTextTimer > ECB->TextDisplayDuration) {
			chtextUI->SetText("");
			CHTextTimer = 0.0f;
		}

		if (displayDR) { DRTextTimer += dt; }
		if (DRTextTimer > ECB->TextDisplayDuration) {
			DRTextUI->SetText("");
			DRTextTimer = 0.0f;
		}

		if (!is_paused && PCB->InCombat) {
			ECB->BoarSpecialAbility(dt, BoarTimer, PCB->maxHP, PCB->physicalRES, PCB->currentHP);

			if (ECB->currentAP < ECB->maxAP && PCB->currentHP > 0 && ECB->currentHP > 0) { ECB->currentAP += ECB->recoverySpd; }
			if (ECB->currentAP > ECB->maxAP) { ECB->currentAP = ECB->maxAP; }

			if (ECB->EnemyATKTimer >= ECB->ATKTimerFreq && ECB->currentHP > 0 && PCB->currentHP > 0) {
				DRTextUI->SetText("-");
				DRTextTimer = 0.0f;
				displayDR = false;
				CHTextTimer = 0.0f;
				displayCH = false;
				chtextUI->SetText("-");

				switch (ECB->GhoulAttack()) {
				case EnemyCombat::EnemyAttack::Fast: // Fast
					if (ECB->CheckIfHit()) {
						if (ECB->currentAP >= 20.f && PCB->currentHP > 0) {
							audio->PlaySfx(10); // Miss - Both Attack and Getting Hit.mp3
							EA->SetTexHandle((ECB->Name + "_Attack"));
							ECB->currentAP -= 20.f;
							float newEnemyDamage = static_cast<float>(ECB->ATK);
							float totalRes = 1.f - PCB->physicalRES;
							if (ECB->CheckIfCrit()) {
								newEnemyDamage = ECB->ATK * ECB->Crit_Multiplier;
								MasterObjectList->MasterAssets.GetSprite(VFXa->GetTexHandle()).idx = 0;
								VFXa->SetTexHandle("CriticalHit");
								PlayerAnimation::PlayAnimation(VFXa);
								Shake = true;
							}
							if (PCB->blocking) {
								totalRes = 1.f - PCB->physicalRES - PCB->BlockDamageReduction;
								PCB->blocking = false;
							}
							else { PA->SetTexHandle("Yu_Damage"); }
							newEnemyDamage *= totalRes;
							if (CombatScript::DamageReductionOn) {
								CombatScript::DamageReductionOn = false;
								newEnemyDamage > 20 ? newEnemyDamage -= 20 : newEnemyDamage = 0;
							}
							ECB->TotalDamageDealt += static_cast<int>(newEnemyDamage);
							PCB->currentHP -= static_cast<int>(newEnemyDamage);
							displayDR = true;
							DRTextUI->SetText("- " + std::to_string(static_cast<int>(newEnemyDamage)) + " HP");
						}
					}
					else {
						MasterObjectList->MasterAssets.GetSprite(VFXa->GetTexHandle()).idx = 0;
						VFXa->SetTexHandle("Miss");
						PlayerAnimation::PlayAnimation(VFXa);
					}
					break;
				case EnemyCombat::EnemyAttack::Strong: // Strong
					if (ECB->CheckIfHit()) {
						if (ECB->currentAP >= 50.f && PCB->currentHP > 0) {
							audio->PlaySfx(10); // Miss - Both Attack and Getting Hit.mp3
							EA->SetTexHandle((ECB->Name + "_Attack"));
							ECB->currentAP -= 50.f;
							float newEnemyDamage = 2.0f * ECB->ATK;
							float totalRes = 1.f - PCB->physicalRES;
							if (ECB->CheckIfCrit()) {
								newEnemyDamage = 2.0f * static_cast<float>(ECB->ATK) * ECB->Crit_Multiplier;
								MasterObjectList->MasterAssets.GetSprite(VFXa->GetTexHandle()).idx = 0;
								VFXa->SetTexHandle("CriticalHit");
								PlayerAnimation::PlayAnimation(VFXa);
								Shake = true;
							}
							if (PCB->blocking) {
								totalRes = 1.f - PCB->physicalRES - PCB->BlockDamageReduction;
								PCB->blocking = false;
							}
							else { PA->SetTexHandle("Yu_Damage"); }
							newEnemyDamage *= totalRes;
							if (CombatScript::DamageReductionOn) {
								CombatScript::DamageReductionOn = false;
								newEnemyDamage > 20 ? newEnemyDamage -= 20 : newEnemyDamage = 0;
							}
							ECB->TotalDamageDealt += static_cast<int>(newEnemyDamage);
							PCB->currentHP -= static_cast<int>(newEnemyDamage);
							displayDR = true;
							DRTextUI->SetText("- " + std::to_string(static_cast<int>(newEnemyDamage)) + " HP");
						}
					}
					else {
						MasterObjectList->MasterAssets.GetSprite(VFXa->GetTexHandle()).idx = 0;
						VFXa->SetTexHandle("Miss");
						PlayerAnimation::PlayAnimation(VFXa);
					}
					break;
				case EnemyCombat::EnemyAttack::StunBleed: // Stun or Bleed
					if (ECB->currentAP >= 25.f) {
						ECB->currentAP -= 25.f;
						audio->PlaySfx(10); // Miss - Both Attack and Getting Hit.mp3
						if (ECB->Name == "Ghoul" || ECB->Name == "CarMimic") {
							EA->SetTexHandle((ECB->Name + "_Attack"));
							PCB->ShockRES = true;
						}
						else if (ECB->Name == "Boar") {
							PCB->bleedRES = true;
						}
						else if (ECB->Name == "Lion") {
							PCB->ShockRES = true;
							PCB->bleedRES = true;
						}
					}
					break;
				case EnemyCombat::EnemyAttack::BuffAttack: // Increase Attack by 5%
					ECB->ATK *= 1.05f;
					displayCH = true;
					chtextUI->SetText("BUFF ATTACK");
					break;
				case EnemyCombat::EnemyAttack::BuffResistance: // Increase Resistance by 5%
					ECB->physicalRES *= 1.05f;
					displayCH = true;
					chtextUI->SetText("BUFF RESISTANCE");
					break;
				}
				phpT->Position.x += PCB->CalculateScaleTransform(phpT->Scale.x, PCB->CalculateScale(EnemyHPMaxScale, static_cast<float>(PCB->currentHP), static_cast<float>(PCB->maxHP)));
				phpT->Scale.x = PCB->CalculateScale(EnemyHPMaxScale, static_cast<float>(PCB->currentHP), static_cast<float>(PCB->maxHP));
				ECB->EnemyATKTimer = 0.0f;

			}
			if (MasterObjectList->MasterAssets.GetSprite(PA->GetTexHandle()).idx == MasterObjectList->MasterAssets.GetSprite(PA->GetTexHandle()).frames_per_row - 1) {
				MasterObjectList->MasterAssets.GetSprite(PA->GetTexHandle()).idx = 0;
				PA->SetTexHandle("Yu_Idle");
			}
			if (MasterObjectList->MasterAssets.GetSprite(EA->GetTexHandle()).idx == MasterObjectList->MasterAssets.GetSprite(EA->GetTexHandle()).frames_per_row - 1) {
				MasterObjectList->MasterAssets.GetSprite(EA->GetTexHandle()).idx = 0;
				EA->SetTexHandle((ECB->Name + "_Idle"));
			}

			if (Shake && PCB->blocking == false) {
				ShakeTimer += dt;
				camT->ShakeObject(ECB->ShakeMagnitude);
			}
			else { camT->Position = baseCamera; }
		}

		eAPGaugeUI->SetText(std::to_string(static_cast<int>(ECB->currentAP)) + "/" + std::to_string(static_cast<int>(ECB->maxAP)));
		eapT->Position.x += PCB->CalculateScaleTransform(eapT->Scale.x, PCB->CalculateScale(EnemyHPMaxScale, ECB->currentAP, ECB->maxAP));
		eapT->Scale.x = PCB->CalculateScale(EnemyHPMaxScale, ECB->currentAP, ECB->maxAP);
	}

	/*!*************************************************************************
	****
	\brief
		Ends EnemyAI script.
	\param[in] GO
		Game Object to apply script on.
	\param[in] dt
		Delta time.
	****************************************************************************
	***/
	void End(GameObject* GO, float dt) {
		(void)GO;
		(void)dt;
	}
}