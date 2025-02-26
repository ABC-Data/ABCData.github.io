#pragma once
#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

/************************************************************************ /
/*!
\file    GameWindow.h
\author  Louis Mineo @ Linn Min Htoo
\par     email: louismineo.l@digipen.edu
\date    September 25, 2022
\brief	 Function declarations of class GameWindow.h

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/


#include "Essential.h"
#include "System/System.h"
#include "Graphics/GLFWsetup.h" // GLSetup, GLWindow
#include "Graphics/Camera.h"
#include "Graphics/BatchRendering.h"
#include "Graphics/FrameBuffer.h"
#include "Input/MouseCodes.h"
#include "Input/Input.h"
#include <imgui_internal.h>
#include "Editor/IMGUIHeaders.h"
#include "Editor/UI/PropertyEditor/PropertyEditor.h"

namespace GW
{
	struct Statics
	{
		static uint64_t texID;
		static bool GameWindowOpen;
	};
}

enum class TransformMode { Translate, Rotate, Scale };
enum class SelectedAxis { None, X, Y, XY, Z };

class GameWindow : public System
{
public:

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
	GameWindow(bool nOpen = true, Vec2 nPos = { 800.0f, 0.0f }, float nWidth = 800, float nHeight = 450.0f, bool nFocusObject = false);

	/**************************************************************************/
	/*!
		\brief
			Destructor for the GameWindow class
	*/
	/**************************************************************************/
	~GameWindow();

	/**************************************************************************/
	/*!
		\brief
			Init() for the GameWindow class, will be called by the Editor.cpp
	*/
	/**************************************************************************/
	virtual void Init();
	/**************************************************************************/
	/*!
		\brief
			Update() for the GameWindow class, will be called by the Editor.cpp
		\param dt
			deleta time
	*/
	/**************************************************************************/
	virtual void Update(float dt);
	/**************************************************************************/
	/*!
		\brief
			Shutdown() for the GameWindow class, will be called by the Editor.cpp
	*/
	/**************************************************************************/
	virtual void Shutdown();
	/**************************************************************************/
	/*!
		\brief
			SetOpen() sets the open or close status of the window
	*/
	/**************************************************************************/
	void SetOpen(const bool nOpen);
	/**************************************************************************/
	/*!
		\brief
			DrawGameWindow() draws the game window, returns the size of the
			game window, which will be parsed into UpdateMousePos()
	*/
	/**************************************************************************/
	ImVec2 DrawGameWindow();
	/**************************************************************************/
	/*!
		\brief
			UpdateMousePos() updates the mouse position in the gamewindow
			itself
	*/
	/**************************************************************************/
	void UpdateMousePos(const ImVec2 window_size);
	/**************************************************************************/
	/*!
		\brief
			UpdateMouseHistory() updates the GameWindow mouse button presses in
			the gamewindow itself
	*/
	/**************************************************************************/
	void UpdateMouseHistory(MouseInput& Curr_GW_Mouse, int& prev_mousebtnPress, int& prev_mousebtnAction);
	/**************************************************************************/
	/*!
		\brief
			GameObjectPicking() picks the specific GameObject that is clicked on
			and sets it as the current object in the propertyeditor
	*/
	/**************************************************************************/
	void GameObjectPicking(MouseInput& GW_Mouse);
	/**************************************************************************/
	/*!
		\brief
			GameObjectShifting() shifts the specific GameObject that is clicked on
			after it has been set as the current object in the propertyeditor
	*/
	/**************************************************************************/
	void GameObjectShifting(MouseInput& GW_Mouse);

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
	void GizmoPicking(const Vec2 mousePos, Transform*& trf, const float gizmosLineClickThreshold = 7.0f);

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
	void GameObjectTransforming(Vec2 mousePos, Transform*& trf, const TransformMode mode, const SelectedAxis sa);
	
	// Whether or not the Gamewindow is open
	bool open;
	float size[2]; // window size

	// Position of hierarchy window
	Vec2 pos;

	bool cameraObjectFocus;

	SelectedAxis selectedAxis = SelectedAxis::None;
	TransformMode currentTransformMode = TransformMode::Translate;

	// struct for containing transform history
	struct TransformHistory
	{
		GameObject* obj = nullptr;
		decltype(Transform::Position) position;
		decltype(Transform::Scale) scale;
		decltype(Transform::RotationAngle) rotation{ 0.0f };
		bool isDeleted = false, deletionAction = false;
	};

	size_t transformHistoryIndex = 0;
	std::vector<TransformHistory> transformHistory;
	
	// the transforms of the children of the selected object
	std::vector<Transform> childGameObjectTransforms;
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
	void UpdateTransformHistory(GameObject* const obj, Transform* const trf);
};
#endif //GAMEWINDOW_H