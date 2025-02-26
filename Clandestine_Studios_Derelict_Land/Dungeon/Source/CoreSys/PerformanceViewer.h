#pragma once
/************************************************************************ /
/*!
\file    PerformanceViewer.h
\author  Louis Mineo @ Linn Min Htoo
\par     email: louismineo.l@digipen.edu
\date    September 25, 2022
\brief	 Function declarations of PerformanceViewer

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "Essential.h"
#include "Graphics/Graphics.h"

#include <map>
#include <iomanip>


typedef std::chrono::time_point<std::chrono::high_resolution_clock> ClockPoint;




class PerformanceViewer
{
public:
	/**************************************************************************/
	/*!
	  \brief
		Constructor of PerformanceViewer Class
	/**************************************************************************/
	PerformanceViewer();
	/**************************************************************************/
	/*!
	  \brief
		Destructor of PerformanceViewer Class
	/**************************************************************************/
	~PerformanceViewer();

	/**************************************************************************/
	/*!
	  \brief
		Initialisation of PerformanceViewer Class
	/**************************************************************************/
	void Init();
	/**************************************************************************/
	/*!
	  \brief
		Update Function of the PerformanceViewer Class
	  \param systemName
		name of system to be updates on its time taken in updateloop
	  \param time_taken
		valude to be updated
	*/
	/**************************************************************************/
	void Update(std::string systemName, double time_taken);
	/**************************************************************************/
	/*!
	  \brief
		Shutdown of PerformanceViewer Class
	/**************************************************************************/
	void Shutdown();

	/**************************************************************************/
	/*!
	  \brief
		Calculate Total Time Taken by all systems 
	/**************************************************************************/
	void GenerateTotalTime();
	/**************************************************************************/
	/*!
	  \brief
		Debug function to print all the time taken by each system, 
		and the total time
	/**************************************************************************/
	void PrintStats();
	/**************************************************************************/
	/*!
	  \brief
		Converts the timetaken as percentage of total time as stores it in the
		percentage map to be printed
	/**************************************************************************/
	void Percentage_Conversion();
	/**************************************************************************/
	/*!
	  \brief
		Getter function to get the total time
	/**************************************************************************/
	double GetTotalTime();
	
private:
	std::map<std::string, double> Sys_To_TimeTaken_MAP;
	double Total_Time;
};
