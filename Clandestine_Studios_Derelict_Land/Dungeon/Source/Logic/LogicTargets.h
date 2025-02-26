#pragma once
#include "Essential.h"
#include "GameObject/GameObject.h"

class LogicTargets : public Component
{
public:
	LogicTargets() { SetType(ComponentType::LogicTargets); };

	/**************************************************************************/
	/*!
		\brief
		Assign Game Object pointer targets based on ID
		\param GlobalGOA
		The whole Game Object Array
	*/
	/**************************************************************************/
	void AssignTargets(std::vector<GameObject*>& GlobalGOA)
	{
		for (size_t i : GOIDs) {
			if (GlobalGOA[i]) {
				Targets.push_back(GlobalGOA[i]);
			}
		}
	}

	/**************************************************************************/
	/*!
		\brief
		Assign Game Object pointer targets based on ID, used if AssignTargets was called before
		\param GlobalGOA
		The whole Game Object Array
	*/
	/**************************************************************************/
	void RefreshTargets(std::vector<GameObject*>& GlobalGOA)
	{
		Targets.clear();
		AssignTargets(GlobalGOA);
	}

	/**************************************************************************/
	/*!
		\brief
		Serialize to file
		\param fp
		Filepath
	*/
	/**************************************************************************/
	virtual void Serialize(FILE* fp) override {
		fprintf(fp, "LogicTargets\n");
		fprintf(fp, "Logic Target IDs: ");
		for (int k = 0; k < GOIDs.size(); ++k) {
			fprintf(fp, "%u", (unsigned int)GOIDs[k]);
			if (k != (int)(GOIDs.size() - 1)) {
				fprintf(fp, " ");
			}
			else {
				fprintf(fp, "|\n");
			}
		}
	}

	/**************************************************************************/
	/*!
		\brief
		Deserialize from file
		\param fp
		Filepath
	*/
	/**************************************************************************/
	virtual void Deserialize(FILE* fp) override {
		int discard = fscanf(fp, "Logic Target IDs: ");
		while (true) {
			unsigned int ID{};
			int success = fscanf(fp, "%u", &ID);
			if (!success) break;
			GOIDs.push_back((size_t)ID);
		}
		discard = fscanf(fp, "|\n");
	}

	/**************************************************************************/
	/*!
		\brief
		Copy data from other component
		\param Target
		Target to get data from
	*/
	/**************************************************************************/
	virtual void CopyData(Component* Target) override {
		LogicTargets* LT = dynamic_cast<LogicTargets*>(Target);

		if (LT != nullptr)
		{
			this->GOIDs = LT->GOIDs;
			this->Targets = LT->Targets;
		}
	}

	std::vector<size_t> GOIDs;
	std::vector<GameObject*> Targets;
};