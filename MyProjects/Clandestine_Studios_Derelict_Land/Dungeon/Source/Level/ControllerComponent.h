/******************************************************************************/
/*!
\file		ControllerComponent.h
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	Jan 25, 2023
\brief		Contains Controller Component for Level Controllers

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#ifndef CONTROLLER_COMP_H
#define CONTROLLER_COMP_H

#include "Essential.h"
#include "GameObject/GameObject.h"

class Controller : public Component {
public:
	Controller();
	Controller(std::string tag, int size);
	/*!*************************************************************************
	****
	\brief
		Serialize to file
	\param[in] fp
		Filepath
	****************************************************************************
	***/
	virtual void Serialize(FILE* fp) override;

	/*!*************************************************************************
	****
	\brief
		Deserialize from file
	\param[in] fp
		Filepath
	****************************************************************************
	***/
	virtual void Deserialize(FILE* fp) override;

	/*!*************************************************************************
	****
	\brief
		Copy other component's data
	\param[in] Target
		Copy from
	****************************************************************************
	***/
	virtual void CopyData(Component* Target) override;

	std::string LevelTag;
	std::vector<bool> LevelFlags;
	int LevelFlagsSize;
};


#endif