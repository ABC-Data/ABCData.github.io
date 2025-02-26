#pragma once
/************************************************************************ /
/*!
\file   MainMenu.h
\author Louis Mineo @ Linn Min Htoo
\par    email: louismineo.l@digipen.edu
\date   January 29, 2023
\brief	Function definitions of script, MainMenu, which prints
		the GAMEUI text of a dialogue box, akin to a typewriter.

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "Essential.h"
#include <vector>
#include "Logic/Script.h"
#include "Logic/Logic.h"
#include "Input/Input.h"
#include "Transform/Transform.h"
#include "GameUI/GameUI.h"
#include "Graphics/GLFWsetup.h"
#include "Audio/Audio.h"
#include "AnimationScripts.h"


#define VEC3_BLACK_COLOR {0.f,0.f,0.f}
#define VEC3_WHITE_COLOR {1.f,1.f,1.f}

extern bool is_paused;
extern MouseInput MyMouse;
extern bool keystateLM;
extern bool prevloop_keystateLM;
extern bool fullscreen;

//int mouse_x, mouse_y_aft_TRS;

namespace IntroCutscene
{
	/*
	* flow

		VO.mp3 (50sec)
		{
			Panel 1: *Explosion sound* No one knows who did it, or what happened. All we knew were that our lives would be forever changed.
			(0:05)

			Panel 2: Almost everything we knew and loved were gone… people, animals, and even inanimate objects were mutating, changing into something else entirely. Luckily, I found a few fellow survivors like me who weren’t affected by the mutations. We have since formed a small community to try and survive in this new world.
			(0:30)

			Panel 3: It has been a few years since then. Resources are running dry in Singapore and the monsters are getting more violent every day. We can’t stay here forever.
			(0:40)
			Panel 4: Our leader has sent me to scavenge an area one last time before we start scouting and exploring other areas. Hopefully we’ll find something useful...
			(0:50)
		}




		original position of the cutscene
		(3200,10)

		(make set renderer to visible)
		1. pan RIGHT to CITY
			(1 sec)
			- lerp the x_pos from 3200 to 1520
			- alpha value lerp from 0.0 to 1.0
		2. stay on this for 10 seconds 			(0:10)


		3. pan RIGHT to HAND
			(5 secs)				(0:15)
			- lerp the x_pos from 1520 to 50
			- lerp the y_pos from 10 to 100
		4. stay on this for 15 seconds			(0:30)


		5. pan BOT LEFT to MONSTER
			(2 secs)				(0:32)
			- lerp the x_pos from 50 to 1560
			- lerp the y_pos from 100 to 960
		6. stay on this for 8 seconds			(0:40)


		7. pan RIGHT to PIER
			(2 secs)				(0:42)
			- lerp the x_pos from 1560 to 60
		6. stay on this for 8 seconds			(0:50)
	*/
	bool introcutscene_is_play = false;
	bool introcutscene_is_done = false;
	/**************************************************************************/
	/*!
		\brief
			This is the function declaration of the updateCutscene function
		\param dt
			delta time into the time_elapsed of the cutscene showing.
		\return
			returns true if the entire cutscene is done, false if not.
	*/
	/**************************************************************************/
	bool Update_CutScenePanning(float dt);
	bool cutscene_PANflags[4] = { false, false, false, false };

	std::vector<std::string> subtitles_vect = { // 16 lines
		"No one knows who did it, or what happened.",
		"All we knew was that our lives would be forever changed.",
		"Almost everything we knew and loved were gone ...",
		"people, animals, and even inanimate objects were mutating,",
		"changing into something else entirely.",
		"Luckily, I found a few fellow survivors like me",
		"who weren't affected by the mutations.", // find timestamp
		"We have since formed a small community",
		"to try and survive in this new world.", 
		"It has been a few years since then.",
		"Resources are running dry in Singapore",
		"and the monsters are getting more violent every day.",
		"We can't stay here forever.",
		"Our leader has sent me to scavenge an area one last time", // too early
		"before we start scouting and exploring other areas.",
		"Hopefully we'll find something useful ..."
	};

	float VO_LINE_timestamp[16] =
	{ 1.1f,5.01f,9.21f,12.16f, 17.06f, 19.22f,
	23.00f,	
	25.05f,
	27.11f,
	30.21f, 33.04f, 35.16f, 38.12f,
	42.20f, 45.18f, 48.19f };
	
	
	int current_line = -1;

	Vec2 PANSTOP_positions[4] =
	{ {1520.f, 10.f},
	{50.f,100.f},
	{1560,900},
	{60,900} };

	float cutscene_timeElapsed = 0.f;

	float frame_ShowTime[4] = { 10.f, 15.f, 8.f, 8.f };
	// city,hand,monsters,pier
	float frame_PanDuration[4] = { 1.f, 5.f, 2.f, 2.f };
	// pan to city, pan to hand, pan to monsters, // pan to pier

	float alpha_delta = 1.0f / (60.f* frame_PanDuration[0]);
	Vec2 start_pos = { 3200.f, 10 };
	Vec2 PANSTOP_LERPS[4] =
	{
		(PANSTOP_positions[0] - start_pos) / (frame_PanDuration[0] * 60),
		(PANSTOP_positions[1] - PANSTOP_positions[0]) / (frame_PanDuration[1] * 60),
		(PANSTOP_positions[2] - PANSTOP_positions[1]) / (frame_PanDuration[2] * 60),
		(PANSTOP_positions[3] - PANSTOP_positions[2]) / (frame_PanDuration[3] * 60)
	};
	/**************************************************************************/
	/*!
		\brief
			This function runs in the update loop of the mainmenu script. It
			aids with the panning and changing of the subtitle text.
		\param dt
			delta time into the time_elapsed of the cutscene showing.
		\return
			returns true if the entire cutscene is done, false if not.
	*/
	/**************************************************************************/
	bool Update_CutScenePanning(float dt)
	{
		Transform* trf_cutscene = Engine->GetGameObject("IntroCutscene")->GetComponent<Transform>();

		cutscene_timeElapsed += dt;

		if (current_line < (16 -1))
		{
			if (cutscene_timeElapsed > VO_LINE_timestamp[(current_line + 1)])
			{
				current_line += 1;
				Engine->GetGameObject("CutSceneDialogueBox")->GetComponent<GameUI>()->SetText(subtitles_vect[current_line]);
			}
		}
		
		
		if (cutscene_PANflags[0] == false)  //PAN TO CITY
		{
			if (trf_cutscene->Position.x > PANSTOP_positions[0].x)
			{
				trf_cutscene->Position = trf_cutscene->Position + PANSTOP_LERPS[0];
				Engine->GetGameObject("IntroCutscene")->GetComponent<Renderer>()->Alpha_0to1 += alpha_delta;
				Engine->GetGameObject("CutSceneDialogueBox")->GetComponent<Renderer>()->Alpha_0to1 += alpha_delta;
				Engine->GetGameObject("CutSceneDialogueBox")->GetComponent<GameUI>()->Alpha += alpha_delta;
				return false;
			}
			else if (cutscene_timeElapsed > (frame_PanDuration[0] + frame_ShowTime[0]))
			{
				std::cout << "moving to hand" << std::endl;
				cutscene_PANflags[0] = true;
			}
		}

		if (cutscene_PANflags[0] == true && cutscene_PANflags[1] == false)  //PAN TO HAND
		{
			if (trf_cutscene->Position.x > PANSTOP_positions[1].x)
			{
				trf_cutscene->Position = trf_cutscene->Position + PANSTOP_LERPS[1];
				return false;
			}
			else if (cutscene_timeElapsed >
				(frame_PanDuration[0] + frame_ShowTime[0] + frame_PanDuration[1] + frame_ShowTime[1]))
			{
				std::cout << "moving to monsters" << std::endl;
				cutscene_PANflags[1] = true;
			}
		}

		if (cutscene_PANflags[0] == true && cutscene_PANflags[1] == true && cutscene_PANflags[2] == false)  //PAN TO MONSTERS
		{
			if (trf_cutscene->Position.x < PANSTOP_positions[2].x && trf_cutscene->Position.y < PANSTOP_positions[2].y)
			{
				trf_cutscene->Position = trf_cutscene->Position + PANSTOP_LERPS[2];
				return false;
			}
			else if (cutscene_timeElapsed >
				(frame_PanDuration[0] + frame_ShowTime[0] + frame_PanDuration[1] + frame_ShowTime[1]
					+ frame_PanDuration[2] + frame_ShowTime[2]))
			{
				std::cout << "moving to pier" << std::endl;
				cutscene_PANflags[2] = true;
			}
		}

		if (cutscene_PANflags[0] == true && cutscene_PANflags[1] == true && cutscene_PANflags[2] == true && cutscene_PANflags[3] == false)  //PAN TO PIER
		{
			if (trf_cutscene->Position.x > PANSTOP_positions[3].x)
			{
				trf_cutscene->Position = trf_cutscene->Position + PANSTOP_LERPS[3];
				return false;
			}
			else if (cutscene_timeElapsed >
				(frame_PanDuration[0] + frame_ShowTime[0] + frame_PanDuration[1] + frame_ShowTime[1]
					+ frame_PanDuration[2] + frame_ShowTime[2] + frame_PanDuration[3] + frame_ShowTime[3]))
			{
				cutscene_PANflags[3] = true;
			}
		}


		if (cutscene_PANflags[0] && cutscene_PANflags[1] && cutscene_PANflags[2] && cutscene_PANflags[3]) return true;
		else return false;
	}

}

namespace MainMenu
{
	std::string SKey{ "MainMenu" };
	/**************************************************************************/
	/*!
		\brief
			This function either displays or hides the OptionsMenu GameObjects
		\param t_f
			True or False, true to show, false to hide
	*/
	/**************************************************************************/
	inline void ShowOptionsGOs(bool t_f)
	{

		dynamic_cast<Renderer*>(Engine->GetGameObject("Options Menu")->GetComponent(ComponentType::Renderer))->SetVisibility(t_f);
		dynamic_cast<GameUI*>(Engine->GetGameObject("Options Menu")->GetComponent(ComponentType::GameUI))->SetFontVisibility(t_f);

		dynamic_cast<Renderer*>(Engine->GetGameObject("Options MasterControl")->GetComponent(ComponentType::Renderer))->SetVisibility(t_f);
		dynamic_cast<Renderer*>(Engine->GetGameObject("Options MasterPointer")->GetComponent(ComponentType::Renderer))->SetVisibility(t_f);

		dynamic_cast<Renderer*>(Engine->GetGameObject("Options MusicControl")->GetComponent(ComponentType::Renderer))->SetVisibility(t_f);
		dynamic_cast<Renderer*>(Engine->GetGameObject("Options MusicPointer")->GetComponent(ComponentType::Renderer))->SetVisibility(t_f);

		dynamic_cast<Renderer*>(Engine->GetGameObject("Options SFXControl")->GetComponent(ComponentType::Renderer))->SetVisibility(t_f);
		dynamic_cast<Renderer*>(Engine->GetGameObject("Options SFXPointer")->GetComponent(ComponentType::Renderer))->SetVisibility(t_f);

		dynamic_cast<Renderer*>(Engine->GetGameObject("Options Close")->GetComponent(ComponentType::Renderer))->SetVisibility(t_f);
		dynamic_cast<GameUI*>(Engine->GetGameObject("Options Close")->GetComponent(ComponentType::GameUI))->SetFontVisibility(t_f);

	}
	/**************************************************************************/
	/*!
		\brief
			This function either displays or hides the main menus's
			start game, options, quit game
		\param t_f
			True or False, true to show, false to hide
	*/
	/**************************************************************************/
	inline void ShowMainMenu_Main3Buttons(bool t_f)
	{

		dynamic_cast<Renderer*>(Engine->GetGameObject("Start Game Btn")->GetComponent(ComponentType::Renderer))->SetVisibility(t_f);
		dynamic_cast<GameUI*>(Engine->GetGameObject("Start Game Btn")->GetComponent(ComponentType::GameUI))->SetFontVisibility(t_f);

		dynamic_cast<Renderer*>(Engine->GetGameObject("Options Btn")->GetComponent(ComponentType::Renderer))->SetVisibility(t_f);
		dynamic_cast<GameUI*>(Engine->GetGameObject("Options Btn")->GetComponent(ComponentType::GameUI))->SetFontVisibility(t_f);

		dynamic_cast<Renderer*>(Engine->GetGameObject("Quit Game Btn")->GetComponent(ComponentType::Renderer))->SetVisibility(t_f);
		dynamic_cast<GameUI*>(Engine->GetGameObject("Quit Game Btn")->GetComponent(ComponentType::GameUI))->SetFontVisibility(t_f);

	}
	/**************************************************************************/
	/*!
		\brief
			This function either displays or hides the quit confirmation GOs
			quitgame confirmation screen, the quitgame confirmation yes and
			quitgame confirmation no.
		\param t_f
			True or False, true to show, false to hide
	*/
	/**************************************************************************/
	inline void ShowMainMenu_QuitGameConfirmation3Buttons(bool t_f)
	{

		dynamic_cast<Renderer*>(Engine->GetGameObject("Quit Game Confirmation")->GetComponent(ComponentType::Renderer))->SetVisibility(t_f);
		dynamic_cast<GameUI*>(Engine->GetGameObject("Quit Game Confirmation")->GetComponent(ComponentType::GameUI))->SetFontVisibility(t_f);

		dynamic_cast<Renderer*>(Engine->GetGameObject("Quit Game Yes")->GetComponent(ComponentType::Renderer))->SetVisibility(t_f);
		dynamic_cast<GameUI*>(Engine->GetGameObject("Quit Game Yes")->GetComponent(ComponentType::GameUI))->SetFontVisibility(t_f);

		dynamic_cast<Renderer*>(Engine->GetGameObject("Quit Game No")->GetComponent(ComponentType::Renderer))->SetVisibility(t_f);
		dynamic_cast<GameUI*>(Engine->GetGameObject("Quit Game No")->GetComponent(ComponentType::GameUI))->SetFontVisibility(t_f);

	}
	/*
	inline void InitOptionsGOs()
	{
		//Dynamically create the options menu GOs from ./Data/OptionsMenuGOs.txt
		std::map<std::string, GameObject*> map_new_GOs;
		map_new_GOs = factory->AddMultipleGameObjects("./Data/OptionsMenuGOs.txt");

		std::cout << "Options Btn Pressed" << std::endl;
		std::cout << map_new_GOs.size() << std::endl;
		for (auto& x : map_new_GOs)
		{
			std::cout << "inserting " << x.first << std::endl;
			Engine->GetLevel()->AddGameObject(x.second);
		}

		//refresh the hierarchylist
		editor->GetHierarchy().RefreshHierarchy(); // Update the hierarchy
	}
	*/

	/**************************************************************************/
	/*!
		\brief
			This function sets the volume pointer positions according to the
			audio system's various volumes
	*/
	/**************************************************************************/
	inline void SetPointersToInitPos()
	{
		float new_xpos;
		float new_ypos;
		float x_width;
		GameUI* gui;
		std::string curr_GO_name;

		// MASTER VOL POINTER
		curr_GO_name = "Options MasterControl";
		gui = dynamic_cast<GameUI*>(Engine->GetGameObject(curr_GO_name)->GetComponent(ComponentType::GameUI));
		x_width = gui->BB.x - gui->AA.x;
		new_xpos = gui->AA.x + (x_width * audio->GetMasterVol());
		new_xpos = (new_xpos / GLSetup::current_width) * GLSetup::width; // Scale according to window dimensions
		new_ypos = dynamic_cast<Transform*>(Engine->GetGameObject(curr_GO_name)->GetComponent(ComponentType::Transform))->Position.y;

		// move the pointer to that location
		dynamic_cast<Transform*>(Engine->GetGameObject("Options MasterPointer")->GetComponent(ComponentType::Transform))->SetPosition({ new_xpos,new_ypos });


		// Music VOL POINTER
		curr_GO_name = "Options MusicControl";
		gui = dynamic_cast<GameUI*>(Engine->GetGameObject(curr_GO_name)->GetComponent(ComponentType::GameUI));
		x_width = gui->BB.x - gui->AA.x;
		new_xpos = gui->AA.x + (x_width * audio->GetMusicVol());
		new_xpos = (new_xpos / GLSetup::current_width) * GLSetup::width; // Scale according to window dimensions
		new_ypos = dynamic_cast<Transform*>(Engine->GetGameObject(curr_GO_name)->GetComponent(ComponentType::Transform))->Position.y;

		// move the pointer to that location
		dynamic_cast<Transform*>(Engine->GetGameObject("Options MusicPointer")->GetComponent(ComponentType::Transform))->SetPosition({ new_xpos,new_ypos });

		// SFX VOL POINTER
		curr_GO_name = "Options SFXControl";
		gui = dynamic_cast<GameUI*>(Engine->GetGameObject(curr_GO_name)->GetComponent(ComponentType::GameUI));
		x_width = gui->BB.x - gui->AA.x;
		new_xpos = gui->AA.x + (x_width * audio->GetSfxVol());
		new_xpos = (new_xpos / GLSetup::current_width) * GLSetup::width; // Scale according to window dimensions
		new_ypos = dynamic_cast<Transform*>(Engine->GetGameObject(curr_GO_name)->GetComponent(ComponentType::Transform))->Position.y;

		// move the pointer to that location
		dynamic_cast<Transform*>(Engine->GetGameObject("Options SFXPointer")->GetComponent(ComponentType::Transform))->SetPosition({ new_xpos,new_ypos });

	}

	/**************************************************************************/
	/*!
		\brief
			Initializer for the MainMenu script. It is called for every
			GO that has this script attached to it.
		\param GO
			The specific Gameobject who is the calling and is affected by
			this script function
	*/
	/**************************************************************************/
	void Init(GameObject* GO, float dt)
	{
		UNREFERENCED_PARAMETER(dt);
		LogicTargets* LT = dynamic_cast<LogicTargets*>(GO->GetComponent(ComponentType::LogicTargets));
		GameUI* self_GUI = dynamic_cast<GameUI*>(GO->GetComponent(ComponentType::GameUI));
		if (LT != nullptr && LT->Targets.size() > 0 && self_GUI != nullptr)
		{
			for (size_t a = 0; a < LT->Targets.size(); a++)
			{
				//All good, targets[0] for now 
				GameUI* OtherGUI = dynamic_cast<GameUI*>(LT->Targets[0]->GetComponent(ComponentType::GameUI));
				if (OtherGUI != nullptr) continue;
				else
				{
					std::cout << "Logic Target ID [" << a << "] has a nullptr for its GameUI Compoenent.\n";
					break;
				}
			}
		}
		else
		{
			LogicComponent* LC = dynamic_cast<LogicComponent*>(GO->GetComponent(ComponentType::Logic));
			//This must always work, otherwise how is this function even being called?
			//Do a simple check anyway for safety, but UDB if it's actually nullptr
			if (LC == nullptr) {
				std::cout << GO->GetName() << " couldn't properly initialise UIFunctions script, removing.\n";
				LC->RemoveScriptKey(SKey);
			}
		}
		Animate* FadeIn = dynamic_cast<Animate*>(Engine->GetGameObject("FadeIn")->GetComponent(ComponentType::Animate));
		FadeIn->SetVisibility(false);
		FadeIn->SetIsPlaying(false);
		MasterObjectList->MasterAssets.GetSprite(FadeIn->GetTexHandle()).idx = 0;
	}
	/**************************************************************************/
	/*!
		\brief
			Update for the MainMenu script. It is called for every
			GO that has this script attached to it.
		\param GO
			The specific Gameobject who is the calling and is affected by
			this script function
	*/
	/**************************************************************************/
	void Update(GameObject* GO, float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		if (Input::GetKeyTriggered(Key::E)) {
			Animate* TeamLogoa = dynamic_cast<Animate*>(Engine->GetGameObject("TeamLogo")->GetComponent(ComponentType::Animate));
			MasterObjectList->MasterAssets.GetSprite(TeamLogoa->GetTexHandle()).idx = MasterObjectList->MasterAssets.GetSprite(TeamLogoa->GetTexHandle()).frames_per_row - 1;
			Animate* GameLogoa = dynamic_cast<Animate*>(Engine->GetGameObject("GameLogo")->GetComponent(ComponentType::Animate));
			MasterObjectList->MasterAssets.GetSprite(GameLogoa->GetTexHandle()).idx = MasterObjectList->MasterAssets.GetSprite(GameLogoa->GetTexHandle()).frames_per_row - 1;
			Animate* MenuFadea = dynamic_cast<Animate*>(Engine->GetGameObject("MenuFade")->GetComponent(ComponentType::Animate));
			MasterObjectList->MasterAssets.GetSprite(MenuFadea->GetTexHandle()).idx = MasterObjectList->MasterAssets.GetSprite(MenuFadea->GetTexHandle()).frames_per_row - 1;
			Renderer* DigiLogo = dynamic_cast<Renderer*>(Engine->GetGameObject("DigipenLogo")->GetComponent(ComponentType::Renderer));
			DigiLogo->Alpha_0to1 = -1;
			GameUI* DigiText = dynamic_cast<GameUI*>(Engine->GetGameObject("DigipenLogo")->GetComponent(ComponentType::GameUI));
			DigiText->Alpha = -1;

			if (IntroCutscene::introcutscene_is_done == false && IntroCutscene::introcutscene_is_play == true)
			{
				IntroCutscene::introcutscene_is_done = true;
			}
		}
		if (!PlayerAnimation::MainMenuAnimation())
		{
			Animate* FadeIn = dynamic_cast<Animate*>(Engine->GetGameObject("FadeIn")->GetComponent(ComponentType::Animate));
			//check if the current GO's is the pressed on
			GameUI* self_GUI = dynamic_cast<GameUI*>(GO->GetComponent(ComponentType::GameUI));
			Renderer* self_rdr = dynamic_cast<Renderer*>(GO->GetComponent(ComponentType::Renderer));

			// if the button is not active or visible , skip
			if (self_rdr->GetVisibility() == false) return;

			int mm_mouse_x = Input::GetMouse().x;
			int mm_mouse_y_aft_TRS = (GLSetup::current_height - Input::GetMouse().y);

			if (IntroCutscene::introcutscene_is_play == false)
			{
				if ((mm_mouse_x > self_GUI->AA.x &&
					mm_mouse_x < self_GUI->BB.x) &&
					(mm_mouse_y_aft_TRS > self_GUI->AA.y &&
						mm_mouse_y_aft_TRS < self_GUI->BB.y))
				{
					self_GUI->FontColor = VEC3_BLACK_COLOR;
					self_rdr->RGB_0to1 = { 0.5f,0.5f,0.5f };

					if (Input::GetMouseTriggeredDepth(MouseButton::Button0, GO))
					{
						if (GO->GetName() == "Start Game Btn")
						{
							audio->StopMusic();
							audio->PlaySfx(30);
							audio->PlayMusic(42);
							std::cout << "Start Game Btn Pressed" << std::endl;
							IntroCutscene::introcutscene_is_play = true;
							Engine->GetGameObject("IntroCutscene")->GetComponent<Renderer>()->SetVisibility(true);
							Engine->GetGameObject("IntroCutscene")->GetComponent<Renderer>()->Alpha_0to1 = 0.f;
							Engine->GetGameObject("CutSceneDialogueBox")->GetComponent<Renderer>()->SetVisibility(true);
							Engine->GetGameObject("CutSceneDialogueBox")->GetComponent<Renderer>()->Alpha_0to1 = 0.f;
							Engine->GetGameObject("CutSceneDialogueBox")->GetComponent<GameUI>()->SetFontVisibility(true);
							Engine->GetGameObject("CutSceneDialogueBox")->GetComponent<GameUI>()->Alpha = 0.f;

							/*
							std::cout << "Switching scenes\n";

							FadeIn->SetVisibility(true);
							FadeIn->SetIsPlaying(true);
							*/
							GameObject* text = Engine->GetGameObject("Title");
							GameUI* ui_text = dynamic_cast<GameUI*>(text->GetComponent(ComponentType::GameUI));
							ui_text->SetFontVisibility(false);

							text = Engine->GetGameObject("Start Game Btn");
							ui_text = dynamic_cast<GameUI*>(text->GetComponent(ComponentType::GameUI));
							ui_text->SetFontVisibility(false);

							text = Engine->GetGameObject("Quit Game Btn");
							ui_text = dynamic_cast<GameUI*>(text->GetComponent(ComponentType::GameUI));
							ui_text->SetFontVisibility(false);

							text = Engine->GetGameObject("Options Btn");
							ui_text = dynamic_cast<GameUI*>(text->GetComponent(ComponentType::GameUI));
							ui_text->SetFontVisibility(false);
						}

						if (GO->GetName() == "Options Btn")
						{
							//InitOptionsGOs();	
							ShowOptionsGOs(true);
							ShowMainMenu_Main3Buttons(false);
							SetPointersToInitPos();
							//dynamic_cast<Renderer*>(Engine->GetGameObject("OptionsMenu")->GetComponent(ComponentType::Renderer))->SetVisibility(true);

						}

						if (GO->GetName() == "Quit Game Btn")
						{
							audio->PlaySfx(30);
							//show the confirmation and yes no buttons
							ShowMainMenu_QuitGameConfirmation3Buttons(true);
						}

						if (GO->GetName() == "Quit Game Yes")
						{
							audio->PlaySfx(30);
							glfwSetWindowShouldClose(GLSetup::ptr_window, true);
							//show the confirmation and yes no buttons

						}

						if (GO->GetName() == "Quit Game No")
						{
							ShowMainMenu_QuitGameConfirmation3Buttons(false);
							//show the confirmation and yes no buttons
							audio->PlaySfx(30);
						}
					}
				}
				else
				{
					self_GUI->FontColor = VEC3_WHITE_COLOR;
					self_rdr->RGB_0to1 = { 1.0f,1.0f,1.0f };
				}
				/*
				if (MasterObjectList->MasterAssets.GetSprite(FadeIn->GetTexHandle()).idx == MasterObjectList->MasterAssets.GetSprite(FadeIn->GetTexHandle()).frames_per_row - 1
					&& GO->GetName() == "Start Game Btn") {

					std::cout << "lol" << std::endl;
					IntroCutscene::introcutscene_is_play = true;
					SceneManager::StoreLevel("./Scene/TutorialLevel.scn");
				}
				*/

			}
			else
			{
				if (GO->GetName() == "IntroCutscene")
				{	
					if (IntroCutscene::introcutscene_is_done == false)
					{
						//std::cout << "CUT SCENE IS PLAYING" << std::endl;
						IntroCutscene::introcutscene_is_done = IntroCutscene::Update_CutScenePanning(dt);
					}
					if (IntroCutscene::introcutscene_is_done == true)
					{
						FadeIn->SetVisibility(true);
						FadeIn->SetIsPlaying(true);
						if (MasterObjectList->MasterAssets.GetSprite(FadeIn->GetTexHandle()).idx == MasterObjectList->MasterAssets.GetSprite(FadeIn->GetTexHandle()).frames_per_row - 1)
						{
							std::cout << "SWITCHING SCENE TO TUTORIAL LEVEL" << std::endl;
							audio->StopMusic();
							SceneManager::StoreLevel("./Scene/TutorialLevel.scn");
							IntroCutscene::cutscene_PANflags[0] = false;
							IntroCutscene::cutscene_PANflags[1] = false;
							IntroCutscene::cutscene_PANflags[2] = false;
							IntroCutscene::cutscene_PANflags[3] = false;
							IntroCutscene::introcutscene_is_play = false;
							IntroCutscene::introcutscene_is_done = false;
							IntroCutscene::cutscene_timeElapsed = 0.f;
							IntroCutscene::current_line = -1;
						}
					}
				}
			}
			mm_mouse_x = mm_mouse_y_aft_TRS = 0;
		}
	}
	/**************************************************************************/
	/*!
		\brief
			End for the MainMenu script. It is called for every
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
	}
}

/*
namespace IntroCutscene
{
	

	Vec2 PANSTOP_positions[4] =
	{ {1520.f, 10.f},
	{50.f,100.f},
	{1560,900},
	{60,900} };

	float cutscene_timeElapsed = 0.f;

	float frame_ShowTime[4] = { 10.f, 16.f, 33.f, 43.f };
	// city,hand,monsters,pier
	float frame_PanDuration[4] = { 1.f, 5.f, 2.f, 2.f };
	// pan to city, pan to hand, pan to monsters, // pan to pier

	float alpha_delta = 1.0f / 60.f;
	Vec2 start_pos = { 3200.f, 10 };
	Vec2 PANSTOP_LERPS[4] =
	{
		(PANSTOP_positions[0] - start_pos) / (frame_PanDuration[0] * 60),
		(PANSTOP_positions[1] - PANSTOP_positions[0]) / (frame_PanDuration[1] * 60),
		(PANSTOP_positions[2] - PANSTOP_positions[1]) / (frame_PanDuration[2] * 60),
		(PANSTOP_positions[3] - PANSTOP_positions[2]) / (frame_PanDuration[3] * 60)
	};

	bool Update_CutScenePanning(float dt)
	{
		Transform* trf_cutscene = Engine->GetGameObject("IntroCutscene")->GetComponent<Transform>();

		cutscene_timeElapsed += dt;
		if (cutscene_PANflags[0] == false)  //PAN TO CITY
		{
			if (trf_cutscene->Position.x > PANSTOP_positions[0].x)
			{
				trf_cutscene->Position = trf_cutscene->Position + PANSTOP_LERPS[0];
				return false;
			}
			else if (cutscene_timeElapsed > (frame_PanDuration[0] + frame_ShowTime[0]))
			{
				std::cout << "moving to hand" << std::endl;
				cutscene_PANflags[0] = true;
			}
		}

		if (cutscene_PANflags[0] == true && cutscene_PANflags[1] == false)  //PAN TO HAND
		{
			if (trf_cutscene->Position.x > PANSTOP_positions[1].x)
			{
				trf_cutscene->Position = trf_cutscene->Position + PANSTOP_LERPS[1];
				return false;
			}
			else if (cutscene_timeElapsed >
				(frame_PanDuration[0] + frame_ShowTime[0] + frame_PanDuration[1] + frame_ShowTime[1]))
			{
				std::cout << "moving to monsters" << std::endl;
				cutscene_PANflags[1] = true;
			}
		}

		if (cutscene_PANflags[0] == true && cutscene_PANflags[1] == true && cutscene_PANflags[2] == false)  //PAN TO MONSTERS
		{
			if (trf_cutscene->Position.x < PANSTOP_positions[2].x && trf_cutscene->Position.y < PANSTOP_positions[2].y)
			{
				trf_cutscene->Position = trf_cutscene->Position + PANSTOP_LERPS[2];
				return false;
			}
			else if (cutscene_timeElapsed >
				(frame_PanDuration[0] + frame_ShowTime[0] + frame_PanDuration[1] + frame_ShowTime[1]
					+ frame_PanDuration[2] + frame_ShowTime[2]))
			{
				std::cout << "moving to pier" << std::endl;
				cutscene_PANflags[2] = true;
			}
		}

		if (cutscene_PANflags[0] == true && cutscene_PANflags[1] == true && cutscene_PANflags[2] == true && cutscene_PANflags[3] == false)  //PAN TO PIER
		{
			if (trf_cutscene->Position.x > PANSTOP_positions[3].x)
			{
				trf_cutscene->Position = trf_cutscene->Position + PANSTOP_LERPS[3];
				return false;
			}
			else if (cutscene_timeElapsed >
				(frame_PanDuration[0] + frame_ShowTime[0] + frame_PanDuration[1] + frame_ShowTime[1]
					+ frame_PanDuration[2] + frame_ShowTime[2] + frame_PanDuration[3] + frame_ShowTime[3]))
			{
				cutscene_PANflags[3] = true;
			}
		}


		if (cutscene_PANflags[0] && cutscene_PANflags[1] && cutscene_PANflags[2] && cutscene_PANflags[3]) return true;
		else return false;
	}

}
*/
