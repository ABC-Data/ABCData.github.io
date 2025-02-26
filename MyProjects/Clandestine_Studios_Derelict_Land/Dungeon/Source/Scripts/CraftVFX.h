/******************************************************************************/
/*!
\file		CraftVFX.h
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	Feb 28, 2023
\brief		Inventory item zoom vfx script

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "Essential.h"
#include "Logic/Logic.h"
#include "Logic/Script.h"
#include "GameObject/GameObject.h"
#include "Input/Input.h"
#include "Graphics/BatchRendering.h"
#include "InventoryComponent.h"
#include "CoreSys/Core.h"
#include "GameUI/GameUI.h"
#include "Graphics/GLFWsetup.h"
#include "Transform/Transform.h"
#include "Renderer/Renderer.h"

namespace CraftVFX {
	std::string SKey{ "CraftVFX" };

	void Init(GameObject* GO, float dt) {
		GO->GetComponent<Lifetime>()->lifetime = 0.f;
		Engine->GetObjectByPrefix("ScreenTint")->GetComponent<Renderer>()->SetVisibility(true);
		(void)dt;
	}

	void Update(GameObject* GO, float dt) {
		GO->GetComponent<Lifetime>()->lifetime += dt;
		float Life = GO->GetComponent<Lifetime>()->lifetime;

		//X axis
		if (Life <= 0.5f) {
			GO->GetComponent<Transform>()->Scale.x = (20.f * Life) * (20.f * Life);
		}
		else if (Life <= 1.f) {
			GO->GetComponent<Transform>()->Scale.x = (75.f + ((10.f * (Life - 1.f)) * (10.f * (Life - 1.f))));
		}

		//Y axis
		if (Life <= 0.6f) {
			GO->GetComponent<Transform>()->Scale.y = (20.f * (Life - 0.1f)) * (20.f * (Life - 0.1f));
		}
		else if (Life <= 1.1f) {
			GO->GetComponent<Transform>()->Scale.y = (75.f + ((10.f * (Life - 1.1f)) * (10.f * (Life - 1.1f))));
		}
		if (Life > 2.5f) {
			GO->GetComponent<LogicComponent>()->RemoveScriptKey(SKey);
			return;
		}
	}

	void End(GameObject* GO, float dt) {
		(void)dt;
		Engine->GetObjectByPrefix("ScreenTint")->GetComponent<Renderer>()->SetVisibility(false);
		std::cout << "Destroyed\n";
		factory->QueueDelete(GO);
	}
}