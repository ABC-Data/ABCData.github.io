/*!*************************************************************************
\file	Editor.cpp
\author Anderson Phua
\email: a.phua@digipen.edu
\date	01-11-22
\brief	This file contains the Editor class which is used to create & run the
		game / level editor window.

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*****************************************************************************/
#include "SceneManager/SceneManager.h"
#include "Editor/UI/Editor.h"
#include "Editor/IMGUIHeaders.h"
#include "Deserializer/Deserializer.h"
#include "Input/Input.h"
#include "Audio/Audio.h"

bool editor_open;

namespace
{
	Keycode editorPlayKey = Key::F5; // Put into editor play mode
	Keycode editorStopKey = Key::F6; // Stops gameplay inside editor
	Keycode editorSaveKey = Key::S;  // Saves the current level
}

/*!*************************************************************************
* \brief Constructor
/*!*************************************************************************/
Editor::Editor(bool nOpen, EditorState nState) : open(nOpen), state(nState),
logger(Logger::GetInstance()), console(Console::GetInstance()), menuBar(new MenuBar()), hierarchy(new Hierarchy()),
propertyEditor(PropertyEditor::GetInstance()), gameWindow(new GameWindow()), fileBrowser(FileBrowser::GetInstance())
{
	systems.reserve(sizeof(System*) * static_cast<int>(EditorSystem::EditorSystemMax));
}

/*!*************************************************************************
* \brief Destructor
/*!*************************************************************************/
Editor::~Editor() {}

/*!*************************************************************************
* \brief Initializes the Editor system.
/****************************************************************************/
void Editor::Init()
{
	InitIMGUI(); // Initialize IMGUI

	// Initialize the systems that are part of the editor
	systems.emplace_back(logger);
	systems.emplace_back(menuBar);
	systems.emplace_back(hierarchy);
	systems.emplace_back(propertyEditor);
	systems.emplace_back(gameWindow);
	systems.emplace_back(fileBrowser);
	systems.emplace_back(console);

	// Initialize systems
	for (auto& system : systems) { system->SetLevel(this->GetLevel()); system->Init(); } editor_open = true;
#if _DEBUG
#else
	// code that locks off the editor (less the console)

	state = EditorState::EditorPlaying;
	editor_open = false;
	// set open all systems
	logger->SetOpen(editor_open);
	menuBar->SetOpen(editor_open);
	hierarchy->SetOpen(editor_open);
	propertyEditor->SetOpen(editor_open);
	gameWindow->SetOpen(editor_open);
	fileBrowser->SetOpen(editor_open);
#endif
}

/*!*************************************************************************
* \brief	Updates the Editor system.
* \param	dt - The delta time.
/****************************************************************************/
void Editor::Update(float dt)
{
	(void)dt;

#if _DEBUG

	// Toggling between editor & game left to debug mode
	if (ImGui::IsKeyPressed(ImGuiKey_F5))
	{
		state = (state == EditorState::EditorPlaying) ? EditorState::EditorStopped : EditorState::EditorPlaying;
		
		// Stop game audio when entering editor mode
		audio->GetInstance()->ToggleMusicPause();
		audio->GetInstance()->ToggleMuteSound();
		
		if (playFullScreen)
		{
			editor_open = !editor_open;
			// set open all systems
			logger->SetOpen(editor_open);
			menuBar->SetOpen(editor_open);
			hierarchy->SetOpen(editor_open);
			propertyEditor->SetOpen(editor_open);
			propertyEditor->DeselectEntity(); // stop gizmos from showing up
			gameWindow->SetOpen(editor_open);
			fileBrowser->SetOpen(editor_open);
			console->SetOpen(false);
		}
	}
#endif

	NewFrame();

	// CTRL+S to save
	if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyReleased(ImGuiKey_S))
	{
		FilePtr::SaveLevel(this->gameWindow->GetLevel());
		Log("Saved " + this->GetLevel()->GetId());
	}

	// Removing this line may cause IMGUI windows to flicker when resizing them.
	if (editor_open) { ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()); }

	// Update sub-systems
	for (auto& system : systems) { system->Update(dt); }

	Render();
}

/**************************************************************************/
/*!
* \brief
* 	Shutdown the Editor system. Clears memory allocated for its sub-systems,
*	as well as its own, and for any IMGUI-related operations.
*/
/**************************************************************************/
void Editor::Shutdown()
{
	for (auto& system : systems) { system->Shutdown(); }
	ShutdownIMGUI();
}

/**************************************************************************/
/*! \brief Initializes the IMGUI library as equired to create the editor window.
/**************************************************************************/
void Editor::InitIMGUI()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION(); ImGui::CreateContext(); ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
	// The flag below makes the IMGUI windows to be moved around outside of the main window
	// io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags) { style.WindowRounding = 2.0f; style.Colors[ImGuiCol_WindowBg].w = 1.0f; }

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(GLSetup::ptr_window, true);
	ImGui_ImplOpenGL3_Init("#version 450"); // same as in Fonts.cpp
}

/**************************************************************************/
/*! \brief Shutdown the IMGUI libraries used.
/**************************************************************************/
void Editor::ShutdownIMGUI() { ImGui_ImplOpenGL3_Shutdown(); ImGui_ImplGlfw_Shutdown(); ImGui::DestroyContext(); }

/**************************************************************************/
/*! \brief IMGUI side initialization of the next new frame.
/**************************************************************************/
void Editor::NewFrame() { ImGui_ImplOpenGL3_NewFrame(); ImGui_ImplGlfw_NewFrame(); ImGui::NewFrame(); }

/**************************************************************************/
/*! \brief IMGUI side rendering of the current frame.
/**************************************************************************/
void Editor::Render()
{
	// Rendering
	ImGui::Render(); ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context)
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(glfwGetCurrentContext());
	}
}

/**************************************************************************/
/*!
*	\brief
*	Sets whether the editor is open and visible to the user.
*	\param nOpen
*	The new visible state of the editor.
/**************************************************************************/
void Editor::SetOpen(const bool nOpen) { open = nOpen; editor_open = nOpen; }

/**************************************************************************/
/*!
*	\brief
*	Retrieves state of the editor - whether in edit or game mode.
*	\return
*	State of the editor - as an EditorState enum.
/**************************************************************************/
EditorState Editor::GetState() const { return state; }

/**************************************************************************/
/*!
*	\brief
*	Set state of editor.
*	\param nState
*	New state of the editor.
/**************************************************************************/
void Editor::SetState(const EditorState nState) { state = nState; }

/**************************************************************************/
/*!
*	\brief
*	Sets whether the other windows besides the Game Window are visible when
*	the editor is in game mode.
*	\param nFullScreen
*	New state of the non-Game Window IMGUI windows.
/**************************************************************************/
void Editor::SetFullScreen(const bool nFullScreen) { playFullScreen = nFullScreen; }

/**************************************************************************/
/*!
*	\brief
*	Prints text into the logger window.
*	\param text
*	The string of text to be printed.
/**************************************************************************/
void Editor::Log(const std::string text, const Logger::LogType type) { logger->Log(std::string(text), type); }

/**************************************************************************/
/*!
*	\brief
*	Loads in a new level file. Also unloads the	previous level used.
*	\param	nLevel
*	Pointer to the incoming level object to be loaded in.
*/
/**************************************************************************/
void Editor::LoadLevel(Level* nLevel) { SceneManager::LoadLevel(nLevel); }

/**************************************************************************/
/*!
*	\brief
*	Loads in new level scene through its file name specfied. Also unloads the
*	previous level used, before having all systems point to the new one.
*	\param	nLevelName
*	Name of the file of the incoming level scene file, w/ the .scn extension.
*/
/**************************************************************************/
void Editor::LoadLevel(const std::string& nLevelName) { SceneManager::LoadLevel(nLevelName.c_str()); }

/**************************************************************************/
/*!
*	\brief
*	Returns the only instance of the editor, and allocates memory for it if
*	being called for the first time.
*	\return
*	Pointer to the editor instance.
/**************************************************************************/
Editor* Editor::GetInstance() { if (!editor) { editor = new Editor(); } return editor; }

#pragma region Editor System & Sub-systems

/**************************************************************************/
/*!
*	\brief
*	Gets the editor systems vector.
*	\return
*	Normal pointer to a vector of unique pointers to the editor sub-systems.
/**************************************************************************/
std::vector<std::unique_ptr<System>>* Editor::GetSystems() { return &systems; }

/**************************************************************************/
/*! \brief Returns a reference to the menu bar sub-system.
/**************************************************************************/
MenuBar& Editor::GetMenuBar() { return *menuBar; }

/**************************************************************************/
/*! \brief Returns a reference to the logger window.
/**************************************************************************/
Logger& Editor::GetLogger() { return *logger; }

/**************************************************************************/
/*! \brief Returns a reference to the property editor / inspector window.
/**************************************************************************/
PropertyEditor& Editor::GetPropertyEditor() { return *propertyEditor; }

/**************************************************************************/
/*! \brief Returns a reference to the hierarchy window.
/**************************************************************************/
FileBrowser& Editor::GetFileBrowser() { return *fileBrowser; }

/**************************************************************************/
/*! \brief Returns a reference to the console.
/**************************************************************************/
Console& Editor::GetConsole() { return *console; }

/**************************************************************************/
/*! \brief Returns a reference to the hierarchy window.
/**************************************************************************/
Hierarchy& Editor::GetHierarchy() { return *hierarchy; }

/**************************************************************************/
/*! \brief Returns a reference to the game window.
/**************************************************************************/
GameWindow& Editor::GetGameWindow() { return *gameWindow; }

#pragma endregion