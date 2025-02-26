/******************************************************************************/
/*!
\file    AnimationScripts.h
\author  Warren Ang Jun Xuan
\par     email: a.warrenjunxuan@digipen.edu
\date    March 13, 2022
\brief   Scripts for animations

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "CoreSys/Core.h"

namespace PlayerAnimation{
#define Speed 1350.f
	bool sfx = false, btns = false, playonce = false, fadetrigger = false;
float prevPos{};
int teamlogoCounter{}, gamelogoCounter{};
/**************************************************************************/
/*!
  \brief
	Runs the animation once.

  \param animation
	Animate component to play.
*/
/**************************************************************************/
	bool PlayAnimation(Animate* animation) {		

		if (MasterObjectList->MasterAssets.GetSprite(animation->GetTexHandle()).idx == MasterObjectList->MasterAssets.GetSprite(animation->GetTexHandle()).frames_per_row - 1) {
			animation->SetVisibility(false);
			animation->SetIsPlaying(false);
			return false;
		}
		else {
			if (!animation->GetIsPlaying())
				animation->SetIsPlaying(true);
			if (!animation->GetVisibility())
				animation->SetVisibility(true);
		}
		return true;
	}
/**************************************************************************/
/*!
  \brief
	Checks to stop animation.

  \param animation
	Animate component to check and stop.
*/
/**************************************************************************/
	void CheckStopAnimation(Animate* animation) {
		if (MasterObjectList->MasterAssets.GetSprite(animation->GetTexHandle()).idx == MasterObjectList->MasterAssets.GetSprite(animation->GetTexHandle()).frames_per_row - 1) {
			animation->SetVisibility(false);
			animation->SetIsPlaying(false);
			MasterObjectList->MasterAssets.GetSprite(animation->GetTexHandle()).idx = 0;
		}
	}
/**************************************************************************/
/*!
  \brief
	Script for playing melee attack animation during combat.

  \param Player
	Player character to animate.

  \param Enemy
	Enemy character to animate.

  \param Crit
	Check if crit.

  \param Miss
	Check if miss.

  \param Shake
	Set shake to true if crit.

  \param dt
	delta time.
*/
/**************************************************************************/
	bool Attack(GameObject* Player, GameObject* Enemy, bool Crit, bool Miss, bool& Shake, float dt) {
		Animate* pa = dynamic_cast<Animate*>(Player->GetComponent(ComponentType::Animate));
		Transform* pt = dynamic_cast<Transform*>(Player->GetComponent(ComponentType::Transform));
		RigidBody* pr = dynamic_cast<RigidBody*>(Player->GetComponent(ComponentType::RigidBody));

		Transform* et = dynamic_cast<Transform*>(Enemy->GetComponent(ComponentType::Transform));
		Animate* ea = dynamic_cast<Animate*>(Enemy->GetComponent(ComponentType::Animate));
		EnemyCombat* ecb = dynamic_cast<EnemyCombat*>(Enemy->GetComponent(ComponentType::EnemyCombat));

		GameObject* VFX = Engine->GetGameObject("VFX");
		Animate* VFXa = dynamic_cast<Animate*>(VFX->GetComponent(ComponentType::Animate));
		Animate* BlockVFX = dynamic_cast<Animate*>(Engine->GetGameObject("BlockVFX")->GetComponent(ComponentType::Animate));
		Animate* BlockEffect = dynamic_cast<Animate*>(Engine->GetGameObject("BlockEffect")->GetComponent(ComponentType::Animate));

		if (MasterObjectList->MasterAssets.GetSprite(ecb->Name + "_Damage").idx == MasterObjectList->MasterAssets.GetSprite(ecb->Name + "_Damage").frames_per_row - 1
			|| MasterObjectList->MasterAssets.GetSprite(ecb->Name + "_Block").idx == MasterObjectList->MasterAssets.GetSprite(ecb->Name + "_Block").frames_per_row - 1)
			ea->SetTexHandle(ecb->Name + "_Idle");

		CheckStopAnimation(VFXa);
		CheckStopAnimation(BlockVFX);
		CheckStopAnimation(BlockEffect);

		
		if (MasterObjectList->MasterAssets.GetSprite("Yu_Attack").idx == MasterObjectList->MasterAssets.GetSprite("Yu_Attack").frames_per_row - 1) {
			if (pa->GetTexHandle() != "Yu_Run")
				pa->SetTexHandle("Yu_Run");			

			Vec2 DV, NDV;
			DV = pr->BuildDistanceVector(pt->Position, { prevPos - 200.f, pt->Position.y });
			Vec2Normalize(NDV, DV);
			pr->Accelerate(NDV, dt);
			pr->SetVelocity({ -Speed, 0.f });

			if ((DV.x < 0 && pt->Dimensions.x > 0) || (DV.x > 0 && pt->Dimensions.x < 0))
				pt->Dimensions.x = -pt->Dimensions.x;

			pr->Move(pt->Position, dt);

			if (pt->Position.x <= prevPos - 200.f) {
				MasterObjectList->MasterAssets.GetSprite(BlockVFX->GetTexHandle()).idx = MasterObjectList->MasterAssets.GetSprite(BlockVFX->GetTexHandle()).frames_per_row - 1;
				MasterObjectList->MasterAssets.GetSprite(BlockEffect->GetTexHandle()).idx = MasterObjectList->MasterAssets.GetSprite(BlockEffect->GetTexHandle()).frames_per_row - 1;
				CheckStopAnimation(VFXa);
				VFXa->SetVisibility(false);
				CheckStopAnimation(BlockVFX);
				CheckStopAnimation(BlockEffect);
				MasterObjectList->MasterAssets.GetSprite("Yu_Attack").idx = 0;
				if (ea->GetTexHandle() != ecb->Name + "_Idle")
					ea->SetTexHandle(ecb->Name + "_Idle");
				MasterObjectList->MasterAssets.GetSprite(ecb->Name + "_Damage").idx = 0;
				MasterObjectList->MasterAssets.GetSprite(ecb->Name + "_Block").idx = 0;				
				pt->Dimensions.x = -pt->Dimensions.x;
				pt->SetPosition({ prevPos, pt->Position.y });
				pa->SetTexHandle("Yu_Idle");
				prevPos = 0;
				sfx = false;
				return false;
			}			
		}
		else if (pt->Position.x <= et->Position.x - 200.f) {
			if (!prevPos) 
				prevPos = pt->Position.x;
			Vec2 DV, NDV;
			DV = pr->BuildDistanceVector(pt->Position, { et->Position.x - 200.f, pt->Position.y });
			Vec2Normalize(NDV, DV);
			pr->Accelerate(NDV, dt);
			pr->SetVelocity({ Speed, 0.f });

			if (pa->GetTexHandle() != "Yu_Run")
				pa->SetTexHandle("Yu_Run");
			if ((DV.x < 0 && pt->Dimensions.x > 0) || (DV.x > 0 && pt->Dimensions.x < 0))
				pt->Dimensions.x = -pt->Dimensions.x;

			pr->Move(pt->Position, dt);			
		}
		else {
			pr->SetVelocity({ 0.f, 0.f });
			pa->SetTexHandle("Yu_Attack");
			if (Miss) {
				MasterObjectList->MasterAssets.GetSprite(VFXa->GetTexHandle()).idx = 0;
				VFXa->SetTexHandle("Miss");
				PlayerAnimation::PlayAnimation(VFXa);
			}
			if (Crit) {
				Shake = true;
				MasterObjectList->MasterAssets.GetSprite(VFXa->GetTexHandle()).idx = 0;
				VFXa->SetTexHandle("CriticalHit");
				PlayAnimation(VFXa);
			}				
			if (ecb->blocking) {
				ea->SetTexHandle(ecb->Name + "_Block");				
				PlayAnimation(BlockVFX);
				PlayAnimation(BlockEffect);
			}
			else
				ea->SetTexHandle(ecb->Name + "_Damage");

			if (!sfx) {
				if (Crit)
					audio->PlaySfx(8);	//Crit sound
				else
					audio->PlaySfx(9);	//Atk sound

				audio->PlaySfx(16);		//Enemy Dmg sound
				sfx = true;
			}
		}
		return true;
	}
/**************************************************************************/
/*!
  \brief
	Script for playing ranged attack animation during combat.

  \param Player
	Player character to animate.

  \param Enemy
	Enemy character to animate.

  \param Crit
	Check if crit.

  \param Miss
	Check if miss.

  \param Shake
	Set shake to true if crit.

  \param dt
	delta time.
*/
/**************************************************************************/
	bool RangeAttack(GameObject* Player, GameObject* Enemy, bool Crit, bool Miss, bool& Shake, float dt) {
		(void)dt;
		Animate* pa = dynamic_cast<Animate*>(Player->GetComponent(ComponentType::Animate));

		Animate* ea = dynamic_cast<Animate*>(Enemy->GetComponent(ComponentType::Animate));
		EnemyCombat* ecb = dynamic_cast<EnemyCombat*>(Enemy->GetComponent(ComponentType::EnemyCombat));

		GameObject* VFX = Engine->GetGameObject("VFX");
		Animate* VFXa = dynamic_cast<Animate*>(VFX->GetComponent(ComponentType::Animate));
		Animate* BlockVFX = dynamic_cast<Animate*>(Engine->GetGameObject("BlockVFX")->GetComponent(ComponentType::Animate));
		Animate* BlockEffect = dynamic_cast<Animate*>(Engine->GetGameObject("BlockEffect")->GetComponent(ComponentType::Animate));

		CheckStopAnimation(VFXa);
		CheckStopAnimation(BlockVFX);
		CheckStopAnimation(BlockEffect);

		if (MasterObjectList->MasterAssets.GetSprite("Yu_Attack").idx == MasterObjectList->MasterAssets.GetSprite("Yu_Attack").frames_per_row - 1) {
			MasterObjectList->MasterAssets.GetSprite(BlockVFX->GetTexHandle()).idx = MasterObjectList->MasterAssets.GetSprite(BlockVFX->GetTexHandle()).frames_per_row - 1;
			MasterObjectList->MasterAssets.GetSprite(BlockEffect->GetTexHandle()).idx = MasterObjectList->MasterAssets.GetSprite(BlockEffect->GetTexHandle()).frames_per_row - 1;
			CheckStopAnimation(VFXa);
			VFXa->SetVisibility(false);
			CheckStopAnimation(BlockVFX);
			CheckStopAnimation(BlockEffect);
			MasterObjectList->MasterAssets.GetSprite("Yu_Attack").idx = 0;
			if (ea->GetTexHandle() != ecb->Name + "_Idle")
				ea->SetTexHandle(ecb->Name + "_Idle");
			MasterObjectList->MasterAssets.GetSprite(ecb->Name + "_Damage").idx = 0;
			MasterObjectList->MasterAssets.GetSprite(ecb->Name + "_Block").idx = 0;
			pa->SetTexHandle("Yu_Idle");
			prevPos = 0;
			sfx = false;
			return false;
		}
		else {
			pa->SetTexHandle("Yu_Attack");
			if (Miss) {
				MasterObjectList->MasterAssets.GetSprite(VFXa->GetTexHandle()).idx = 0;
				VFXa->SetTexHandle("Miss");
				PlayerAnimation::PlayAnimation(VFXa);
			}
			if (Crit) {
				Shake = true;
				MasterObjectList->MasterAssets.GetSprite(VFXa->GetTexHandle()).idx = 0;
				VFXa->SetTexHandle("CriticalHit");
				PlayAnimation(VFXa);
			}
			if (ecb->blocking) {
				ea->SetTexHandle(ecb->Name + "_Block");
				PlayAnimation(BlockVFX);
				PlayAnimation(BlockEffect);
			}
			else
				ea->SetTexHandle(ecb->Name + "_Damage");

			if (!sfx) {
				if (Crit)
					audio->PlaySfx(8);	//Crit sound
				else
					audio->PlaySfx(9);	//Atk sound

				audio->PlaySfx(16);		//Enemy Dmg sound
				sfx = true;
			}
		}		
		return true;
	}
/**************************************************************************/
/*!
  \brief
	Script for playing Yu's death.

  \param Yu
	Animate component to play.

*/
/**************************************************************************/
	bool YuDeath(Animate* Yu) {
		if (Yu->GetTexHandle() != "Yu_Death")
			Yu->SetTexHandle("Yu_Death");

		if (MasterObjectList->MasterAssets.GetSprite("Yu_Death").idx == MasterObjectList->MasterAssets.GetSprite("Yu_Death").frames_per_row - 1) {
			Yu->SetVisibility(false);
			return false;
		}
			
		return true;
	}
/**************************************************************************/
/*!
  \brief
	Script for playing Enemy's death.

  \param Ghoul
	Animate component to play.

  \param EnemyName
	Enemy's name.

*/
/**************************************************************************/
	bool GhoulDeath(Animate* Ghoul, std::string EnemyName) {
		GameObject* VFX = Engine->GetGameObject("VFX");
		Animate* VFXa = dynamic_cast<Animate*>(VFX->GetComponent(ComponentType::Animate));
		VFXa->SetVisibility(false);

		if (Ghoul->GetTexHandle() != (EnemyName + "_Death"))
			Ghoul->SetTexHandle((EnemyName + "_Death"));

		if (MasterObjectList->MasterAssets.GetSprite((EnemyName + "_Death")).idx == MasterObjectList->MasterAssets.GetSprite((EnemyName + "_Death")).frames_per_row - 1) {			
			Ghoul->SetVisibility(false);
			return false;
		}
			
		return true;
	}
/**************************************************************************/
/*!
  \brief
	Script for fading alpha value

  \param image
	Image to fade.

  \param text
	Text to fade.

*/
/**************************************************************************/
	bool AlphaFade(Renderer* image, GameUI* text) {
		if (image->Alpha_0to1 < 0 || text->Alpha < 0) {
			image->Alpha_0to1 = 0;
			text->Alpha = 0;
			image->SetVisibility(false);
			text->SetFontVisibility(false);
			return false;
		}

		if (!image->GetVisibility())
			image->SetVisibility(true);
		if (!text->GetFontVisibility()) 
			text->SetFontVisibility(true);		

		if (image->Alpha_0to1 >= 1)
			fadetrigger = true;

		if (!fadetrigger){
			image->Alpha_0to1 += 0.001f;
			text->Alpha = image->Alpha_0to1;
		}
		else {
			image->Alpha_0to1 -= 0.001f;
			text->Alpha = image->Alpha_0to1;
		}			

		if (image->Alpha_0to1 <= 0) {
			image->Alpha_0to1 = 0;
			text->Alpha = 0;
			image->SetVisibility(false);
			text->SetFontVisibility(false);
			return false;
		}

		return true;
	}
/**************************************************************************/
/*!
  \brief
	Script for main menu animation
*/
/**************************************************************************/
	bool MainMenuAnimation() {
		Animate* TeamLogoa = dynamic_cast<Animate*>(Engine->GetGameObject("TeamLogo")->GetComponent(ComponentType::Animate));
		Animate* GameLogoa = dynamic_cast<Animate*>(Engine->GetGameObject("GameLogo")->GetComponent(ComponentType::Animate));
		Animate* MenuFadea = dynamic_cast<Animate*>(Engine->GetGameObject("MenuFade")->GetComponent(ComponentType::Animate));
		Renderer* DigiLogo = dynamic_cast<Renderer*>(Engine->GetGameObject("DigipenLogo")->GetComponent(ComponentType::Renderer));
		GameUI* DigiText = dynamic_cast<GameUI*>(Engine->GetGameObject("DigipenLogo")->GetComponent(ComponentType::GameUI));

		if (!playonce) {
			if (!AlphaFade(DigiLogo, DigiText)) {
				if (!PlayAnimation(TeamLogoa)) {
					if (!PlayAnimation(GameLogoa)) {
						Renderer* BGr = dynamic_cast<Renderer*>(Engine->GetGameObject("Background")->GetComponent(ComponentType::Renderer));
						BGr->SetVisibility(true);
						if (!PlayAnimation(MenuFadea)) {
							if (!btns) {
								btns = true;

								Renderer* Titler = dynamic_cast<Renderer*>(Engine->GetGameObject("GameTitle")->GetComponent(ComponentType::Renderer));
								Titler->SetVisibility(true);

								GameObject* text = Engine->GetGameObject("Start Game Btn");
								GameUI* ui_text = dynamic_cast<GameUI*>(text->GetComponent(ComponentType::GameUI));
								Renderer* btn = dynamic_cast<Renderer*>(text->GetComponent(ComponentType::Renderer));
								ui_text->SetFontVisibility(true);
								btn->SetVisibility(true);

								text = Engine->GetGameObject("Quit Game Btn");
								ui_text = dynamic_cast<GameUI*>(text->GetComponent(ComponentType::GameUI));
								btn = dynamic_cast<Renderer*>(text->GetComponent(ComponentType::Renderer));
								ui_text->SetFontVisibility(true);
								btn->SetVisibility(true);

								text = Engine->GetGameObject("Options Btn");
								ui_text = dynamic_cast<GameUI*>(text->GetComponent(ComponentType::GameUI));
								btn = dynamic_cast<Renderer*>(text->GetComponent(ComponentType::Renderer));
								ui_text->SetFontVisibility(true);
								btn->SetVisibility(true);
							}
							MenuFadea->SetVisibility(true);
							playonce = true;
							return false;
						}
					}
				}
			}			

			if (MasterObjectList->MasterAssets.GetSprite(TeamLogoa->GetTexHandle()).idx > 10 && TeamLogoa->GetIsPlaying()) {
				GameLogoa->SetVisibility(true);
				if (teamlogoCounter <= 750) {
					TeamLogoa->SetIsPlaying(false);
					++teamlogoCounter;
				}
			}
			else if (MasterObjectList->MasterAssets.GetSprite(GameLogoa->GetTexHandle()).idx < 2 && GameLogoa->GetIsPlaying()) {
				if (gamelogoCounter <= 750) {
					GameLogoa->SetIsPlaying(false);
					++gamelogoCounter;
				}
			}
		}
		else
			return false;
		return true;
	}
/**************************************************************************/
/*!
  \brief
	Script for smoke animation
*/
/**************************************************************************/
	void SmokeAnimation() {
		Transform* Smoke1 = dynamic_cast<Transform*>(Engine->GetGameObject("Smoke1")->GetComponent(ComponentType::Transform));
		Transform* Smoke2 = dynamic_cast<Transform*>(Engine->GetGameObject("Smoke2")->GetComponent(ComponentType::Transform));

		if (Smoke1->Position.x >= 17310) {
			Smoke1->Position.x = -8734;
		}
		if (Smoke2->Position.x >= 17310) {
			Smoke2->Position.x = -8734;
		}

		Smoke1->Position.x += 1;
		Smoke2->Position.x += 1;
	}
}
