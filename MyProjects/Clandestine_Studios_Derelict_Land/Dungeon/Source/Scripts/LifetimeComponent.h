#ifndef LIFETIME_COMP_H
#define LIFETIME_COMP_H
#pragma once
#include "Essential.h"
#include "GameObject/GameObject.h"

class Lifetime : public Component {
public:
	Lifetime();

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

	//Data
	float lifetime;
};

#endif