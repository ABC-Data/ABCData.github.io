#pragma once
/************************************************************************ /
/*!
\file    FrameBuffer.h
\author  Louis Mineo @ Linn Min Htoo
\par     email: louismineo.l@digipen.edu
\date    October 25, 2022
\brief   FrameBuffer Rendering to IMGUI Gamewindow 

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include <cstdint>
#include "Graphics/GLFWsetup.h"
#include <iostream>

class FrameBuffer
{
public:
/**************************************************************************/
    /*!
      \brief
        Constructor of frame buffer
	  \param width
		width of the glfwwindow
	  \param height
		height of the glfwwindow
    */
/**************************************************************************/
	FrameBuffer(uint32_t width, uint32_t height);
/**************************************************************************/
	/*!
	  \brief
		Destructor of frame buffer
	*/
/**************************************************************************/
	~FrameBuffer();

/**************************************************************************/
	/*!
	  \brief
		Returns the FrameBufferObject's ID
	*/
/**************************************************************************/
	inline uint32_t GetFBO() const { return mFbo; }

/**************************************************************************/
	/*!
	  \brief
		Returns the FrameBufferObject's ID
	*/
/**************************************************************************/
	inline uint32_t GetTextureID() const { return mTextureID; }
/**************************************************************************/
	/*!
	  \brief
		Returns the RenderBuffer's ID
	*/
/**************************************************************************/
	inline uint32_t GetRenderBufferID() const { return mRenderBufferID; }

/**************************************************************************/
	/*!
	  \brief
		Binds the generated FrameBuffer
	*/
/**************************************************************************/
	inline void BindFrameBuffer() { glBindFramebuffer(GL_FRAMEBUFFER, mFbo); }
/**************************************************************************/
	/*!
	  \brief
		Uninds the generated FrameBuffer
	*/
/**************************************************************************/
	inline void UnbindFrameBuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
/**************************************************************************/
	/*!
	  \brief
		Gets the windows size from this framebuffer class and sets it to vars
	  \param w
		variable to get as width val
	  \param h
		variable to get as height val
	*/
/**************************************************************************/
	inline void GetSize(uint32_t& w, uint32_t& h) { w = mWidth; h = mHeight; }
/**************************************************************************/
	/*!
	  \brief
		Sets the color variables of the framebuffer
	  \param r
		variable to set as red val 
	  \param g
		variable to set as green val
	  \param b
		variable to set as blue val
	  \param a
		variable to set as alpha val
	*/
/**************************************************************************/
	inline void SetClearColour(float r, float g, float b, float a) { mCCR = r; mCCG = g; mCCB = b; mCCA = a; }
/**************************************************************************/
	/*!
	  \brief
		Gets the color variables of the framebuffer
	  \param r
		variable to get as red val 
	  \param g
		variable to get as green val
	  \param b
		variable to get as blue val
	  \param a
		variable to get as alpha val
	*/
/**************************************************************************/
	inline void GetClearColour(float& r, float& g, float& b, float& a) { r = mCCR; g = mCCG; b = mCCB; a = mCCA; }

private:
	uint32_t mFbo;
	uint32_t mTextureID;
	uint32_t mRenderBufferID;

	uint32_t mWidth, mHeight;
	float mCCR, mCCG, mCCB, mCCA;

};
