/******************************************************************************/
/*!
\file		InventoryZoom.h
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

namespace InventoryZoom {
	std::string SKey{ "Inventoryzoom" };

	void Init(GameObject* GO, float dt) {
		GO->GetComponent<Lifetime>()->lifetime = 0.f;
		GO->GetComponent<Transform>()->Scale = { 0.f, 0.f };
		(void)dt;
	}

	void Update(GameObject* GO, float dt) {
		GO->GetComponent<Lifetime>()->lifetime += dt;
		float Life = GO->GetComponent<Lifetime>()->lifetime;
		if (Life <= 1.f) {
			GO->GetComponent<Transform>()->Position.y += (15.f * sinf(3.14159265358979323846f * 1.5f * Life));
			GO->GetComponent<Transform>()->Scale += {75.f * dt, 75.f * dt};
		}
		else if (Life >= 2.f) {
			Vec2 translated_pos = Engine->GetObjectByPrefix("Bag Icon")->GetComponent<Transform>()->Position;
			Vec2 campos = { Batch::camera.GetCameraPosition().x, Batch::camera.GetCameraPosition().y };
			translated_pos += campos;
			Vec2 distance = translated_pos - GO->GetComponent<Transform>()->Position;
			if (Vec2Length(distance) < 60.f) {
				GO->GetComponent<LogicComponent>()->RemoveScriptKey(SKey);
				return;
			}
			if (Life <= 3.f) {
				distance = distance * (dt * 5.f);
			}
			else {
				distance = distance / (Life / 3.5f);
			}
			GO->GetComponent<Transform>()->Position += distance;
			if (GO->GetComponent<Transform>()->Scale.x >= 37.5f) {
				GO->GetComponent<Transform>()->Scale -= {37.5f * dt, 37.5f * dt};
			}
		}
		if (Life > 5.f) {
			GO->GetComponent<LogicComponent>()->RemoveScriptKey(SKey); // Hard clear
			return;
		}
	}

	void End(GameObject* GO, float dt) {
		(void)dt;
		std::cout << "Destroyed\n";
		factory->QueueDelete(GO);
	}
}