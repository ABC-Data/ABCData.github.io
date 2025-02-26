/******************************************************************************/
/*!
\file		Followmouse.h
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

namespace Followmouse {
	std::string SKey{ "Followmouse" };

	void Init(GameObject* GO, float dt) {
		float new_x = (float)(Input::GetMouse().x / (float)((float)GLSetup::current_width / (float)GLSetup::width));
		float new_y = (float)((GLSetup::current_height - Input::GetMouse().y) / (float)((float)GLSetup::current_height / (float)GLSetup::height));
		GO->GetComponent<Transform>()->Position = { new_x, new_y };
		(void)dt;
	}

	void Update(GameObject* GO, float dt) {
		(void)dt;
		float new_x = (float)(Input::GetMouse().x / (float)((float)GLSetup::current_width / (float)GLSetup::width));
		float new_y = (float)((GLSetup::current_height - Input::GetMouse().y) / (float)((float)GLSetup::current_height / (float)GLSetup::height));
		GO->GetComponent<Transform>()->Position = { new_x, new_y };
	}

	void End(GameObject* GO, float dt) {
		(void)dt;
		(void)GO;
	}
}