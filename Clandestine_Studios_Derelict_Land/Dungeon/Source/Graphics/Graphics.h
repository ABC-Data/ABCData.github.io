/******************************************************************************/
/*!
\file    Graphics.h
\author  Warren Ang Jun Xuan
\par     email: a.warrenjunxuan@digipen.edu
\co-author	Louis Mineo @ Linn Min Htoo
\par     email: louismineo.l@digipen.edu
\date    September 27, 2022
\brief   Graphics System Declarations

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once

#include "Essential.h"
#include "System/System.h"
#include "Graphics/GLFWsetup.h"
#include "Transform/Transform.h"
#include "Collision/Collision.h"
#include "Graphics/BatchRendering.h"
#include "Graphics/AssetManager.h"
#include "Graphics/FrameBuffer.h"
#include "Editor/UI/GameWindow/GameWindow.h"
#include "Editor/UI/Editor.h"
#include "GameUI/GameUI.h"
#include "Deserializer/Deserializer.h"
#include "Animation/Animation.h"
#include "CoreSys/MasterObjects.h"
#include "Input/Input.h"


class FilePtr;

class Graphics : public System
{
public:
	/**************************************************************************/
	/*!
	  \brief
		Constructor for Graphics
	*/
	/**************************************************************************/
	Graphics();
	/**************************************************************************/
	/*!
	  \brief
		Destructor for Graphics.
	*/
	/**************************************************************************/
	~Graphics();
	/**************************************************************************/
	/*!
	  \brief
		Initialises GLFW window and Graphics.

	  \param width
		Sets width of window size.(default set to 1600).

	  \param height
		Sets height of window size.(default set to 900).

	  \param name
		Displays Game Name on window bar.(default set to "Game Name").
	*/
	/**************************************************************************/
	virtual void Init();
	/**************************************************************************/
	/*!
	  \brief
		Graphics updates and draw functions.

	  \param dt
		Delta time.
	*/
	/**************************************************************************/
	virtual void Update(float dt);

	virtual void AdditionalUpdate(float dt);
	/**************************************************************************/
	/*!
	  \brief
		Cleanup function for Graphics.
	*/
	/**************************************************************************/
	virtual void Shutdown();


	/**************************************************************************/
	/*!
	  \brief
		Sets the Window parameters

	  \param width
		Sets width of window size.

	  \param height
		Sets height of window size.

	  \param name
		Set window bar title
	*/
	/**************************************************************************/
	void SetWindowParams(GLint width, GLint height, std::string name);

	/**************************************************************************/
	/*!
	  \brief
		Gets the Window's width
	/**************************************************************************/
	GLint GetWindowWidth();
	/**************************************************************************/
	/*!
	  \brief
		Gets the Window's height
	/**************************************************************************/
	GLint GetWindowHeight();

	std::shared_ptr<FrameBuffer> mFrameBuffer;

	static glm::vec2 WindowScale;

	void SetVSync(const bool bool_param);
private:
	GLint WindowWidth;
	GLint WindowHeight;
	std::string WindowName;
	Assets* AssetsManager;
	GLint CurrentWidth;
	GLint CurrentHeight;	
};