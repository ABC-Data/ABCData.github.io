/*!*************************************************************************
\file   Logger.cpp
\author Anderson Phua
\email: a.phua@digipen.edu
\date   14-10-22
\brief  This source file contains the required functionalities in detail to
		operate the Logger system. This system is used to log messages to
		the console.

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*****************************************************************************/
#include "Logger.h"
#include "Input/Input.h"

#include "Editor/IMGUIHeaders.h"

// Message type colours
namespace LogTypeColors
{
	// Normal - Green | Warning - Yellow | Error - Red
	constexpr ImVec4 Normal = { 0.125f, 0.75f, 0.125f, 1.0f };
	constexpr ImVec4 Warning = { 1.0f, 1.0f, 0.0f, 1.0f };
	constexpr ImVec4 Error = { 0.75f, 0.125f, 0.125f, 1.0f };
}

namespace { std::string msgQuantityMsg{}; }

/**************************************************************************/
/*! \brief Constructor.
/**************************************************************************/
Logger::Logger(bool nOpen, Vec2 nPos) : logs{ { "" } }, loggerPos{ nPos }, loggerOpen{ nOpen }, debugData{ nOpen } {}

/**************************************************************************/
/*! \brief Destructor.
/**************************************************************************/
Logger::~Logger() { }

/**************************************************************************/
/*! \brief Initializes the Logger system.
/**************************************************************************/
void Logger::Init()
{
	// Always set the window to loggerPos, ignore the imugi .ini file
	ImGui::SetNextWindowPos(ImVec2(loggerPos.x, loggerPos.y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(loggerWidth, loggerHeight));
	ImGui::SetNextWindowCollapsed(!loggerOpen, ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(0.75f);
}

/**************************************************************************/
/*! \brief Shuts down the Logger system. Clears the vector of strings in its logs.
/**************************************************************************/
void Logger::Shutdown() { LogClear(); }

/**************************************************************************/
/*!
\brief
	Updates the Logger system.
\param dt
	The delta time - time between frames.
*/
/**************************************************************************/
void Logger::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	if (ImGui::IsKeyDown(ImGuiKey_Backslash)) { SetOpen(!LogOpen()); }

	if (LogOpen())
	{
		ImGui::SetNextWindowSize(ImVec2(loggerWidth, loggerHeight));
		ImGui::Begin("Logger", 0, ImGuiWindowFlags_NoCollapse);

		// Clear console
		if (ImGui::Button("Clear")) { LogClear(); } ImGui::SameLine(); ImGui::Checkbox("Debug Data", &debugData);
		if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Shows FPS, delta time, performance, etc."); } // Help marker

		ImGui::SameLine(); ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);

		// number of msgs of each type
		msgQuantityMsg = "Normal: " + std::to_string(normalMsgs);							ImGui::SameLine();
		if (ImGui::Button(msgQuantityMsg.c_str())) { hideNormalMsgs = !hideNormalMsgs; }	ImGui::SameLine();
		msgQuantityMsg = "Warnings: " + std::to_string(warningMsgs);						ImGui::SameLine();
		if (ImGui::Button(msgQuantityMsg.c_str())) { hideWarningMsgs = !hideWarningMsgs; }	ImGui::SameLine();
		msgQuantityMsg = "Errors: " + std::to_string(errorMsgs);							ImGui::SameLine();
		if (ImGui::Button(msgQuantityMsg.c_str())) { hideErrorMsgs = !hideErrorMsgs; }		ImGui::SameLine();

		ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

		// Create input textbox for console commands
		ImGui::SameLine(); ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.35f);
		ImGui::InputText("##Command", &logStringFilter); ImGui::PopItemWidth();
		if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Log filter (\"\" = all, \"error\" = errors, etc.)"); } // Help marker
		

		ImGui::Separator();

		// Place the logs into a child window so that when the logs exceed the height 
		// of the window, so that the clear (and other?) buttons is always visible.
		ImGui::BeginChild("ScrolllingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

		if (debugData) { DisplayDebugData(); }

		// Display non-debug data log text
		PrintConsole(logs);

		ImGui::EndChild();

		ImGui::End();
	}
}

/**************************************************************************/
/*!
* \brief
*	Sets the logger's visiblity.
* \param open
* 	The new visible state of the logger.
*/
/**************************************************************************/
void Logger::SetOpen(const bool open) { loggerOpen = open; }

/**************************************************************************/
/*!
* \brief
*	Returns visible state of the logger.
* \return
* 	True if the logger is visible, false otherwise.
*/
/**************************************************************************/
bool Logger::LogOpen() const { return loggerOpen; }

/**************************************************************************/
/*!
* \brief
*	Gets the current time in a string format.
* \return
* 	The current time in a string format.
*/
/**************************************************************************/
std::string GetCurrentTimestamp()
{
	time_t rawtime; struct tm timeinfo; char buffer[80];

	time(&rawtime); localtime_s(&timeinfo, &rawtime);

	strftime(buffer, sizeof(buffer), "%H:%M:%S", &timeinfo);
	return std::string(buffer);
}

/**************************************************************************/
/*!
* \brief
*	Emplaces a new string into the logText vector.
* \param line
*  The string to be put into the logText vector.
*/
/**************************************************************************/
void Logger::Log(std::string line, const LogType type)
{
	logs.emplace_back("[" + std::move(GetCurrentTimestamp()) + "] " + line, type);
	switch (type)
	{
	case LogType::Normal:	++normalMsgs;	break;
	case LogType::Warning:	++warningMsgs;	break;
	case LogType::Error:	++errorMsgs;	break;
	}
}

/**************************************************************************/
/*!
* \brief
*  Clears all memory for the texts entered into the logger thus far.
*/
/**************************************************************************/
void Logger::LogClear() { for (auto& text : logs) { text.message.clear(); } logs.clear(); normalMsgs = warningMsgs = errorMsgs = 0; }

/**************************************************************************/
/*!
* \brief
*  Returns instance of the Logger system. Ensures strictly
*  singular instance of the Logger system without global access that could
*  cause unintentional coupling issues between systems.
*
*  Said singular instance is created in the Editor's constructor's init list.
* \return
*  Returns the singleton instance of the Logger system.
*/
/**************************************************************************/
// The assert stops any attempt to output log text directly through the logger.
Logger* Logger::GetInstance() { assert(!instantiated); instantiated = true; return new Logger(true, Vec2(0.0f, 20.0f)); }

/**************************************************************************/
/*!
* \brief
*  Prints a line of text to the console, in a particular color depending on
*  the type of message it is.
* \param line
*  The line to print.
* \param type
* The type of message printed.
*/
/**************************************************************************/
void Logger::PrintLine(const std::string line, const LogType type)
{
	if (type == LogType::Normal) { ImGui::PushStyleColor(ImGuiCol_Text, LogTypeColors::Normal); }
	else if (type == LogType::Warning) { ImGui::PushStyleColor(ImGuiCol_Text, LogTypeColors::Warning); }
	else if (type == LogType::Error) { ImGui::PushStyleColor(ImGuiCol_Text, LogTypeColors::Error); }
	else { ImGui::PushStyleColor(ImGuiCol_Text, LogTypeColors::Normal); }
	ImGui::TextWrapped(line.c_str()); ImGui::PopStyleColor();
}

/**************************************************************************/
/*!
* \brief
* Prints the console text.
* \param consoleText
* The text to be printed.
*/
/**************************************************************************/
void Logger::PrintConsole(std::vector<LogMessage>& consoleText)
{
	for (auto& log : consoleText)
	{
		// Pass logs that don't match the case-sensitive filter
		if (!logStringFilter.empty()) { if (log.message.find(logStringFilter) == std::string::npos) { continue; } }

		if (hideErrorMsgs && log.type == LogType::Error || hideWarningMsgs && log.type == LogType::Warning
			|| hideNormalMsgs && log.type == LogType::Normal) { continue; }
		PrintLine(log.message.data(), log.type);
	}
}

/**************************************************************************/
/*!
* \brief
* 	Displays the debug data in the logger console.
*/
/**************************************************************************/
void Logger::DisplayDebugData()
{
	// Title, window width & height
	ImGui::TextColored(LogTypeColors::Normal, GLSetup::title.c_str());
	ImGui::TextColored(LogTypeColors::Normal, std::string_view("Window Width: " + std::to_string(GLSetup::current_width) + " | Window Height: " + std::to_string(GLSetup::current_height)).data());
	// FPS, DT, Frames
	ImGui::TextColored(LogTypeColors::Normal, std::string_view("FPS: " + std::to_string(GLSetup::fps) + " | Delta Time: " + std::to_string(GLSetup::delta_time)).data());
	ImGui::SameLine();
	ImGui::TextColored(LogTypeColors::Normal, std::string_view("| Frames: " + std::to_string(GLSetup::frame_no)).data());
	// Last GLFW mouse input data
	ImGui::TextColored(LogTypeColors::Normal, std::string_view(LogKeyInput()).data());
	ImGui::TextColored(LogTypeColors::Normal, std::string_view(LogMouseInput()).data());
	// IMGUI Mouse input data
	ImGui::TextColored(LogTypeColors::Normal, std::string_view("ImGui Mouse Pos: " + std::to_string(ImGui::GetMousePos().x) + ", " + std::to_string(ImGui::GetMousePos().y)).data());
}