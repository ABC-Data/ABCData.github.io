#pragma once
/************************************************************************ /
/*!
\file   OptionsMenu.h
\author Louis Mineo @ Linn Min Htoo
\par    email: louismineo.l@digipen.edu
\date   March 7, 2023
\brief	Function definitions of script, OptionsMenu, which handles the 
		user interface of the options menu;

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
#include "Audio/Audio.h"
#include "MainMenu.h"

extern bool is_paused;
extern MouseInput MyMouse;
extern bool keystateLM;
extern bool prevloop_keystateLM;

namespace OptionsMenu
{
	std::string SKey{ "OptionsMenu" };

	enum class Volumes
	{
		MASTER =0,
		MUSIC,
		SFX
	};
	/**************************************************************************/
	/*!
		\brief
			This function move the pointer position to the specific x positions,
			after it has been scaled accordingly to the appropriate window
			widths;
		\param pointerGO_name
			The name of the pointer GameObject to set its position
		\param x_pos
			The parsed in value is the x_position before scaling (mousepos.x)
		\param y_pos
			The parsed in value is the y_position, y value of the underlyng
			"sliderbar:
	*/
	/**************************************************************************/
	inline void MoveVolPointerHORIZONTAL(std::string pointerGO_name, float x_pos, float y_pos)
	{
		// only do the scale and translation for X this bar only moves sideways
		float new_xpos = (x_pos/GLSetup::current_width)* GLSetup::width;
		
		dynamic_cast<Transform*>(Engine->GetGameObject(pointerGO_name)->GetComponent(ComponentType::Transform))->SetPosition({ new_xpos,y_pos });
	}

	/**************************************************************************/
	/*!
		\brief
			This function sets the volume of the various settings, master,
			music or sfx.
		\param vol_0to1
			The volume to set.
		\param vol_type
			The type of volume to change.
	*/
	/**************************************************************************/
	inline void SetNewVol(float vol_0to1, Volumes vol_type)
	{
		if (vol_type == Volumes::MASTER) audio->SetMasterVol(vol_0to1);
		else if (vol_type == Volumes::MUSIC) audio->SetMusicVol(vol_0to1);
		else if (vol_type == Volumes::SFX) audio->SetSfxVol(vol_0to1);
	}

	/**************************************************************************/
	/*!
		\brief
			This function sets calculates the new volume to set after pressing 
			of the "volume bar", then it sets the vol in the specific setting and
			then it lastly moves the pointer's position.
		\param GO_CONTROL_Name
			The volume to set.
		\param GO_POINTER_Name
			The type of volume to change.
		\param vol_type
			The volume type to set.
	*/
	/**************************************************************************/
	inline void MovePointerSetNewVol(std::string GO_CONTROL_Name, std::string GO_POINTER_Name, Volumes vol_type)
	{
		float new_xpos = static_cast<float>(Input::mouseObj.x);
		float new_ypos = dynamic_cast<Transform*>(Engine->GetGameObject(GO_CONTROL_Name)->GetComponent(ComponentType::Transform))->Position.y;

		// calculate the new vol and set the new master vol
		GameUI* gui = dynamic_cast<GameUI*>(Engine->GetGameObject(GO_CONTROL_Name)->GetComponent(ComponentType::GameUI));
		float x_width = gui->BB.x - gui->AA.x;
		float numerator = new_xpos - gui->AA.x;
		float new_vol = numerator / x_width;

		//set the new volume
		SetNewVol(new_vol, vol_type);

		// move the pointer to that location
		MoveVolPointerHORIZONTAL(GO_POINTER_Name, new_xpos, new_ypos);
		
	}
	/**************************************************************************/
	/*!
		\brief
			This function prints the various voluness of the audio system.
			Used for debugging.
	*/
	/**************************************************************************/
	inline void PrintAllAudioVols()
	{
		std::cout << "Master Vol:" << audio->GetMasterVol() << std::endl;
		std::cout << "Music Vol:" << audio->GetMusicVol() << std::endl;
		std::cout << "SFX Vol:" << audio->GetSfxVol() << std::endl;
		for (int i = 0; i < audio->GetSfxMaxChannels(); i++)
		{
			std::cout << "SFX Channel "<< i <<" resultant Vol:" << audio->GetSfxChannelResultantVol(i) << std::endl;	
		}
	}

	

	/**************************************************************************/
	/*!
		\brief
			Initializer for the OptionsMenu script. It is called for every
			GO that has this script attached to it.
		\param GO
			The specific Gameobject who is the calling and is affected by
			this script function
	*/
	/**************************************************************************/
	void Init(GameObject* GO, float dt)
	{
		UNREFERENCED_PARAMETER(GO);
		UNREFERENCED_PARAMETER(dt);

		MainMenu::SetPointersToInitPos();
	}
	/**************************************************************************/
	/*!
		\brief
			Update for the OptionsMenu script. It is called for every
			GO that has this script attached to it.
		\param GO
			The specific Gameobject who is the calling and is affected by
			this script function
	*/
	/**************************************************************************/
	void Update(GameObject* GO, float dt)
	{
		UNREFERENCED_PARAMETER(dt);
		mouse_x = Input::mouseObj.x;
		mouse_y_aft_TRS = GLSetup::current_height - Input::mouseObj.y;
		
		

		GameUI* self_GUI = dynamic_cast<GameUI*>(GO->GetComponent(ComponentType::GameUI));
		//Transform* self_trf = dynamic_cast<Transform*>(GO->GetComponent(ComponentType::Transform));
		Renderer* self_rdr = dynamic_cast<Renderer*>(GO->GetComponent(ComponentType::Renderer));

		//if (self_GUI->GetFontVisibility() == false) return;
		if (self_rdr->GetVisibility() == false) return;

		if ((mouse_x > self_GUI->AA.x && mouse_x < self_GUI->BB.x)
			&& (mouse_y_aft_TRS > self_GUI->AA.y && mouse_y_aft_TRS < self_GUI->BB.y))
		{

			if (Input::GetMouseTriggeredDepth(MouseButton::Button0, Engine->GetGameObject("Options MasterControl")))
			{
				
				MovePointerSetNewVol("Options MasterControl", "Options MasterPointer", Volumes::MASTER);
				audio->PlaySfx(30);
			}

			if (Input::GetMouseTriggeredDepth(MouseButton::Button0, Engine->GetGameObject("Options MusicControl")))
			{
				MovePointerSetNewVol("Options MusicControl", "Options MusicPointer", Volumes::MUSIC);
				audio->PlaySfx(30);
			}

			if (Input::GetMouseTriggeredDepth(MouseButton::Button0, Engine->GetGameObject("Options SFXControl")))
			{
				MovePointerSetNewVol("Options SFXControl", "Options SFXPointer", Volumes::SFX);
				//PrintAllAudioVols();
				audio->PlaySfx(30);
				
			}

			if (Input::GetMouseTriggeredDepth(MouseButton::Button0, Engine->GetGameObject("Options Close")))
			{
				MainMenu::ShowOptionsGOs(false);
				MainMenu::ShowMainMenu_Main3Buttons(true);
				audio->PlaySfx(30);
			}
			
		}
		

	}
	/**************************************************************************/
	/*!
		\brief
			End for the OptionsMenu script. It is called for every
			GO that has this script attached to it.
		\param GO
			The specific Gameobject who is the calling and is affected by
			this script function
	*/
	/**************************************************************************/
	void End(GameObject* GO, float dt)
	{
		UNREFERENCED_PARAMETER(GO);
		UNREFERENCED_PARAMETER(dt);
	}
}




