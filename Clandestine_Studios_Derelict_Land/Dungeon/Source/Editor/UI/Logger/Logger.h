/*!*************************************************************************
\file   Logger.h
\author Anderson Phua
\email: a.phua@digipen.edu
\date   14-10-22
\brief  This header file contains the necessary functionalities to operate
		the Logger system. This system is used to log messages to the console.

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*****************************************************************************/
#ifndef LOGGER_H
#define LOGGER_H

#include "System/System.h"

class Logger : public System
{
public:
	enum class LogType { Normal = 0, Warning, Error, LogTypeMax };
	const float loggerWidth = 500.0f; // Width ...
	const float loggerHeight = 400.0f; // ... and height of logger window
private:
	struct LogMessage { std::string message; LogType type; };
	/**************************************************************************/
	/*!
	\brief
		Constructor.
	*/
	/**************************************************************************/
	Logger(bool nOpen = true, Vec2 nPos = { 0.0f, 0.0f });
	Logger(Logger const&) = delete; // Copy constructor
	Logger(Logger&&) = delete; // Move constructor
	Logger& operator=(Logger const&) = delete; // Copy assignment operator
	Logger& operator=(Logger&&) = delete; // Move assignment operator
	
	/**************************************************************************/
	/*!
	\brief
		Destructor.
	*/
	/**************************************************************************/
	~Logger();
public:
	/**************************************************************************/
	/*!
	\brief
		Initializes the Console system.
	*/
	/**************************************************************************/
	virtual void Init();

	/**************************************************************************/
	/*!
	\brief
		Shuts down the Console system.
	*/
	/**************************************************************************/
	virtual void Shutdown();

	/**************************************************************************/
	/*!
	\brief
		Updates the Console system.
	\param dt
		The delta time - time between frames.
	*/
	/**************************************************************************/
	virtual void Update(float dt);

	/**************************************************************************/
	/*!
	\brief
		Toggles the console.
	\param open
		New status of the console - open (true) or closed (false).
	*/
	/**************************************************************************/
	void SetOpen(const bool open);

	/**************************************************************************/
	/*!
	\brief
		Checks if the console is open or closed.
	\return
		Returns true if the console is open, false otherwise.
	*/
	/**************************************************************************/
	bool LogOpen() const;
	/**************************************************************************/
	/*!
	\brief
		Adds new line to the console.
	*/
	/**************************************************************************/
	void Log(std::string line, const LogType type = LogType::Normal);
	/**************************************************************************/
	/*!
	\brief
		Checks if the console is toggled.
	\return
		Returns true if the console is toggled.
	*/
	/**************************************************************************/
	void LogClear();

	/**************************************************************************/
	/*!
	* \brief
	*  Returns the singleton instance of the Console system.
	* \return
	* Returns the singleton instance of the Console system.
	*/
	/**************************************************************************/
	static Logger* GetInstance();

	/**************************************************************************/
	/*!
	* \brief
	* Prints a string to the logger.
	* \param line
	* The string to be printed.
	*/
	/**************************************************************************/
	void PrintLine(const std::string line, const LogType type = LogType::Normal);
private:
	/**************************************************************************/
	/*!
	* \brief
	* Prints the console text.
	* \param consoleText
	* The text to be printed.
	*/
	/**************************************************************************/
	void PrintConsole(std::vector<LogMessage>&);
	
	void DisplayDebugData();
	
	std::vector<LogMessage> logs;
	Vec2 loggerPos; // Top left corner
	bool loggerOpen = true, debugData = true;
	std::string logStringFilter{};

	// Number of texts of each type
	unsigned int normalMsgs = 0, warningMsgs = 0, errorMsgs = 0;
	bool hideNormalMsgs = false, hideWarningMsgs = false, hideErrorMsgs = false;
};

namespace { bool instantiated = false; }

#endif // LOGGER_H