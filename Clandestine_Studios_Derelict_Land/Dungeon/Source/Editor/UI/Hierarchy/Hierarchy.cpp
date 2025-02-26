/*!*************************************************************************
\file	Hierarchy.cpp
\author Anderson Phua,
\email: a.phua@digipen.edu
\date	01-11-22
\brief	Functionalities for the Hierarchy system, including creation, reordering
		, adding and removal of GameObject entities.

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*****************************************************************************/
#include "Editor/UI/Editor.h"
#include "Editor/IMGUIHeaders.h"
#include "Factory/Factory.h"

namespace {
	char renameBuffer[256]{ 0 };
	std::stringstream goss;	std::string searchBuffer, compHeaderText;
	ComponentType cTypeToSearch = ComponentType::Transform;
	SearchFilter searchFilter = SearchFilter::Name;
}

/**************************************************************************/
/*!
* \brief
*	Finds the index of the GameObject inside the hierarchy
* \param id
*	Unique ID of the GameObject
* \return
*	Index of the GameObject in the hierarchy
*/
/**************************************************************************/
int Hierarchy::GetHierarchyListIndex(const size_t id) const
{
	for (size_t idx = 0; idx < hierarchyList.size(); ++idx) { if (hierarchyIds[idx].first == id) { return (int)idx; } }
	return -1;
}

/**************************************************************************/
/*!
* \brief
*	Sets a GameObject up for renaming.
* \param go
*	Pointer to the GameObject
*/
/**************************************************************************/
void Hierarchy::SetRenameTarget(GameObject* go) { renameTarget = go; strcpy_s(renameBuffer, 64, renameTarget->GetName().c_str()); }

/**************************************************************************/
/*!
* \brief
*  Returns whether a GameObject is currently being renamed.
* \return
* True if a GameObject is being renamed, false otherwise.
*/
/**************************************************************************/
bool Hierarchy::IsRenaming() const { return renaming; }

/**************************************************************************/
/*! \brief Constructor.
/**************************************************************************/
Hierarchy::Hierarchy(bool nOpen, Vec2 nPos) : open(nOpen), pos(nPos) { }

/**************************************************************************/
/*! \brief Destructor.
/**************************************************************************/
Hierarchy::~Hierarchy() { }

/**************************************************************************/
/*! \brief Initialize the Hierarchy system.
/**************************************************************************/
void Hierarchy::Init() { searchBuffer = ""; RefreshHierarchy(); }

/**************************************************************************/
/*!
* \brief
*	Update the Hierarchy system.
* \param dt
* 	Delta time.
*/
/**************************************************************************/
void Hierarchy::Update(float dt) { (void)dt; if (open) { DrawHierarchy(); } }

/**************************************************************************/
/*! \brief Shutdown the Hierarchy system. Clears the hierarchy list.
/**************************************************************************/
void Hierarchy::Shutdown() { hierarchyList.clear(); }

/**************************************************************************/
/*! \brief
*	Returns the open state of the Hierarchy window.
* \return
* 	True if the Hierarchy window is open, false otherwise.
/**************************************************************************/
bool Hierarchy::GetOpen() const { return open; }

/**************************************************************************/
/*! \brief Sets the open state of the Hierarchy window.
/**************************************************************************/
void Hierarchy::SetOpen(const bool nOpen) { open = nOpen; }

/**************************************************************************/
/*!
* \brief
*	Updates the gameObjects list in the current game level, when one is added or
*	deleted.
*/
/**************************************************************************/
void Hierarchy::RefreshHierarchy()
{
	// Clear the hierarchy list if it's not empty and has previous level's game objects' references.
	// Do if entering a new level. Get the current level's game objects, and add them to the hierarchy
	hierarchyList = this->GetLevel()->GetGameObjectArray();

	if (hierarchyIds.size()) { hierarchyIds.clear(); } // In event of creating or destroying game objects, refresh the list

	for (int i = 0; i < hierarchyList.size(); ++i) { hierarchyIds.emplace_back(std::make_pair(hierarchyList[i]->GetObjectId(), hierarchyList[i]->GetActive())); }
}

/**************************************************************************/
/*!
* \brief
* 	Displays the game objects' list in current game scene, and handles
*	related functionalities.
*/
/**************************************************************************/
void Hierarchy::DrawHierarchy()
{
	// Set the window position if it's not already set in .ini file (if any)
	ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize({ 300.0f, 300.0f }, ImGuiCond_FirstUseEver);
	ImGui::Begin("Hierarchy", &open, ImGuiWindowFlags_NoCollapse);

	// Filtering options section
	DisplayGameObjectFilter();
	// Search for game objects that meet the filtering criteria, if any
	FilterHierarchy(searchFilter, searchBuffer, cTypeToSearch);

	// Iterate through the (filtered) hierarchy list and print out the object names
	DisplayHierarchy();

	// Deselect the GO / asset if the user clicks within the hierarchy window...
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows))
		{
			if (editor->GetPropertyEditor().GetSelectedGameObject() != nullptr)
			{
				editor->GetPropertyEditor().DeselectEntity();
				editor->GetPropertyEditor().GetTextBuffer().str(""); editor->GetPropertyEditor().GetTextBuffer().clear();

				// update the transform history found in the game window
				// check if the transform history index is at the end of history. if not, then clear the rest of the history
				/*if (!editor->GetGameWindow().transformHistory.empty()
					&& editor->GetGameWindow().transformHistoryIndex != editor->GetGameWindow().transformHistory.size() - 1)
				{
					editor->GetGameWindow().transformHistory.erase(editor->GetGameWindow().transformHistory.begin()
						+ editor->GetGameWindow().transformHistoryIndex + 1, editor->GetGameWindow().transformHistory.end());
				}
				editor->GetGameWindow().transformHistory.push_back({});
				editor->GetGameWindow().transformHistoryIndex = editor->GetGameWindow().transformHistory.size() - 1;*/
				
				editor->GetGameWindow().UpdateTransformHistory(nullptr, nullptr);
				// std::cout << "TI: " << editor->GetGameWindow().transformHistoryIndex << std::endl;
			}

		}
	}
	else if (ImGui::IsKeyPressed(ImGuiKey_Escape)) // ... or presses ESC.
	{
		editor->GetPropertyEditor().DeselectEntity();
		editor->GetPropertyEditor().GetTextBuffer().str(""); 
		editor->GetPropertyEditor().GetTextBuffer().clear();

		// update game window's transform history for deselection
		if (editor->GetPropertyEditor().GetSelectedGameObject())
			editor->GetGameWindow().UpdateTransformHistory(nullptr, nullptr);
	}

	ImGui::End();
}

/**************************************************************************/
/*!
* \brief
*	This section draws and handles filtering mechanisms for the hierarchy list.
*/
/**************************************************************************/
void Hierarchy::DisplayGameObjectFilter()
{
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.4f);

	// Drop down menu for search filters
	if (ImGui::BeginCombo("##SearchFilter", "Filter"))
	{
		if (ImGui::Selectable("Name", searchFilter == SearchFilter::Name)) { searchFilter = SearchFilter::Name; }
		if (ImGui::Selectable("Component", searchFilter == SearchFilter::Component)) { searchFilter = SearchFilter::Component; }
		if (ImGui::Selectable("Tag", searchFilter == SearchFilter::Tag)) { searchFilter = SearchFilter::Tag; }

		ImGui::EndCombo();
	}

	ImGui::PopItemWidth();

	ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.5f);

	// Display search bar
	if (searchFilter == SearchFilter::Name || searchFilter == SearchFilter::Tag) { ImGui::InputText("##SearchName", &searchBuffer); }
	else if (searchFilter == SearchFilter::Component)
	{
		// Dropdown menu for component search
		if (cTypeToSearch == ComponentType::Transform) { compHeaderText = "Transform"; }
		else if (cTypeToSearch == ComponentType::Collision) { compHeaderText = "Collision"; }
		else if (cTypeToSearch == ComponentType::RigidBody) { compHeaderText = "Rigidbody"; }
		else if (cTypeToSearch == ComponentType::GameUI) { compHeaderText = "GameUI"; }

		if (ImGui::BeginCombo("##SearchComponent", compHeaderText.c_str()))
		{
			if (ImGui::Selectable("Transform", cTypeToSearch == ComponentType::Transform)) { cTypeToSearch = ComponentType::Transform; }
			if (ImGui::Selectable("Collision", cTypeToSearch == ComponentType::Collision)) { cTypeToSearch = ComponentType::Collision; }
			if (ImGui::Selectable("Rigidbody", cTypeToSearch == ComponentType::RigidBody)) { cTypeToSearch = ComponentType::RigidBody; }
			if (ImGui::Selectable("GameUI", cTypeToSearch == ComponentType::GameUI)) { cTypeToSearch = ComponentType::GameUI; }
			ImGui::EndCombo();
		}
	}

	ImGui::PopItemWidth();
}

/**************************************************************************/
/*!
* \brief
*   Display the GO's name in the hierarchy list.
* \param gameObject
*  The game object to display.
* \param underParent
* True if the game object is under a parent game object, false otherwise.
/**************************************************************************/
void Hierarchy::DisplayGameObject(GameObject* gameObject, const bool underParent)
{
	if (gameObject->parent && !underParent) { return; }
	
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth
		| (gameObject->HasChildren() ? 0 : ImGuiTreeNodeFlags_Leaf
		| (editor->GetPropertyEditor().GetSelectedGameObject() != nullptr
		&& editor->GetPropertyEditor().GetSelectedGameObject() == gameObject ? ImGuiTreeNodeFlags_Selected : 0));


	if (ImGui::TreeNodeEx(const_cast<const char*>(gameObject->GetName().c_str()), flags))
	{
		if (ImGui::IsItemClicked()) // if click on game object, select it
		{
			// if this game object is not selected, then select it, and display its properties in the property editor / inspector.
			if (editor->GetPropertyEditor().GetSelectedGameObject() != gameObject)
			{
				SelectGameObject(*gameObject, gameObject->GetObjectId() - 1); // - 1 for index
			}
			else { SetRenameTarget(gameObject); renaming = true; }
			editor->GetGameWindow().UpdateTransformHistory(gameObject, gameObject->GetComponent<Transform>());
		}

		// if this gameObject has any children, then display them
		if (gameObject != nullptr && gameObject->GetChildren().size())
		{
			for (auto& child : gameObject->GetChildren()) { DisplayGameObject(child, true); }
		}

		ImGui::TreePop();
	}
}

/**************************************************************************/
/*!
* \brief
*	Display the game objects' names, and provide handles for functionalities
*	such as reordering, adding and removing game objects.
*/
/**************************************************************************/
void Hierarchy::DisplayHierarchy()
{
	// make tree of game objects, with header of level name, open by default
	if (ImGui::TreeNodeEx(this->GetLevel()->GetId().data(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick
		| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (int i = 0; i < hierarchyIds.size(); ++i)
		{
			size_t id = hierarchyIds[i].first; std::string name = hierarchyList[i]->GetName() + " [";
			name += std::to_string(hierarchyList[i]->GetObjectId()); name += ']';
			ImGui::PushID((int)id); // Ensures no multiple 'Delete', 'Duplicate' options are shown upon RMB click.

			if (hierarchyIds[i].second) // if showing (e.g. passed through filter, or no filtering)
			{
				if (renameTarget != hierarchyList[i])
				{
					// white for active, grey for inactive
					if (!hierarchyList[i]->GetActive()) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); }
					else { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); }
					
					DisplayGameObject(hierarchyList[i], false);

					// Click, drag to move game objects up and down in hierarchy
					if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
					{
						int n_next = i + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
						if (n_next >= 0 && n_next < hierarchyIds.size())
						{
							// The first gameObject in the hierarchy list is the root gameObject, and cannot be moved
							if (i != 0 && n_next != 0)
							{
								// Swap the game objects' positions in the hierarchy list
								std::swap(hierarchyIds[i], hierarchyIds[n_next]);
								std::swap(hierarchyList[i], hierarchyList[n_next]);
								// Swap the game objects' positions in the level's game object array
								std::swap(this->GetLevel()->GetGameObjectArray()[i], this->GetLevel()->GetGameObjectArray()[n_next]);
								// swap ids
								/*
								std::swap(hierarchyIds[i].first, hierarchyIds[n_next].first);
								int temp = this->GetLevel()->GetGameObjectArray()[i]->GetObjectId();
								this->GetLevel()->GetGameObjectArray()[i]->SetObjectId(this->GetLevel()->GetGameObjectArray()[n_next]->GetObjectId());
								this->GetLevel()->GetGameObjectArray()[n_next]->SetObjectId(temp);
								*/
							}
							ImGui::ResetMouseDragDelta();
						}
					}

					ImGui::PopStyleColor();
				}
				else // if game object is being renamed inside hierarchy
				{
					ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.5f);
					// This sets the cursor into the textbox that follows.
					ImGui::PushAllowKeyboardFocus(true); ImGui::SetKeyboardFocusHere();

					if (ImGui::InputText("##Rename", renameBuffer, sizeof(renameBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
					{
						// if the name is not empty, then rename the game object.
						if (strlen(renameBuffer) > 0) { hierarchyList[i]->SetName(renameBuffer); }
						renameBuffer[0] = '\0'; renameTarget = nullptr;
					}
					if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_Escape)) || (!ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)))
					{
						renameTarget = nullptr; renameBuffer[0] = '\0'; renaming = false;
					}
					ImGui::PopAllowKeyboardFocus();
					ImGui::PopItemWidth();
				}
			}

			// First gameobject is the Camera - deleting it doesn't crash, but moves the camera somewhere
			if (this->GetLevel()->GetGOASize() > 1 && i != 0)
			{
				// if RMB is clicked, display the popup box of hidden(##) name DeletePopup
				if (ImGui::BeginPopupContextItem("##DeletePopup"))
				{
					if (ImGui::MenuItem("Delete"))
					{
						// Remove the game object from the lists and from the GOA itself...
						this->GetLevel()->RemoveGOsPtrsFromAllLayers(this->GetLevel()->GetGameObjectArray()[i]);
						factory->Destroy(this->GetLevel()->GetGameObjectArray()[i]);
						this->GetLevel()->GetGameObjectArray().erase(this->GetLevel()->GetGameObjectArray().begin() + i);
						editor->GetPropertyEditor().DeselectEntity(); RefreshHierarchy();

						// and also adjust the ids of the game objects that come after the deleted one.
						// Thus the order of ids should still be in the same order - though without the highest ID.
						for (int j = i; j < hierarchyIds.size(); ++j) { hierarchyIds[j].first = j + 1; }
					}
					else if (ImGui::MenuItem("Duplicate"))
					{
						// Duplicate the game object
						int HLID = GetHierarchyListIndex(id);
						if (HLID > 0)
						{
							this->GetLevel()->AddGameObject(factory->Clone(hierarchyList[HLID]));

							// set the duplicate's position to the wherever the camera is (assumed to be center of screen)
							this->GetLevel()->GetGameObjectArray().back()->GetComponent<Transform>()->Position =
								GameObject::FindWithTag("Camera", this->GetLevel()->GetGameObjectArray())->GetComponent<Transform>()->Position;

							hierarchyList.emplace_back(this->GetLevel()->GetGameObjectArray().back());
							hierarchyIds.emplace_back(std::make_pair(hierarchyList.back()->GetObjectId(), hierarchyList.back()->GetActive()));
							SetRenameTarget(hierarchyList.back());
						}
						else { editor->Log("Could not duplicate [Invalid ID]", Logger::LogType::Error); }
					}
					else if (ImGui::MenuItem("Rename")) { SetRenameTarget(hierarchyList[i]); }

					ImGui::EndPopup();
				}
			}

			ImGui::PopID();
		}

		ImGui::TreePop();
	}
}

/**************************************************************************/
/*!
* \brief
*	Search for game object(s) by the name of the filter type object.
*	Game object(s) that have the filter type object's name will be displayed.
*
*	E.g. if the filter type is "Name", and the filter object's name is "Player",
*	then all game objects that have "Player" in their name will be displayed.
*	An empty filter object name will display all game objects.
* \param filter
*	The search filter to use.
* \param searchStr
*	The search string to use.
* \param cType
*	The component type enum to find its corresponding component object with.
*/
/**************************************************************************/
void Hierarchy::FilterHierarchy(const SearchFilter filter, std::string searchStr, const ComponentType compType)
{
	switch (filter)
	{
	case SearchFilter::Name: // Search for game objects by name
		for (auto& go : hierarchyIds)
		{
			std::string name = hierarchyList[GetHierarchyListIndex(go.first)]->GetName();
			// set both inputs to lowercase for case-insensitive search
			std::transform(name.begin(), name.end(), name.begin(), ::tolower);
			std::transform(searchStr.begin(), searchStr.end(), searchStr.begin(), ::tolower);
			// If the game object's name contains the search string, display it. Otherwise, don't.
			go.second = (name.find(searchStr) != std::string::npos);
		}
		break;
	case SearchFilter::Component: // Search for game objects by component
		for (auto& go : hierarchyIds)
		{
			for (auto& comp : hierarchyList[GetHierarchyListIndex(go.first)]->GetComponentList())
			{
				// If the game object has the component, display it. Otherwise, don't.
				go.second = (comp->GetType() == compType); if (go.second) { break; }
			}
		}
		break;
	case SearchFilter::Tag: // Search for game objects by tag
		for (auto& go : hierarchyIds)
		{
			std::string tag = hierarchyList[GetHierarchyListIndex(go.first)]->tag;
			// set both inputs to lowercase for case-insensitive search
			std::transform(tag.begin(), tag.end(), tag.begin(), ::tolower);
			std::transform(searchStr.begin(), searchStr.end(), searchStr.begin(), ::tolower);
			// If the game object's tag contains the search string, display it. Otherwise, don't.
			go.second = (tag.find(searchStr) != std::string::npos);
		}
		break;
	default:
		break;
	}
}

/**************************************************************************/
/*!
* \brief
*	Sets the selected game object to the one at	the specified index in the
*	hierarchy list. Upon clicking on a game object in the hierarchy, this
*	function should display the game object's properties in the property editor.
* \param index
*	The index of the game object in the hierarchy list.
* \param go
*	Reference to the game object to set as selected. Used for debug purposes.
*/
/**************************************************************************/
void Hierarchy::SelectGameObject(GameObject& go, const int index)
{
	editor->Log("Selected " + go.GetName() + "[" + std::to_string(go.GetObjectId()) + "]");
	editor->GetPropertyEditor().SetSelectedEntity(index);
}
