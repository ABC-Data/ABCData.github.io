#pragma once
/************************************************************************ /
/*!
\file   FlasingGameUIText.h
\author Louis Mineo @ Linn Min Htoo
\par    email: louismineo.l@digipen.edu
\date   January 29, 2023
\brief	Function definitions of script, FlasingGameUIText, which renders
		the GAMEUI text of a GO with its rgb value modulating

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "Essential.h"
#include "Logic/Script.h"
#include "Logic/Logic.h"
#include "GameUI/GameUI.h"
#include "Essential.h"

namespace FlasingGameUIText
{
	std::string SKey{ "FlasingGameUIText" };
	float total_dt;
	/**************************************************************************/
	/*!
		\brief
			Initializer for the FlasingGameUIText script. It is called for every
			GO that has this script attached to it.
		\param GO
			The specific Gameobject who is the calling and is affected by
			this script function
	*/
	/**************************************************************************/
	void Init(GameObject* GO, float dt)
	{
		(void)dt;
		GameUI* g_ui = dynamic_cast<GameUI*>(GO->GetComponent(ComponentType::GameUI));
		g_ui->Alpha = 1;
		total_dt = 0;
	}
	/**************************************************************************/
	/*!
		\brief
			Update for the FlasingGameUIText script. It is called for every
			GO that has this script attached to it.
		\param GO
			The specific Gameobject who is the calling and is affected by
			this script function
	*/
	/**************************************************************************/
	void Update(GameObject* GO, float dt)
	{
		total_dt += dt;
		//std::cout << sinf(total_dt) << std::endl;;

		GameUI* g_ui = dynamic_cast<GameUI*>(GO->GetComponent(ComponentType::GameUI));
		g_ui->FontColor = { abs(sinf(total_dt * 2)), abs(sinf(total_dt * 2)), abs(sinf(total_dt * 2)) };
		//g_ui->Alpha = abs(sinf(total_dt*2));
	}
	/**************************************************************************/
	/*!
		\brief
			End for the FlasingGameUIText script. It is called for every
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
		GameUI* g_ui = dynamic_cast<GameUI*>(GO->GetComponent(ComponentType::GameUI));
		g_ui->Alpha = 0;
	}
};