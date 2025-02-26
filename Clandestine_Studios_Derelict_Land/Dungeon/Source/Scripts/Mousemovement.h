/******************************************************************************/
/*!
\file		Mousemovement.h
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	Nov 15, 2022
\brief		Contains script for mouse movement

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "Essential.h"
#include "Logic/Script.h"
#include "Logic/Logic.h"
#include "Input/Input.h"
#include "Transform/Transform.h"
#include "RigidBody/RigidBody.h"
#include "Collision/Collision.h"
#include "Graphics/BatchRendering.h"
#include "Animation/Animate.h"

extern int WINDOW_HEIGHT;
extern int currentNumberOfSteps;
extern MouseInput MyMouse;
extern bool is_paused;

namespace MMovement {
	int RADIUS{ 10 };
	std::string SKey{ "MMovement" };
	float fixedDeltaTime{ 1.f / 60.f };
	Vec2 clicked_pos{};
	float aux_timer = 0.f;

	void Init(GameObject* GO, float dt) {
		(void)dt;
		Transform* t = dynamic_cast<Transform*>(GO->GetComponent(ComponentType::Transform));
		RigidBody* r = dynamic_cast<RigidBody*>(GO->GetComponent(ComponentType::RigidBody));
		Collision* c = dynamic_cast<Collision*>(GO->GetComponent(ComponentType::Collision));

		if (t != nullptr && r != nullptr && c != nullptr) {
			aux_timer = 5.f;
		}
		else {
			LogicComponent* LC = dynamic_cast<LogicComponent*>(GO->GetComponent(ComponentType::Logic));
			//This must always work, otherwise how is this function even being called?
			//Do a simple check anyway for safety, but UDB if it's actually nullptr
			if (LC != nullptr) {
				LC->RemoveScriptKey(SKey);
			}
		}
	}

	void Update(GameObject* GO, float dt) {
		(void)dt;
		if (!is_paused)
		{
			for (int step = 0; step < ::currentNumberOfSteps; ++step) {
				Vec2 DV, NDV;
				Transform* t = dynamic_cast<Transform*>(GO->GetComponent(ComponentType::Transform));
				RigidBody* r = dynamic_cast<RigidBody*>(GO->GetComponent(ComponentType::RigidBody));
				Collision* c = dynamic_cast<Collision*>(GO->GetComponent(ComponentType::Collision));
				Animate* a = dynamic_cast<Animate*>(GO->GetComponent(ComponentType::Animate));

				if (Input::GetMouseTriggeredDepth(MouseButton::Button0, GO)) {
					clicked_pos.x = Batch::camera.GetCameraPosition().x  + ((float)MyMouse.x / ((float)GLSetup::current_width / (float)GLSetup::width));
					clicked_pos.y = Batch::camera.GetCameraPosition().y  + (float)(::WINDOW_HEIGHT - MyMouse.y);
					DV = r->BuildDistanceVector(t->Position, { clicked_pos.x, t->Position.y });
					Vec2Normalize(NDV, DV);
					r->Accelerate(NDV, fixedDeltaTime);
					
					a->SetTexHandle("Yu_Run");
					if ((DV.x < 0 && t->Dimensions.x > 0) || (DV.x > 0 && t->Dimensions.x < 0)) {
						t->Dimensions.x = -t->Dimensions.x;
					}
				}
				if (Vec2SquareLength(r->Velocity) > 0.f) {
					aux_timer += dt;
					if (aux_timer > 0.5f) {
						int footstep_index = rand() % 6;
						audio->PlaySfx(36 + footstep_index);
						aux_timer = 0.f;
					}
				}
				if (!c->go_is_colliding && !(t->Position.x >= clicked_pos.x - RADIUS && t->Position.x <= clicked_pos.x + RADIUS)) {
					r->Move(t->Position, fixedDeltaTime);
				}
				else if (c->go_is_colliding) {
					Vec2 Reverse = -r->Velocity;
					r->SetVelocity(Reverse);
					r->Move(t->Position, fixedDeltaTime);
					r->SetVelocity({ 0.0f,0.0f });
					r->Move(t->Position, fixedDeltaTime);
					c->go_is_colliding = false;
					a->SetTexHandle("Yu_Idle");
					aux_timer = 5.f;
				}
				else {
					r->SetVelocity({ 0.0f,0.0f });
					a->SetTexHandle("Yu_Idle");
					aux_timer = 5.f;
				}
			}
		}
	}

	void End(GameObject* GO, float dt) {
		(void)GO;
		(void)dt;
	}
}