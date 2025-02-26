/************************************************************************ /
/*!
\file    PerformanceViewer.h
\author  Louis Mineo @ Linn Min Htoo
\par     email: louismineo.l@digipen.edu
\date    September 25, 2022
\brief	 Function definitions of class PerformanceViewer

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include"CoreSys/PerformanceViewer.h"


//std::map<std::string, double> PerformanceViewer::Sys_To_TimeTaken_MAP;

//std::map<std::string, double> PerformanceViewer::Sys_To_Percentage_MAP;

//std::map<std::string, double> Render::Object::Sys_To_Percentage_MAP;

std::map<std::string, double> FontsRender::Sys_To_Percentage_MAP;
/**************************************************************************/
/*!
	\brief
	Constructor of PerformanceViewer Class
/**************************************************************************/
PerformanceViewer::PerformanceViewer() : Total_Time{0.f}
{

	std::vector<std::string> SysNames{ "Graphics", "Input", "Logic", "Physics", "GO Factory", "IMGUI Editor", "Audio" };

	for (int i = 0; i < SysNames.size(); i++)
	{
		Sys_To_TimeTaken_MAP.insert({ SysNames[i],0.f });
		FontsRender::Sys_To_Percentage_MAP.insert({ SysNames[i],0.f });
	}
}
/**************************************************************************/
/*!
	\brief
	Destructor of PerformanceViewer Class
/**************************************************************************/
PerformanceViewer::~PerformanceViewer()
{

}
/**************************************************************************/
/*!
	\brief
	Initialisation of PerformanceViewer Class
/**************************************************************************/
void PerformanceViewer::Init()
{

}
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
void PerformanceViewer::Update(std::string systemName, double time_taken)
{
	Sys_To_TimeTaken_MAP[systemName] = time_taken;
}

/**************************************************************************/
/*!
	\brief
	Shutdown of PerformanceViewer Class
/**************************************************************************/
void PerformanceViewer::Shutdown()
{
	Sys_To_TimeTaken_MAP.clear();
}

/**************************************************************************/
/*!
	\brief
	Calculate Total Time Taken by all systems
/**************************************************************************/
void PerformanceViewer::GenerateTotalTime()
{	
	double total = 0.f;

	for (auto const& x : Sys_To_TimeTaken_MAP)
	{
		total += x.second;
	}
	Total_Time = total;
}
/**************************************************************************/
/*!
  \brief
	Getter function to get the total time
/**************************************************************************/
double PerformanceViewer::GetTotalTime()
{
	return Total_Time;
}
/**************************************************************************/
/*!
	\brief
	Debug function to print all the time taken by each system,
	and the total time
/**************************************************************************/
void PerformanceViewer::PrintStats()
{
	std::cout << "---------------( This print call is from Core.cpp, Core::Update() )--------------------" << std::endl;
	std::cout << "Total Taken Per Loop : " << std::setprecision(10) << GetTotalTime() << std::endl;
	for (auto const& x : FontsRender::Sys_To_Percentage_MAP)
	{
		std::cout << "System: " << std::setw(15) << x.first
			<< "-------- "
			<< "% :"
			<< std::setw(7)
			<< std::setprecision(2) << x.second//(x.second / GetTotalTime()) * 100 
			<< "---- "
			<< "Time :"
			<< std::setprecision(10)
			<< Sys_To_TimeTaken_MAP[x.first]
			<< std::endl;
	}
	
}

/**************************************************************************/
/*!
	\brief
	Converts the timetaken as percentage of total time as stores it in the
	percentage map to be printed
/**************************************************************************/
void PerformanceViewer::Percentage_Conversion()
{
	for (auto const& x : Sys_To_TimeTaken_MAP)
	{
		FontsRender::Sys_To_Percentage_MAP[x.first] = (x.second / GetTotalTime()) * 100;
	}
}