/******************************************************************************/
/*!
\file    Animation.h
\author  Warren Ang Jun Xuan
\par     email: a.warrenjunxuan@digipen.edu
\date    February 2, 2023
\brief   Animation System

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once

#include "Essential.h"
#pragma once

#include "System/System.h"
#include "Graphics/AssetManager.h"
#include "Renderer/Renderer.h"
#include "Graphics/AssetManager.h"
#include "Graphics/BatchRendering.h"
#include "Animation/Animate.h"

class Animation : public System {
public:
	Animation();
	~Animation();

	/**************************************************************************/
	/*!
	  \brief
		Initializes Animation System
	*/
	/**************************************************************************/
	virtual void Init();
	/**************************************************************************/
	/*!
	  \brief
		Initializes Animation System

	  \param dt
		delta time.
	*/
	/**************************************************************************/
	virtual void Update(float dt);
	/**************************************************************************/
	/*!
	  \brief
		Shuts down Animation System
	*/
	/**************************************************************************/
	virtual void Shutdown();
	/**************************************************************************/
	/*!
	  \brief
		Play Animation Once (WIP)

	  \param Object
		Object with Animate Component.
	*/
	/**************************************************************************/
	static bool PlayAnimation(Animate* Object);
	/**************************************************************************/
	/*!
	  \brief
		Get Time Elapsed calculated in Animation System
	*/
	/**************************************************************************/
	float& GetTimeElapsed() { return time_elapsed; };
	/**************************************************************************/
	/*!
	  \brief
		Get current Time calculated in Animation System
	*/
	/**************************************************************************/
	float& GetTime() { return time; };
	/**************************************************************************/
	/*!
	  \brief
		Get Sprite Counter for Time calculated in Animation System
	*/
	/**************************************************************************/
	int& GetSpriteCounter() { return sprite_counter; };
	/**************************************************************************/
	/*!
	  \brief
		Get Sprite Duration calculated in Animation System
	*/
	/**************************************************************************/
	float& GetSpriteDuration() { return SPRITE_DURATION; };
	/**************************************************************************/
	/*!
	  \brief
		Get dt in Animation System for Time calculated in Animation System
	*/
	/**************************************************************************/
	float& GetDT() { return anim_dt; };
private:
	float time_elapsed;
	int sprite_counter;
	float time;
	float SPRITE_DURATION{ 0.1f };
	float anim_dt;
};