/*!*************************************************************************
\file   FileBrowser.cpp
\author Anderson Phua
\email: a.phua@digipen.edu
\date   26-11-22
\brief	Functionalities for the FileBrowser system. This system is used to
		allow the user to browse through and interact with the files in the
		project directory.

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*****************************************************************************/
#include "Editor/UI/FileBrowser/FileBrowser.h"
#include "Editor/UI/Editor.h"
#include "Editor/IMGUIHeaders.h"

#include "CoreSys/Core.h"
#include "Audio/Audio.h"

#include <Windows.h>

/**************************************************************************/
/*! \brief Constructor. Current path is set to the solution directory. */
/**************************************************************************/
FileBrowser::FileBrowser(bool nOpen, float nWidth, float nHeight)
	: open(nOpen), focused(true), size{ nWidth, nHeight }, currentPath(std::filesystem::current_path().parent_path().string()),
	currentDirectory(currentPath), projectDir(currentPath), pathHistory() {}

/**************************************************************************/
/*! \brief Destructor.
/**************************************************************************/
FileBrowser::~FileBrowser() {}

/**************************************************************************/
/*! \brief Initializes the FileBrowser system.
/**************************************************************************/
void FileBrowser::Init() { pathHistory.emplace_back(currentPath.c_str()); }

/**************************************************************************/
/*! \brief Shuts down the FileBrowser system.
/**************************************************************************/
void FileBrowser::Shutdown() { pathHistory.clear(); pathListIndex = 0; }

/**************************************************************************/
/*!
* \brief
* 	Updates the FileBrowser system.
* \param dt
* 	The delta time.
*/
/**************************************************************************/
void FileBrowser::Update(float dt)
{
	(void)dt;
	if (open)
	{
		DrawFileBrowser();

		if (focused)
		{
			if (ImGui::GetIO().KeyAlt)
			{
				if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow)) // ALT + Left arrow key to go back to the parent directory.
				{
					// the user is not be allowed to go leave the "/CSD2400" directory.
					if (pathListIndex) { currentPath = pathHistory[--pathListIndex]; }
				}
				if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) // ALT + Right arrow key to go forward to the child directory.
				{
					if (pathListIndex < pathHistory.size() - 1) { currentPath = pathHistory[++pathListIndex]; }
				}
			}

			// Deselect the GO / asset if the user clicks on the background of the file browser...
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows))
				{
					editor->GetPropertyEditor().DeselectEntity();
					editor->GetPropertyEditor().GetTextBuffer().str(""); editor->GetPropertyEditor().GetTextBuffer().clear();
				}
			}
			else if (ImGui::IsKeyPressed(ImGuiKey_Escape)) // ... or presses ESC.
			{
				editor->GetPropertyEditor().DeselectEntity();
				editor->GetPropertyEditor().GetTextBuffer().str(""); editor->GetPropertyEditor().GetTextBuffer().clear();
			}
		}
	}
}

/**************************************************************************/
/*!
* \brief
*	Retrieves open status of the FileBrowser system.
* \return
* 	True if the FileBrowser system is open, false otherwise.
*/
/**************************************************************************/
bool FileBrowser::GetOpen() const { return open; }

/**************************************************************************/
/*!
* \brief
* 	Sets the open status of the FileBrowser system.
* \param nOpen
* 	The new open status.
*/
/**************************************************************************/
void FileBrowser::SetOpen(const bool nOpen) { open = nOpen; }

/**************************************************************************/
/*! \brief Produces and / or returns the only instance of the FileBrowser system.
/**************************************************************************/
FileBrowser* FileBrowser::GetInstance() { assert(!instanced); instanced = true; return new FileBrowser(); }

/**************************************************************************/
/*! \brief
*	Draws all directories only under the input path.
* \param root
* 	The root path from which to show all its child directories.
*/
/**************************************************************************/
void FileBrowser::DrawDirectoryTreeView(const std::filesystem::directory_entry& root)
{
	if (root.is_directory())
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnArrow;
		// The start path shall be left open by default.
		if (root.path() == std::filesystem::current_path().parent_path().string()) { flags |= ImGuiTreeNodeFlags_DefaultOpen; }

		// The user can change directories in the second column. When done so, the first column will be updated accordingly.
		if (currentPath.find(root.path().filename().string()) != std::string::npos) { flags |= ImGuiTreeNodeFlags_DefaultOpen; }

		// If the user has navigated to another directory through the second column, the directory tree view will be updated accordingly.
		if (root.path() == currentPath) { flags |= ImGuiTreeNodeFlags_Selected; }

		// if this directory has no child directories, then it is a leaf node.
		if (!ChildrenCount(root.path().string().c_str()).second) { flags |= ImGuiTreeNodeFlags_Leaf; }

		if (ImGui::TreeNodeEx(root.path().filename().string().c_str(), flags))
		{
			// if the user clicks on the directory, then the current path is set to the clicked directory.
			if (ImGui::IsItemClicked())
			{
				SetCurrentPath(root.path().string().c_str());
				// Adjust the path history accordingly.
				if (pathListIndex < pathHistory.size() - 1) { pathHistory.erase(pathHistory.begin() + pathListIndex + 1, pathHistory.end()); }
				pathHistory.emplace_back(currentPath.c_str()); pathListIndex = (unsigned short)(pathHistory.size() - 1);
			}

			for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(root))
			{
				DrawDirectoryTreeView(entry); // Recursively draw the directory tree, inside out.
			}
			ImGui::TreePop();
		}
	}
}

/**************************************************************************/
/*! \brief Draws the FileBrowser system, and handles its functionalities.
/**************************************************************************/
void FileBrowser::DrawFileBrowser()
{
	ImGui::SetWindowPos(ImGui::GetWindowPos());
	ImGui::Begin("Project", &open);

	// if the user is focused on the FileBrowser window, then the Editor window will not be able to receive input.
	if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)) { focused = true; } else { focused = false; }

	// Back button - goes back to the previous page.
	if (ImGui::ArrowButton("PageBack", ImGuiDir_Left)) { if (pathListIndex) { currentPath = pathHistory[--pathListIndex]; } } ImGui::SameLine();
	// Forward button to go to the next page in its history.
	if (ImGui::ArrowButton("PageForward", ImGuiDir_Right)) { if (pathListIndex < pathHistory.size() - 1) { currentPath = pathHistory[++pathListIndex]; } }

	ImGui::SameLine(); ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
	ImGui::SameLine(); ImGui::TextWrapped(currentPath.c_str()); ImGui::Separator();

	// The file browser window will be split into two columns.
	ImGui::Columns(2, "FileBrowser", false);

	// The left column will be used to display the directory tree view.
	ImGui::BeginChild("DirectoryTreeView", ImVec2(0, 0), true);
	// DrawDirectoryTreeView(std::filesystem::directory_entry(std::filesystem::current_path().parent_path().string()));
	DrawDirectoryTreeView(std::filesystem::directory_entry(projectDir));
	ImGui::EndChild();

	ImGui::NextColumn();

	// The right column will be used to display the files in the current directory.
	ImGui::BeginChild("Files", ImVec2(0, 0), true);

	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(currentPath))
	{
		ImGui::PushID(entry.path().filename().string().c_str());

		// If this directory / file is being renamed by the user, then the user will be able to rename it.
		if (renameFlag && entry.path().filename().string() == fileToRename.data()) { RenameFile(entry); ImGui::PopID(); continue; }

		if (entry.is_directory())
		{
			if (ImGui::Selectable(entry.path().filename().string().c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
			{
				currentDirectory = entry.path().filename().string();

				if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					SetCurrentPath(entry.path().string());
					// if this is not the last directory in the path history, the path history is truncated.
					if (pathListIndex < pathHistory.size() - 1) { pathHistory.erase(pathHistory.begin() + pathListIndex + 1, pathHistory.end()); }
					pathHistory.emplace_back(currentPath); pathListIndex = static_cast<unsigned short>(pathHistory.size() - 1);
				}
			}
		}
		else // if entry is an actual file
		{
			if (ImGui::Selectable(entry.path().filename().string().c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
			{
				if (entry.is_regular_file()) // e.g. .cpp/.h, .vert,even .scn files.
				{
					// if file is an audio file, display it in the audio player.
					if (entry.path().extension() == ".wav" || entry.path().extension() == ".mp3") {
						editor->GetPropertyEditor().DisplayAudioFile(entry.path().string());
					}
					// if file is a texture file, display it in the texture viewer.
					else if (entry.path().extension() == ".png") {editor->GetPropertyEditor().DisplayTextureFile(entry.path().string());}
					else { editor->GetPropertyEditor().DisplayTextFileContents(entry.path().string()); }
				}
				else
				{
					// if the user double clicks on a symlink, then the symlink will be opened.
					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						ShellExecuteA(NULL, "open", entry.path().string().c_str(), NULL, NULL, SW_SHOWNORMAL);
					}
				}

				if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) // Open file by double click
				{
					if (entry.path().filename().extension() == ".scn") {editor->LoadLevel(entry.path().string());}
					else
					{
#ifdef _WIN32
						// ShellExecute will open the file in the default application associated with the file type.
						ShellExecuteA(NULL, "open", entry.path().string().c_str(), NULL, NULL, SW_SHOWNORMAL);
#else
						// if not on Windows, then the file is opened in the default text editor.
						// xdg-open is a Linux command that opens a file in the default application associated with the file type.
						std::string command = "xdg-open " + entry.path().string(); system(command.c_str());
#endif
					}
				}
			}
		}

		UseContextMenu(entry);

		// Drag and drop... into other directories, but not out
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			ImGui::SetDragDropPayload("FILE_PATH", entry.path().string().c_str(), entry.path().string().size() + 1);
			ImGui::Text(entry.path().filename().string().c_str());
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_PATH"))
			{
				std::string payloadPath = (char*)payload->Data;
				std::filesystem::rename(payloadPath, entry.path().string() + "\\" + std::filesystem::path(payloadPath).filename().string());
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::PopID();
	}
	ImGui::EndChild();

	ImGui::End();
}

/**************************************************************************/
/*!
* \brief
*	If the path is invalid, the current path will not be changed.
	Otherwise, the current path will be set to the path specified.
* \param nPath
* The path to set the current path to.
*/
/**************************************************************************/
void FileBrowser::SetCurrentPath(const std::string_view nPath)
{
	if (std::filesystem::exists(nPath))
	{
		if (std::filesystem::is_directory(nPath)) { currentPath = nPath; }
		else { editor->Log("The path is not a directory.", Logger::LogType::Error); }
	}
	else { editor->Log("The path is not valid.", Logger::LogType::Error); }
}

/**************************************************************************/
/*!
* \brief
*	Returns total number of child files & directories under the input path.
* \param nPath
*	The path to count the number of child files & directories under.
* \return
*	A pair of 2 int's. First number's the file count, second's the number of
*	directories / folders.
*/
/**************************************************************************/
std::pair<int, int> FileBrowser::ChildrenCount(const std::string_view nPath) const
{
	int fileCount, directoryCount; fileCount = directoryCount = 0;
	for (const std::filesystem::directory_entry& child : std::filesystem::directory_iterator(nPath))
	{
		(void)child; if (child.is_directory()) ++directoryCount; else ++fileCount;
	}
	return std::pair<int, int>(fileCount, directoryCount);
}

/**************************************************************************/
/*!
* \brief
*	Helper function to delete the file / directory specified by the path.
*	Depending on the contents of the path, the file / directory will be sent
*	to the Windows Recycle bin or permanently deleted.
* \param entry
*	The entry to be deleted.
*/
/**************************************************************************/
void DeleteEntry(const std::filesystem::directory_entry& entry)
{
#ifdef _WIN32 // if this machine is running 32 or 64-bit Windows
	// Reference: https://learn.microsoft.com/en-sg/windows/win32/api/shellapi/ns-shellapi-shfileopstructa?redirectedfrom=MSDN

	// Zero out the struct, lest error 87 (ERROR_INVALID_PARAMETER) is shown.
	SHFILEOPSTRUCT fileOp = { 0 }; fileOp.wFunc = FO_DELETE;

	// Set up the input entry file path.
	WCHAR from[MAX_PATH] = { 0 }; wcscpy_s(from, MAX_PATH, entry.path().wstring().c_str());
	from[entry.path().wstring().size() + 1] = 0; // Important - double null terminate the entry's name!

	// Set the flags to allow undo and silent operation, before finally performing it.
	fileOp.pFrom = from; fileOp.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_SILENT; int r = SHFileOperation(&fileOp);

	// Error Handling
	if (fileOp.fAnyOperationsAborted) { editor->Log("Deletion abortion error " + std::to_string(r), Logger::LogType::Error); }
#else
	// Alternatively, C++ 17 has std::filesystem::remove_all() which can be used to delete a directory and all its contents.
	// Keep in mind that it deletes the file(s) and / or directories permanently - they don't go to the recycle bin!
	if (entry.is_directory()) { std::filesystem::remove_all(entry.path()); } std::filesystem::remove(entry.path());
#endif
}

/**************************************************************************/
/*!
* \brief
*	Draws, and operates the context menu operations for the file / directory
*	specified by the path, when the user right-clicks on it.
* \param path
* The path to the file / directory to draw the context menu for.
*/
/**************************************************************************/
void FileBrowser::UseContextMenu(const std::filesystem::directory_entry& entry)
{
	if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) { ImGui::OpenPopup("##ContextMenu"); }
	if (ImGui::BeginPopupContextItem("##ContextMenu"))
	{
		if (ImGui::MenuItem("Open")) { ShellExecuteA(NULL, "open", entry.path().string().c_str(), NULL, NULL, SW_SHOWNORMAL); }

#ifdef _WIN32
		if (ImGui::MenuItem("Open in Explorer")) {
			ShellExecuteA(NULL, "open",
				entry.is_directory() ? entry.path().string().c_str() : entry.path().parent_path().string().c_str(), NULL, NULL, SW_SHOWNORMAL);
		}
#endif
		if (ImGui::MenuItem("Delete")) { DeleteEntry(entry); }
		if (ImGui::MenuItem("New Folder"))
		{
			std::string newFolderName = "New Folder";

			// If the file of the same name already exists, append a number to the end of it.
			for (int folderCount = 0; std::filesystem::exists(currentPath + "/" + newFolderName);)
			{
				newFolderName = "New Folder (" + std::to_string(++folderCount) + ")";
			}

			std::filesystem::create_directory(currentPath + "/" + newFolderName);
		}
		if (ImGui::MenuItem("Rename")) { renameFlag = true; fileToRename = entry.path().filename().string().c_str(); }
		ImGui::EndPopup();
	}
}

/**************************************************************************/
/*!
* \brief
*	Allows the user to rename the file / directory specified by the path
*	from within the FileBrowser editor subsystem.
* \param entry
*	The file / directory to rename.
*/
/**************************************************************************/
void FileBrowser::RenameFile(const std::filesystem::directory_entry& entry)
{
	char buffer[256]; strcpy_s(buffer, fileToRename.data());
	ImGui::PushItemWidth(-1);
	ImGui::InputText("##Rename", buffer, sizeof(buffer));
	ImGui::PopItemWidth();

	if (ImGui::IsKeyPressed(ImGuiKey_Enter))
	{
		std::filesystem::rename(entry.path(), entry.path().parent_path() / buffer);
		renameFlag = false; fileToRename = "";
	}

	if (ImGui::IsKeyPressed(ImGuiKey_Escape)) { renameFlag = false; fileToRename = ""; }
}