/*!*************************************************************************
\file	PropertyEditor.cpp
\author Anderson Phua
\email: a.phua@digipen.edu
\author Louis Mineo
\email: louismineo.l@digipen.edu (only the renderer and GameUI Property Windows)
\date	27-11-22
\brief	This source file contains the necessary functionalities to operate
		the PropertyEditor system. This system is used to display the
		property editor window on the game / level editor window.

		This editor can view and edit the properties of the selected
		object, as well as view any text files.

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*****************************************************************************/
#include "Editor/UI/PropertyEditor/PropertyEditor.h"
#include "Editor/IMGUIHeaders.h"
#include "Editor/UI/Editor.h"
#include "Audio/Audio.h"
#include "CoreSys/Core.h"
#include "Graphics/AssetManager.h"

// Speed at dragging the IMGUI sliders.
constexpr const float DRAG_SPEED = 4.0f;

namespace {
	// if changing selected obj's transfom, sets a new log in history and this to true
	bool tHistChanged = false; 
	
	std::string nLayerNameBuffer;

	FMOD::Sound* ps = nullptr;

	// Values to track the current play duration of a selected audio file.
	float currentDurationMark = 0.0f; float dt_ = 0.0f;
	bool audioPlaying = false;

	char* fileData = nullptr;
	PNGHeader selectedImageHeader; GLuint texID = 0;
	ImVec4 tintCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	short tint = 0; // 0 = no tint, 1 = red, 2 = green, 3 = blue
}

int selectedAudioIndex = 0; // Index of the audio file in the audio map.

/**************************************************************************/
/*! \brief Constructor.
/**************************************************************************/
PropertyEditor::PropertyEditor(bool nOpen, Vec2 nPos, float nWidth, float nHeight)
	: open(nOpen), size{ nWidth, nHeight }, pos(nPos), selectedGameObject(nullptr) {
	sPropEditor = this;
}

/**************************************************************************/
/*! \brief Destructor.
/**************************************************************************/
PropertyEditor::~PropertyEditor() { }

/**************************************************************************/
/*! \brief Initializes the PropertyEditor system.
/**************************************************************************/
void PropertyEditor::Init() { }

/**************************************************************************/
/*!
* \brief
*	Update the PropertyEditor system.
* \param dt
* 	Delta time.
*/
/**************************************************************************/
void PropertyEditor::Update(float dt) { dt_ = dt; if (open) { DrawPropertyEditor(); } }

/**************************************************************************/
/*!
* \brief
* 	Shuts down the PropertyEditor system. Typically for deleting any memory
*	involved in related operations for this system.
*/
/**************************************************************************/
void PropertyEditor::Shutdown() { if (ps) ps->release(); }

/**************************************************************************/
/*!
* \brief
* 	Allocates memory for one property editor.
* \return
* 	Returns a pointer to the property editor.
*/
/**************************************************************************/
PropertyEditor* PropertyEditor::GetInstance()
{
	if (!sPropEditor) { sPropEditor = new PropertyEditor(true, Vec2(GLSetup::current_width * 0.85f, GLSetup::current_height * 0.05f)); }
	return sPropEditor;
}

/**************************************************************************/
/*! \brief Returns the open state of the PropertyEditor.
/**************************************************************************/
bool PropertyEditor::GetOpen() const { return open; }

/**************************************************************************/
/*!
* \brief
* 	Sets the open state of the PropertyEditor.
* \param nOpen
* 	The new open state of the PropertyEditor.
*/
/**************************************************************************/
void PropertyEditor::SetOpen(const bool nOpen) { open = nOpen; }

/**************************************************************************/
/*! \brief	Draws the PropertyEditor and handles its functionalities.
/**************************************************************************/
void PropertyEditor::DrawPropertyEditor()
{
	ImGui::Begin("Inspector", &open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_HorizontalScrollbar);

	// An empty level has no game objects - thus there shouldn't be one selected
	if (!this->GetLevel()->GetGOASize() || (assetType != SelectedType::GameObject && selectedGameObject)) { DeselectEntity(); }

	if (assetType == SelectedType::Text) // Print the text file contents
	{
		ImGui::TextWrapped(textFileName.c_str()); ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
		std::string scriptText = scriptTextBuffer.str();
		ImGui::InputTextMultiline("##Script", &scriptText, ImVec2(ImGui::GetWindowSize().x - 20.0f, ImGui::GetWindowSize().y - 20.0f),
			ImGuiInputTextFlags_ReadOnly);
	}
	else if (assetType == SelectedType::Audio)
	{
		ImGui::TextWrapped(textFileName.c_str()); ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
		if (ps) DisplaySoundData(ps);
	}
	else if (assetType == SelectedType::Texture)
	{
		//Assets::Sprite& texture = this->GetLevel()->GetAssets().GetTexture(textFileName); texID = texture.texid;
		if (!texID) { editor->Log("[PropertyEditor::DrawPropertyEditor] Failed to find texture: " + textFileName, Logger::LogType::Error); assetType = SelectedType::None; }
		else
		{
			// RGB, R, G, B buttons, right alligned, toggling the tint of the image.
			ImGui::TextWrapped(textFileName.substr(textFileName.find_last_of('\\') + 1).c_str());
			ImGui::SameLine(ImGui::GetWindowSize().x - 120.0f);
			ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical); ImGui::SameLine();
			if (ImGui::Button("RGB")) { tint = 0; } ImGui::SameLine();
			if (ImGui::Button("R")) { tint = 1; }	ImGui::SameLine();
			if (ImGui::Button("G")) { tint = 2; } ImGui::SameLine();
			if (ImGui::Button("B")) { tint = 3; } ImGui::Separator();

			if (!tint)			tintCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
			else if (tint == 1) tintCol = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
			else if (tint == 2) tintCol = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
			else if (tint == 3) tintCol = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);

			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - selectedImageHeader.width) / 2.0f);
			ImGui::Image(static_cast<ImTextureID>(&texID), ImVec2((float)selectedImageHeader.width, (float)selectedImageHeader.height), ImVec2{ 0,1 }, ImVec2{ 1,0 },
				tintCol, ImVec4{ 1,1,1,1 });
			DisplayTextureData(&selectedImageHeader);
		}
	}
	else if (assetType == SelectedType::GameObject)
	{
		if (ImGui::IsKeyReleased(ImGui::GetKeyIndex(ImGuiKey_Delete))) // Pressing the DEL key will delete the selected game object
		{
			if (selectedGameObject->GetObjectId() > 0) // Deleting the camera causes UDB
			{
				// Update undo/redo history
				GameObject* clone = factory->Clone(selectedGameObject);
				clone->SetName(selectedGameObject->GetName()); // Removing the (Clone) postfix from the clone GO's name.
				clone->SetObjectId(selectedGameObject->GetObjectId());
				editor->GetGameWindow().UpdateTransformHistory(clone, clone->GetComponent<Transform>());
				editor->GetGameWindow().transformHistory.back().isDeleted = true; editor->GetGameWindow().transformHistory.back().deletionAction = true;
				++editor->GetGameWindow().transformHistoryIndex;
				
				// When deleting a game object, any transform history logs that reference it shall now reference the clone.
				// This is to ensure that in the event of redoing that action, the clone will be restored, deselected, etc.
				// This is also to ensure that the transform history is not corrupted by the existence of the deleted original GO.
				
				for (int i = 0; i < editor->GetGameWindow().transformHistory.size(); ++i)
				{
					if (editor->GetGameWindow().transformHistory[i].obj == selectedGameObject)
					{ editor->GetGameWindow().transformHistory[i].obj = clone; }
				}

				this->GetLevel()->GetGameObjectArray().erase(this->GetLevel()->GetGameObjectArray().begin() + selectedGameObject->GetObjectId() - 1);
				this->GetLevel()->RemoveGOsPtrsFromAllLayers(selectedGameObject); factory->Destroy(selectedGameObject);
				DeselectEntity(); editor->GetHierarchy().RefreshHierarchy();
			}
		}
		else // Display the selected game object's data. (E.g. its name, id, position, scale, rotation, etc.)
		{
			DisplayGameObjectProperties(*selectedGameObject); DisplayGameObjectComponents(*selectedGameObject);
		}

		ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

		ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2.0f - 50.0f);

		if (ImGui::Button("Add Component", ImVec2(100.0f, 50.0f))) { ImGui::OpenPopup("Add Component"); }

		if (ImGui::BeginPopup("Add Component"))
		{
			if (ImGui::MenuItem("Transform")) { selectedGameObject->AddComponent(ComponentType::Transform, new Transform()); this->GetLevel()->AddGOsPtrToSpecificLayer(selectedGameObject, ComponentType::Transform); }
			if (ImGui::MenuItem("RigidBody")) { selectedGameObject->AddComponent(ComponentType::RigidBody, new RigidBody()); this->GetLevel()->AddGOsPtrToSpecificLayer(selectedGameObject, ComponentType::RigidBody); }
			if (ImGui::MenuItem("Collider")) { selectedGameObject->AddComponent(ComponentType::Collision, new Collision()); this->GetLevel()->AddGOsPtrToSpecificLayer(selectedGameObject, ComponentType::Collision); }
			if (ImGui::MenuItem("Logic")) { selectedGameObject->AddComponent(ComponentType::Logic, new LogicComponent()); this->GetLevel()->AddGOsPtrToSpecificLayer(selectedGameObject, ComponentType::Logic); }
			
			if (ImGui::MenuItem("Renderer")) { selectedGameObject->AddComponent(ComponentType::Renderer, new Renderer()); this->GetLevel()->AddGOsPtrToSpecificLayer(selectedGameObject, ComponentType::Renderer); }
			if (ImGui::MenuItem("GameUI")) { selectedGameObject->AddComponent(ComponentType::GameUI, new GameUI()); this->GetLevel()->AddGOsPtrToSpecificLayer(selectedGameObject, ComponentType::GameUI); }
			if (ImGui::MenuItem("Controller")) { selectedGameObject->AddComponent(ComponentType::Controller, new Controller()); this->GetLevel()->AddGOsPtrToSpecificLayer(selectedGameObject, ComponentType::Controller); }
			
			ImGui::EndPopup();
		}
	}

	ImGui::End();
}

#pragma region Game Object Selection / Deselection

/**************************************************************************/
/*!
* \brief
*	Gets the selected game object.
* \return
* 	Returns a pointer to the selected game object.
*/
/**************************************************************************/
GameObject* PropertyEditor::GetSelectedGameObject() { return selectedGameObject; }

/**************************************************************************/
/*!
* \brief
*	Sets the selected game object for the PropertyEditor to show its properties.
* \param goId
* 	The ID of the game object to be selected.
*/
/**************************************************************************/
void PropertyEditor::SetSelectedEntity(const int goId)
{
	// Set the selected game object - object id's start from 1
	assetType = SelectedType::GameObject; selectedGameObject = this->GetLevel()->GetGameObjectArray()[goId];
	if (!scriptTextBuffer.str().empty()) { scriptTextBuffer.str(""); scriptTextBuffer.clear(); textFileName.clear(); }
	editor->GetGameWindow().childGameObjectTransforms.clear();
	if (selectedGameObject->HasChildren())
	{
		for (int i = 0; i < selectedGameObject->GetChildren().size(); ++i)
		{ editor->GetGameWindow().childGameObjectTransforms.push_back(*selectedGameObject->GetChildren()[i]->GetComponent<Transform>()); }
	}
}

/**************************************************************************/
/*! \brief Deselect the selected game object - blanks the property editor blank.
/**************************************************************************/
void PropertyEditor::DeselectEntity() 
{ 
	selectedGameObject = nullptr; assetType = SelectedType::None; 
	tintCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	editor->GetGameWindow().childGameObjectTransforms.clear();
}

#pragma endregion

/**************************************************************************/
/*!
* \brief
* 	Displays the properties of the selected game object.
* \param go
* 	The selected game object.
*/
/**************************************************************************/
void PropertyEditor::DisplayGameObjectProperties(GameObject& go)
{
	// Display the game object's active state
	if (ImGui::Checkbox("##Active", &go.GetActive())) { go.SetActive(go.GetActive()); }

	ImGui::SameLine(); ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 0.0f);

	// If the name has changed, update the game object's name
	ImGui::InputText("##SetName", const_cast<std::string*>(&go.GetName()), ImGuiInputTextFlags_EnterReturnsTrue);

	ImGui::PopItemWidth();
	ImGui::Text("Tag:"); ImGui::SameLine(); ImGui::PushItemWidth(ImGui::GetWindowSize().x * 0.5f - 50.0f);

	// Dropdown menu for the game object's tag
	if (ImGui::BeginCombo("##Tag", go.tag.c_str(), ImGuiComboFlags_HeightLargest))
	{
		// Loop through all the tags
		for (auto& tag : GameObject::GetTagMap())
		{
			if (tag.second == "(Removed)") continue; // GameObjects shouldn't be tagged as (Removed)

			ImGui::PushID(tag.second.c_str());

			// If the tag is selected, set the game object's tag to the selected tag
			if (ImGui::Selectable(tag.second.c_str(), tag.second.c_str() == go.tag)) { go.tag = tag.second; }

			if (ImGui::BeginPopupContextItem("##TagDeletePopup"))
			{
				// If the tag is the default tag, don't delete it
				if (tag.second == "Untagged") { continue; }

				// If the user confirms to delete the tag, delete it
				//if (ImGui::MenuItem("Delete")) { GameObject::RemoveTag(tag.second); ImGui::CloseCurrentPopup(); }
				ImGui::EndPopup();
			}
			ImGui::PopID();
		}
		ImGui::Separator();
		// If the "+" button is pressed, create popup window for adding a new tag
		if (ImGui::Button("+")) { ImGui::OpenPopup("Add Tag...", ImGuiPopupFlags_MouseButtonLeft); }
		// If the popup window is open, display the popup window
		if (ImGui::BeginPopupModal("Add Tag...", 0, ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (ImGui::IsKeyPressed(ImGuiKey_Escape)) { ImGui::CloseCurrentPopup(); }

			std::string nTagNameBuffer; ImGui::Text("New Tag: "); ImGui::SameLine();

			// Input field for the new tag name
			if (ImGui::InputTextWithHint("##AddTag3", "ENTER to enter new tag", &nTagNameBuffer, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				GameObject::AddTag(nTagNameBuffer); ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine(); if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }

			ImGui::EndPopup();
		}

		ImGui::SameLine();

		// Fix the window to be at the center of the screen, pivoted around the centre of the window
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), ImGuiCond_Always, ImVec2(0.5f, 0.5f));

		if (ImGui::Button("-")) { ImGui::OpenPopup("Remove Existing Tag", ImGuiPopupFlags_MouseButtonLeft); }

		if (ImGui::BeginPopupModal("Remove Existing Tag", 0, ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (ImGui::IsKeyPressed(ImGuiKey_Escape)) { ImGui::CloseCurrentPopup(); }

			std::string tagBuffer; ImGui::Text("Remove Tag Name: "); ImGui::SameLine();

			if (ImGui::InputTextWithHint("##RemoveTag3", "ENTER to delete", &tagBuffer, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				GameObject::RemoveTag(tagBuffer);
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine(); if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }

			ImGui::EndPopup();
		}

		ImGui::EndCombo();
	}

	ImGui::PopItemWidth(); ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
	ImGui::Text("Layer:"); ImGui::SameLine(); ImGui::SetCursorPosX(ImGui::GetWindowSize().x * 0.5f + 50.0f);
	ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 0.0f);

	if (ImGui::BeginCombo("##Layers", go.layer.c_str(), ImGuiComboFlags_HeightLargest))
	{
		for (const std::string& layer : GameObject::GetLayers())
		{
			if (layer.empty()) continue;

			ImGui::PushID(layer.c_str());

			// If the layer is selected, set the game object's layer to the selected layer
			if (ImGui::Selectable(layer.c_str(), layer.c_str() == go.layer)) { go.layer = layer; }

			ImGui::PopID();
		}

		ImGui::Separator();

		// If the "+" button is pressed, create popup window for adding a new tag
		if (ImGui::Button("+")) { ImGui::OpenPopup("Add Layer...", ImGuiPopupFlags_MouseButtonLeft); }
		// If the popup window is open, display the popup window
		if (ImGui::BeginPopupModal("Add Layer...", 0, ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (ImGui::IsKeyPressed(ImGuiKey_Escape)) { ImGui::CloseCurrentPopup(); }

			ImGui::Text("New Layer: "); ImGui::SameLine();

			// Input field for the new tag name
			if (ImGui::InputTextWithHint("##AddLayer3", "ENTER to enter new layer", &nLayerNameBuffer, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				// find first empty layer with empty name
				for (std::string& layer : GameObject::GetLayers()) { if (layer.empty()) { layer = std::move(nLayerNameBuffer); break; } }
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine(); if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }

			ImGui::EndPopup();
		}

		ImGui::SameLine();

		// Fix the window to be at the center of the screen, pivoted around the centre of the window
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), ImGuiCond_Always, ImVec2(0.5f, 0.5f));

		if (ImGui::Button("-")) { ImGui::OpenPopup("Remove Existing Layer", ImGuiPopupFlags_MouseButtonLeft); }


		if (ImGui::BeginPopupModal("Remove Existing Layer", 0, ImGuiWindowFlags_AlwaysAutoResize))
		{
			// Fix the window to be at the center of the screen, pivoted around the centre of the window
			ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), ImGuiCond_Always, ImVec2(0.5f, 0.5f));

			if (ImGui::IsKeyPressed(ImGuiKey_Escape)) { ImGui::CloseCurrentPopup(); }

			std::string layerBuffer; ImGui::Text("Remove Layer: "); ImGui::SameLine();

			if (ImGui::InputTextWithHint("##RemoveLayer3", "ENTER to delete", &layerBuffer, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				GameObject::RemoveLayer(layerBuffer); ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine(); if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }

			ImGui::EndPopup();
		}



		ImGui::EndCombo();
	}

	ImGui::PopItemWidth();
}

#pragma region Component Displaying

/**************************************************************************/
/*!
* \brief
* 	Displays the components of the selected game object.
* \param go
* 	The selected game object.
*/
/**************************************************************************/
void PropertyEditor::DisplayGameObjectComponents(GameObject& go)
{
	ImGui::Separator();

	// Display the game object's components
	for (Component* comp : go.GetComponentList())
	{
		switch (comp->GetType())
		{
		case ComponentType::Transform: // Display the transform component
			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
			{
				DisplayGameObjectTransform(*go.GetComponent<Transform>());
			}
			break;
		case ComponentType::Collision: // Display the collision component
			if (ImGui::CollapsingHeader("Collision", ImGuiTreeNodeFlags_DefaultOpen))
			{
				// RMB context menu to remove component
				if (ImGui::BeginPopupContextItem("Remove Collision Component"))
				{
					if (ImGui::Selectable("Delete Component"))
					{
						go.RemoveComponent(comp->GetType());
						this->GetLevel()->RemoveGOsPtrsFromSpecificLayers(comp->GetComponentOwner(), comp->GetType());
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
				DisplayGameObjectCollider(*go.GetComponent<Collision>());
				break;
			}
			break;
		case ComponentType::RigidBody: // Display the rigid body component
			if (ImGui::CollapsingHeader("RigidBody", ImGuiTreeNodeFlags_DefaultOpen))
			{
				// RMB context menu to remove component
				if (ImGui::BeginPopupContextItem("##Component RigidBody"))
				{
					if (ImGui::Selectable("Delete Component"))
					{
						go.RemoveComponent(comp->GetType());
						this->GetLevel()->RemoveGOsPtrsFromSpecificLayers(comp->GetComponentOwner(), comp->GetType());
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
				DisplayGameObjectPhysics(*go.GetComponent<RigidBody>());
			}
			break;
		case ComponentType::GameUI: // Display the game UI component
			if (ImGui::CollapsingHeader("GameUI", ImGuiTreeNodeFlags_DefaultOpen))
			{
				// RMB context menu to remove component
				if (ImGui::BeginPopupContextItem("##Component GameUI"))
				{
					if (ImGui::Selectable("Delete Component"))
					{
						go.RemoveComponent(comp->GetType());
						this->GetLevel()->RemoveGOsPtrsFromSpecificLayers(comp->GetComponentOwner(), comp->GetType());
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
				DisplayGameObjectGameUI(*go.GetComponent<GameUI>());
			}
			break;
		case ComponentType::Renderer:
			if (ImGui::CollapsingHeader("Renderer", ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (ImGui::BeginPopupContextItem("##Component Renderer"))
				{
					if (ImGui::Selectable("Delete Component"))
					{
						go.RemoveComponent(comp->GetType());
						this->GetLevel()->RemoveGOsPtrsFromSpecificLayers(comp->GetComponentOwner(), comp->GetType());
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
				DisplayGameObjectRenderer(*go.GetComponent<Renderer>());
			}
			break;
		case ComponentType::Inventory:
		{
			if (ImGui::CollapsingHeader("Inventory", ImGuiTreeNodeFlags_DefaultOpen))
			{
				DisplayInventoryComponent(*go.GetComponent<Inventory>());
			}
			break;
		}
		case ComponentType::PlayerCombat:
		{
			if (ImGui::CollapsingHeader("PlayerCombat", ImGuiTreeNodeFlags_DefaultOpen))
			{
				DisplayCombatComponent(*go.GetComponent<PlayerCombat>());
			}
			break;
		}
		case ComponentType::EnemyCombat:
		{
			if (ImGui::CollapsingHeader("EnemyCombat", ImGuiTreeNodeFlags_DefaultOpen))
			{
				DisplayEnemyCombatComponent(*go.GetComponent<EnemyCombat>());
			}
			break;
		}
		case ComponentType::Animate:
		{
			if (ImGui::CollapsingHeader("Animate", ImGuiTreeNodeFlags_DefaultOpen))
			{
				DisplayGameObjectAnimate(*go.GetComponent<Animate>());
			}
			break;
		}
		case ComponentType::Controller:
		{
			if (ImGui::CollapsingHeader("Controller", ImGuiTreeNodeFlags_DefaultOpen))
			{
				DisplayGameObjectController(*go.GetComponent<Controller>());
			}
			break;
		}
		
		case ComponentType::Lighting:
		{
			if (ImGui::CollapsingHeader("Lighting", ImGuiTreeNodeFlags_DefaultOpen))
			{
				DisplayGameObjectLighting(*go.GetComponent<Lighting>());
			}
			break;
		}
		case ComponentType::Logic:
		{
			if (ImGui::CollapsingHeader("Logic", ImGuiTreeNodeFlags_DefaultOpen))
			{
				DisplayGameObjectLogic(*go.GetComponent<LogicComponent>());
			}
			break;
		}
		default:
		{
			break;
		}
		break;
		}
	}

}

/**************************************************************************/
/*!
* \brief
*	Common functionalities when right-clicking on a component.
* \param comp
* 	The component selected.
*/
/**************************************************************************/
void ComponentContextMenu(Component* comp)
{
	// RMB context menu to remove component
	if (ImGui::BeginPopupContextItem("##Component Context Menu"))
	{
		if (ImGui::Selectable("Delete Component"))
		{
			comp->GetComponentOwner()->RemoveComponent(comp->GetType());
			//Engine->GetLevel()->RemoveGOsPtrsFromSpecificLayers(comp->GetComponentOwner(), comp->GetType());
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

/**************************************************************************/
/*!
* \brief
* 	Displays the transform component of the selected game object.
* \param transform
* 	The transform component of the selected game object.
*/
/**************************************************************************/
void PropertyEditor::DisplayGameObjectTransform(Transform& transform)
{
	if (ImGui::BeginPopupContextItem("##TransformResetPopup"))
	{
		if (ImGui::MenuItem("Reset")) // Reset values
		{
			transform.Position = Vector2(0, 0);
			transform.RotationAngle = transform.RotationSpeed = 0;
			transform.Scale = Vector2(1, 1);
		}

		ImGui::EndPopup();
	}

	// Position
	if (ImGui::DragFloat2("Position", (float*)&transform.Position, DRAG_SPEED, -FLT_MAX, FLT_MAX))
	{
		if (editor->GetGameWindow().transformHistory[editor->GetGameWindow().transformHistoryIndex].obj
			== transform.GetComponentOwner() && !tHistChanged)
		{
			editor->GetGameWindow().UpdateTransformHistory(transform.GetComponentOwner(), transform.GetComponentOwner()->GetComponent<Transform>());
			tHistChanged = true; // Prevents the history from being updated multiple times
		}
		
		editor->GetGameWindow().transformHistory[editor->GetGameWindow().transformHistoryIndex].position
			= editor->GetPropertyEditor().GetSelectedGameObject()->GetComponent<Transform>()->Position;
	}
	else { if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) tHistChanged = false; }

	// Rotation as on 2d
	if (ImGui::SliderAngle("Rotation", &transform.RotationAngle, -360, 360))
	{
		if (editor->GetGameWindow().transformHistory[editor->GetGameWindow().transformHistoryIndex].obj
			== transform.GetComponentOwner() && !tHistChanged)
		{
			editor->GetGameWindow().UpdateTransformHistory(transform.GetComponentOwner(), transform.GetComponentOwner()->GetComponent<Transform>());
			tHistChanged = true; // Prevents the history from being updated multiple times
		}

		editor->GetGameWindow().transformHistory[editor->GetGameWindow().transformHistoryIndex].rotation = transform.RotationAngle;
	}
	else { if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) tHistChanged = false; }

	// Scale
	if (ImGui::DragFloat2("Scale", (float*)&transform.Scale, DRAG_SPEED, 0.0f, FLT_MAX))
	{
		if (editor->GetGameWindow().transformHistory[editor->GetGameWindow().transformHistoryIndex].obj
			== transform.GetComponentOwner() && !tHistChanged)
		{
			editor->GetGameWindow().UpdateTransformHistory(transform.GetComponentOwner(), transform.GetComponentOwner()->GetComponent<Transform>());
			tHistChanged = true; // Prevents the history from being updated multiple times
		}

		editor->GetGameWindow().transformHistory[editor->GetGameWindow().transformHistoryIndex].scale = transform.Scale;
	}
	else { if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) tHistChanged = false; }
	if (ImGui::IsItemHovered()) ImGui::SetTooltip("Scale values of this GameObject, in X and Y values respectively.");

	// Dimensions
	ImGui::DragFloat2("Dimensions", (float*)&transform.Dimensions, DRAG_SPEED, 0.0f, FLT_MAX);
	if (ImGui::IsItemHovered()) ImGui::SetTooltip("Dimensions of this GameObject.");

	// Depth
	ImGui::SliderFloat("Depth", &transform.Depth, 0.0f, 1.0f);
	if (ImGui::IsItemHovered()) ImGui::SetTooltip("Depth of this GameObject. Objects with higher depth will be rendered on top of objects with lower depth.");
}

/**************************************************************************/
/*!
* \brief
* 	Displays the Rigidbody physics component of the selected game object.
* \param pComp
*	Rigidbody component of the selected game object.
*/
/**************************************************************************/
void PropertyEditor::DisplayGameObjectPhysics(RigidBody& pComp)
{
	//ComponentContextMenu(&pComp);
	if (!&pComp) return;

	// Velocity Slider
	ImGui::PushID("Velocity");
	ImGui::DragFloat2("Velocity", (float*)&pComp.Velocity, DRAG_SPEED, -FLT_MAX, FLT_MAX);
	ImGui::PopID();

	// Mass Slider
	ImGui::DragFloat2("Acceleration", (float*)&pComp.Acceleration, DRAG_SPEED, -FLT_MAX, FLT_MAX);

	ImGui::DragFloat("Inverse Mass", (float*)&pComp.InverseMass, DRAG_SPEED, -FLT_MAX, FLT_MAX);
	if (ImGui::IsItemHovered()) ImGui::SetTooltip("Higher mass affects the velocity of this GameObject.");

	ImGui::Checkbox("Solid", &pComp.Solid);
}

/**************************************************************************/
/*!
* \brief
* 	Displays the rigid body component of the selected game object.
* \param cComp
* 	Collision component of the selected game object.
*/
/**************************************************************************/
void PropertyEditor::DisplayGameObjectCollider(Collision& cComp)
{
	if (&cComp != nullptr)
	{
		ImGui::Checkbox("Show Box", &cComp.showBox);
	}
}

/**************************************************************************/
/*!
* \brief
*	Display a game object's collider in the property editor.
* \param cComp
* 	GameUI component of the selected game object.
*/
/**************************************************************************/
void PropertyEditor::DisplayGameObjectGameUI(GameUI& uiComp)
{
	if (!&uiComp) return;

	// Visibility

	ImGui::Checkbox("Font Visibility", &uiComp.Visibility);

	// Depth

	ImGui::DragFloat("Font Render Depth", &uiComp.depth, 0.1f, 0.f, 1.f);

	// AABB vals
	ImGui::DragFloat2("AA", (float*)&uiComp.AA, DRAG_SPEED, -FLT_MAX, FLT_MAX);
	ImGui::DragFloat2("BB", (float*)&uiComp.BB, DRAG_SPEED, -FLT_MAX, FLT_MAX);
	// if the texHandle is changed from editor, change also

	// Font Family Handle
	std::string FontFamilyHDL = uiComp.FontFamily;
	if (ImGui::InputText("Font", &FontFamilyHDL, ImGuiInputTextFlags_EnterReturnsTrue)) {
		uiComp.SetTextFontFamily(FontFamilyHDL);
	}

	// Text Handle
	std::string Text = uiComp.Text;
	if (ImGui::InputText("Text", &Text, ImGuiInputTextFlags_EnterReturnsTrue)) {
		uiComp.SetText(Text);
	}

	// Size

	ImGui::DragFloat("Size", &uiComp.size, 0.1f, 0.f, 20.f);

	// X offset

	ImGui::DragFloat("X Offset", &uiComp.x_offset, 0.1f, -FLT_MAX, FLT_MAX);

	// Y offset

	ImGui::DragFloat("Y Offset", &uiComp.y_offset, 0.1f, -FLT_MAX, FLT_MAX);

	//RGB Colour picker

	ImGui::ColorPicker3("Font RGB", (float*)&uiComp.FontColor, ImGuiColorEditFlags_NoAlpha);

	// Alpha

	ImGui::DragFloat("Font Alpha", &uiComp.Alpha, 0.1f, 0.f, 1.f);

	// Line

	ImGui::DragInt("Line Spacing (Y - px)", &uiComp.Line_spacing, 1, 0, 1000);

}

/**************************************************************************/
/*!
* \brief
* 	Displays the renderer component of the selected game object.
* \param RendererComp
* 	Reference to the renderer component of the selected game object.
*/
/**************************************************************************/
void PropertyEditor::DisplayGameObjectRenderer(Renderer& RendererComp)
{
	//isVisible checkbox
	ImGui::Checkbox("Renderer Visibility", &RendererComp.isVisible);

	//isSpritesheet checkbox
	//ImGui::Checkbox("Is SpriteSheet", &RendererComp.isSpritesheet);

	//No. of sprites value field
	//ImGui::DragInt("No. of Sprites", &RendererComp.no_of_sprites, 1, 0, 20);
	
	//texhandle
	//std::string TexHandle = RendererComp.TexHandle; //RendererComp.GetTextureHandle();
	/*if (ImGui::InputText("Texture Handle", &TexHandle, ImGuiInputTextFlags_EnterReturnsTrue)) {
		RendererComp.SetTexHandle(TexHandle);
	}*/

	// dropdown menu
	if (ImGui::BeginCombo("Texture Handle", RendererComp.TexHandle.c_str()))
	{
		for (auto& tex : MasterObjectList->MasterAssets.GetTextureMap())
		{
			bool is_selected = (RendererComp.TexHandle == tex.first);
			if (ImGui::Selectable(tex.first.c_str(), is_selected))
			{
				RendererComp.TexHandle = tex.first;
				RendererComp.SetTexHandle(tex.first);
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	//UV Min
	ImGui::DragFloat2("UV Min", (float*)&RendererComp.Tex_UV_Min_COORD, 1.f, 0.f, FLT_MAX);
	//UV Max
	ImGui::DragFloat2("UV Max", (float*)&RendererComp.Tex_UV_Max_COORD, 1.f, 0.f, FLT_MAX);

	//RGB Colour picker
	ImGui::ColorPicker3("RGB", (float*)&RendererComp.RGB_0to1, ImGuiColorEditFlags_NoAlpha);

	//float temp_alpha = 1.f;
	//Alpha Value
	ImGui::DragFloat("Alpha", &RendererComp.Alpha_0to1, 0.001f, 0.f, 1.f);
}

/**************************************************************************/
/*!
* \brief
* 	Displays the inventory component of the selected game object.
* \param inventory
* 	Reference to the inventory script component of the selected game object.
*/
/**************************************************************************/
void PropertyEditor::DisplayInventoryComponent(Inventory& inventory)
{
	for (int i = 0; i < inventory.Invent.size(); ++i)
	{
		if (inventory.Invent[i].ID)
		{
			ImGui::Text("ID:"); ImGui::SameLine();
			ImGui::Text(std::to_string(inventory.Invent[i].ID).c_str());

			ImGui::Text("Name:"); ImGui::SameLine();
			ImGui::Text(inventory.Invent[i].Name.c_str());

			ImGui::Text("Description:"); ImGui::SameLine();
			ImGui::TextWrapped(inventory.Invent[i].Lore.c_str());

			ImGui::Text("Type:"); ImGui::SameLine();
			ImGui::Text(inventory.Invent[i].Type.c_str());

			ImGui::Text("Weight:"); ImGui::SameLine();
			ImGui::Text(std::to_string(inventory.Invent[i].Weight).c_str());
			ImGui::Separator();
		}
		else { break; }
	}
}

/**************************************************************************/
/*!
	\brief
		Displays the player's combat stats.
	\param combat
		Player's combat stats script as a component.
*/
/**************************************************************************/
void PropertyEditor::DisplayCombatComponent(PlayerCombat& combat)
{
	// Go to editor play mode to see the changes
	ImGui::Text("Current Player Health"); ImGui::SameLine();
	ImGui::SliderInt("##PCurrentHP", &combat.currentHP, 0, combat.maxHP, "%d", ImGuiSliderFlags_AlwaysClamp);
	ImGui::Text("Max Player Health"); ImGui::SameLine();
	ImGui::InputInt("##PMaxHP", &combat.maxHP, 1, 1000, ImGuiInputTextFlags_EnterReturnsTrue);

	ImGui::Text("Player Action Points"); ImGui::SameLine();
	ImGui::DragFloat("PCurrentAP", &combat.currentAP, 0.001f, 0.f, 100.f);
	ImGui::Text("Max Player Action Points"); ImGui::SameLine();
	ImGui::DragFloat("##PMaxAP", &combat.maxAP, 0.001f, 0.f, 100.f);

	ImGui::Text("Recovery Speed"); ImGui::SameLine();
	ImGui::DragFloat("##RecoverySpeed", &combat.recoverySpd, 0.001f, 0.f, 100.f);

	ImGui::Text("Player Flee Chance"); ImGui::SameLine();
	ImGui::DragFloat("##PFleeChance", &combat.FleeChance, 0.001f, 0.f, 100.f);

	ImGui::Text("Block Damage Reduction"); ImGui::SameLine();
	ImGui::DragFloat("##BlockDamageReduction", &combat.BlockDamageReduction, 0.001f, 0.f, 100.f);

	ImGui::Text("Player Attack Delay Timer Limit"); ImGui::SameLine();
	ImGui::DragFloat("##PAttackDelayTimerLimit", &combat.ATKDelayTimerLimit, 0.001f, 0.f, 100.f);
}

/**************************************************************************/
/*!
	\brief
		Displays the enemy's combat stats.
	\param combat
		Enemy's combat stats script as a component.
*/
/**************************************************************************/
void PropertyEditor::DisplayEnemyCombatComponent(EnemyCombat& combat)
{
	// Go to editor play mode to see the changes
	std::string Nombre = combat.Name;
	if (ImGui::InputText("Name", &Nombre, ImGuiInputTextFlags_EnterReturnsTrue)) { combat.Name = Nombre; }

	ImGui::Text("Attack"); ImGui::SameLine();
	ImGui::DragFloat("##EAttack", &combat.ATK, 0.001f, 0.f, 100.f);

	ImGui::Text("Current Enemy Health"); ImGui::SameLine();
	ImGui::SliderInt("##ECurrentHP", &combat.currentHP, 0, combat.maxHP, "%d", ImGuiSliderFlags_AlwaysClamp);
	ImGui::Text("Max Enemy Health"); ImGui::SameLine();
	ImGui::InputInt("##EMaxHP", &combat.maxHP, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue);

	ImGui::Text("Current Enemy Action Points"); ImGui::SameLine();
	ImGui::DragFloat("ECurrentAP", &combat.currentAP, 0.001f, 0.f, 100.f);
	ImGui::Text("Max Enemy Action Points"); ImGui::SameLine();
	ImGui::DragFloat("##EMaxAP", &combat.maxAP, 0.001f, 0.f, 100.f);

	ImGui::Text("Physical Resistance"); ImGui::SameLine();
	ImGui::DragFloat("##EPhysicalResistance", &combat.physicalRES, 0.001f, 0.f, 100.f);

	ImGui::Text("Hit Chance"); ImGui::SameLine();
	ImGui::DragFloat("##EHitChance", &combat.Hit_Chance, 0.001f, 0.f, 100.f);

	ImGui::Text("Critical Hit Chance"); ImGui::SameLine();
	ImGui::DragFloat("##ECritChance", &combat.Crit_Chance, 0.001f, 0.f, 100.f);

	ImGui::Text("Critical Hit Multiplier"); ImGui::SameLine();
	ImGui::InputFloat("##ECritMultiplier", &combat.Crit_Multiplier, 0.001f, 100.f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);

	ImGui::Text("Recovery Speed"); ImGui::SameLine();
	ImGui::DragFloat("##ERecoverySpeed", &combat.recoverySpd, 0.001f, 0.f, 100.f);

	ImGui::Text("Enemy Attack Timer"); ImGui::SameLine();
	if (ImGui::IsItemHovered()) { ImGui::BeginTooltip(); ImGui::Text("Interval between enemy attacks."); ImGui::EndTooltip(); }
	ImGui::InputFloat("##EAttackTimer", &combat.EnemyATKTimer, 0.001f, 100.f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);

	ImGui::Text("Enemy Attack Frequency"); ImGui::SameLine();
	ImGui::InputFloat("##EAttackFrequency", &combat.ATKTimerFreq, 0.001f, 100.f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);

	ImGui::Text("Text Display Duration"); ImGui::SameLine();
	ImGui::DragFloat("##TextDisplayDuration", &combat.TextDisplayDuration, 0.001f, 0.f, 100.f);

	ImGui::Text("Item Drop 1"); ImGui::SameLine();
	ImGui::InputInt("##EItemDrop", &combat.ItemDrop[0], 0, 14, ImGuiInputTextFlags_EnterReturnsTrue);

	ImGui::Text("Item Drop 2"); ImGui::SameLine();
	ImGui::InputInt("##EItemDrop", &combat.ItemDrop[1], 0, 14, ImGuiInputTextFlags_EnterReturnsTrue);

	ImGui::Text("Item Drop 3"); ImGui::SameLine();
	ImGui::InputInt("##EItemDrop", &combat.ItemDrop[2], 0, 14, ImGuiInputTextFlags_EnterReturnsTrue);

	// Total Dmg Dealt
	ImGui::Text("Total Damage Dealt"); ImGui::SameLine(); ImGui::Text(std::to_string(combat.TotalDamageDealt).c_str());

	ImGui::Text("Shake Magnitude"); ImGui::SameLine();
	ImGui::DragFloat("##ShakeMagnitude", &combat.ShakeMagnitude, 0.001f, 0.f, 100.f);

	ImGui::Text("Shake Duration"); ImGui::SameLine();
	ImGui::DragFloat("##ShakeDuration", &combat.ShakeDuration, 0.001f, 0.f, 100.f);
}

/**************************************************************************/
/*!
	\brief
		Display the Animate component of the selected game object.
	\param AnimateComp
		Reference to the Animate component of the selected game object.
*/
/**************************************************************************/
void PropertyEditor::DisplayGameObjectAnimate(Animate& AnimateComp) {
	//isVisible checkbox
	ImGui::Checkbox("Visibility", &AnimateComp.isVisible);

	//isSpritesheet checkbox
	ImGui::Checkbox("Is Playing", &AnimateComp.isPlaying);

	//MilliSecsPerSprite value field
	ImGui::DragInt("MilliSecs Per Sprite", &AnimateComp.milliseconds_per_sprite, 1, 0, INT_MAX);

	//texhandle
	std::string TexHandle = AnimateComp.TexHandle; //RendererComp.GetTextureHandle();
	if (ImGui::InputText("Texture Handle", &TexHandle, ImGuiInputTextFlags_EnterReturnsTrue)) {
		AnimateComp.SetTexHandle(TexHandle);
	}

	//RGB Colour picker
	ImGui::ColorPicker3("RGB", (float*)&AnimateComp.RGB_0to1, ImGuiColorEditFlags_NoAlpha);

	//float temp_alpha = 1.f;
	//Alpha Value
	ImGui::DragFloat("Alpha", &AnimateComp.Alpha_0to1, 0.001f, 0.f, 1.f);
}

void PropertyEditor::DisplayGameObjectController(Controller& control)
{
	// Controller level tag input
	std::string tag = control.LevelTag;
	if (ImGui::InputText("Tag", &tag, ImGuiInputTextFlags_EnterReturnsTrue)) {
		control.LevelTag = tag;
	}

	// Level Flags
	ImGui::Text("Level Flags"); ImGui::SameLine();
	if (ImGui::IsItemHovered()) { ImGui::BeginTooltip(); ImGui::Text("Flags for the level."); ImGui::EndTooltip(); }
	
	bool flag = control.LevelFlags.front();
	for (int i = 0; i < control.LevelFlagsSize; ++i) {
		flag = control.LevelFlags[i];
		if (ImGui::Checkbox(std::to_string(i).c_str(), &flag))
		{
			control.LevelFlags[i] = flag;
		}
	}
	
}

void PropertyEditor::DisplayGameObjectLighting(Lighting& LightingComp) {
	//isVisible checkbox
	ImGui::Checkbox("Visibility", &LightingComp.isVisible);

	//Light Intensity
	ImGui::DragFloat("LightIntensity", &LightingComp.Light.LightIntensity, 0.001f, 0.f, 10.f);

	//RGB Colour picker
	ImGui::ColorPicker3("Color", (float*)&LightingComp.Light.Color, ImGuiColorEditFlags_NoAlpha);
}
/**************************************************************************/
/*!
	\brief
		Display the Logic component of the selected game object.
	\param logic
		Reference to the Logic component of the selected game object.
*/
/**************************************************************************/
void PropertyEditor::DisplayGameObjectLogic(LogicComponent& logic)
{
	// Display the logic script IDs
	ImGui::Text("Scripts Attached:");
	for (int i = 0; i < logic.GetLogicIDs().size(); ++i)
	{
		ImGui::Checkbox("##Enabled:", &logic.GetLogicIDs()[i].second);
		ImGui::SameLine();
		ImGui::Text(logic.GetLogicIDs()[i].first.c_str());

		// rmb click to remove scripto

		// context menu
		ImGui::PushID("##LogicContextMenu");
		if (ImGui::BeginPopupContextItem("##LogicContextMenu"))
		{
			std::string c("Remove " + logic.GetLogicIDs()[i].first);
			ImGui::PushID(c.c_str());
			if (ImGui::Selectable(c.c_str()))
			{
				logic.RemoveScriptKey(logic.GetLogicIDs()[i].first);
			}
			ImGui::PopID();
			ImGui::EndPopup();
		}
		ImGui::PopID();
	}
}

#pragma endregion

#pragma region Asset Displaying

/**************************************************************************/
/*! \brief Gets the script text buffer.
/**************************************************************************/
std::stringstream& PropertyEditor::GetTextBuffer() { return scriptTextBuffer; }

/**************************************************************************/
/*!
* \brief
* 	Displays a text file's contents in the property editor.
* \param textFilePath
* 	The path to the text file.
*/
/**************************************************************************/
void PropertyEditor::DisplayTextFileContents(const std::string_view textFilePath)
{
	assetType = SelectedType::Text;
	// if there is text from the previous file, clear it
	if (scriptTextBuffer.str().size() > 0) { scriptTextBuffer.str(""); scriptTextBuffer.clear(); }

	textFileName = textFilePath.substr(textFilePath.find_last_of("\\") + 1);

	std::ifstream file(textFilePath);
	if (!file.is_open()) { editor->Log("[PropertyEditor::DisplayTextFileContents] Failed to open text file: " + textFileName, Logger::LogType::Error); file.close(); }
	else { std::string line; while (std::getline(file, line)) { scriptTextBuffer << line << std::endl; } }
	file.close();
}

/**************************************************************************/
/*!
* \brief
*	Displays data found from a Sound object from FMOD, and allows the user to
* 	control playback of the sound. Sound object is initialized earlier by the
*	audio file selected in the file explorer.
* \param ps
* 	Pointer to the FMOD sound object to be displayed.
*/
/**************************************************************************/
void PropertyEditor::DisplaySoundData(FMOD::Sound* const s)
{
	// Get the audio file's length
	unsigned int length; ps->getLength(&length, FMOD_TIMEUNIT_MS);
	// Get the audio file's length in seconds
	int lengthInSeconds = length / 1000;
	// Get the audio file's length in minutes
	int lengthInMinutes = lengthInSeconds / 60;
	lengthInSeconds = lengthInSeconds % 60;
	// Get the audio file's length in hours
	int lengthInHours = lengthInMinutes / 60;
	lengthInMinutes = lengthInMinutes % 60;

	// Display the audio file's length
	ImGui::Text("Length (ms) : %d", length);
	ImGui::Text("Length: %02d:%02d:%02d", lengthInHours, lengthInMinutes, lengthInSeconds);

	// Display the audio file's frequency
	float frequency; s->getDefaults(&frequency, nullptr); ImGui::Text("Frequency: %d", frequency);

	// Get the audio file's format
	FMOD_SOUND_TYPE type; FMOD_SOUND_FORMAT format; int channels; int bits;
	s->getFormat(&type, &format, &channels, &bits);

	{
		if (type == FMOD_SOUND_TYPE_MPEG) { ImGui::Text("Type: MPEG"); }
		else if (type == FMOD_SOUND_TYPE_WAV) { ImGui::Text("Type: WAV"); }
		else if (type == FMOD_SOUND_TYPE_AIFF) { ImGui::Text("Type: AIFF"); }
		else if (type == FMOD_SOUND_TYPE_ASF) { ImGui::Text("Type: ASF"); }
		else if (type == FMOD_SOUND_TYPE_DLS) { ImGui::Text("Type: DLS"); }
		else if (type == FMOD_SOUND_TYPE_FLAC) { ImGui::Text("Type: FLAC"); }
		else if (type == FMOD_SOUND_TYPE_FSB) { ImGui::Text("Type: FSB"); }
		else if (type == FMOD_SOUND_TYPE_IT) { ImGui::Text("Type: IT"); }
		else if (type == FMOD_SOUND_TYPE_MIDI) { ImGui::Text("Type: MIDI"); }
		else if (type == FMOD_SOUND_TYPE_MOD) { ImGui::Text("Type: MOD"); }
		else if (type == FMOD_SOUND_TYPE_MPEG) { ImGui::Text("Type: MPEG"); }
		else if (type == FMOD_SOUND_TYPE_OGGVORBIS) { ImGui::Text("Type: OGGVORBIS"); }
		else if (type == FMOD_SOUND_TYPE_PLAYLIST) { ImGui::Text("Type: PLAYLIST"); }
		else if (type == FMOD_SOUND_TYPE_RAW) { ImGui::Text("Type: RAW"); }
		else if (type == FMOD_SOUND_TYPE_S3M) { ImGui::Text("Type: S3M"); }
		else if (type == FMOD_SOUND_TYPE_USER) { ImGui::Text("Type: USER"); }
		else if (type == FMOD_SOUND_TYPE_XM) { ImGui::Text("Type: XM"); }
		else if (type == FMOD_SOUND_TYPE_XMA) { ImGui::Text("Type: XMA"); }
		else if (type == FMOD_SOUND_TYPE_AUDIOQUEUE) { ImGui::Text("Type: AUDIOQUEUE"); }
		else if (type == FMOD_SOUND_TYPE_AT9) { ImGui::Text("Type: AT9"); }
		else if (type == FMOD_SOUND_TYPE_VORBIS) { ImGui::Text("Type: VORBIS"); }
		else if (type == FMOD_SOUND_TYPE_MEDIA_FOUNDATION) { ImGui::Text("Type: MEDIA_FOUNDATION"); }
		else if (type == FMOD_SOUND_TYPE_MEDIACODEC) { ImGui::Text("Type: MEDIACODEC"); }
		else if (type == FMOD_SOUND_TYPE_FADPCM) { ImGui::Text("Type: FADPCM"); }
		else if (type == FMOD_SOUND_TYPE_MAX) { ImGui::Text("Type: MAX"); }
		else if (type == FMOD_SOUND_TYPE_FORCEINT) { ImGui::Text("Type: FORCEINT"); }
		else { ImGui::Text("Type: Unknown"); }

		if (format == FMOD_SOUND_FORMAT_PCM8) { ImGui::Text("Format: PCM8"); }
		else if (format == FMOD_SOUND_FORMAT_PCM16) { ImGui::Text("Format: PCM16"); }
		else if (format == FMOD_SOUND_FORMAT_PCM24) { ImGui::Text("Format: PCM24"); }
		else if (format == FMOD_SOUND_FORMAT_PCM32) { ImGui::Text("Format: PCM32"); }
		else if (format == FMOD_SOUND_FORMAT_PCMFLOAT) { ImGui::Text("Format: PCMFLOAT"); }
		else if (format == FMOD_SOUND_FORMAT_BITSTREAM) { ImGui::Text("Format: BITSTREAM"); }
		else if (format == FMOD_SOUND_FORMAT_MAX) { ImGui::Text("Format: MAX"); }
		else if (format == FMOD_SOUND_FORMAT_FORCEINT) { ImGui::Text("Format: FORCEINT"); }
		else { ImGui::Text("Format: Unknown"); }
	}

	// Display the audio file's number of subsounds
	int subsounds; s->getNumSubSounds(&subsounds); ImGui::Text("Subsounds: %d", subsounds);

	// Get the audio file's looping mode & number of loops
	FMOD_MODE mode; s->getMode(&mode); ImGui::Text("Looping: %s", (mode & FMOD_LOOP_NORMAL) ? "Yes" : "No");
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("0 = no loop, N = loop N times, -1 = loop forever"); }

	int loopCount; s->getLoopCount(&loopCount);
	ImGui::Text("Loop Count: %d", loopCount);
	ImGui::SameLine(); ImGui::PushItemWidth(50);
	if (ImGui::InputInt("##loopCount", &loopCount)) { ps->setLoopCount(std::clamp(loopCount, -1, 255)); }
	ImGui::PopItemWidth();

	// Display the audio file's number of tags
	int tags; s->getNumTags(&tags, nullptr); ImGui::Text("Tags: %d", tags);

	// Display the audio file's number of sync points
	int syncPoints; s->getNumSyncPoints(&syncPoints); ImGui::Text("Sync Points: %d", syncPoints);
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Sync points are used to trigger callbacks / events at specific points in the audio file."); }

	// Display the audio file's number of channels, bits
	void* numChannels; s->getUserData(&numChannels); ImGui::Text("Channels: %d", numChannels);
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("The number of channels in the audio file. 1 = mono, 2 = stereo, 3 = 5.1 surround, 4 = 7.1 surround, etc."); }
	ImGui::Text("Bits: %d", bits);

	// Find the audio file size
	unsigned int fileSize; s->getLength(&fileSize, FMOD_TIMEUNIT_PCMBYTES); ImGui::Text("File Size: %0.2f MB", (float)fileSize / 1048576.f);
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("The size of the audio file in bytes."); }

	// Play & Stop the audio file
	// Time marker in the format 00:00 / (min::secs)
	ImGui::Text("%02d:%02d / %02d:%02d", (int)currentDurationMark / 60, (int)currentDurationMark % 60, lengthInMinutes, lengthInSeconds);

	if (audioPlaying) { currentDurationMark += dt_; } // dt_ from Update

	if (ImGui::Button("Play")) { audio->PlaySfx(selectedAudioIndex); audioPlaying = true; }

	ImGui::SameLine();

	if (ImGui::Button("Stop")) { audio->StopSfx(); currentDurationMark = 0.0f; audioPlaying = false; }

	ImGui::SameLine();

#if 0
	// WIP: Pause the audio file
	if (ImGui::Button("Pause")) { /*selectedAudioIndex < 4 ? audio->ToggleMusicPause() : audio->StopSfx();*/ }
	else if (ImGui::IsItemHovered()) { ImGui::BeginTooltip(); ImGui::Text("(WIP) Pause the audio file"); ImGui::EndTooltip(); }

	ImGui::SameLine();

	// WIP: Resume the audio file
	if (ImGui::Button("Resume")) { /*selectedAudioIndex < 4 ? audio->ToggleMusicPause() : audio->PlaySfx(selectedAudioIndex);*/ }
	else if (ImGui::IsItemHovered()) { ImGui::BeginTooltip(); ImGui::Text("(WIP) Resume the audio file"); ImGui::EndTooltip(); }
#endif
}

/**************************************************************************/
/*!
* \brief
* 	Displays an audio file's contents in the property editor. Searches through
*	the asset manager's audio map database to find the sound file.
* \param audioFilePath
* 	The path to the audio file.
*/
/**************************************************************************/
void PropertyEditor::DisplayAudioFile(const std::string_view audioFilePath)
{
	assetType = SelectedType::Audio;
	textFileName = audioFilePath.substr(audioFilePath.find_last_of("\\") + 1);
	// Find the sound file from assets manager
	ps = this->GetLevel()->GetAssets()->GetAudio(textFileName, &selectedAudioIndex);
	if (!ps) { editor->Log("[PropertyEditor::DisplayAudioFile] Failed to find sound file: " + textFileName, Logger::LogType::Error); }
	else { std::cout << selectedAudioIndex << std::endl; }
}

bool PowerOfTwo(const int a)
{
	if (a == 0) return false;
	else if (a == 1) return true;
	else if (a % 2 == 0) return PowerOfTwo(a / 2);
	else return false;
}

/**************************************************************************/
/*!
* \brief
* 	Prepares the contents of a .png image for display in the inspector. This
* 	initializes the data inside the image file header into a struct, whose
* 	contents are then displayed in the inspector.
* \param spriteFilePath
* 	The path to the image file.
*/
/**************************************************************************/
void PropertyEditor::DisplayTextureFile(const std::string_view spriteFilePath)
{
	// Reference: https://www.fileformat.info/format/png/corion.htm
	// Also: https://www.nayuki.io/page/png-file-chunk-inspector

	assetType = SelectedType::Texture; textFileName = spriteFilePath;
	std::ifstream file(spriteFilePath, std::ios::binary);
	if (!file.is_open()) { editor->Log("[PropertyEditor::DisplayTextureFile] Failed to open file: " + textFileName, Logger::LogType::Error); return; }
	file.seekg(0, std::ios::end);
	std::streampos len = file.tellg(); fileData = new char[len]; // Allocate memory for the file data
	file.seekg(0, std::ios::beg);
	file.read(fileData, len);
	selectedImageHeader.fileSize = len;
	/*
		In a simple PNG data file header, the 1st 8 bytes is the PNG's signature.
		The signature is a fixed sequence of 8 bytes that is always the same.
		The 1st - 4th hex values uniquely identify it as a PNG.
	*/
	for (int i = 0; i < 8; ++i) { selectedImageHeader.signature[i] = *reinterpret_cast<uint8_t*>(fileData + i); }
	/*
		After the header signature comes the file "chunks" of the PNG image. First chunk is
		the IHDR chunk, This (I)mage (H)ea(D)e(R) contains the data length, width, height,
		bit depth, color type, compression, filter & interlace methods, in that order.

		Typically, the first 3 are 4 bytes each, and the rest 1-2 byte(s) each.
		Note that all the values above are by default big-endian. (A hex editor would
		help for easier viewing of the raw data).
	*/
	selectedImageHeader.width = *reinterpret_cast<int*>(const_cast<char*>(fileData + 16));
	selectedImageHeader.height = *reinterpret_cast<int*>(const_cast<char*>(fileData + 20));
	selectedImageHeader.bitDepth = *(const_cast<char*>(fileData + 24));

	if constexpr (std::endian::native == std::endian::little) // Endian-ness resolving, C++20 style
	{
		selectedImageHeader.width = _byteswap_ulong(selectedImageHeader.width);
		selectedImageHeader.height = _byteswap_ulong(selectedImageHeader.height);
	}

	selectedImageHeader.colorType = *reinterpret_cast<short*>(const_cast<char*>(fileData + 25));
	selectedImageHeader.compression = *reinterpret_cast<short*>(const_cast<char*>(fileData + 26));
	selectedImageHeader.filter = *reinterpret_cast<short*>(const_cast<char*>(fileData + 27));
	selectedImageHeader.interlace = *(const_cast<char*>(fileData + 28));

	// Beyond this lies 2 more chunks, the IDAT chunk, which contains the (I)mage (DAT)a, and the 12 byte-long IEND chunk.

#if _DEBUG
	std::cout << selectedImageHeader;
#endif

	// Cleanup
	delete[] fileData; // Free the memory allocated for the file data, unless if need to edit its properties
	file.close();

	// Get ilename without extension
	std::string filename = textFileName.substr(textFileName.find_last_of('\\') + 1);
	// if the asset manager has a matching texture ID, display it.

	// (!!!) See Graphics.cpp for a list of files where the textures are sourced.

	texID = this->GetLevel()->GetAssets()->GetTexture(filename.substr(0, filename.find_last_of('.'))).texid;
	// If, for some reason, the texture cannot be found inside the texture map, it will be directly loaded from the file.
	if (!texID)
	{
		texID = Texture::STBI_Load_Texture(textFileName.c_str(), false);
		editor->Log("Texture not found in asset base, loading directly by STBI instead.", Logger::LogType::Warning);
	}
}

/**************************************************************************/
/*!
*	\brief
* 	Prints the contents of the PNG image header struct to the inspector window.
*	\param texture
* 	Pointer to the PNG image header struct.
*/
/**************************************************************************/
void PropertyEditor::DisplayTextureData(const PNGHeader* texture)
{
	// Display the image file's properties
	ImGui::Text("Width: %d", texture->width); ImGui::SameLine(); ImGui::Text("Height: %d", texture->height);
	ImGui::SameLine(); ImGui::Text(PowerOfTwo(texture->width) && PowerOfTwo(texture->height) ? "(POT)" : "(NPOT)");
	ImGui::Text("File Size: %.2f KB", texture->fileSize / 1024.0f);

	ImGui::Text("Bit Depth Per Channel: %d-bit", texture->bitDepth);
	ImGui::Text("Color Type: %d", texture->colorType); ImGui::SameLine();
	if (texture->colorType == 0) { ImGui::SameLine(); ImGui::Text("(Grayscale)"); }
	else if (texture->colorType == 2) { ImGui::SameLine(); ImGui::Text("(RGB)"); }
	else if (texture->colorType == 3) { ImGui::SameLine(); ImGui::Text("(Indexed)"); }
	else if (texture->colorType == 4) { ImGui::SameLine(); ImGui::Text("(Grayscale + Alpha)"); }
	else if (texture->colorType == 6) { ImGui::SameLine(); ImGui::Text("(RGB + Alpha)"); }

	ImGui::Text("Compression Method: %d", texture->compression);  ImGui::SameLine();
	ImGui::Text(texture->compression == 0 ? "(Deflate)" : "(Unknown)");

	ImGui::Text("Filter Method: %d", texture->filter); ImGui::SameLine();
	ImGui::Text(texture->filter == 0 ? "(Adaptive)" : "(Unknown)");

	ImGui::Text("Interlace Method: %d", texture->interlace); ImGui::SameLine();
	ImGui::Text(texture->interlace == 0 ? "(None)" : "(Adam7)");
}

// Debug output for the PNG image header struct
std::ostream& operator<<(std::ostream& os, const PNGHeader& ph)
{
	os << "PNG Header" << std::endl;
	// os << "Signature: " << ph.signature << std::endl;
	os << "Length: " << ph.fileSize << std::endl;
	os << "Width: " << ph.width << std::endl;
	os << "Height: " << ph.height << std::endl;
	os << "Bit Depth Per Channel: " << (int)ph.bitDepth << std::endl;
	os << "Color Type: " << ph.colorType << std::endl;
	os << "Compression Method: " << ph.compression << std::endl;
	os << "Filter Method: " << ph.filter << std::endl;
	os << "Interlace Method: " << (int)ph.interlace << std::endl;
	return os;
}

#pragma endregion
