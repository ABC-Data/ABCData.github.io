/******************************************************************************/
/*!
\file    Lighting.h
\author  Warren Ang Jun Xuan
\par     email: a.warrenjunxuan@digipen.edu
\date    March 13, 2023
\brief   Lighting Component

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "Essential.h"
#include "GameObject/Component.h"
#include <glm/glm.hpp>
#include <GL/glew.h>

class Lighting : public Component {
public:
	struct PointLight
	{		
		glm::vec3 Pos = glm::vec3(100.0f, 100.0f, 0.0f);
		glm::vec3 Color = glm::vec3(1.0f, 1.0f, 1.0f);		
		float LightIntensity = 0.0f;
	};

public:
	bool isVisible;
	PointLight Light;

	Lighting();
	~Lighting();
	/**************************************************************************/
	/*!
	  \brief
		Sets Lighting component with parameters given.

	  \param isVisibile_param
		Whether Lighting is visible.

	  \param RGB_to_set
		RGB tint of light.

	  \param Ambient_Strength
		Brightness of lighting.
	*/
	/**************************************************************************/
	void SetLighting(bool isVisible_param, glm::vec3 RGB_to_set, float Ambient_Strength);
	/**************************************************************************/
	/*!
	  \brief
		Sets Lighting component position.

	  \param Pos
		Position to set.
	*/
	/**************************************************************************/
	void SetLightingPos(glm::vec3 Pos);
	/**************************************************************************/
	/*!
	  \brief
		Gets Lighting component visibility.
	*/
	/**************************************************************************/
	bool GetVisibility();
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
};