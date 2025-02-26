/*!*************************************************************************
\file	MenuBar.cpp
\author Anderson Phua
\email: a.phua@digipen.edu
\date	03-02-23
\brief	Functionalities for the MenuBar system.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*****************************************************************************/
#include "MenuBar.h"
#include "Factory/Factory.h"
#include "Editor/UI/Editor.h"
#include "Editor/IMGUIHeaders.h"


/*
	When importing an asset (e.g. texture,audio,etc.), the current file path
	is deemed to be where the asset-to-be is, rather than the project's root
	directory. projectDir ensures there remains a way to access the data 
	text files containing the project's assets' records.
*/
namespace { std::string projectDir = ""; }

namespace { std::string selectedAssetName = ""; char sceneName[256] = ""; bool nSceneByHotkey = false; }

/**************************************************************************/
/*! \brief	Constructor.
/**************************************************************************/
MenuBar::MenuBar(bool nOpen) : open(nOpen) { }

/**************************************************************************/
/*! \brief	Destructor.
/**************************************************************************/
MenuBar::~MenuBar() { }

/**************************************************************************/
/*! \brief	Initialize the MenuBar system.
/**************************************************************************/
void MenuBar::Init() {
	projectDir = std::filesystem::current_path().string().c_str();
}

/**************************************************************************/
/*! \brief Shutdown the MenuBar system.
/**************************************************************************/
void MenuBar::Shutdown() { }

/**************************************************************************/
/*!
* \brief
* 	Update the MenuBar system.
* \param dt
* 	Delta time.
*/
/**************************************************************************/
void MenuBar::Update(float dt)
{
	(void)dt;
	if (open)
	{
		// CTRL-SHIFT-N - New GO
		if (ImGui::GetIO().KeyMods & (ImGuiKey_ModCtrl) && ImGui::GetIO().KeyMods & (ImGuiKey_ModShift) && ImGui::IsKeyPressed(ImGuiKey_N))
		{
			NewEmptyGameObject();
		}
		// CTRL-N - New Scene
		else if (ImGui::GetIO().KeyMods & ImGuiKey_ModCtrl && ImGui::IsKeyPressed(ImGuiKey_N)) { nSceneByHotkey = true; }

		DrawMenuBar();
	}
}

/**************************************************************************/
/*!
* \brief
* 	Set the open state of the MenuBar.
* \param nOpen
* 	New open state - whether the menu bar is displayed.
*/
/**************************************************************************/
void MenuBar::SetOpen(bool nOpen) { open = nOpen; }

void MenuBar::NewEmptyGameObject()
{
	this->GetLevel()->AddGameObject(factory->AddGameObject(projectDir + "/Data/EmptyGameObject.txt"));
	//factory->QueueAdd(factory->AddGameObject(projectDir + "/Data/EmptyGameObject.txt"));

	// camera positon (assumed to be center of main window)
	this->GetLevel()->GetGameObjectArray().back()->GetComponent<Transform>()->Position =
		GameObject::FindWithTag("Camera", this->GetLevel()->GetGameObjectArray())->GetComponent<Transform>()->Position;

	editor->GetHierarchy().RefreshHierarchy(); // Update the hierarchy

	// Set the selected game object to the newly created one
	editor->GetPropertyEditor().SetSelectedEntity((int)(this->GetLevel()->GetGOASize() - 1));
	editor->GetHierarchy().SetRenameTarget(this->GetLevel()->GetGameObjectArray().back());
}

/**************************************************************************/
/*! \brief Draw the MenuBar, and handle the menu bar's functionalities. */
/**************************************************************************/
void MenuBar::DrawMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("Create New Scene");
			if (ImGui::IsItemClicked(ImGuiMouseButton_Left) || nSceneByHotkey)
			{
				if (nSceneByHotkey) { nSceneByHotkey = false; }
				ImGui::OpenPopup("New Scene");
			}
			if (ImGui::BeginPopup("New Scene"))
			{
				ImGui::Text("Scene Name:"); 
				
				// keyboard focus on input box only
				ImGui::PushAllowKeyboardFocus(true);
				
				ImGui::InputText("##SceneName", sceneName, sizeof(sceneName));
				
				// if enter is pressed, create new scene
				if (ImGui::Button("Create") || ImGui::IsKeyPressedEx(ImGuiKey_Enter))
				{
					try {
						std::filesystem::copy_file(projectDir + "\\Scene\\EmptyScene.scn",projectDir + "\\Scene\\" + sceneName + ".scn", 
							std::filesystem::copy_options::overwrite_existing);
					} catch (std::filesystem::filesystem_error& e) { std::cout << e.what() << std::endl; }
					
					editor->Log("Created new scene: " + std::string(sceneName));
					
					strchr(sceneName, '\0')[0] = '\0';

					ImGui::CloseCurrentPopup();
				}

				ImGui::SameLine();

				if (ImGui::Button("Cancel")) { strchr(sceneName, '\0')[0] = '\0'; ImGui::CloseCurrentPopup(); }

				ImGui::PopAllowKeyboardFocus();

				ImGui::EndPopup();
			}

			if (ImGui::MenuItem("Load Scene"))
			{
				// Get windows search file window
				OPENFILENAME ofn = { 0 };
				char szFile[260] = { 0 };
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = GetActiveWindow();
				ofn.lpstrFile = (LPWSTR)szFile;
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = L"Scene Files (*.scn)\0*.scn\0"; // Filter / file extension to look for
				ofn.nFilterIndex = 1;
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = NULL;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

				// if the user selected a file, load it
				if (GetOpenFileName(&ofn))
				{
					// Get the file path
					std::wstring ws = ofn.lpstrFile;
					std::string s = std::string(ws.begin(), ws.end());
					editor->LoadLevel(s);
				}
			}

			if (ImGui::MenuItem("Save Scene", "CTRL + S"))
			{
				FilePtr::SaveLevel(this->GetLevel());
				editor->Log(std::string("Saved ") + this->GetLevel()->GetId().data());
			}

			if (ImGui::BeginMenu("Style")) // Change editor style
			{
				if (ImGui::MenuItem("Dark")) { ImGui::StyleColorsDark(); }
				if (ImGui::MenuItem("Light")) { ImGui::StyleColorsLight(); }
				if (ImGui::MenuItem("Classic")) { ImGui::StyleColorsClassic(); }

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Exit", "Alt + F4"))
			{
				// Quit by setting this window to close - the engine's main update loop runs so long as the main window is running

				glfwSetWindowShouldClose(GLSetup::ptr_window, true);
				// PostQuitMessage(0); // formerly the Win32 way --> quit app & close window
				// DestroyWindow(glfwGetWin32Window(GLSetup::ptr_window));
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Assets"))
		{
			if (ImGui::MenuItem("Import New Texture"))
			{
				// Get windows search file window
				OPENFILENAME ofn = { 0 }; char szFile[260] = { 0 };
				ofn.lStructSize = sizeof(ofn); ofn.hwndOwner = GetActiveWindow();
				ofn.lpstrFile = (LPWSTR)szFile; ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = L"Texture Files (*.png)\0*.png\0"; // Filter / file extension to look for
				ofn.nFilterIndex = 1; ofn.lpstrFileTitle = NULL; ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = L"./Data/Images"; ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

				// if the user selected a file, get the file path and load the file
				if (GetOpenFileName(&ofn)) { std::wstring ws = ofn.lpstrFile; LoadNewTexture(std::string(ws.begin(), ws.end())); }
			}

			if (ImGui::MenuItem("Import New Audio"))
			{
				// Get windows search file window
				OPENFILENAME ofn = { 0 }; char szFile[260] = { 0 };
				ofn.lStructSize = sizeof(ofn); ofn.hwndOwner = GetActiveWindow();
				ofn.lpstrFile = (LPWSTR)szFile; ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = L"Audio Files (*.mp3)\0*.mp3\0"; // Filter / file extension to look for
				ofn.nFilterIndex = 1; ofn.lpstrFileTitle = NULL; ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = L"./Audio"; ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

				// if the user selected a file, get filepath, load file
				if (GetOpenFileName(&ofn)) { std::wstring ws = ofn.lpstrFile; LoadNewAudio(std::string(ws.begin(), ws.end()).c_str()); }
			}

			ImGui::MenuItem("Remove Texture"); if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) { ImGui::OpenPopup("Remove Existing Texture"); selectedAssetName = "Textures"; }
			if (ImGui::BeginPopupModal("Remove Existing Texture", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("Enter the texture to remove:");
				Assets& mAssets = MasterObjectList->MasterAssets;

				// Combo box to select the texture to remove
				if (ImGui::BeginCombo("##TextureMapCombo", selectedAssetName.c_str(), ImGuiComboFlags_PopupAlignLeft))
				{
					for (auto& tex : mAssets.GetTextureMap())
					{
						if (ImGui::Selectable(tex.first.c_str(), selectedAssetName == tex.first, ImGuiSelectableFlags_SelectOnRelease)) { selectedAssetName = tex.first; }
					}
					ImGui::EndCombo();
				}

				if (ImGui::Button("Remove", ImVec2(120, 0)))
				{
					if (!selectedAssetName.empty())
					{
						mAssets.RemoveTexture(selectedAssetName);
						editor->Log(selectedAssetName + " removed.");
						selectedAssetName.clear(); ImGui::CloseCurrentPopup();
					}
				}

				ImGui::SameLine();

				if (ImGui::Button("Cancel", ImVec2(120, 0))) { selectedAssetName.clear(); ImGui::CloseCurrentPopup(); }

				ImGui::EndPopup();
			}

			ImGui::MenuItem("Remove Audio"); if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) { ImGui::OpenPopup("Remove Existing Audio"); selectedAssetName = "Audio"; }
			if (ImGui::BeginPopupModal("Remove Existing Audio", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("Enter the audio to remove:");
				Assets& mAssets = MasterObjectList->MasterAssets;

				// Dropdown box to select the audio to remove
				if (ImGui::BeginCombo("##AudioMapCombo", selectedAssetName.c_str(), ImGuiComboFlags_PopupAlignLeft))
				{
					char audioName[256];
					for (auto& sound : mAssets.GetAudioMap())
					{
						(*sound.get())->getName(audioName, 256);
						if (ImGui::Selectable(audioName, selectedAssetName == audioName, ImGuiSelectableFlags_SelectOnRelease)) { selectedAssetName = audioName; }
					}
					ImGui::EndCombo();
				}

				if (ImGui::Button("Remove", ImVec2(120, 0)))
				{
					if (!selectedAssetName.empty())
					{
						mAssets.RemoveAudio(selectedAssetName);
						editor->Log(selectedAssetName + " removed.");
						selectedAssetName.clear(); ImGui::CloseCurrentPopup();
					}
				}

				ImGui::SameLine();

				if (ImGui::Button("Cancel", ImVec2(120, 0))) { selectedAssetName.clear(); ImGui::CloseCurrentPopup(); }

				ImGui::EndPopup();
			}
			
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Logger", 0, editor->GetLogger().LogOpen())) { editor->GetLogger().SetOpen(!editor->GetLogger().LogOpen()); }
			if (ImGui::MenuItem("Inspector", 0, editor->GetPropertyEditor().GetOpen())) { editor->GetPropertyEditor().SetOpen(!editor->GetPropertyEditor().GetOpen()); }
			if (ImGui::MenuItem("FileBrowser", 0, editor->GetFileBrowser().GetOpen())) { editor->GetFileBrowser().SetOpen(!editor->GetFileBrowser().GetOpen()); }
			if (ImGui::MenuItem("Hierarchy", 0, editor->GetHierarchy().GetOpen())) { editor->GetHierarchy().SetOpen(!editor->GetHierarchy().GetOpen()); }
			if (ImGui::MenuItem("Project", 0, editor->GetFileBrowser().GetOpen())) { editor->GetFileBrowser().SetOpen(!editor->GetFileBrowser().GetOpen()); }
			
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("GameObject"))
		{
			if (ImGui::MenuItem("New GameObject", "CTRL + SHIFT + N")) { NewEmptyGameObject(); }

			if (ImGui::BeginMenu("Tag Manager"))
			{
				ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Reorderable;
				if (ImGui::BeginTable("Table", 2, flags))
				{
					ImGui::TableSetupColumn("Tag", ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableHeadersRow();

					// Displaying each tag name in the format of "Tag n		<TagName>"
					for (auto& tag : GameObject::GetTagMap())
					{
						ImGui::TableNextRow(); // Move to the next row.
						// This should either move to the next column, or circle back to the 1st.
						ImGui::TableNextColumn(); ImGui::Text(std::to_string(tag.first).c_str());
						ImGui::TableNextColumn(); ImGui::Text(tag.second.c_str());
					}

					ImGui::EndTable();
				}

				// + button to add a new tag
				if (ImGui::Button("+")) { ImGui::OpenPopup("Add Tag", ImGuiPopupFlags_MouseButtonLeft); }

				// Set the next window size to take 10% of the screen width and 10% of the screen height
				ImGui::SetNextWindowSize(ImVec2(GLSetup::current_width * 0.35f, GLSetup::current_height * 0.07f), ImGuiCond_Always);
				ImGui::SetNextWindowBgAlpha(0.9f); // Transparent background
				// Set the next window position to be centered on the screen. The pivot will decide how the window is drawn.
				ImGui::SetNextWindowPos(ImVec2(GLSetup::current_width * 0.5f, GLSetup::current_height * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f)); // Center of screen

				// If the popup window is open, display the popup window
				if (ImGui::BeginPopupModal("Add Tag", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
				{
					if (ImGui::IsKeyPressed(ImGuiKey_Escape)) { ImGui::CloseCurrentPopup(); }

					std::string nTagNameBuffer; ImGui::Text("New Tag Name: "); ImGui::SameLine();

					// Input field for the new tag name
					if (ImGui::InputTextWithHint("##AddTag2", "ENTER to save", &nTagNameBuffer, ImGuiInputTextFlags_EnterReturnsTrue))
					{
						GameObject::AddTag(nTagNameBuffer); ImGui::CloseCurrentPopup();
					}

					ImGui::SameLine(); if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }

					ImGui::EndPopup();
				}

				ImGui::SameLine();
				if (ImGui::Button("-")) { ImGui::OpenPopup("Remove Tag", ImGuiPopupFlags_MouseButtonLeft); }

				if (ImGui::BeginPopupModal("Remove Tag", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
				{
					if (ImGui::IsKeyPressed(ImGuiKey_Escape)) { ImGui::CloseCurrentPopup(); }

					std::string tagBuffer; ImGui::Text("Remove Tag Name: "); ImGui::SameLine();

					// Input field for the new tag name
					if (ImGui::InputTextWithHint("##RemoveTag2", "ENTER to remove", &tagBuffer, ImGuiInputTextFlags_EnterReturnsTrue))
					{
						GameObject::RemoveTag(tagBuffer); ImGui::CloseCurrentPopup();
					}

					ImGui::SameLine(); if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }

					ImGui::EndPopup();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Layer Manager"))
			{
				ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Reorderable;
				if (ImGui::BeginTable("Layers", 2, flags))
				{
					ImGui::TableSetupColumn("Layer", ImGuiTableColumnFlags_WidthStretch);

					// have the layer name column width be 10% of the screen width
					ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, (GLSetup::current_width * 0.1f));

					ImGui::TableHeadersRow();

					// Display is in format of "Layer number | Layer name"
					for (size_t i = 0; i < 32; i++)
					{
						ImGui::TableNextRow(); ImGui::TableNextColumn();
						ImGui::Text(std::to_string(i).c_str());
						ImGui::TableNextColumn();

						ImGui::PushID((int)i);
						if (GameObject::GetLayers()[i] != "Default")
							ImGui::InputText("##LayerName", &GameObject::GetLayers()[i], ImGuiInputTextFlags_EnterReturnsTrue);
						else // Default layer cannot be renamed or deleted
							ImGui::Text(GameObject::GetLayers()[i].c_str());
						ImGui::PopID();
					}

					ImGui::EndTable();
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Settings"))
		{
			ImGui::MenuItem("Volume"); 
			if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			{
				ImGui::OpenPopup("Volume Controls", ImGuiPopupFlags_MouseButtonLeft);
				// Set the next window size to take 10% of the screen width and 10% of the screen height
				ImGui::SetNextWindowBgAlpha(0.9f); // Transparent background
				// Set the next window position to be centered on the screen. The pivot will decide how the window is drawn.
				ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
			}

			// If the popup window is open, display the popup window
			if (ImGui::BeginPopupModal("Volume Controls", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize))
			{
				if (ImGui::IsKeyPressed(ImGuiKey_Escape)) { ImGui::CloseCurrentPopup(); }

				ImGui::Text("Master Volume: "); ImGui::SameLine();
				if (ImGui::SliderFloat("##MasterVolume", &audio->GetMasterVol(), 0.0f, 1.0f, "%.2f"))
				{
					audio->SetMasterVol(audio->GetMasterVol());
				}

				// ImGui::Text("Music Volume: "); ImGui::SameLine();
				//ImGui::SliderFloat("##MusicVolume", &Audio::musicVolume, 0.0f, 1.0f, "%.2f");

				// ImGui::Text("SFX Volume: "); ImGui::SameLine();
				//ImGui::SliderFloat("##SFXVolume", &Audio::GetSfxVol, 0.0f, 1.0f, "%.2f");

				ImGui::SameLine(); if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }

				ImGui::EndPopup();
			}
			ImGui::EndMenu();
		}

#pragma region Play / Pause / Stop Editor Buttons

		ImGui::SetCursorPosX((ImGui::GetWindowSize().x / 2.0f) - (ImGui::GetFontSize() * 3.0f));
		// Play button
		if (ImGui::Button(">", ImVec2(ImGui::GetFontSize() * 1.5f, ImGui::GetFontSize() * 1.5f)))
		{
			// If the game is not currently running, start the game
			if (editor->GetState() == EditorState::EditorStopped)
			{
				editor->SetState(EditorState::EditorPlaying);
				editor->SetOpen(false);
				if (editor->playFullScreen)
				{
					editor->GetLogger().SetOpen(false);
					editor->GetHierarchy().SetOpen(false);
					editor->GetPropertyEditor().SetOpen(false);
					editor->GetFileBrowser().SetOpen(false);

					editor->GetConsole().SetOpen(false);
					editor->GetMenuBar().SetOpen(false);
					editor->GetGameWindow().SetOpen(false);
				}

			}
			else
			{
				editor->SetState(EditorState::EditorStopped);
				//editor->SetOpen(true);
				editor->GetLogger().SetOpen(true);
				editor->GetHierarchy().SetOpen(true);
				editor->GetPropertyEditor().SetOpen(true);
				editor->GetFileBrowser().SetOpen(true);

				audio->StopMusic(); audio->StopSfx(); // Stop all audio
			} // If the game is currently running, pause the game	

			editor->GetPropertyEditor().DeselectEntity(); // stop gizmos from showing up
		}
		ImGui::SameLine();
		// Pause button
		if (ImGui::Button("||", ImVec2(ImGui::GetFontSize() * 1.5f, ImGui::GetFontSize() * 1.5f)))
		{
			// If the game is currently running, pause the game
			if (editor->GetState() == EditorState::EditorPlaying)
			{
				editor->SetState(EditorState::EditorPaused);
				editor->SetOpen(true);
				editor->GetLogger().SetOpen(true);
				editor->GetHierarchy().SetOpen(true);
				editor->GetPropertyEditor().SetOpen(true);
				editor->GetFileBrowser().SetOpen(true);

				audio->ToggleMusicPause(); // Pause music
			}

			editor->Log("Game Paused");
		}
		ImGui::SameLine();
		// Stop button
		if (ImGui::Button(">|", ImVec2(ImGui::GetFontSize() * 1.5f, ImGui::GetFontSize() * 1.5f)))
		{
			// If the game is currently running, stop the game
			if (editor->GetState() == EditorState::EditorPlaying || editor->GetState() == EditorState::EditorPaused)
			{
				editor->SetState(EditorState::EditorStopped);
				editor->SetOpen(true);
				editor->GetLogger().SetOpen(true);
				editor->GetHierarchy().SetOpen(true);
				editor->GetPropertyEditor().SetOpen(true);
				editor->GetFileBrowser().SetOpen(true);
			}

			editor->Log("Game Stopped");
		}

		ImGui::SetNextItemWidth(ImGui::CalcTextSize("Layers").x * 2.0f);
		ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Layers ").x * 2.0f));

#pragma endregion

#pragma region Layers Visibility Dropdown

		if (ImGui::BeginCombo("##LayerVisibilityFilter", "Layers", ImGuiComboFlags_PopupAlignLeft))
		{
			ImGui::Text("Layer Visibility"); ImGui::Separator();
			for (int i = 0; i < GameObject::GetLayers().size(); ++i)
			{
				if (GameObject::GetLayers()[i].empty()) continue;

				bool layerVisible = GameObject::GetLayerMask()[i];

				ImGui::PushID(i);
				if (ImGui::Checkbox(GameObject::GetLayers()[i].c_str(), &layerVisible)) {
					GameObject::GetLayerMask().set(i, layerVisible);
				}
				ImGui::PopID();
			}

			ImGui::EndCombo();
		}

#pragma endregion

		ImGui::EndMainMenuBar();
	}
}

/**************************************************************************/
/*!
	\brief
		Loads in a new texture file into the asset database.
	\param textureDirPath
		The path to the texture file.
*/
/**************************************************************************/
void MenuBar::LoadNewTexture(std::string_view textureDirPath)
{
	// get the texture list of the asset manager
	Assets& assetManager = MasterObjectList->MasterAssets;
	std::string name(textureDirPath.substr(textureDirPath.find_last_of("\\") + 1));
	std::fstream textureTextFile(projectDir + "/Data/SpriteSheets.txt");
	if (!textureTextFile) { editor->Log("Cannot find SpriteSheets.txt", Logger::LogType::Error); textureTextFile.close(); return; }

	try {
		if (assetManager.GetTexture(name).texid == 0)
		{
			const std::string localDirPath = projectDir + "\\Images\\" + name;

			// if this texture file is outside the project directory
			if (textureDirPath.find("\\CSD2400\\") == std::string::npos)
			{
				// Copy & paste the texture to the project's Images folder
				if (!std::filesystem::copy_file(textureDirPath, localDirPath, std::filesystem::copy_options::overwrite_existing))
				{
					editor->Log("Unable to copy texture to Images folder.\n", Logger::LogType::Error); textureTextFile.close(); return;
				}
			}

			std::string localDirName = ".";
			if (textureDirPath.find("\\CSD2400\\") == std::string::npos) { localDirName += "/Images/" + name; }
			else // if the file is inside the project directory, use the portion of the path after "Audio"
			{
				if (textureDirPath.find("\\Images\\") != std::string::npos)
				{
					localDirName += textureDirPath.substr(textureDirPath.find("\\Images\\"));
					// replace all backslashes with forward slashes
					std::replace(localDirName.begin(), localDirName.end(), '\\', '/');
				}
				else { textureTextFile.close(); throw std::exception("Improperly placed image / texture asset."); }
			}

			// Records the new texture's name and its location
			// Format: "<id/name>\t\t<filepathWithinProject>" (excl. quotes)
			// The id/name is the filename w/o extension

			textureTextFile.seekg(0, std::ios::end);
			textureTextFile.write("\n", 1);
			name = name.substr(0, name.find_last_of("."));
			textureTextFile.write(name.c_str(), name.size());
			textureTextFile.write("\t\t", 2);
			textureTextFile.write(localDirName.c_str(), localDirName.length());
			textureTextFile.close();

			// Load the incoming texture into the asset base
			assetManager.AddTexture(name, localDirPath.data());
			editor->Log("Loaded " + name);

			// reset current path to the project's root directory
			std::filesystem::current_path() = std::filesystem::path(projectDir);
		}
	}
	catch (std::exception& e) // e.g. file to be copied being used by another process
	{
		std::cout << e.what() << std::endl; textureTextFile.close();
	}
}

/**************************************************************************/
/*!
	\brief
		Loads in a new audio file into the asset database.
	\param audioDirPath
		The path to the audio file.
*/
/**************************************************************************/
void MenuBar::LoadNewAudio(std::string_view audioFilename)
{
	//Assets& assetManager = this->GetLevel()->GetAssets();
	Assets& assetManager = MasterObjectList->MasterAssets;
	std::string name(audioFilename.substr(audioFilename.find_last_of("\\") + 1));

	int index = 1;
	// Find the AudioData.txt file and update accordingly
	std::fstream audioTextFile(projectDir + "\\Data\\AudioData.txt"); std::cout << projectDir + "\\Data\\AudioData.txt" << std::endl;
	//if (!audioTextFile.is_open()) { editor->Log("-------------AudioData.txt not found", Logger::LogType::Error); audioTextFile.close(); return; }
	try {
		if (!assetManager.GetAudio(name.c_str(), &index)) // error
		{
			// Copy & paste the audio file to the project's Audio folder
			const std::string localFileDir = projectDir + "/Audio/" + name;

			// If input file is not within the project directory, ...
			if (audioFilename.find("\\CSD2400\\") == std::string::npos)
			{
				// ... then copy it into the project's Audio folder.
				if (!std::filesystem::copy_file(audioFilename, localFileDir, std::filesystem::copy_options::overwrite_existing))
				{
					editor->Log("Unable to copy audio to Audio folder.\n", Logger::LogType::Error); audioTextFile.close(); return;
				}
			}

			std::string localDirName = ".";
			if (audioFilename.find("\\CSD2400\\") == std::string::npos)
			{
				localDirName += "/Audio/" + name;
			}
			else // if the audio file is inside the project directory, use the portion of the path after "Audio"
			{
				if (audioFilename.find("\\Audio\\") != std::string::npos)
				{
					//localDirName += audioFilename.substr(audioFilename.find("Audio"));
					localDirName += audioFilename.substr(audioFilename.find("\\Audio\\"));
					// replace all backslashes with forward slashes
					std::replace(localDirName.begin(), localDirName.end(), '\\', '/');
				}
				else { throw std::exception("Improperly placed audio asset."); }
			}

			// Construction of AudioData.txt serialization data
			// Format: "<localDirPath>" <loop/no loop> (incl. quotes)

			audioTextFile.seekg(0, std::ios::end);
			audioTextFile.write("\n", 1);
			audioTextFile.write("\"", 1);
			audioTextFile.write(localDirName.data(), localDirName.size());
			audioTextFile.write("\"", 1); audioTextFile.write(" 0", 2); // 0 - no loop , 1 - loop
			audioTextFile.close();

			// Loading incoming audio file into audio asset base
			audio->LoadAudio(audioFilename.data(), 0); // error?
			editor->Log("Loaded " + *localDirName.data());

			// reset current path to the project's root directory
			std::filesystem::current_path() = std::filesystem::path(projectDir);
		}
	}
	catch (std::exception& e) { std::cout << e.what() << std::endl; audioTextFile.close(); }
}