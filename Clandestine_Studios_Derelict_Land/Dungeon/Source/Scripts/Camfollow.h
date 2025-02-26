/******************************************************************************/
/*!
\file		Camfollow.h
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	Nov 15, 2022
\brief		Contains camera lerp follow script

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "Logic/Script.h"
#include "Logic/Logic.h"
#include "Transform/Transform.h"
#include "CoreSys/Core.h"

namespace Camfollow {
	std::string SKey{ "Camfollow" };

	void Init(GameObject* GO, float dt) {
		(void)dt;
		GameObject* Cam = Engine->GetGameObjectAdvanced(GOSearchPattern::PREFIX, GOSearchComponent::WITH_COMPONENT, ComponentType::Transform, std::string("Camera"));
		Transform* SelfT = dynamic_cast<Transform*>(GO->GetComponent(ComponentType::Transform));
		if (SelfT != nullptr && Cam != nullptr) {
			//All good
			return;
		}
		else {
			LogicComponent* LC = dynamic_cast<LogicComponent*>(GO->GetComponent(ComponentType::Logic));
			//This must always work, otherwise how is this function even being called?
			//Do a simple check anyway for safety, but UDB if it's actually nullptr
			if (LC != nullptr) {
				std::cout << GO->GetName() << " couldn't properly initialise Camfollow script, removing.\n";
				LC->RemoveScriptKey(SKey);
			}
		}
	}

	void Update(GameObject* GO, float dt) {
		GameObject* Cam = Engine->GetObjectByPrefix("Camera");
		Transform* SelfT = dynamic_cast<Transform*>(GO->GetComponent(ComponentType::Transform));
		Transform* OtherT = dynamic_cast<Transform*>(Cam->GetComponent(ComponentType::Transform));
		Vec2 BoundedPos = { SelfT->Position.x, OtherT->Position.y };
		BoundedPos.x = SelfT->Position.x < Engine->GetLevel()->GetLevelMin() ? Engine->GetLevel()->GetLevelMin() : SelfT->Position.x > Engine->GetLevel()->GetLevelMax() ? Engine->GetLevel()->GetLevelMax() : SelfT->Position.x;
		Vec2 distance = BoundedPos - OtherT->Position;
		distance = distance * (dt / 0.15f);
		OtherT->Position += distance;
	}

	void End(GameObject* GO, float dt) {
		(void)dt;
		(void)GO;
	}
}