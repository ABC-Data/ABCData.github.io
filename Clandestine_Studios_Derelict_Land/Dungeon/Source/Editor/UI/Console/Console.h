/*!*************************************************************************
\file   Console.h
\author Anderson Phua
\email: a.phua@digipen.edu
\date   26-11-22
\brief  Console used in game for cheat codes and debugging purposes.
		Also used to display messages to the user.
		
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*****************************************************************************/
#ifndef CONSOLE_H
#define CONSOLE_H

#include "System/System.h"

class Console : public System
{
public:
	const float consoleWidth = 200.0f; // Width ...
	const float consoleHeight = 300.0f; // ... and height of console window
private:
	/**************************************************************************/
	/*!
	\brief
		Constructor.
	*/
	/**************************************************************************/
	Console(bool nOpen = true, Vec2 nPos = { 100.0f, 50.0f }, bool nEnter = false);

	/**************************************************************************/
	/*!
	\brief
		Destructor.
	*/
	/**************************************************************************/
	~Console();
public:
	// Init, update, draw & shutdown the console
	virtual void Init();
	virtual void Shutdown();
	virtual void Update(float dt);
	virtual void Draw();

	/**************************************************************************/
	/*!
	* \brief
	* Returns whether the console is open.
	* \return
	* True if the console is open, false otherwise.
	*/
	/**************************************************************************/
	bool GetOpen() const;
	
	/**************************************************************************/
	/*!
	* \brief
	* Sets whether the console is open.
	* \param nOpen
	* Next state of the console.
	*/
	/**************************************************************************/
	void SetOpen(const bool nOpen);
	
	/**************************************************************************/
	/*!
	* \brief
	* Returns the last input.
	* \return
	* Last input entered into the console as a C++ string.
	*/
	/**************************************************************************/
	std::string GetLastInput();
	
	/**************************************************************************/
	/*!
	* \brief
	* Returns the one and only console instance.
	* \return
	* Console instance.
	*/
	/**************************************************************************/
	static Console* GetInstance();
	
	bool entered = false; // msg entered
private:
	std::vector<std::string> messages;
	bool scrollToBottom = true;
	std::string lastInput; // last input entered
	bool open; // console open state
	Vec2 pos; // Position of console window
};

namespace { bool consoleInst = false; }

#endif // !CONSOLE_H