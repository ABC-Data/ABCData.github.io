/******************************************************************************/
/*!
\file    Physics.cpp
\author  Lim Hui Ching, l.huiching, 620004221
\par     email: l.huiching\@digipen.edu
\date    September 07, 2022
\brief   Function definitions to implement physics movement that will be
		 used to update the positions of the game object instances.
		 The functions include:
		 - Physics
		 Default constructor.
		 - Init
		 Initialize the Physics system
		 - Shutdown
		 Shut down the Physics system
		 - Update
		 Updates all game objects

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "Physics.h"

/*!*************************************************************************
****
\brief
	Default constructor.
****************************************************************************
***/
Physics::Physics()
{
	// Empty
}

/*!*************************************************************************
****
\brief
	Initialize the Physics system
****************************************************************************
***/
void Physics::Init()
{
	// Empty
}

/*!*************************************************************************
****
\brief
	Shut down the Physics system
****************************************************************************
***/
void Physics::Shutdown()
{
	// Empty
}

extern int currentNumberOfSteps;
const float fixedDeltaTime = 1.0f / 60.0f;
/*!*************************************************************************
****
\brief
	Updates all game objects
\param[in] dt
	delta time
****************************************************************************
***/
void Physics::Update(float dt)
{
	for (int step = 0; step < currentNumberOfSteps; ++step)
	{
		for (int i = 1; i < this->GetLevel()->GetGOASize(); ++i)
		{
			Collision* c = dynamic_cast<Collision*>(this->GetLevel()->GetGameObjectArray()[i]->GetComponent(ComponentType::Collision));
			if (c != nullptr) c->go_is_colliding = false;
		}

		for (int i = 1; i < this->GetLevel()->GetGOASize(); ++i) {

			if (!this->GetLevel()->GetGameObjectArray()[i]->GetActive()) continue; // Skip inactive game objects

			if (this->GetLevel()->GetGameObjectArray()[i]->GetComponent(ComponentType::Transform) != nullptr)
			{
				Transform* t = dynamic_cast<Transform*>(this->GetLevel()->GetGameObjectArray()[i]->GetComponent(ComponentType::Transform));
				Collision* c = dynamic_cast<Collision*>(this->GetLevel()->GetGameObjectArray()[i]->GetComponent(ComponentType::Collision));
				if (c != nullptr) c->SetBoundingBox(t->Position, t->Scale, t->Dimensions);
				for (int j = 1; j < this->GetLevel()->GetGOASize(); ++j)
				{
					if (i == j || !this->GetLevel()->GetGameObjectArray()[j]->GetActive()) continue; // Skip inactive game objects

					Collision* c1 = dynamic_cast<Collision*>(this->GetLevel()->GetGameObjectArray()[j]->GetComponent(ComponentType::Collision));
					if (c1 != nullptr)
					{
						Transform* t1 = dynamic_cast<Transform*>(this->GetLevel()->GetGameObjectArray()[j]->GetComponent(ComponentType::Transform));
						c1->SetBoundingBox(t1->Position, t1->Scale, t1->Dimensions);
						if (c != nullptr) {
							if (CollisionIntersection_RectRect(c->BoundingBox, c->Vel, c1->BoundingBox, c1->Vel, dt))
							{
								if (!c->go_is_colliding)c->go_is_colliding = true;
								if (!c1->go_is_colliding)c1->go_is_colliding = true;
							}
						}
					}
				}
			}
		}
	}
}