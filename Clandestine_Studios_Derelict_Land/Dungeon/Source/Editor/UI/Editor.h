/*!*************************************************************************
\file   Editor.h
\author Anderson Phua
\email: a.phua@digipen.edu
\date   01-11-22
\brief  This header file contains the necessary functionalities to operate
		the Editor system. This system is used to control, update, and draw
		the game / level editor.

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*****************************************************************************/
#pragma once
#ifndef EDITOR_H
#define EDITOR_H

#include "Editor/UI/Logger/Logger.h"
#include "Editor/UI/MenuBar/MenuBar.h"
#include "Editor/UI/Hierarchy/Hierarchy.h"
#include "Editor/UI/PropertyEditor/PropertyEditor.h"
#include "Editor/UI/GameWindow/GameWindow.h"
#include "Editor/UI/FileBrowser/FileBrowser.h"
#include "Editor/UI/Console/Console.h"

enum class EditorState { EditorPlaying = 0, EditorPaused, EditorStopped, EditorStateMax };

class PropertyEditor;
class GameWindow;

enum class EditorSystem
{
	Logger = 0,
	MenuBar,
	Hierarchy,
	Inspector,
	GameWindow,
	FileBrowser,
	Console,
	// ASSET_MANAGER,
	// SCENE_MANAGER,
	// AUDIO,
	EditorSystemMax
};

class Editor : public System
{
	Editor(bool nOpen = true, EditorState nState = EditorState::EditorStopped); // Constructor
	~Editor();
	
	Editor(const Editor&) = delete;				// Copy Constructor
	Editor& operator=(const Editor&) = delete;	// Copy Assignment Operator
	Editor(Editor&&) = delete;					// Move Constructor
	Editor& operator=(Editor&&) = delete;		// Move Assignment Operator
public:

	virtual void Init();
	virtual void Update(float dt);
	virtual void Shutdown();

	// IMGUI-related functions
	void InitIMGUI();
	void ShutdownIMGUI();
	void NewFrame();
	void Render();
	
	void SetOpen(const bool nOpen);

	// Editor-related functions
	EditorState GetState() const;
	void SetState(const EditorState nState);

	// Set the full screen state of the play mode of the Editor
	void SetFullScreen(const bool nFullScreen);

	// Output debug text
	void Log(const std::string, const Logger::LogType t = Logger::LogType::Normal);
	
	void LoadLevel(Level* nLevel);
	void LoadLevel(const std::string& nLevelName);

	static Editor* GetInstance();

	std::vector<std::unique_ptr<System>>* GetSystems();
	// Editor sub-system accessors
	MenuBar& GetMenuBar();
	Logger& GetLogger();
	Hierarchy& GetHierarchy();
	PropertyEditor& GetPropertyEditor();
	FileBrowser& GetFileBrowser();
	Console& GetConsole();
	GameWindow& GetGameWindow();
	
	bool open; // editor visible state
	bool playFullScreen = true; // play current scene with all other windows closed or not
private:
	EditorState state; // The play/pause state of the Editor

	// Sub-systems
	Logger* logger;
	Console* console;
	MenuBar* menuBar;
	Hierarchy* hierarchy;
	PropertyEditor* propertyEditor;
	FileBrowser* fileBrowser;
	GameWindow* gameWindow;
	
private:
	// Vector of systems that are exclusively part of the editor
	std::vector<std::unique_ptr<System>> systems; 
};

static Editor* editor = Editor::GetInstance();

#endif