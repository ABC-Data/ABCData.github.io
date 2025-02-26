#include "Graphics/FrameBuffer.h"

/************************************************************************ /
/*!
\file    FrameBuffer.cpp
\author  Louis Mineo @ Linn Min Htoo
\par     email: louismineo.l@digipen.edu
\date    September 25, 2022
\brief   FrameBuffer Rendering to IMGUI Gamewindow

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/


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
FrameBuffer::FrameBuffer(uint32_t width, uint32_t height)
	: mWidth(width)
	, mHeight(height)
	, mFbo(0)
	, mTextureID(0)
	, mRenderBufferID(0)
	, mCCR(1)
	, mCCG(1)
	, mCCB(1)
	, mCCA(1)
{
	//framebuffer configuration
	glGenFramebuffers(1, &mFbo);				// create frame buffer obj
	//
	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);	// bind it as an active framebuffer, to do some ops

	//Create colour attachment texture
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// Attaches it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextureID, 0);
	//glBindTexture(GL_TEXTURE_2D, 0);

	//Create depth/stencil renderbuffer
	glGenRenderbuffers(1, &mRenderBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, mRenderBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mWidth, mHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//attach the renderbuffer object
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRenderBufferID);

	//Check for Completness

	int32_t complete_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (complete_status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Failed to create framebuffer. Complete status : " << complete_status << std::endl;
	}

	//
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // return whatever is rendered, back to the glfw window

	//std::cout << "[FrameBuffer.cpp] mFBO : " << mFbo << std::endl;
}

/**************************************************************************/
	/*!
	  \brief
		Destructor of frame buffer
	*/
/**************************************************************************/
FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &mFbo); // when done with all framebuffer ops, delete the framebuffer obj
	mFbo = 0;
	mTextureID = 0;
	mRenderBufferID = 0;
}