#pragma once
/************************************************************************ /
/*!
\file   TypeWriterEffect.h
\author Louis Mineo @ Linn Min Htoo
\par    email: louismineo.l@digipen.edu
\date   January 29, 2023
\brief	Function definitions of script, TypewriterEffect, which prints
		the GAMEUI text of a dialogue box, akin to a typewriter.

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "Essential.h"
#include "Logic/Script.h"
#include "Logic/Logic.h"
#include "Input/Input.h"
#include "Transform/Transform.h"
#include "GameUI/GameUI.h"
#include "Graphics/GLFWsetup.h"

extern bool is_paused;
extern MouseInput MyMouse;
extern bool keystateLM;
extern bool prevloop_keystateLM;

namespace TypewriterEffect
{
	std::string SKey{ "TypewriterEffect" };

	size_t last_idx;
	std::string saved_text;
	float char_per_second = 0.025f;
	float total_dt;
	bool done;

	void SetNewText(std::string text) {
		saved_text = text;
		done = false;
		last_idx = 0;
		total_dt = 0.f;
	}

	/**************************************************************************/
	/*!
		\brief
			Initializer for the TypewriterEffect script. It is called for every
			GO that has this script attached to it.
		\param GO
			The specific Gameobject who is the calling and is affected by
			this script function
	*/
	/**************************************************************************/
	void Init(GameObject* GO, float dt)
	{
		
		UNREFERENCED_PARAMETER(dt);
		GameUI* self_GUI = dynamic_cast<GameUI*>(GO->GetComponent(ComponentType::GameUI));
		last_idx = 0;
		saved_text = self_GUI->GetText();
		done = false;
		total_dt = 0.f;
		//printed_text = saved_text.substr(0, last_idx);

		//std::cout << saved_text << std::endl;
	}
	/**************************************************************************/
	/*!
		\brief
			Update for the TypewriterEffect script. It is called for every
			GO that has this script attached to it. 
		\param GO
			The specific Gameobject who is the calling and is affected by
			this script function
	*/
	/**************************************************************************/
	void Update(GameObject* GO, float dt)
	{
		//if paused, dont update
		if (!is_paused)
		{
			Renderer* self_rdr = dynamic_cast<Renderer*>(GO->GetComponent(ComponentType::Renderer));
			GameUI* self_GUI = dynamic_cast<GameUI*>(GO->GetComponent(ComponentType::GameUI));

			if (self_rdr->GetVisibility() == false || self_GUI->Alpha <= 0.f)
				return;

			

			total_dt += dt;
		

			if (last_idx < (saved_text.length()-1))
			{
				if (Input::CheckKeyTriggered(Key::E)) {
					self_GUI->SetText(saved_text);
					last_idx = saved_text.length();
					done = true;
				}
				else {
					//std::cout << "add more to last_idx," << last_idx << std::endl;
					done = false;
					last_idx = static_cast<size_t>(total_dt / char_per_second);
					//std::cout << saved_text.substr(0, last_idx) << std::endl;
					self_GUI->SetText(saved_text.substr(0, last_idx));
				}
			}
			//else if (last_idx == saved_text.length() - 1)
			else
			{
				//std::cout << "no need add more to last_idx," << last_idx << std::endl;
				self_GUI->SetText(saved_text);
				done = true;
			}
			//std::cout << total_dt <<":" << last_idx << std::endl;;
		}
		//std::cout << done << std::endl;
	}
	/**************************************************************************/
	/*!
		\brief
			End for the TypewriterEffect script. It is called for every
			GO that has this script attached to it.
		\param GO
			The specific Gameobject who is the calling and is affected by
			this script function
	*/
	/**************************************************************************/
	void End(GameObject* GO, float dt)
	{
		(void)dt;
		(void)GO;
		done = false;
		last_idx = 0;
	}
}




