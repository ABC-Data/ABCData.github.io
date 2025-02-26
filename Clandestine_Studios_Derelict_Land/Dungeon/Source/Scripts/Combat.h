/******************************************************************************/
/*!
\file		Combat.h
\author 	Warren Ang Jun Xuan
\author		Lim Hui Ching
\par    	email: a.warrenjunxuan\@digipen.edu
\par		email: l.huiching\@digipen.edu
\date   	Nov 16, 2022
\brief		Combat Script.

	Copyright (C) 2022 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "./Logic/Script.h"
#include "Logic/Logic.h"
#include "CombatComponent.h"
#include "EnemyComponent.h"
#include "Input/Input.h"
#include "CoreSys/Core.h"
#include "SceneManager/SceneManager.h"
#include "Scripts/AnimationScripts.h"

extern bool is_paused;

namespace CombatScript {
	std::string SKey{ "Combat" };
	float PlayerATKDelayTimer = 0.0f, maxAPscale = 0.f, RegenSpeed = 20.0f, EnemyHPMaxScale = 0.0f, BlockTimer = 0.f, InitialEHPPos = 0.f, APRegenMultiplier = 0.f;
	bool Crit = false, Range = false, Miss = false, ToMainMenu = false, DamageReductionOn = false;
	Vec2 baseCamera = { 0.f,0.f };
	int AttackLandCounter = 0;

	struct TimedObject {
		float Timer;
		bool On;
	};

	struct TimedObject APRegen, DamageDealtText, CamShake, DamageBoost, ImmuneStatus, StunText, BleedText, Bleed, FleeFailText;


	/*!*************************************************************************
	****
	\brief
		Initializes combat script.
	\param[in] GO
		Game Object to apply script on.
	\param[in] dt
		Delta time.
	****************************************************************************
	***/
	void Init(GameObject* GO, float dt) {
		(void)dt;
		GameObject* EHP = Engine->GetGameObject("Enemy HP");
		GameObject* PHP = Engine->GetGameObject("Yu HP");
		GameObject* AP = Engine->GetGameObject("Action Points");

		GameObject* Block = Engine->GetGameObject("Block");
		GameObject* Flee = Engine->GetGameObject("Flee");

		Inventory* IC = dynamic_cast<Inventory*>(GO->GetComponent(ComponentType::Inventory));
		Transform* apT = dynamic_cast<Transform*>(AP->GetComponent(ComponentType::Transform));
		Transform* ehpT = dynamic_cast<Transform*>(EHP->GetComponent(ComponentType::Transform));

		GameObject* Camera = Engine->GetGameObject("Camera WITH SPACE");
		Transform* camT = dynamic_cast<Transform*>(Camera->GetComponent(ComponentType::Transform));

		PlayerCombat* PCB = dynamic_cast<PlayerCombat*>(GO->GetComponent(ComponentType::PlayerCombat));
		PCB->playerAttacking = false;

		GameObject* VFX = Engine->GetGameObject("VFX");
		Animate* VFXa = dynamic_cast<Animate*>(VFX->GetComponent(ComponentType::Animate));
		VFXa->SetVisibility(false);
		VFXa->SetIsPlaying(false);
		MasterObjectList->MasterAssets.GetSprite(VFXa->GetTexHandle()).idx = 0;

		VFX = Engine->GetGameObject("PlayerVFX");
		VFXa = dynamic_cast<Animate*>(VFX->GetComponent(ComponentType::Animate));
		VFXa->SetVisibility(false);
		VFXa->SetIsPlaying(false);
		MasterObjectList->MasterAssets.GetSprite(VFXa->GetTexHandle()).idx = 0;

		VFX = Engine->GetGameObject("PlayerBlockEffect");
		VFXa = dynamic_cast<Animate*>(VFX->GetComponent(ComponentType::Animate));
		VFXa->SetVisibility(false);
		VFXa->SetIsPlaying(false);
		MasterObjectList->MasterAssets.GetSprite(VFXa->GetTexHandle()).idx = 0;

		VFX = Engine->GetGameObject("PlayerBlockVFX");
		VFXa = dynamic_cast<Animate*>(VFX->GetComponent(ComponentType::Animate));
		VFXa->SetVisibility(false);
		VFXa->SetIsPlaying(false);
		MasterObjectList->MasterAssets.GetSprite(VFXa->GetTexHandle()).idx = 0;

		Animate* Bleeda = dynamic_cast<Animate*>(Engine->GetGameObject("BleedVFX")->GetComponent(ComponentType::Animate));
		Bleeda->SetVisibility(false);
		Bleeda->SetIsPlaying(false);

		if (IC != nullptr && AP != nullptr && EHP != nullptr && Block != nullptr && Flee != nullptr && PHP != nullptr) {
			maxAPscale = apT->Scale.x;
			EnemyHPMaxScale = ehpT->Scale.x;
			InitialEHPPos = ehpT->Position.x;
			baseCamera = camT->Position;

			for (std::map<std::string, SetEffect>::iterator it = MasterObjectList->MasterSetEffectList.begin(); it != MasterObjectList->MasterSetEffectList.end(); ++it) {
				switch (IC->VerifySetBonus(it->first)) {
				case 1:
					PCB->maxAP += it->second.AP_Bonus_2;
					PCB->maxHP += it->second.HP_Bonus_2;
					break;
				case 2:
					PCB->maxAP += it->second.AP_Bonus_4;
					PCB->maxHP += it->second.HP_Bonus_4;
					break;
				}
			}

			for (InventoryItem x : IC->Equipment) {
				PCB->physicalRES += x.EStats.Phys_Resist;
				PCB->bleedChance += x.EStats.Bleed_Resist;
				PCB->stunChance += x.EStats.Stun_Resist;
			}

			return;
		}
		else {
			LogicComponent* LC = dynamic_cast<LogicComponent*>(GO->GetComponent(ComponentType::Logic));
			//This must always work, otherwise how is this function even being called?
			//Do a simple check anyway for safety, but UDB if it's actually nullptr
			if (LC != nullptr) {
				std::cout << GO->GetName() << " couldn't properly initialise Combat script, removing.\n";
				LC->RemoveScriptKey(SKey);
			}
		}
	}

	/*!*************************************************************************
	****
	\brief
		Updates combat.
	\param[in] GO
		Game Object to apply script on.
	\param[in] dt
		Delta time.
	****************************************************************************
	***/
	void Update(GameObject* GO, float dt) {
		PlayerATKDelayTimer += dt;
		GameObject* Camera = Engine->GetGameObject("Camera WITH SPACE");
		Transform* camT = dynamic_cast<Transform*>(Camera->GetComponent(ComponentType::Transform));

		GameObject* Player = Engine->GetGameObject("Player");
		GameObject* PHP = Engine->GetGameObject("Yu HP");
		Transform* phpT = dynamic_cast<Transform*>(PHP->GetComponent(ComponentType::Transform));
		GameObject* AP = Engine->GetGameObject("Action Points");
		Transform* apT = dynamic_cast<Transform*>(AP->GetComponent(ComponentType::Transform));
		Inventory* IC = dynamic_cast<Inventory*>(GO->GetComponent(ComponentType::Inventory));
		PlayerCombat* PCB = dynamic_cast<PlayerCombat*>(GO->GetComponent(ComponentType::PlayerCombat));

		Animate* Bleeda = dynamic_cast<Animate*>(Engine->GetGameObject("BleedVFX")->GetComponent(ComponentType::Animate));


		if (MasterObjectList->StoredController->LevelTag == "Tutorial" && PCB->currentHP <= 15) { PCB->physicalRES = 1.f; }
		if (PCB->currentHP <= 0) { // Lose
			PCB->currentHP = 0;
			if (PCB->AudioCounter == 0) {
				audio->PlaySfx(34); // LoseCombat.mp3
				++PCB->AudioCounter;
			}
		}

		///// Status Immune Effect
		if (ImmuneStatus.On) {
			PCB->bleedRES = false;
			PCB->ShockRES = false;
			Bleed.On = false;
			Bleed.Timer = 0.f;
			MasterObjectList->MasterAssets.GetSprite(Bleeda->GetTexHandle()).idx = 0;
			Bleeda->SetVisibility(false);
			Bleeda->SetIsPlaying(false);
		}
		if (ImmuneStatus.On) {
			ImmuneStatus.Timer += dt;
			MasterObjectList->MasterAssets.GetSprite(Bleeda->GetTexHandle()).idx = 0;
			Bleeda->SetVisibility(false);
			Bleeda->SetIsPlaying(false);
		}
		if (ImmuneStatus.Timer > 10.f) {
			ImmuneStatus.On = false;
			ImmuneStatus.Timer = 0.f;
		}

		GameObject* LCHText = Engine->GetGameObject("Left Critical Hit Text");
		GameUI* LCHTextUI = dynamic_cast<GameUI*>(LCHText->GetComponent(ComponentType::GameUI));
		GameObject* Enemy = Engine->GetGameObject("Enemy");
		GameObject* EHP = Engine->GetGameObject("Enemy HP");
		Transform* ehpT = dynamic_cast<Transform*>(EHP->GetComponent(ComponentType::Transform));
		EnemyCombat* ECB = dynamic_cast<EnemyCombat*>(Enemy->GetComponent(ComponentType::EnemyCombat));

		if (StunText.On) { StunText.Timer += dt; }
		if (StunText.Timer > ECB->TextDisplayDuration) {
			LCHTextUI->SetText("-");
			StunText.Timer = 0.0f;
		}

		GameObject* BText = Engine->GetGameObject("Bleed Text");
		GameUI* BTextUI = dynamic_cast<GameUI*>(BText->GetComponent(ComponentType::GameUI));

		if (BleedText.On) { BleedText.Timer += dt; }
		if (BleedText.Timer > ECB->TextDisplayDuration) {
			BTextUI->SetText("-");
			BleedText.Timer = 0.0f;
		}
		//////// Bleed
		if (!ImmuneStatus.On && PCB->bleedRES && (rand() % 100 + 1) <= (PCB->bleedChance * 100.f)) {
			Bleed.On = true;
			PCB->bleedRES = false;
			Bleeda->SetVisibility(true);
			Bleeda->SetIsPlaying(true);
		}

		if (Bleed.On && !ImmuneStatus.On) { Bleed.Timer += dt; }

		if (!ImmuneStatus.On && Bleed.Timer >= 2.f) {
			BTextUI->SetText("BLEED");
			BleedText.On = true;
			Bleed.Timer = 0.f;
			PCB->currentHP -= PCB->maxHP / 100 * 4;
		}

		/////// AP REGEN
		if (APRegen.On) { APRegen.Timer += dt; }
		if (APRegen.Timer >= 2.f) {
			APRegen.Timer = 0.f;
			APRegen.On = false;
			PCB->recoverySpd /= APRegenMultiplier;
			APRegenMultiplier = 0.f;
		}

		////// DAMAGE BOOST
		if (DamageBoost.On) { DamageBoost.Timer += dt; }
		if (DamageBoost.Timer > 2.f) { DamageBoost.On = false; }

		if (PCB->currentHP > PCB->maxHP) { PCB->currentHP = PCB->maxHP; }

		if (CamShake.On && ECB->blocking == false) {
			CamShake.Timer += dt;
			camT->ShakeObject(ECB->ShakeMagnitude);
		}
		else { camT->Position = baseCamera; }

		if (CamShake.Timer > ECB->ShakeDuration) {
			CamShake.On = false;
			CamShake.Timer = 0.f;
		}

		GameObject* VFX = Engine->GetGameObject("VFX");
		Animate* VFXa = dynamic_cast<Animate*>(VFX->GetComponent(ComponentType::Animate));
		PlayerAnimation::CheckStopAnimation(VFXa);

		GameObject* PVFX = Engine->GetGameObject("PlayerVFX");
		Animate* PVFXa = dynamic_cast<Animate*>(PVFX->GetComponent(ComponentType::Animate));
		PlayerAnimation::CheckStopAnimation(PVFXa);

		GameObject* PlayerBlockEffect = Engine->GetGameObject("PlayerBlockEffect");
		Animate* PlayerBlockEffecta = dynamic_cast<Animate*>(PlayerBlockEffect->GetComponent(ComponentType::Animate));
		PlayerAnimation::CheckStopAnimation(PlayerBlockEffecta);

		GameObject* PlayerBlockVFX = Engine->GetGameObject("PlayerBlockVFX");
		Animate* PlayerBlockVFXa = dynamic_cast<Animate*>(PlayerBlockVFX->GetComponent(ComponentType::Animate));
		PlayerAnimation::CheckStopAnimation(PlayerBlockVFXa);

		GameObject* Block = Engine->GetGameObject("Block");
		GameUI* blockUI = dynamic_cast<GameUI*>(Block->GetComponent(ComponentType::GameUI));

		GameObject* Flee = Engine->GetGameObject("Flee");
		GameUI* fleeUI = dynamic_cast<GameUI*>(Flee->GetComponent(ComponentType::GameUI));

		Animate* Player_Anim = dynamic_cast<Animate*>(Player->GetComponent(ComponentType::Animate));
		Animate* Enemy_Anim = dynamic_cast<Animate*>(Enemy->GetComponent(ComponentType::Animate));

		GameObject* DDText = Engine->GetGameObject("Damage Dealt Text");
		GameUI* ddtextUI = dynamic_cast<GameUI*>(DDText->GetComponent(ComponentType::GameUI));

		GameObject* CHText = Engine->GetGameObject("UI Text");
		GameUI* chtextUI = dynamic_cast<GameUI*>(CHText->GetComponent(ComponentType::GameUI));

		GameObject* DRText = Engine->GetGameObject("Damage Recieved Text");
		GameUI* DRTextUI = dynamic_cast<GameUI*>(DRText->GetComponent(ComponentType::GameUI));
		std::vector<GameObject*> InventoryVector = {};
		std::vector<GameUI*> GameUIVector = {};
		Engine->GetAllObjectsByPrefix(InventoryVector, "Inventory Slot ");
		for (int i = 0; i < IC->InventorySize; ++i) {
			GameUIVector.emplace_back(dynamic_cast<GameUI*>(InventoryVector[i]->GetComponent(ComponentType::GameUI)));
		}

		if (!is_paused && PCB->InCombat) {
			int clicked_pos = static_cast<int>(GLSetup::current_height - Input::GetMouse().y);
			//////// Block
			if (PCB->currentHP > 0 && ECB->currentHP > 0 && ((blockUI->MouseHoverClick(Input::GetMouse().x, clicked_pos) && Input::GetMouseTriggered(MouseButton::Button0) && PCB->currentAP >= 50.f) ||
				Input::GetKeyTriggered(Key::B) && PCB->currentAP >= 50.f)) {
				Player_Anim->SetTexHandle("Yu_Block");
				PlayerAnimation::PlayAnimation(PlayerBlockEffecta);
				PlayerAnimation::PlayAnimation(PlayerBlockVFXa);
				PCB->currentAP -= 50.f;
				PCB->blocking = true;
				audio->PlaySfx(15); // Battle_Block.mp3
			}

			if (PCB->blocking) { BlockTimer += dt; }
			if (BlockTimer >= 1.5f) {
				PCB->blocking = false;
				BlockTimer = 0.f;
			}

			if (!ImmuneStatus.On && PCB->ShockRES) {
				PCB->ShockRES = false;
				if ((rand() % 100 + 1) <= (PCB->stunChance * 100.f)) {
					LCHTextUI->SetText("STUN");
					PVFXa->SetTexHandle("Stun");
					PlayerAnimation::PlayAnimation(PVFXa);
					PlayerATKDelayTimer = -2.5f;
					StunText.On = true;
				}
			}

			//////// Flee
			if (PCB->currentHP > 0 && ECB->currentHP > 0 && ((fleeUI->MouseHoverClick(Input::GetMouse().x, clicked_pos) && Input::GetMouseTriggered(MouseButton::Button0) && PCB->currentAP >= 20.f) ||
				Input::GetKeyTriggered(Key::F) && PCB->currentAP >= 20.f)) {
				PCB->currentAP -= 20.f;

				float randomValue = static_cast<float>(rand() % 100 + 1);
				randomValue /= 100;
				if (randomValue <= PCB->FleeChance) {
					std::cout << "Flee success with " << PCB->FleeChance << "\n";
					if (PCB->FleeChance <= 0.6f) { PCB->FleeChance += 0.05f; }
					PCB->FleeSuccess = true;
				}
				else {
					audio->PlaySfx(18);
					fleeUI->SetText("FLEE FAIL");
					FleeFailText.On = true;
				} // Flee_Fail.mp3				
			}

			if (FleeFailText.On) { FleeFailText.Timer += dt; }
			if (FleeFailText.Timer > ECB->TextDisplayDuration) {
				fleeUI->SetText("-");
				FleeFailText.Timer = 0.0f;
				FleeFailText.On = false;
			}

			if (DamageDealtText.On) { DamageDealtText.Timer += dt; }
			if (DamageDealtText.Timer > ECB->TextDisplayDuration) {
				ddtextUI->SetText("-");
				DamageDealtText.Timer = 0.0f;
			}

			if (PCB->playerAttacking) {
				if (!Range) {
					Collision* PCollision = dynamic_cast<Collision*>(Player->GetComponent(ComponentType::Collision));
					if (PCollision->go_is_colliding) {
						if (AttackLandCounter == 0) { Crit ? audio->PlaySfx(13) : audio->PlaySfx(14); } // Battle_AttackLand_Crit.mp3, Battle_AttackLand_Normal.mp3
						++AttackLandCounter;
					}
					else { AttackLandCounter = 0; }
					if (PlayerAnimation::Attack(Player, Enemy, Crit, Miss, CamShake.On, dt)) {
						dynamic_cast<LogicComponent*>(Engine->GetGameObject("Enemy")->GetComponent(ComponentType::Logic))->SetEnabled("EnemyAI", false);
					}
					else {
						dynamic_cast<LogicComponent*>(Engine->GetGameObject("Enemy")->GetComponent(ComponentType::Logic))->SetEnabled("EnemyAI", true);
						PCB->playerAttacking = false;
						ECB->blocking = false;
						CamShake.On = false;
						Miss = false;
					}
				}
				else {
					if (PlayerAnimation::RangeAttack(Player, Enemy, Crit, Miss, CamShake.On, dt)) {
						dynamic_cast<LogicComponent*>(Engine->GetGameObject("Enemy")->GetComponent(ComponentType::Logic))->SetEnabled("EnemyAI", false);
					}
					else {
						dynamic_cast<LogicComponent*>(Engine->GetGameObject("Enemy")->GetComponent(ComponentType::Logic))->SetEnabled("EnemyAI", true);
						Range = false;
						PCB->playerAttacking = false;
						ECB->blocking = false;
						CamShake.On = false;
					}
				}
			}
			else {
				if (PCB->currentAP < PCB->maxAP && PCB->currentHP > 0 && ECB->currentHP > 0) { PCB->currentAP += PCB->recoverySpd; }
				if (PCB->currentAP > PCB->maxAP) { PCB->currentAP = PCB->maxAP; }

				int PlayerDamage = 0;
				DamageDealtText.On = false;
				ddtextUI->SetText("-");
				DamageDealtText.Timer = 0.0f;

				for (int i = 0, key = '1'; i < IC->InventorySize; ++i, ++key) {
					if (key == ':') { key = '0'; }
					if (IC->Invent[i].ID) {
						if (((GameUIVector[i]->MouseHoverClick(Input::GetMouse().x, clicked_pos) && Input::GetMouseTriggered(MouseButton::Button0)) || Input::GetKeyTriggered(key)) && PCB->currentHP > 0 && ECB->currentHP > 0) {
							if (IC->Invent[i].Type == "Consumable" && PCB->currentAP >= 20.f) {
								audio->PlaySfx(30); // General_Button_Clicked_SFX.mp3
								PCB->currentAP -= 20.f; // -AP Part
								switch (IC->Invent[i].CStats.Effect) {
								case EffectMods::HEAL_INSTANT:
									PCB->currentHP += static_cast<int>(IC->Invent[i].CStats.EffectValue);
									phpT->Position.x += PCB->CalculateScaleTransform(phpT->Scale.x, PCB->CalculateScale(EnemyHPMaxScale, static_cast<float>(PCB->currentHP), static_cast<float>(PCB->maxHP)));
									phpT->Scale.x = PCB->CalculateScale(EnemyHPMaxScale, static_cast<float>(PCB->currentHP), static_cast<float>(PCB->maxHP));
									break;
								case EffectMods::HEAL_BLEED:
									PCB->bleedRES = false;
									Bleed.On = false;
									Bleed.Timer = 0.f;
									MasterObjectList->MasterAssets.GetSprite(Bleeda->GetTexHandle()).idx = 0;
									Bleeda->SetVisibility(false);
									Bleeda->SetIsPlaying(false);
									break;
								case EffectMods::HEAL_STUN:
									PCB->ShockRES = false;
									PlayerATKDelayTimer = PCB->ATKDelayTimerLimit;
									break;
								case EffectMods::CRIT_CHANCE_BOOST:
									DamageBoost.On = true;
									break;
								case EffectMods::DAMAGE_REDUCTION_INSTANT:
									DamageReductionOn = true;
									break;
								case EffectMods::AP_REGEN:
									APRegen.On = true;
									APRegenMultiplier = IC->Invent[i].CStats.EffectValue;
									PCB->recoverySpd *= IC->Invent[i].CStats.EffectValue;
									break;
								case EffectMods::STATUS_IMMUNE:
									ImmuneStatus.On = true;
									break;
								default:
									break;
								}
								IC->RemoveItem(i);
								break;
							}
							else if (PCB->currentAP >= IC->Invent[i].WStats.APCost && PlayerATKDelayTimer >= PCB->ATKDelayTimerLimit && IC->Invent[i].Type == "Weapon") {
								audio->PlaySfx(30); // General_Button_Clicked_SFX.mp3
								PCB->currentAP -= IC->Invent[i].WStats.APCost; // -AP Part
								PlayerATKDelayTimer = 0.0f;

								if (IC->Invent[i].WStats.WeaponType == "Range")
									Range = true;

								if (PCB->CheckIfHit(IC->Invent[i].WStats.Accuracy)) {
									PCB->playerAttacking = true;
									PlayerDamage = static_cast<int>(IC->Invent[i].WStats.Damage * (1.f - ECB->physicalRES));
									DamageBoost.On ? Crit = PCB->CheckIfCrit(1.05f * IC->Invent[i].WStats.CritChance) : Crit = PCB->CheckIfCrit(IC->Invent[i].WStats.CritChance);

									if (Crit) {
										PlayerDamage = static_cast<int>(PlayerDamage * IC->Invent[i].WStats.CritMultiplier);
									}

									///// Blocking done by enemy
									if (Enemy_Anim->GetTexHandle() == (ECB->Name + "_Idle")) { // if enemy is not performing a move
										if (ECB->currentAP >= 50.f && ECB->blocking == false && (rand() % 100 + 1) <= 30) {
											ECB->currentAP -= 50.f;
											ECB->blocking = true;
											PlayerDamage /= 2;
										}
									}
									PCB->TotalDamageDealt += PlayerDamage;
									ECB->currentHP -= PlayerDamage;
									ECB->currentHP = ECB->currentHP < 0 ? 0 : ECB->currentHP;
									std::string DDTextString = "- " + std::to_string(PlayerDamage) + " HP";
									ddtextUI->SetText(DDTextString);
									DamageDealtText.On = true;

									ehpT->Position.x += PCB->CalculateScaleTransform(ehpT->Scale.x, PCB->CalculateScale(EnemyHPMaxScale, static_cast<float>(ECB->currentHP), static_cast<float>(ECB->maxHP)));
									ehpT->Scale.x = PCB->CalculateScale(EnemyHPMaxScale, static_cast<float>(ECB->currentHP), static_cast<float>(ECB->maxHP));
								}
								else {
									PCB->playerAttacking = true;
									Miss = true;
								}
								break;
							}
						}
					}
					if (key == '0') { key = ':'; }
				}
				phpT->Position.x += PCB->CalculateScaleTransform(phpT->Scale.x, PCB->CalculateScale(EnemyHPMaxScale, static_cast<float>(PCB->currentHP), static_cast<float>(PCB->maxHP)));
				phpT->Scale.x = PCB->CalculateScale(EnemyHPMaxScale, static_cast<float>(PCB->currentHP), static_cast<float>(PCB->maxHP));
			}

			if (!PCB->playerAttacking && ECB->currentHP <= 0) {
				ECB->currentHP = 0;
				if (PCB->AudioCounter == 0) {
					audio->PlaySfx(35); // WinCombat.mp3
					++PCB->AudioCounter;
				}
				if (!PlayerAnimation::GhoulDeath(Enemy_Anim, ECB->Name)) {
					chtextUI->SetText("");
					ddtextUI->SetText("");
					DRTextUI->SetText("");
					PCB->InCombat = false;
				}
			}
			else if (!PCB->playerAttacking && PCB->currentHP <= 0) {
				if (!PlayerAnimation::YuDeath(Player_Anim)) {
					PCB->InCombat = false;
				}
			}
			apT->Position.x += PCB->CalculateScaleTransform(apT->Scale.x, PCB->CalculateScale(maxAPscale, PCB->currentAP, PCB->maxAP));
			apT->Scale.x = PCB->CalculateScale(maxAPscale, PCB->currentAP, PCB->maxAP);
		}
	}

	/*!*************************************************************************
	****
	\brief
		End combat script.
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