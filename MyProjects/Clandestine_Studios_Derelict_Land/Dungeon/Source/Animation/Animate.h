/******************************************************************************/
/*!
\file    Animate.h
\author  Warren Ang Jun Xuan
\par     email: a.warrenjunxuan@digipen.edu
\date    February 2, 2023
\brief   Animate Component

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "Essential.h"
#include "GameObject/Component.h"

class Animate : public Component
{
public:
	bool isVisible;
	bool isPlaying;
	int milliseconds_per_sprite;
	std::string TexHandle;
	Vec3 RGB_0to1;
	float Alpha_0to1;
	int delay{};

	Animate();
	~Animate();
	/**************************************************************************/
	/*!
	  \brief
		Sets Animate component with parameters given.

	  \param isVisibile_param
		Whether Animated sprite is visible.

	  \param Playing
		Whether Animated sprite is playing.

	  \param millisecs_per_sprite
		Delay for speed of sprite.

	  \param tex_hdl
		Texture handle of sprite.

	  \param RGB_to_set
		RGB tint of sprite.

	  \param alpha_param
		Alpha value of sprite.
	*/
	/**************************************************************************/
	void SetAnimate(bool isVisibile_param, bool Playing, int millisecs_per_sprite,
		std::string tex_hdl, Vec3 RGB_to_set, float alpha_param);
	/**************************************************************************/
	/*!
	  \brief
		Get Texture Handle of sprite.
	*/
	/**************************************************************************/
	std::string GetTexHandle();
	/**************************************************************************/
	/*!
	  \brief
		Set Texture Handle of sprite.

	  \param texhandle_param
		Name of texture handle to be given.
	*/
	/**************************************************************************/
	void SetTexHandle(std::string texhandle_param);
	/**************************************************************************/
	/*!
	  \brief
		Get Visibility flag of sprite.
	*/
	/**************************************************************************/
	bool GetVisibility();
	/**************************************************************************/
	/*!
	  \brief
		Set Visibility flag of sprite.

	  \param vis_param
		true or false.
	*/
	/**************************************************************************/
	void SetVisibility(bool vis_param);
	/**************************************************************************/
	/*!
	  \brief
		Get IsPlaying flag of sprite.
	*/
	/**************************************************************************/
	bool GetIsPlaying();
	/**************************************************************************/
	/*!
	  \brief
		Set IsPlaying flag of sprite.

	  \param ply_param
		true or false.
	*/
	/**************************************************************************/
	void SetIsPlaying(bool ply_param);
	/**************************************************************************/
	/*!
	  \brief
		Get total no. of frame of sprite.
	*/
	/**************************************************************************/
	int GetTotalFrame();
	/**************************************************************************/
	/*!
	  \brief
		Set total no. of frame of sprite.

	  \param frames
		no. of frames.
	*/
	/**************************************************************************/
	void SetTotalFrame(int frames);
	/**************************************************************************/
	/*!
	  \brief
		Get current no. of frame of sprite.
	*/
	/**************************************************************************/
	int GetCurrentFrame();
	/**************************************************************************/
	/*!
	  \brief
		Set current no. of frame of sprite.

	  \param frames
		no. of frames.
	*/
	/**************************************************************************/
	void SetCurrentFrame(int frames);
	/**************************************************************************/
	/*!
	  \brief
		Check if last frame of sprite.
	*/
	/**************************************************************************/
	bool CheckLastFrame();
	/**************************************************************************/
	/*!
	  \brief
		Set last frame of sprite.

	  \param state
		true or false.
	*/
	/**************************************************************************/
	void SetCheckLastFrame(bool state);
	/**************************************************************************/
	/*!
	  \brief
		Get sprite's speed.
	*/
	/**************************************************************************/
	int GetSpriteSpeed();
	/**************************************************************************/
	/*!
	  \brief
		Set sprite's speed.

	  \param millisec
		Delay in millisec.
	*/
	/**************************************************************************/
	void SetSpriteSpeed(int millisec);
	/**************************************************************************/
	/*!
		\brief
			CopyData() virtual override function to copy data to another
			component
	*/
	/**************************************************************************/
	virtual void CopyData(Component* Target) override;
	/**************************************************************************/
	/*!
		\brief
			Serialize() virtual override function to serialise component to the
			txt file
	*/
	/**************************************************************************/
	virtual void Serialize(FILE* fp) override;
	/**************************************************************************/
	/*!
		\brief
			Deserialize() virtual override function to deserialize component
			from the txt file
	*/
	/**************************************************************************/
	virtual void Deserialize(FILE* fp) override;

private:
	int total_frame, curr_frame;
	bool last_frame;
};

