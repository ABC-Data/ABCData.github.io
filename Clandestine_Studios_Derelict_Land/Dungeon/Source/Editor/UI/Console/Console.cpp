/*!*************************************************************************
\file   Console.cpp
\author Anderson Phua
\email: a.phua@digipen.edu
\date	26-11-22
\brief  Console used in game for cheat codes and debugging purposes.
		Also used to display messages to the user.

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*****************************************************************************/
#include "Console.h"
#include "Editor/IMGUIHeaders.h"

/**************************************************************************/
/*!
* \brief
* Constructor.
*/
/**************************************************************************/
Console::Console(bool nOpen, Vec2 nPos, bool nEnter) : open(nOpen), pos(nPos), entered(nEnter) {}

/**************************************************************************/
/*!
* \brief
* Destructor.
*/
/**************************************************************************/
Console::~Console() {}

/**************************************************************************/
/*!
* \brief
* Initializes the console.
*/
/**************************************************************************/
void Console::Init()
{
	ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize({ consoleWidth, consoleHeight });
}

/**************************************************************************/
/*!
* \brief
* Shuts down the console.
*/
/**************************************************************************/
void Console::Shutdown() { messages.clear(); }

/**************************************************************************/
/*!
* \brief
* Updates the console.
*/
/**************************************************************************/
void Console::Update(float dt) { (void)dt; if (GetOpen()) Draw(); }

/**************************************************************************/
/*!
* \brief
* Draws the console.
*/
/**************************************************************************/
void Console::Draw()
{
	ImGui::Begin("Console", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
	{
		// Draw messages
		ImGui::BeginChild("Messages", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
		{
			for (auto& msg : messages)
			{
				ImGui::TextUnformatted(msg.c_str());
			}
			if (scrollToBottom) ImGui::SetScrollHereY(1.0f);
			scrollToBottom = false;
		}
		ImGui::EndChild();

		// Draw input
		ImGui::PushItemWidth(-1);
		char input[256]{ 0 };
		if (ImGui::InputText("Input", input, sizeof(input), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			entered = true; lastInput = input;
			messages.emplace_back(lastInput);
			scrollToBottom = true;
			memset(input, 0, sizeof(input));
		}
		ImGui::PopItemWidth();
		if (ImGui::IsItemHovered()) { ImGui::SetTooltip(
			"Console input box. Meant for enter cheat codes.\n"
			"Commands list:\ngod\n""win\n""hp\n""speed\n""slow\n","lose\n");}
	}
	
	ImGui::End();
}

/**************************************************************************/
/*!
* \brief
* Returns whether the console is open.
* \return
* True if the console is open, false otherwise.
*/
/**************************************************************************/
bool Console::GetOpen() const { return open; }

/**************************************************************************/
/*!
* \brief
* Sets whether the console is open.
* \param nOpen
* Next state of the console.
*/
/**************************************************************************/
void Console::SetOpen(const bool nOpen) { open = nOpen; }

/**************************************************************************/
/*!
* \brief
* Returns the last input.
* \return
* Last input entered into the console as a C++ string.
*/
/**************************************************************************/
std::string Console::GetLastInput() { return lastInput; }

/**************************************************************************/
/*!
* \brief
* Returns the one and only console instance.
* \return
* Console instance.
*/
/**************************************************************************/
Console* Console::GetInstance() { assert(!consoleInst); consoleInst = true; return new Console(false, Vec2(GLSetup::current_width * 0.5f, GLSetup::current_height * 0.5f)); }