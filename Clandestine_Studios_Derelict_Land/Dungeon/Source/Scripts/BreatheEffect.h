#pragma once
/************************************************************************ /
/*!
\file   BreatheEffect.h
\author Brandon Hsu
\par    email: b.hsu\@digipen.edu , 620007421
\date   February 2, 2023
\brief	Breathe effect script for game objects (In particular, interactables)

		Copyright (C) 2023 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "Essential.h"
#include "Logic/Script.h"
#include "Logic/Logic.h"
#include "Renderer/Renderer.h"
#include "Essential.h"

namespace BreatheEffect
{
	std::string SKey{ "BreatheEffect" };
	float total_dt;

	void Init(GameObject* GO, float dt)
	{
		(void)dt;
		total_dt = 0;
		if (!GO->HasComponent(ComponentType::Renderer)) {
			GO->GetComponent<LogicComponent>()->RemoveScriptKey(SKey);
		}
		else {
			//Off by default
			LogicComponent* LC = GO->GetComponent<LogicComponent>();
			LC->SetEnabled(SKey, false);
		}
	}

	void Update(GameObject* GO, float dt)
	{
		total_dt += dt;
		GO->GetComponent<Renderer>()->Alpha_0to1 = 0.8f + cosf(total_dt * 5.f) / 5.f;
	}
	void End(GameObject* GO, float dt)
	{
		(void)dt;
		(void)GO;
		GO->GetComponent<Renderer>()->Alpha_0to1 = 1.f;
	}
};