#include "Editor/UI/GameWindow/GameWindow.h"
#include "Editor/UI/Editor.h"

/************************************************************************ /
/*!
\file    GameWindow.cpp
\author  Louis Mineo @ Linn Min Htoo (30%)
\coaut	 Anderson Phua (70%)
\par     email: louismineo.l@digipen.edu
\par     email: a.phua@digipen.edu
\date    September 25, 2022
\brief	 Function definitions of class GameWindow.h. Governs the mouse picking
		 of GameObjects via gizmos and relevant transform modifications of
		 GameObjects' data.

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
constexpr float PI = 3.14f;

bool GW_MOUSE_OUT_OF_BOUNDS;

extern bool editor_open;
MouseInput GameWindow_Mouse;
constexpr int OUT_OF_BOUNDS = 123456789;

int prev_mousebuttonPressed = -1;
int prev_mousebuttonAction = -1;

// Key to frame the selected object in the center of the viewport
ImGuiKey objectFocusKey = ImGuiKey_F;
// Distance from the camera to the object before it is considered in focus
float cameraFocusDistance = 5.0f;
// Speed at which the camera moves to the object that is to be in center focus
float cameraFocusSpeed = 2.0f;
// time it takes to focus on the object
float cameraFocusTime = 0.5f; 
// time since the camera started moving towards the object
float moveTimeElapsed = 0.0f; 

// Keys to change the transform mode
ImGuiKey translateModeKey = ImGuiKey_W, scalingModeKey = ImGuiKey_R, rotationModeKey = ImGuiKey_E;
extern int gizmosXLength, gizmosYLength; // see Graphics.cpp
bool draggingActive = false, windowFocused = false, objectPicked = false;

Vec2 translateOffset = { 0, 0 };									// offset between the selectedObj & GameWindow_Mouse position at moment of click
Vec2 startingGameObjectDragPos = { 0,0 };							// the starting position of the gameobject when it is being dragged
decltype(Transform::RotationAngle) startingGameObjectRot = 0.0f;	// the starting z-axis rotation of the gameobject when it is being rotated
Vec2 startingGameObjectScale = { 1,1 };								// the starting scale of the gameobject when it is being scaled
/**************************************************************************/
/*!
	\brief
		Constructor for the GameWindow class
	\param nOpen
		open status of the window
	\param nPos
		position of the window
	\param nWidth
		width of the window
	\param nHeight
		height of the window
*/
/**************************************************************************/
GameWindow::GameWindow(bool nOpen, Vec2 nPos, float nWidth, float nHeight, bool nFocusObject) : open(nOpen), pos(nPos), size{ nWidth, nHeight }, cameraObjectFocus(nFocusObject) {}

/**************************************************************************/
/*!
	\brief
		Destructor for the GameWindow class
*/
/**************************************************************************/
GameWindow::~GameWindow() {}

/**************************************************************************/
/*!
	\brief
		Init() for the GameWindow class, will be called by the Editor.cpp
*/
/**************************************************************************/
void GameWindow::Init() {}

/**************************************************************************/
/*!
	\brief
		Update() for the GameWindow class, will be called by the Editor.cpp
	\param dt
		delta time
*/
/**************************************************************************/
void GameWindow::Update(float dt)
{
	ImVec2 win_size;
	(void)dt; if (open) win_size = DrawGameWindow();
	GW::Statics::GameWindowOpen = open;
	if (open) UpdateMousePos(win_size);

	/* {
		std::cout << "[GameWindow::Update] " <<
			"GameWindow_Mouse X : " << GameWindow_Mouse.x <<
			"\nGameWindow_Mouse Y : " << GameWindow_Mouse.y << "\n";
	}*/
	if (editor_open && editor->GetState() == EditorState::EditorStopped)
	{
		if (windowFocused && !GW_MOUSE_OUT_OF_BOUNDS) // if clicked inside of window
		{
			if ((prev_mousebuttonPressed == 0) && (prev_mousebuttonAction != GameWindow_Mouse.action)) // mouse release
			{
				//std::cout << "prev_mousebuttonAction :" << prev_mousebuttonAction << std::endl;
				//std::cout << "GameWindow_Mouse : " << GameWindow_Mouse.action << std::endl;

				UpdateMouseHistory(GameWindow_Mouse, prev_mousebuttonPressed, prev_mousebuttonAction);
			}
			else if ((GameWindow_Mouse.buttonPressed == 0 && GameWindow_Mouse.action == 1)) // upon mouse click
			{
				// Set the current GameObject into Property Handle
				if (!draggingActive && !objectPicked) { GameObjectPicking(GameWindow_Mouse); }
				// shifts via how much the mouse moves 
				GameObjectShifting(GameWindow_Mouse);
				UpdateMouseHistory(GameWindow_Mouse, prev_mousebuttonPressed, prev_mousebuttonAction);
			}

			// when user finishes moving GO and releases mouse, turn off the bool to signal that the GO is no longer being dragged around
			if (GameWindow_Mouse.buttonPressed == 0 && GameWindow_Mouse.action == 0 && draggingActive)
			{
				// latest transform history cmd updated if obj was selected beforehand and is now being dragged
				if (transformHistoryIndex && transformHistory.size() > 1 &&
					transformHistory[transformHistoryIndex].obj == transformHistory[transformHistoryIndex - 1].obj)
				{
					// note: check position or check transform mode?
					if (currentTransformMode == TransformMode::Translate)
					{
						transformHistory[transformHistoryIndex].position = editor->GetPropertyEditor().GetSelectedGameObject()->GetComponent<Transform>()->Position;
					}
					else if (currentTransformMode == TransformMode::Rotate)
					{
						transformHistory[transformHistoryIndex].rotation = editor->GetPropertyEditor().GetSelectedGameObject()->GetComponent<Transform>()->RotationAngle;
					}
					else if (currentTransformMode == TransformMode::Scale)
					{
						transformHistory[transformHistoryIndex].scale = editor->GetPropertyEditor().GetSelectedGameObject()->GetComponent<Transform>()->Scale;
					}
				}
				else
				{
					// In the event that picking and releasing the gizmos does not update the transform history, add one to correct
					// for it. Comment this line out and try moving a GO up & down by dragging its Y-axis gizmos.
					UpdateTransformHistory(editor->GetPropertyEditor().GetSelectedGameObject(), editor->GetPropertyEditor().GetSelectedGameObject()->GetComponent<Transform>());
				}

				draggingActive = objectPicked = false; startingGameObjectDragPos = { 0,0 }; selectedAxis = SelectedAxis::None;
				startingGameObjectScale = { 1,1 }; startingGameObjectRot = 0.f;
			}
		}

		// Undo transform changes with CTRL + Z and redo them with CTRL + Y.
		if (!transformHistory.empty())
		{
			if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyReleased(ImGuiKey_Z))
			{
				// Typically if there's only one transform history command, it's the initial selection of a GO.
				if (transformHistory.size() == 1 || !transformHistoryIndex)
				{
					editor->GetPropertyEditor().DeselectEntity(); draggingActive = objectPicked = false;
				}
				else if (transformHistoryIndex > 0)
				{
					// if the transform history is a delete, then we need to create a new gameobject
					if (transformHistory[transformHistoryIndex - 1].deletionAction && transformHistory[transformHistoryIndex - 1].isDeleted)
					{
						//this->GetLevel()->AddGameObject(transformHistory[transformHistoryIndex - 1].obj);
						// insert the new gameobject back into the nth pos of the game object array
						this->GetLevel()->GetGameObjectArray().insert(this->GetLevel()->GetGameObjectArray().begin()
							+ transformHistory[transformHistoryIndex - 1].obj->GetObjectId() - 1, transformHistory[transformHistoryIndex - 1].obj);
						this->GetLevel()->AddGOsPtrsToRespectiveLayers(this->GetLevel()->GetGameObjectArray()[transformHistory[transformHistoryIndex - 1].obj->GetObjectId() - 1]);

						//std::cout << this->GetLevel()->GetGameObjectArray()[transformHistory[transformHistoryIndex - 1].obj->GetObjectId() - 1]->GetObjectId() << std::endl;
						
						for (int i = 0; i < transformHistoryIndex; ++i)
						{
							if (transformHistory[i].obj->GetName() == 
								this->GetLevel()->GetGameObjectArray()[transformHistory[transformHistoryIndex - 1].obj->GetObjectId() - 1]->GetName())
							{
								// set the transform history to the back newly restored GO
								transformHistory[i].obj = this->GetLevel()->GetGameObjectArray()[transformHistory[transformHistoryIndex - 1].obj->GetObjectId() - 1];
							}
						}
						editor->GetHierarchy().RefreshHierarchy(); editor->GetPropertyEditor().SetSelectedEntity(transformHistory[transformHistoryIndex - 1].obj->GetObjectId() - 1);
						transformHistory[transformHistoryIndex - 1].isDeleted = false; // prevent duplicate if CTRL+Z again
						--transformHistoryIndex; //std::cout << "Transform History Index: " << transformHistoryIndex << std::endl;
					}
					else 
					
					if (editor->GetPropertyEditor().GetSelectedGameObject() == transformHistory[transformHistoryIndex].obj)
					{
						// -1 because id's start at 1
						if (transformHistory[transformHistoryIndex - 1].obj)
						{
							editor->GetPropertyEditor().SetSelectedEntity(transformHistory[transformHistoryIndex - 1].obj->GetObjectId() - 1);
						}
						else // going back to a deselection action
						{
							editor->GetPropertyEditor().DeselectEntity(); draggingActive = objectPicked = false;
						}

						--transformHistoryIndex;

						// After undoing, the selected object is the same as before - there may have been a change in the transformation data
						if (transformHistory[transformHistoryIndex].obj != nullptr 
						&& transformHistory[transformHistoryIndex].obj == transformHistory[transformHistoryIndex + 1].obj)
						{
							// Undo the transform
							editor->GetPropertyEditor().GetSelectedGameObject()->GetComponent<Transform>()->
								SetPosition(transformHistory[transformHistoryIndex].position);
							editor->GetPropertyEditor().GetSelectedGameObject()->GetComponent<Transform>()->
								Scale = transformHistory[transformHistoryIndex].scale;
							editor->GetPropertyEditor().GetSelectedGameObject()->GetComponent<Transform>()->
								RotationAngle = transformHistory[transformHistoryIndex].rotation;
						}
					}
					else if (!transformHistory[transformHistoryIndex].obj) // if last action was a deselection
					{
						editor->GetPropertyEditor().DeselectEntity(); draggingActive = objectPicked = false; --transformHistoryIndex;
					}
					else if (!editor->GetPropertyEditor().GetSelectedGameObject()) // if deselection was most recent user action
					{
						// if no object is selected, then we need to select the previous object
						// note the index is not decreased - that is settled if the user undos again.
						editor->GetPropertyEditor().SetSelectedEntity(transformHistory[transformHistoryIndex].obj->GetObjectId() - 1);
					}
				}
			}
			else if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyReleased(ImGuiKey_Y))
			{
				if (!transformHistoryIndex)
				{
					if (editor->GetPropertyEditor().GetSelectedGameObject() == transformHistory[transformHistoryIndex].obj && transformHistory.size() > 1)
					{
						if (transformHistory[transformHistoryIndex + 1].obj != nullptr)
						{
							// if the transform history is a delete, then we need to create a new gameobject
							if (transformHistory[transformHistoryIndex + 1].deletionAction && !transformHistory[transformHistoryIndex + 1].isDeleted)
							{
								// find the game object that was added and delete it
								for (auto& obj : this->GetLevel()->GetGameObjectArray())
								{
									if (obj->GetObjectId() == transformHistory[transformHistoryIndex + 1].obj->GetObjectId())
									{
										this->GetLevel()->GetGameObjectArray().erase(this->GetLevel()->GetGameObjectArray().begin() + obj->GetObjectId() - 1);
										this->GetLevel()->RemoveGOsPtrsFromAllLayers(obj);
										// factory->Destroy(obj);
										
										editor->GetPropertyEditor().DeselectEntity(); editor->GetHierarchy().RefreshHierarchy();
										transformHistory[transformHistoryIndex++ + 1].isDeleted = true; // prevent error if CTRL+Y again
										return;
									}
								}
							}
							else
							{
								editor->GetPropertyEditor().SetSelectedEntity(transformHistory[transformHistoryIndex + 1].obj->GetObjectId() - 1); // id's start at 1
							}
						}
						else { editor->GetPropertyEditor().DeselectEntity(); draggingActive = objectPicked = false; } // redoing a deselection action

						++transformHistoryIndex;

						// This object is the same as before - there may have been a change in the transformation data
						if (transformHistory[transformHistoryIndex].obj && transformHistory[transformHistoryIndex].obj == transformHistory[transformHistoryIndex - 1].obj)
						{
							// Redo the transform
							editor->GetPropertyEditor().GetSelectedGameObject()->GetComponent<Transform>()->SetPosition(transformHistory[transformHistoryIndex].position);
							editor->GetPropertyEditor().GetSelectedGameObject()->GetComponent<Transform>()->Scale = transformHistory[transformHistoryIndex].scale;
							editor->GetPropertyEditor().GetSelectedGameObject()->GetComponent<Transform>()->RotationAngle = transformHistory[transformHistoryIndex].rotation;
						}
						objectPicked = true;
					}
					else if (transformHistory[transformHistoryIndex].obj != nullptr)
					{
						editor->GetPropertyEditor().SetSelectedEntity(transformHistory[transformHistoryIndex].obj->GetObjectId() - 1);
						objectPicked = true;
					}
					else if (transformHistory[transformHistoryIndex].obj == nullptr)
					{
						editor->GetPropertyEditor().DeselectEntity(); draggingActive = objectPicked = false;
						++transformHistoryIndex; // increment ?
					}
				}
				else if (transformHistoryIndex < transformHistory.size() - 1)
				{
					// if the transform history is a delete, then we need to create a new gameobject
					if (transformHistory[transformHistoryIndex + 1].deletionAction && !transformHistory[transformHistoryIndex + 1].isDeleted)
					{
						// find the game object that was added and delete it
						for (auto& obj : this->GetLevel()->GetGameObjectArray())
						{
							if (obj->GetObjectId() == transformHistory[transformHistoryIndex + 1].obj->GetObjectId())
							{
								this->GetLevel()->GetGameObjectArray().erase(this->GetLevel()->GetGameObjectArray().begin() + obj->GetObjectId() - 1);
								this->GetLevel()->RemoveGOsPtrsFromAllLayers(obj); // factory->Destroy(obj); 
								editor->GetPropertyEditor().DeselectEntity(); editor->GetHierarchy().RefreshHierarchy();
								transformHistory[transformHistoryIndex].isDeleted = true; // prevent error if CTRL+Y again
								++transformHistoryIndex; 
								//std::cout << "Transform history index: " << transformHistoryIndex << std::endl;
								break;
							}
						}
					}
					else

					// This object is the same as before - there may have been a change in the transformation data
					if (editor->GetPropertyEditor().GetSelectedGameObject() == transformHistory[transformHistoryIndex].obj)
					{
						editor->GetPropertyEditor().DeselectEntity();

						if (transformHistory[transformHistoryIndex + 1].obj)
						{
							// -1 because id's start at 1
							editor->GetPropertyEditor().SetSelectedEntity(transformHistory[transformHistoryIndex + 1].obj->GetObjectId() - 1);
							objectPicked = true;
						}
						else
						{
							editor->GetPropertyEditor().DeselectEntity(); draggingActive = objectPicked = false;
						}


						if (transformHistory[transformHistoryIndex].obj != nullptr
						&& transformHistory[transformHistoryIndex].obj == transformHistory[transformHistoryIndex + 1].obj)
						{
							// Redo the transform
							editor->GetPropertyEditor().GetSelectedGameObject()->GetComponent<Transform>()->SetPosition(transformHistory[transformHistoryIndex + 1].position);
							editor->GetPropertyEditor().GetSelectedGameObject()->GetComponent<Transform>()->Scale = transformHistory[transformHistoryIndex + 1].scale;
							editor->GetPropertyEditor().GetSelectedGameObject()->GetComponent<Transform>()->RotationAngle = transformHistory[transformHistoryIndex + 1].rotation;
						}
						++transformHistoryIndex;
					}
				}
				std::cout << "Transform history index: " << transformHistoryIndex << "\n";
			}

		}

		if (!cameraObjectFocus)
		{
			//  Pressing the 'F' key will set the camera to move to focus on the selected object, tweening to the object's position
			if (ImGui::IsKeyPressed(objectFocusKey) && editor->GetPropertyEditor().GetSelectedGameObject() != nullptr) { cameraObjectFocus = true; }
		}
		else // camera is moving towards focusing the selected object to the viewport centre
		{
			//  Pressing MMB or RMB will stop the camera from moving to focus on the selected object.
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle) || ImGui::IsMouseClicked(ImGuiMouseButton_Right)) { cameraObjectFocus = false; }

			GameObject* camera = GameObject::FindWithTag(std::string("Camera"), this->GetLevel()->GetGameObjectArray());
			Vec2& cam = camera->GetComponent<Transform>()->Position;
			Vec2& obj = editor->GetPropertyEditor().GetSelectedGameObject()->GetComponent<Transform>()->Position;
			
			// If the camera is within set distance of the object's position, stop moving the camera
			// Depending on the values set, the camera position may suddenly jerk at this point. (E.g, low cFSpeed but high cFDist)
			if (std::abs(Vec2Distance(cam, obj)) < cameraFocusDistance) { cam = obj; cameraObjectFocus = false; moveTimeElapsed = 0.0f; }
			else // move the camera over to the object over a span of focusTime seconds
			{
				moveTimeElapsed += dt; float percentage = (float)(moveTimeElapsed / cameraFocusTime);
				cam.x += (obj.x - cam.x) * cameraFocusSpeed * percentage;
				cam.y += (obj.y - cam.y) * cameraFocusSpeed * percentage;
			}

			// else move the camera to the object's position in the direction of the object from the camera
			//else { Vec2 dir = obj - cam; Vec2Normalize(dir, dir); cam += dir * cameraFocusSpeed; }
		}
	}

}


/**************************************************************************/
/*!
	\brief
		SetOpen() sets the open or close status of the window
*/
/**************************************************************************/
void GameWindow::SetOpen(const bool nOpen) { open = nOpen; }

/**************************************************************************/
/*!
	\brief
		Shutdown() for the GameWindow class, will be called by the Editor.cpp
*/
/**************************************************************************/
void GameWindow::Shutdown() {}

/**************************************************************************/
/*!
	\brief
		DrawGameWindow() draws the game window, returns the size of the
		game window, which will be parsed into UpdateMousePos()
*/
/**************************************************************************/
ImVec2 GameWindow::DrawGameWindow()
{

	ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize({ size[0], size[1] });
	ImGui::Begin("Game Window", &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	ImVec2 window_size = ImGui::GetWindowSize();
	
	// main menu bar showing the current transform mode
	if (ImGui::BeginMenuBar())
	{
		// 3 selectables for the 3 transform modes, sized to the their respective words in pixels
		if (ImGui::Selectable("Translate", currentTransformMode == TransformMode::Translate, ImGuiSelectableFlags_None, ImGui::CalcTextSize("Transform")))
		{
			currentTransformMode = TransformMode::Translate;
		}

		ImGui::Spacing();

		if (ImGui::Selectable("Rotate", currentTransformMode == TransformMode::Rotate, ImGuiSelectableFlags_None, ImGui::CalcTextSize("Rotate")))
		{
			currentTransformMode = TransformMode::Rotate;
		}

		ImGui::Spacing();

		if (ImGui::Selectable("Scale", currentTransformMode == TransformMode::Scale, ImGuiSelectableFlags_None, ImGui::CalcTextSize("Scale")))
		{
			currentTransformMode = TransformMode::Scale;
		}

		ImGui::Spacing();

		// toggle to display the game window in play mode w/ or w/o the other editor windows.
		if (ImGui::Selectable("Play Maximised", editor->playFullScreen, ImGuiSelectableFlags_None, ImGui::CalcTextSize("Play Maximised")))
		{
			editor->SetFullScreen(!editor->playFullScreen);
		}

		ImGui::EndMenuBar();
	}

	// this stops accidental object picking from other windows (i.e. dragging values in the inspector left into the GW)
	windowFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);
	
	if (!editor->GetHierarchy().IsRenaming())
	{
		if (ImGui::IsKeyPressed(translateModeKey)) { currentTransformMode = TransformMode::Translate; }
		else if (ImGui::IsKeyPressed(rotationModeKey)) { currentTransformMode = TransformMode::Rotate; }
		else if (ImGui::IsKeyPressed(scalingModeKey)) { currentTransformMode = TransformMode::Scale; }
	}

	ImGui::Image((ImTextureID*)(GW::Statics::texID), window_size, ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
	return window_size;
}

/**************************************************************************/
/*!
	\brief
		UpdateMousePos() updates the mouse position in the gamewindow itself
*/
/**************************************************************************/
void GameWindow::UpdateMousePos(const ImVec2 window_size)
{
	// Set mouse position origin at top left of glfwwindow
	ImVec2 m_ImGuiMousePos = ImGui::GetMousePos();

	if (m_ImGuiMousePos.x < 0.f ||
		m_ImGuiMousePos.x > GLSetup::current_width ||
		m_ImGuiMousePos.y < 0.f ||
		m_ImGuiMousePos.y > GLSetup::current_height
		)
	{
		//std::cout << "MOUSE OUT OF GLFW WINDOW\n";
		// this means that the mouse if outside of the GLFW window
		return;
	}

	// Find the Editor panel
	ImGuiWindow* gamewindow = ImGui::FindWindowByName("Game Window");

	// Get window position  // origin btm left
	ImVec2 m_ImGuiWindowPos = gamewindow->DC.CursorPos;

	Vec2 mousePos{ m_ImGuiMousePos.x - m_ImGuiWindowPos.x,
		-m_ImGuiMousePos.y + m_ImGuiWindowPos.y };


	ImVec2 vMin = ImGui::GetWindowContentRegionMin();
	ImVec2 vMax = ImGui::GetWindowContentRegionMax();

	//ImVec2 win_size;
	//win_size.x = vMax.x - vMin.x;
	//win_size.y = vMax.y - vMin.x;

	//std::cout << "[GameWindow::UpdateMousePos]ImGui::GetWindowSize() X:" << window_size.x << " Y:" << window_size.y << std::endl;

	// Get NDC coords of mouse pos
	mousePos.x = 2.f * (mousePos.x / window_size.x) - 1.f;
	mousePos.y = 2.f * (mousePos.y / window_size.y) - 1.f;

	//std::cout << "[GameWindow::UpdateMousePos] NDC Game window X:" << mousePos.x << " Y:" << mousePos.y << std::endl;


	// ensure the mouse pos is within the NDC range
	if (mousePos.x >= -1.f && mousePos.x <= 1.f && mousePos.y >= -1.f && mousePos.y <= 1.f)
	{
		// Convert mouse pos from screen space to world space
			// Projection mtx

		mousePos.x = mousePos.x + 1.f;
		mousePos.y = -1.f * (mousePos.y - 1.f);

		mousePos.x = (mousePos.x / 2.f) * GLSetup::current_width;
		mousePos.y = (mousePos.y / 2.f) * GLSetup::current_height;

		//std::cout << "[GameWindow::UpdateMousePos] Mouse pos in window X:" << mousePos.x << " Y:" << mousePos.y << std::endl;

		GW_MOUSE_OUT_OF_BOUNDS = false;

		//std::cout << "[GameWindow::UpdateMousePos] Mouse pos in window X:" << mousePos.x << " Y:" << mousePos.y << std::endl;
	}
	else
	{

		GW_MOUSE_OUT_OF_BOUNDS = true;
		//return;  
	}


	if (!GW_MOUSE_OUT_OF_BOUNDS)
	{
		//std::cout << "MOUSE IS INSIDE OF ImGUI Game WINDOW\n";
		GameWindow_Mouse.x = (int)mousePos.x;
		GameWindow_Mouse.y = (int)mousePos.y;
	}
	else
	{
		//std::cout << "MOUSE OUT OF ImGUI Game WINDOW\n";
		GameWindow_Mouse.x = OUT_OF_BOUNDS;
		GameWindow_Mouse.y = OUT_OF_BOUNDS;
	}
}

/**************************************************************************/
/*!
	\brief
		UpdateMouseHistory() updates the GameWindow mouse button presses in
		the gamewindow itself
*/
/**************************************************************************/
void GameWindow::UpdateMouseHistory(MouseInput& Curr_GW_Mouse, int& prev_mousebtnPress, int& prev_mousebtnAction)
{
	prev_mousebtnPress = Curr_GW_Mouse.buttonPressed;
	prev_mousebtnAction = Curr_GW_Mouse.action;
}
/**************************************************************************/
/*!
	\brief
		GameObjectPicking() picks the specific GameObject that is clicked on
		and sets it as the current object in the propertyeditor
*/
/**************************************************************************/
void GameWindow::GameObjectPicking(MouseInput& GW_Mouse)
{
	//WS stands for WORLD SPACE

	//get camera AABB first
	Vec2 camPos_WS = dynamic_cast<Transform*>
		(GameObject::FindWithTag("Camera", this->GetLevel()->GetGameObjectArray())
			->GetComponent(ComponentType::Transform))->Position;

	// Main Camera covers the whole screen
	Vec2 camAA_WS = camPos_WS - Vec2((float)GLSetup::current_width / 2.f, (float)GLSetup::current_height / 2.f);
	Vec2 camBB_WS = camPos_WS + Vec2((float)GLSetup::current_width / 2.f, (float)GLSetup::current_height / 2.f);

	/*----------------------------

		NOTE:
		GAME UI GOs are bounded within the GLFW window coords.
		0 to GLFWSetup Width and Height

		Others are bounded within the world space
		0 to endless
	*//*-------------------------*/
	// loop thru all the game objs
	for (size_t i = this->GetLevel()->GetGameObjectArray().size() - 1; i > 0; --i)
	{
		// ignore the camera from picking
		if (this->GetLevel()->GetGameObjectArray()[i]->tag == "Camera") continue;

		Transform* trf = dynamic_cast<Transform*>(this->GetLevel()->GetGameObjectArray()[i]->GetComponent(ComponentType::Transform));
		GameUI* g_ui = dynamic_cast<GameUI*>((this->GetLevel()->GetGameObjectArray())[i]->GetComponent(ComponentType::GameUI));
		if (g_ui == nullptr) continue; // means it has no AABB bounding boxes i.e Camera

		// NOTE
		// GO's used solely for GAMEUI have their AABBs in WINDOW SPACE
		// Other GO' have their AABBs in GAME'S WORLD SPACE
		Vec2 GO_AA = g_ui->AA, GO_BB = g_ui->BB;

		// In the event where the picked GO is flipped across its axis, its respective scale values will be negative.
		// As a result, the AA will end up being greater than the BB. To fix this, simply swap the values of AA and BB,
		// so that the GO is picked as if it were scaled normally & upright.
		if (trf->Scale.x < 0.0f) { GO_AA.x = g_ui->BB.x; GO_BB.x = g_ui->AA.x; }
		if (trf->Scale.y < 0.0f) { GO_AA.y = g_ui->BB.y; GO_BB.y = g_ui->AA.y; }

		if (this->GetLevel()->GetGameObjectArray()[i]->layer == "UI")
		{
			Renderer* rdr = dynamic_cast<Renderer*>((this->GetLevel()->GetGameObjectArray())[i]->GetComponent(ComponentType::Renderer));

			// check if the UI obj is not active aka not showing.
			if (!rdr || !rdr->GetVisibility()) continue; //skip

			Vec2 Mouse_in_WINDOWSPACE = Vec2{ (float)GW_Mouse.x, (float)(GLSetup::current_height - GW_Mouse.y) };

			//std::cout << "Mouse_in_WINDOWSPACE pos " << Mouse_in_WINDOWSPACE.x << " , " << Mouse_in_WINDOWSPACE.y << std::endl;

			if ((Mouse_in_WINDOWSPACE.x > GO_AA.x && Mouse_in_WINDOWSPACE.x < GO_BB.x)
				&& (Mouse_in_WINDOWSPACE.y > GO_AA.y && Mouse_in_WINDOWSPACE.y < GO_BB.y))
			{
				// DEPTH TEST IN next implementation
				editor->GetPropertyEditor().SetSelectedEntity((int)i);
				startingGameObjectDragPos = trf->Position; // set up the original pos for dragging
				startingGameObjectRot = trf->RotationAngle; // set up the original rot for dragging
				startingGameObjectScale = trf->Scale; // set up the original scale for dragging

				// update transform history
				UpdateTransformHistory(this->GetLevel()->GetGameObjectArray()[i], this->GetLevel()->GetGameObjectArray()[i]->GetComponent<Transform>());

				// this should stop the mouse from picking another object by way of 
				// dragging the mouse even if another object's been picked.
				objectPicked = true; return;
			}
			else { editor->GetPropertyEditor().DeselectEntity(); } // if the mouse ptr is not within its bounds.
		}
		else
		{
			if ((trf->Position.x > camAA_WS.x && trf->Position.x < camBB_WS.x) &&
				(trf->Position.y > camAA_WS.y && trf->Position.y < camBB_WS.y))
			{
				Vec2 Mouse_in_WORLDSPACE = camAA_WS + Vec2{ (float)GW_Mouse.x, (float)(GLSetup::current_height - GW_Mouse.y) };
				//std::cout << "Mouse_in_WORLDSPACE pos " << Mouse_in_WORLDSPACE.x << " , " << Mouse_in_WORLDSPACE.y << std::endl;

				//now check if the mousepos in WS is within the non collision bounding box of the GO
				//std::cout << "GO_AA pos " << GO_AA.x << " , " << GO_AA.y << std::endl << "GO_BB pos " << GO_BB.x << " , " << GO_BB.y << std::endl;
				if ((Mouse_in_WORLDSPACE.x > GO_AA.x && Mouse_in_WORLDSPACE.x < GO_BB.x) &&
					(Mouse_in_WORLDSPACE.y > GO_AA.y && Mouse_in_WORLDSPACE.y < GO_BB.y))
				{
					// DEPTH TEST IN next implementation
					editor->GetPropertyEditor().SetSelectedEntity((int)i);
					startingGameObjectDragPos = trf->Position; // set up the original pos for dragging
					startingGameObjectRot = trf->RotationAngle; // set up the original rot for dragging
					startingGameObjectScale = trf->Scale; // set up the original scale for dragging

					// update transform history
					UpdateTransformHistory(this->GetLevel()->GetGameObjectArray()[i], this->GetLevel()->GetGameObjectArray()[i]->GetComponent<Transform>());

					// this should stop the mouse from picking another object by way of 
					// dragging the mouse even if another object's been picked.
					objectPicked = true; return;
				}
				else { editor->GetPropertyEditor().DeselectEntity(); } // if the mouse ptr is not within its bounds.
			}
		}
	}

	// update transform history
	if (!objectPicked)
	{
		// check if the transform history index is at the end of history. if not, then clear the rest of the history
		if (!transformHistory.empty() && transformHistoryIndex != transformHistory.size() - 1)
		{
			transformHistory.erase(transformHistory.begin() + transformHistoryIndex + 1, transformHistory.end());
		}

		transformHistory.push_back({ GameWindow::TransformHistory() });
		std::cout << transformHistory.front().rotation << std::endl;
		transformHistoryIndex = transformHistory.size() - 1;
		std::cout << transformHistoryIndex << std::endl;
	}
}
/**************************************************************************/
/*!
	\brief
		GameObjectShifting() shifts the specific GameObject that is clicked on
		after it has been set as the current object in the propertyeditor
*/
/**************************************************************************/
void GameWindow::GameObjectShifting(MouseInput& GW_Mouse)
{
	if (editor->GetPropertyEditor().GetSelectedGameObject() != nullptr)
	{
		GameObject* selectedGO = editor->GetPropertyEditor().GetSelectedGameObject();

		Transform* trf = dynamic_cast<Transform*>(selectedGO->GetComponent(ComponentType::Transform));

		if (selectedGO->layer == "UI")// no need for IsActive check as it would not wont even be selected by GameObjectPicking()
		{
			Vec2 Mouse_in_WINDOWSPACE = Vec2{ (float)GW_Mouse.x, (float)(GLSetup::current_height - GW_Mouse.y) };

			// if the mouse click position is on the gizmos, then transform the GO accordingly.
			if (!draggingActive) { GizmoPicking(Mouse_in_WINDOWSPACE, trf); }

			if (draggingActive) { GameObjectTransforming(Mouse_in_WINDOWSPACE, trf, currentTransformMode, selectedAxis); }
		}
		else // a GO like Yu or Mi
		{
			Vec2 camPos_WS = dynamic_cast<Transform*>(GameObject::FindWithTag("Camera",
				this->GetLevel()->GetGameObjectArray())->GetComponent(ComponentType::Transform))->Position;
			Vec2 camAA_WS = camPos_WS - Vec2((float)GLSetup::current_width / 2.f, (float)GLSetup::current_height / 2.f);
			Vec2 Mouse_in_WORLDSPACE = camAA_WS + Vec2{ (float)GW_Mouse.x, (float)(GLSetup::current_height - GW_Mouse.y) };

			// if the mouse click position is on the gizmos, then transform the GO accordingly.
			if (!draggingActive) { GizmoPicking(Mouse_in_WORLDSPACE, trf); }

			if (draggingActive) { GameObjectTransforming(Mouse_in_WORLDSPACE, trf, currentTransformMode, selectedAxis); }
		}
	}
}

/**************************************************************************/
/*!
	\brief
		Handles the mouse cursor input events when clicking on the gizmos.
	\param mousePos
		Position of the mouse cursor, in world space.
	\param trf
		Lval. reference to the transform component of the selected object.
	\param gizmosLineClickThreshold
		Threshold for the gizmos click detection.
*/
/**************************************************************************/
void GameWindow::GizmoPicking(const Vec2 mousePos, Transform*& trf, const float gizmosLineClickThreshold)
{
	if (currentTransformMode == TransformMode::Translate)
	{
		// Checking the mouse input WS position against the X axis...
		if (mousePos.x < trf->Position.x + gizmosXLength && std::abs(mousePos.y - trf->Position.y) < gizmosLineClickThreshold)
		{
			selectedAxis = SelectedAxis::X; draggingActive = true; translateOffset = trf->Position - mousePos;
		}

		// ,the Y axis...
		else if (mousePos.y < trf->Position.y + gizmosYLength && std::abs(mousePos.x - trf->Position.x) < gizmosLineClickThreshold)
		{
			selectedAxis = SelectedAxis::Y; draggingActive = true; translateOffset = trf->Position - mousePos;
		}

		// ...and on the square.
		else if ((mousePos.x - trf->Position.x) < 35.0f && (mousePos.x - trf->Position.x) > 0.0f &&
			(mousePos.y - trf->Position.y) < 35.0f && (mousePos.y - trf->Position.y) > 0.0f)
		{
			selectedAxis = SelectedAxis::XY; draggingActive = true; translateOffset = trf->Position - mousePos;
		}
	}
	else if (currentTransformMode == TransformMode::Rotate)
	{
		// if mouse click is on the circle
		if (Vec2SquareDistance(mousePos, trf->Position) < 55.0f * 55.0f) { selectedAxis = SelectedAxis::Z; draggingActive = true; }
	}
	else if (currentTransformMode == TransformMode::Scale)
	{
		// Similar checking as in the Translate mode, albeit different checking order.
		if (std::abs(mousePos.x - trf->Position.x) < 15.0f && std::abs(mousePos.y - trf->Position.y) < 15.0f)
		{ // on the center quad
			selectedAxis = SelectedAxis::XY; draggingActive = true;
		}
		else if (mousePos.x < trf->Position.x + gizmosXLength && std::abs(mousePos.y - trf->Position.y) < gizmosLineClickThreshold)
		{
			selectedAxis = SelectedAxis::X; draggingActive = true;
		}
		else if (mousePos.y < trf->Position.y + gizmosYLength && std::abs(mousePos.x - trf->Position.x) < gizmosLineClickThreshold)
		{
			selectedAxis = SelectedAxis::Y; draggingActive = true;
		}
	}
}

/**************************************************************************/
/*!
	\brief
		Handles object transform modification with respect to the mouse &
		selected object positions, along with the axis and transform mode
		selected.
	\param mousePos
		Position of the mouse cursor, in world space.
	\param trf
		Lval. reference to the transform component of the selected object.
	\param mode
		Selected transform mode (translate, rotate, scale).
	\param selectedAxis
		Selected axis (X, Y, XY, Z).
*/
/**************************************************************************/
void GameWindow::GameObjectTransforming(Vec2 mousePos, Transform*& trf, const TransformMode mode, const SelectedAxis sa)
{
	// read the drag delta from imgui
	ImVec2 dragDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
	// add the drag delta to the GO's position
	if (mode == TransformMode::Translate)
	{
		if (sa == SelectedAxis::X) { trf->SetPosition(Vec2{ mousePos.x + translateOffset.x, trf->Position.y }); }
		else if (sa == SelectedAxis::Y) { trf->SetPosition(Vec2{ trf->Position.x, mousePos.y + translateOffset.y }); }
		else if (sa == SelectedAxis::XY) { trf->SetPosition(mousePos + translateOffset); }
	}
	else if (mode == TransformMode::Rotate)
	{
		if (sa == SelectedAxis::Z)
		{
			// depending on the distance between the GO and the mouse drag delta, the amount of rotation is different
			float rotationAmount = (dragDelta.x / Vec2Distance(trf->Position, mousePos)) * 0.01f; // numbers
			if (trf->RotationAngle + rotationAmount > 2 * PI) { trf->RotationAngle = 2 * PI; }
			else if (trf->RotationAngle + rotationAmount < -2 * PI) { trf->RotationAngle = -2 * PI; }
			else { trf->RotationAngle = startingGameObjectRot + rotationAmount * Vec2Distance(trf->Position, mousePos); }
		}
	}
	else if (mode == TransformMode::Scale)
	{
		if (sa == SelectedAxis::X) { trf->Scale = startingGameObjectScale + Vec2{ dragDelta.x, 0.0f }; }
		else if (sa == SelectedAxis::Y) { trf->Scale = startingGameObjectScale + Vec2{ 0.0f, -dragDelta.y }; }
		else if (sa == SelectedAxis::XY) // General scaling
		{
			Vec2 goToMouse; Vec2Normalize(goToMouse, mousePos - trf->Position);
			// checking by dot product w/ a line from the center of the GO to the mouse && a unit vector
			float dot = Vec2DotProduct(goToMouse, Vec2{ 1.0f, 1.0f });

			/*
				.___.
				|\  | Unit vector is in centre, pointing NE. Let the screen be split into these triangles.
				| \ | If cursor in lower left tri, scale goes down, else (if in top right tri), it goes up.
				|  \|
				+---+
			*/
			// set the scale change depending on the dot product and distance between the mouse and the GO
			float distance = Vec2Length(mousePos - trf->Position);
			trf->Scale = startingGameObjectScale + Vec2{ dot * distance, dot * distance };

			// alt. trf->Scale += Vec2{ dragDelta.x / 100.0f, -dragDelta.y / 100.0f }; 
		}
	}

	// (hopefully) temporary fix that should stop the layering system from breaking the mouse picking.
	trf->GetComponentOwner()->GetComponent<GameUI>()->SetAABB(trf->Position, trf->Scale, trf->Dimensions);
}

/**************************************************************************/
/*!
	\brief
		Updates the history of the transform changes made.
	\param obj
		Pointer to the game object to be checked.
	\param trf
		Pointer to the transform component of the game object.
*/
/**************************************************************************/
void GameWindow::UpdateTransformHistory(GameObject* const obj, Transform* const trf)
{
	(void)obj;
	// check if the transform history index is at the end of history. if not, then clear the rest of the history
	if (transformHistoryIndex != transformHistory.size() - 1)
	{
		transformHistory.erase(transformHistory.begin() + transformHistoryIndex, transformHistory.end());
	}

	if (trf) transformHistory.push_back({ trf->GetComponentOwner(), trf->Position, trf->Scale, trf->RotationAngle, false });
	transformHistoryIndex = transformHistory.size() - 1;

	// print name of all the GOs in the history
	std::cout << "\nTransform History: \n";
	for (size_t i = 0; i < transformHistory.size(); ++i)
	{
		if (!transformHistory[i].obj) { "No obj selected\n"; continue; }
		std::cout << transformHistory[i].obj->GetName() << "\n";
		//std::cout << transformHistory[i].position.x << "," << transformHistory[i].position.y << "\n";
		//std::cout << transformHistory[i].scale.x << "," << transformHistory[i].scale.y << "\n";
		//std::cout << transformHistory[i].rotation << "\n";
	}
	std::cout << "Transform Index: " << transformHistoryIndex << "\n";
}