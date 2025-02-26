#pragma once
/************************************************************************ /
/*!
\file    UIfunctions.h
\author  Louis Mineo @ Linn Min Htoo
\par     email: louismineo.l@digipen.edu
\date    November 25, 2022
\brief	 Function definitions of script UIfunctions.h

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

int mouse_x, mouse_y_aft_TRS;
#define half {0.5f,0.5f,0.5f}
#define full {1.f,1.f,1.f}

namespace PauseBtn
{
/**************************************************************************/
/*!
	\brief
		PauseUnpause() will pause the game when the in game pause button is
		pressed
	\param GO
		The specific Gameobject who is the owner this script function
	\param pause_bool
		The global pause bool which can pause or unpause the game
*/
/**************************************************************************/
	void PauseUnpause(GameObject* GO, bool& pause_bool);
/**************************************************************************/
/*!
	\brief
		PauseUnpauseKeyPress() will pause the game when the ESC key is 
		pressed.
	\param GO
		The specific Gameobject who is the owner this script function
	\param pause_bool
		The global pause bool which can pause or unpause the game
*/
/**************************************************************************/
	void PauseUnpauseKeyPress(GameObject* GO, bool& pause_bool);
};

namespace QuitGameBtn
{
	bool QuitGameConfirmation_Active = false;
/**************************************************************************/
/*!
	\brief
		QuitGame() will bringup the confirmation screen for quitting the game 
		when this button is pressed
		\param GO
		The specific Gameobject who is the owner this script function
*/
/**************************************************************************/
	void QuitGame(GameObject* GO);
/**************************************************************************/
/*!
	\brief
		QuitGameYes() will quit the game and close the window
*/
/**************************************************************************/
	void QuitGameYes();
/**************************************************************************/
/*!
	\brief
		QuitGameNo() will bring the user back to the pause menu
		\param GO
		The specific Gameobject who is the owner this script function
*/
/**************************************************************************/
	void QuitGameNo(GameObject* GO);
};

namespace ResumeGameBtn
{
/**************************************************************************/
/*!
	\brief
		ResumeGame() will unpause the game
	\param pause_bool
		The global pause bool which can pause or unpause the game
*/
/**************************************************************************/
	void ResumeGame(bool& pause_bool);
};

namespace HowToPlayBtn
{
	//std::vector<std::string> TEXHDLS_vect {"HTPF1", "HTPF2", "HTPF3"};
	size_t max_HTP_Screens = 3;
	std::vector <Vec2> uv_min_arr{ {0.f,1352.f}, {0.f,676.f}, {0.f,0.f} };
	std::vector <Vec2> uv_max_arr{ {1202.f,2028.f}, {1202.f,1352.f}, {1202.f,676.f} };

	size_t frame_idx{0};
	bool HowToPlayScreen_Active;
	/**************************************************************************/
	/*!
		\brief
			HowToPlay() will bring the user activate the HowToPlayScreen() GO
		\param GO
			The specific Gameobject who is the owner this script function
	*/
	/**************************************************************************/
	void HowToPlay(GameObject* GO);
	/**************************************************************************/
	/*!
		\brief
			HowToPlayScreen() will show the users the user the instructions of 
			the game.
		\param GO
			The specific Gameobject who is the owner this script function
	*/
	/**************************************************************************/
	void HowToPlayScreen(GameObject* GO);
};

namespace UIFunctions
{
	std::string SKey{ "UIFunctions" };
/**************************************************************************/
/*!
	\brief
		Initializer for the UIFunctions script. It is called for every
		GO that has this script attached to it.
	\param GO
		The specific Gameobject who is the calling and is affected by
		this script function
*/
/**************************************************************************/
	void Init(GameObject* GO, float dt)
	{
		(void)dt;
		//set the visibility of these UI elements
		Renderer* self_rdr = dynamic_cast<Renderer*>(GO->GetComponent(ComponentType::Renderer));
		if (GO->GetName() == "Play/Pause Button") self_rdr->SetVisibility(true);

		else if (GO->GetName() != "Play/Pause Button") self_rdr->SetVisibility(false);
		/* the line above does the same thing as the lines below as UI functions is only for the pause menu buttons
		if (GO->GetName() == "Resume Game Button") self_rdr->SetVisibility(false);
		if (GO->GetName() == "How To Play Button") self_rdr->SetVisibility(false);
		if (GO->GetName() == "HowToPlay Screen") self_rdr->SetVisibility(false);
		if (GO->GetName() == "Return to Main Menu Button") self_rdr->SetVisibility(false);
		if (GO->GetName() == "Quit Game Yes") self_rdr->SetVisibility(false);
		if (GO->GetName() == "Quit Game No") self_rdr->SetVisibility(false);
		*/
	}
/**************************************************************************/
/*!
	\brief
		Update for the UIFunctions script. It is called for every
		GO that has this script attached to it. This is where there
		is a long if-else statement as it will determine which buttons
		being pressed will execute which function
	\param GO
		The specific Gameobject who is the calling and is affected by
		this script function
*/
/**************************************************************************/
	void Update(GameObject* GO, float dt)
	{

		UNREFERENCED_PARAMETER(dt);
	
		if (QuitGameBtn::QuitGameConfirmation_Active || HowToPlayBtn::HowToPlayScreen_Active) is_paused = true;
		
		//std::cout << "[UIFunctions::Update] is_paused =" << std::boolalpha << is_paused << std::endl;
		//std::cout << GO->GetName() << " - " << std::boolalpha << dynamic_cast<GameUI*>(GO->GetComponent(ComponentType::GameUI))->GetActive() << std::endl;
		//std::cout << MyMouse.buttonPressed << std::endl;
		//std::cout << "[UIFunctions::Update] Prev Keystate LM ---- " << std::boolalpha << prevloop_keystateLM << std::endl;
		//std::cout << "[UIFunctions::Update] Keystate LM --------- " << std::boolalpha <<keystateLM << std::endl;		
		Renderer* self_rdr = dynamic_cast<Renderer*>(GO->GetComponent(ComponentType::Renderer));
		// if the button is not active or visible , skip
		if (self_rdr->GetVisibility() == false) return;

		GameUI* self_GUI = dynamic_cast<GameUI*>(GO->GetComponent(ComponentType::GameUI));
		
		int hover_x = Input::GetMouse().x;
		int hover_y_aft_TRS = (GLSetup::current_height - Input::GetMouse().y);

		if ((hover_x > self_GUI->AA.x && hover_x < self_GUI->BB.x)
			&& (hover_y_aft_TRS > self_GUI->AA.y && hover_y_aft_TRS < self_GUI->BB.y)&& GO->GetName() != "HowToPlay Screen")
		{
			self_rdr->RGB_0to1 = half;
		}
		else
		{
			self_rdr->RGB_0to1 = full;

		}

		mouse_x = MyMouse.x;
		mouse_y_aft_TRS = GLSetup::current_height - MyMouse.y;

		if (Input::GetMouseTriggeredDepth(MouseButton::Button0, GO))
		{
			//check if the current GO's is the pressed on
			//GameUI* self_GUI = dynamic_cast<GameUI*>(GO->GetComponent(ComponentType::GameUI));
			
		
			
			
			//std::cout << GO->GetName() << std::endl; // uncomment and press LMB to check which UI GOs are still active 
			mouse_x = MyMouse.x;
			mouse_y_aft_TRS = GLSetup::current_height - MyMouse.y;


			if (MyMouse.x != 0.f && MyMouse.y != 0.f) // for testing purposes
			{
				//std::cout << "mouse_x_aft_TRS : " << mouse_x << std::endl;
				//std::cout << "mouse_y_aft_TRS : "  << mouse_y_aft_TRS << std::endl;
				//std::cout << "GO NAME		  : " << GO->GetName() << std::endl;
			}

			if ((mouse_x > self_GUI->AA.x && mouse_x < self_GUI->BB.x)
				&& (mouse_y_aft_TRS > self_GUI->AA.y && mouse_y_aft_TRS < self_GUI->BB.y))
			{
				audio->PlaySfx(30);

				if (GO->GetName() == "Play/Pause Button")
				{
					//std::cout << GO->GetName() << " Pressed" << std::endl;
					PauseBtn::PauseUnpause(GO, is_paused);
				}
				
				if (GO->GetName() == "Resume Game Button")
				{
					//std::cout << GO->GetName() << " Pressed" << std::endl;
					ResumeGameBtn::ResumeGame(is_paused);
				}

				if (GO->GetName() == "How To Play Button")
				{
					//std::cout << GO->GetName() << " Pressed" << std::endl;
					HowToPlayBtn::HowToPlay(GO);
				}

				if (GO->GetName() == "HowToPlay Screen")
				{
					//std::cout << GO->GetName() << " Pressed" << std::endl;
					HowToPlayBtn::HowToPlayScreen(GO);
				}

				if (GO->GetName() == "Return to Main Menu Button")
				{
					//std::cout << GO->GetName() << " Pressed" << std::endl;
					QuitGameBtn::QuitGame(GO);
				}

				if (GO->GetName() == "Quit Game Yes")
				{
					//std::cout << GO->GetName() << " Pressed" << std::endl;
					QuitGameBtn::QuitGameYes();
				}

				if (GO->GetName() == "Quit Game No")
				{
					//std::cout << GO->GetName() << " Pressed" << std::endl;
					QuitGameBtn::QuitGameNo(GO);
				}
				MyMouse.x = MyMouse.y = 0;				
			}
		}
		else 
		{
			if (GO->GetName() == "Play/Pause Button")
			{
				PauseBtn::PauseUnpauseKeyPress(GO, is_paused);;
			}
		}
	}
/**************************************************************************/
/*!
	\brief
		End for the UIFunctions script. It is called for every
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

namespace PauseBtn
{
	/**************************************************************************/
	/*!
		\brief
			PauseUnpause() will pause the game when the in game pause button is
			pressed
		\param GO
			The specific Gameobject who is the owner this script function
		\param pause_bool
			The global pause bool which can pause or unpause the game
	*/
	/**************************************************************************/
	void PauseUnpause(GameObject* GO, bool& pause_bool)
	{
		// flip the pause status
		pause_bool = !pause_bool;

		Renderer* rdr = dynamic_cast<Renderer*>(GO->GetComponent(ComponentType::Renderer));
		// set texture UV if paused or not
		if (is_paused)
		{
			//PLAY
			rdr->SetUVMin({ 0.f,4193.f });
			rdr->SetUVMax({ 87.f, 4280.f });
		}
		else
		{
			rdr->SetUVMin({ 0.f, 4280.f });
			rdr->SetUVMax({ 87.f, 4367.f });
		}


		
		// setting the corresponding buttons as visible or not
		{
			
			Renderer* resumeGO_RDR = dynamic_cast<Renderer*>(Engine->GetGameObject("Resume Game Button")->GetComponent(ComponentType::Renderer));
			Renderer* HowToPlayBtnGO_RDR = dynamic_cast<Renderer*>(Engine->GetGameObject("How To Play Button")->GetComponent(ComponentType::Renderer));
			Renderer* QuitGameGO_RDR = dynamic_cast<Renderer*>(Engine->GetGameObject("Return to Main Menu Button")->GetComponent(ComponentType::Renderer));

			resumeGO_RDR->SetVisibility(is_paused);
			HowToPlayBtnGO_RDR->SetVisibility(is_paused);
			QuitGameGO_RDR->SetVisibility(is_paused);
		}
		Engine->GetObjectByPrefix("Player")->GetComponent<Animate>()->SetTexHandle("Yu_Idle");
		/*
		LogicTargets* LT = dynamic_cast<LogicTargets*>(GO->GetComponent(ComponentType::LogicTargets));
		for (size_t a = 0; a < LT->Targets.size(); a++)
		{
			// visibility determined by bool if game is paused or not
			Renderer* rdr_tgt = dynamic_cast<Renderer*>(LT->Targets[a]->GetComponent(ComponentType::Renderer));
			rdr_tgt->SetVisibility(is_paused);
		}
		*/

	}
	/**************************************************************************/
	/*!
		\brief
			PauseUnpauseKeyPress() will pause the game when the ESC key is
			pressed.
		\param GO
			The specific Gameobject who is the owner this script function
		\param pause_bool
			The global pause bool which can pause or unpause the game
	*/
	/**************************************************************************/
	void PauseUnpauseKeyPress(GameObject* GO, bool& pause_bool)
	{
			
		Renderer* rdr = dynamic_cast<Renderer*>(GO->GetComponent(ComponentType::Renderer));
		// set texture UV if paused or not
		if (is_paused)
		{
			//PLAY
			rdr->SetUVMin({ 0.f,4193.f });
			rdr->SetUVMax({ 87.f, 4280.f });
		}
		else
		{
			rdr->SetUVMin({ 0.f, 4280.f });
			rdr->SetUVMax({ 87.f, 4367.f });
		}

		Renderer* resumeGO_RDR = dynamic_cast<Renderer*>(Engine->GetGameObject("Resume Game Button")->GetComponent(ComponentType::Renderer));
		Renderer* HowToPlayBtnGO_RDR = dynamic_cast<Renderer*>(Engine->GetGameObject("How To Play Button")->GetComponent(ComponentType::Renderer));
		Renderer* QuitGameGO_RDR = dynamic_cast<Renderer*>(Engine->GetGameObject("Return to Main Menu Button")->GetComponent(ComponentType::Renderer));

		

		if (QuitGameBtn::QuitGameConfirmation_Active || HowToPlayBtn::HowToPlayScreen_Active)
		{
			resumeGO_RDR->SetVisibility(false);
			HowToPlayBtnGO_RDR->SetVisibility(false);
			QuitGameGO_RDR->SetVisibility(false);
		}
		else
		{
			resumeGO_RDR->SetVisibility(pause_bool);
			HowToPlayBtnGO_RDR->SetVisibility(pause_bool);
			QuitGameGO_RDR->SetVisibility(pause_bool);
		}

		/*
		LogicTargets* LT = dynamic_cast<LogicTargets*>(GO->GetComponent(ComponentType::LogicTargets));
		for (size_t a = 0; a < LT->Targets.size(); a++)
		{
			//GameUI* gui_go_tgt = dynamic_cast<GameUI*>(LT->Targets[a]->GetComponent(ComponentType::GameUI));
			Renderer* rdr_go_tgt = dynamic_cast<Renderer*>(LT->Targets[a]->GetComponent(ComponentType::Renderer));
			if (QuitGameBtn::QuitGameConfirmation_Active || HowToPlayBtn::HowToPlayScreen_Active)
			{
				//gui_go_tgt->SetActive(false);
				rdr_go_tgt->SetVisibility(false);

			
			}
			else
			{
				//gui_go_tgt->SetActive(pause_bool);
				rdr_go_tgt->SetVisibility(pause_bool);
			}
		}
		*/
	}
}
namespace QuitGameBtn
{
	/**************************************************************************/
	/*!
		\brief
			QuitGame() will bringup the confirmation screen for quitting the game
			when this button is pressed
			\param GO
			The specific Gameobject who is the owner this script function
	*/
	/**************************************************************************/
	void QuitGame(GameObject* GO)
	{
		(void)GO;
		QuitGameBtn::QuitGameConfirmation_Active = true;
		

		//set the initial pause menu button as not visible
		Renderer* resumeGO_RDR = dynamic_cast<Renderer*>(Engine->GetGameObject("Resume Game Button")->GetComponent(ComponentType::Renderer));
		Renderer* HowToPlayBtnGO_RDR = dynamic_cast<Renderer*>(Engine->GetGameObject("How To Play Button")->GetComponent(ComponentType::Renderer));
		Renderer* QuitGameGO_RDR = dynamic_cast<Renderer*>(Engine->GetGameObject("Return to Main Menu Button")->GetComponent(ComponentType::Renderer));

		resumeGO_RDR->SetVisibility(false);
		HowToPlayBtnGO_RDR->SetVisibility(false);
		QuitGameGO_RDR->SetVisibility(false);


		//show the buttons, for the quit game confirmation
		Renderer* quitgame_Confirmation = dynamic_cast<Renderer*>(Engine->GetGameObject("Quit Game Confirmation")->GetComponent(ComponentType::Renderer));
		Renderer* quitgame_Yes = dynamic_cast<Renderer*>(Engine->GetGameObject("Quit Game Yes")->GetComponent(ComponentType::Renderer));
		Renderer* quitgame_No = dynamic_cast<Renderer*>(Engine->GetGameObject("Quit Game No")->GetComponent(ComponentType::Renderer));


		quitgame_Confirmation->SetVisibility(true);
		quitgame_Yes->SetVisibility(true);
		quitgame_No->SetVisibility(true);


		/*
		LogicTargets* LT = dynamic_cast<LogicTargets*>(GO->GetComponent(ComponentType::LogicTargets));
		for (size_t a = 0; a < LT->Targets.size(); a++)
		{
			//GameUI* gui_go_tgt = dynamic_cast<GameUI*>(LT->Targets[a]->GetComponent(ComponentType::GameUI));
			Renderer* rdr_go_tgt = dynamic_cast<Renderer*>(LT->Targets[a]->GetComponent(ComponentType::Renderer));
			if (LT->Targets[a]->GetName() == "Play/Pause Button" ||
				LT->Targets[a]->GetName() == "Resume Game Button" ||
				LT->Targets[a]->GetName() == "How To Play Button" ||
				LT->Targets[a]->GetName() == "Return to Main Menu Button")
			{
				
				//gui_go_tgt->SetActive(false);
				rdr_go_tgt->SetVisibility(false);
				//std::cout << LT->Targets[a]->GetName() << "set to inactive" << std::endl;
			}
			else
			{
				//gui_go_tgt->SetActive(true);
				rdr_go_tgt->SetVisibility(true);
			}
			
		}
		*/

		//set the 3 underlying buttons to inactive

	}
	/**************************************************************************/
	/*!
		\brief
			QuitGameYes() will quit the game and close the window
	*/
	/**************************************************************************/
	void QuitGameYes()
	{
		//glfwSetWindowShouldClose(GLSetup::ptr_window, true);
		is_paused = false;
		QuitGameBtn::QuitGameConfirmation_Active = false;
		SceneManager::StoreLevel("./Scene/MainMenu.scn");
		
	}
	void QuitGameNo(GameObject* GO)
	{
		QuitGameBtn::QuitGameConfirmation_Active = false;
	
		
		//set the quit game confirmation stuff to visibility FALSE
		Renderer* quitgame_Confirmation = dynamic_cast<Renderer*>(Engine->GetGameObject("Quit Game Confirmation")->GetComponent(ComponentType::Renderer));
		Renderer* quitgame_Yes = dynamic_cast<Renderer*>(Engine->GetGameObject("Quit Game Yes")->GetComponent(ComponentType::Renderer));
		Renderer* quitgame_No = dynamic_cast<Renderer*>(Engine->GetGameObject("Quit Game No")->GetComponent(ComponentType::Renderer));


		quitgame_Confirmation->SetVisibility(false);
		quitgame_Yes->SetVisibility(false);
		quitgame_No->SetVisibility(false);


		//set the 4 pause menu to visibility TRUE
		Renderer* resumeGO_RDR = dynamic_cast<Renderer*>(Engine->GetGameObject("Resume Game Button")->GetComponent(ComponentType::Renderer));
		Renderer* HowToPlayBtnGO_RDR = dynamic_cast<Renderer*>(Engine->GetGameObject("How To Play Button")->GetComponent(ComponentType::Renderer));
		Renderer* QuitGameGO_RDR = dynamic_cast<Renderer*>(Engine->GetGameObject("Return to Main Menu Button")->GetComponent(ComponentType::Renderer));
		Renderer* PlayPauseGO_RDR = dynamic_cast<Renderer*>(Engine->GetGameObject("Play/Pause Button")->GetComponent(ComponentType::Renderer));

		resumeGO_RDR->SetVisibility(true);
		HowToPlayBtnGO_RDR->SetVisibility(true);
		QuitGameGO_RDR->SetVisibility(true);
		PlayPauseGO_RDR->SetVisibility(true);

		/*
		LogicTargets* LT = dynamic_cast<LogicTargets*>(GO->GetComponent(ComponentType::LogicTargets));
		for (size_t a = 0; a < LT->Targets.size(); a++)
		{
			//GameUI* gui_go_tgt = dynamic_cast<GameUI*>(LT->Targets[a]->GetComponent(ComponentType::GameUI));
			Renderer* rdr_go_tgt = dynamic_cast<Renderer*>(LT->Targets[a]->GetComponent(ComponentType::Renderer));

			// dont have to set the other buttons to active as the 
			// Play/Pause Button and the is_paused global static handles its activeness
			if (LT->Targets[a]->GetName() == "Play/Pause Button") 
			{

				//gui_go_tgt->SetActive(true);
				//std::cout << LT->Targets[a]->GetName() << "set to inactive" << std::endl;
				rdr_go_tgt->SetVisibility(true);
			}
			else
			{
				//gui_go_tgt->SetActive(false);
				rdr_go_tgt->SetVisibility(false);
			}
		}
		*/

		//set ownself to inactive also
		dynamic_cast<Renderer*>(GO->GetComponent(ComponentType::Renderer))->SetVisibility(false);
	}
}
namespace ResumeGameBtn
{
	/**************************************************************************/
	/*!
		\brief
			ResumeGame() will unpause the game
		\param pause_bool
			The global pause bool which can pause or unpause the game
	*/
	/**************************************************************************/
	void ResumeGame(bool& pause_bool)
	{
		pause_bool = !pause_bool;
	}
};

namespace HowToPlayBtn
{
	/**************************************************************************/
	/*!
		\brief
			HowToPlay() will bring the user activate the HowToPlayScreen() GO
		\param GO
			The specific Gameobject who is the owner this script function
	*/
	/**************************************************************************/
	void HowToPlay(GameObject* GO)
	{
		(void)GO;
		std::cout << "HowToPlay btn pressed" << std::endl;
		HowToPlayBtn::HowToPlayScreen_Active = true;

		//set the 4 pause menu as visbility false
		Renderer* resumeGO_RDR = dynamic_cast<Renderer*>(Engine->GetGameObject("Resume Game Button")->GetComponent(ComponentType::Renderer));
		Renderer* HowToPlayBtnGO_RDR = dynamic_cast<Renderer*>(Engine->GetGameObject("How To Play Button")->GetComponent(ComponentType::Renderer));
		Renderer* QuitGameGO_RDR = dynamic_cast<Renderer*>(Engine->GetGameObject("Return to Main Menu Button")->GetComponent(ComponentType::Renderer));
		Renderer* PlayPauseGO_RDR = dynamic_cast<Renderer*>(Engine->GetGameObject("Play/Pause Button")->GetComponent(ComponentType::Renderer));
		
		//check if the bag icon exists or not, not all scenes has a bagicon GO
		if (Engine->GetGameObject("Bag Icon") != nullptr)
		{
			dynamic_cast<Renderer*>(Engine->GetGameObject("Bag Icon")->GetComponent(ComponentType::Renderer))->SetVisibility(false);
		}
	
		
	
		resumeGO_RDR->SetVisibility(false);
		HowToPlayBtnGO_RDR->SetVisibility(false);
		QuitGameGO_RDR->SetVisibility(false);
		PlayPauseGO_RDR->SetVisibility(false);

		//set the how to play screen to set the 4 as visbility true
		Renderer* HTPScreenGO_RDR = dynamic_cast<Renderer*>(Engine->GetGameObject("HowToPlay Screen")->GetComponent(ComponentType::Renderer));
		HTPScreenGO_RDR->SetVisibility(true);
		// set the correct text uv min max for the first page
		HowToPlayBtn::frame_idx = 0;
		HTPScreenGO_RDR->SetUVMin(HowToPlayBtn::uv_min_arr[HowToPlayBtn::frame_idx]);
		HTPScreenGO_RDR->SetUVMax(HowToPlayBtn::uv_max_arr[HowToPlayBtn::frame_idx]);

		

		/*
		LogicTargets* LT = dynamic_cast<LogicTargets*>(GO->GetComponent(ComponentType::LogicTargets));
		
		for (size_t a = 0; a < LT->Targets.size(); a++)
		{
			GameUI* gui_go_tgt = dynamic_cast<GameUI*>(LT->Targets[a]->GetComponent(ComponentType::GameUI));
			Renderer* rdr_go_tgt = dynamic_cast<Renderer*>(LT->Targets[a]->GetComponent(ComponentType::Renderer));
			//std::cout << LT->Targets[a]->GetName() << std::endl;
			
			if (LT->Targets[a]->GetName() != "HowToPlay Screen")
			{
				// sets all other GUI targets at inactive
				//gui_go_tgt->SetActive(false);
				rdr_go_tgt->SetVisibility(false);
			}
			else
			{
				// sets the HowToPlay Screen at active
				//gui_go_tgt->SetActive(true);
				rdr_go_tgt->SetVisibility(true);
				// init the first image
				//rdr_go_tgt->SetTexHandle(HowToPlayBtn::TEXHDLS_vect[HowToPlayBtn::frame_idx]);
			}
		}
		*/
	}
	/**************************************************************************/
	/*!
		\brief
			HowToPlayScreen() will show the users the user the instructions of
			the game.
		\param GO
			The specific Gameobject who is the owner this script function
	*/
	/**************************************************************************/
	void HowToPlayScreen(GameObject* GO)
	{
		// increment the frame
		++HowToPlayBtn::frame_idx;
		//GameUI* gui_go = dynamic_cast<GameUI*>(GO->GetComponent(ComponentType::GameUI));
		Renderer* rdr_go = dynamic_cast<Renderer*>(GO->GetComponent(ComponentType::Renderer));
		// if it isnt the last instruction frame
		if ((HowToPlayBtn::frame_idx) < HowToPlayBtn::max_HTP_Screens)
		{
			rdr_go->SetUVMin(HowToPlayBtn::uv_min_arr[HowToPlayBtn::frame_idx]);
			rdr_go->SetUVMax(HowToPlayBtn::uv_max_arr[HowToPlayBtn::frame_idx]);
			
		}
		else
		{
			HowToPlayBtn::frame_idx = 0;
			rdr_go->SetVisibility(false);
			HowToPlayBtn::HowToPlayScreen_Active = false;


			Renderer* resumeGO_RDR = dynamic_cast<Renderer*>(Engine->GetGameObject("Resume Game Button")->GetComponent(ComponentType::Renderer));
			Renderer* HowToPlayBtnGO_RDR = dynamic_cast<Renderer*>(Engine->GetGameObject("How To Play Button")->GetComponent(ComponentType::Renderer));
			Renderer* QuitGameGO_RDR = dynamic_cast<Renderer*>(Engine->GetGameObject("Return to Main Menu Button")->GetComponent(ComponentType::Renderer));
			Renderer* PlayPauseGO_RDR = dynamic_cast<Renderer*>(Engine->GetGameObject("Play/Pause Button")->GetComponent(ComponentType::Renderer));
			//check if the bag icon exists or not, not all scenes has a bagicon GO
			if (Engine->GetGameObject("Bag Icon") != nullptr)
			{
				dynamic_cast<Renderer*>(Engine->GetGameObject("Bag Icon")->GetComponent(ComponentType::Renderer))->SetVisibility(true);
			}

			resumeGO_RDR->SetVisibility(true);
			HowToPlayBtnGO_RDR->SetVisibility(true);
			QuitGameGO_RDR->SetVisibility(true);
			PlayPauseGO_RDR->SetVisibility(true);

			/*
			LogicTargets* LT = dynamic_cast<LogicTargets*>(GO->GetComponent(ComponentType::LogicTargets));

			for (size_t a = 0; a < LT->Targets.size(); a++)
			{
				Renderer* rdr_go_tgt = dynamic_cast<Renderer*>(LT->Targets[a]->GetComponent(ComponentType::Renderer));
				rdr_go_tgt->SetVisibility(is_paused);
			}
			*/
		}

	}
};