#include "LevelManager.h"

#include <fstream>

LevelManager LevelManager::s_Instance;

void LevelManager::Init_Impl()
{
	std::fstream file("test.data", std::ios::binary);

	file << 64;

	file.close();

	Level firstLevel;

#if 0
	firstLevel.Height = 15;
	firstLevel.Width = 68;
	firstLevel.MapSkeleton += "####################################################################";
	firstLevel.MapSkeleton += "#------BBB---------------------------------------------------------#";
	firstLevel.MapSkeleton += "#-#-#-#-#-#-#------------------------------------------------------#";
	firstLevel.MapSkeleton += "#----BBBB----------------------------------------------------------#";
	firstLevel.MapSkeleton += "#-#-#-#-#-#-#------------------------------------------------------#";
	firstLevel.MapSkeleton += "#--------B---------------------------------------------------------#";
	firstLevel.MapSkeleton += "#-#-#-#-#-#B#------------------------------------------------------#";
	firstLevel.MapSkeleton += "#---BBBBB--BB------------------------------------------------------#";
	firstLevel.MapSkeleton += "#-#-#-#-#-#B#------------------------------------------------------#";
	firstLevel.MapSkeleton += "#------BBBB--------------------------------------------------------#";
	firstLevel.MapSkeleton += "#-#-#-#-#-#-#------------------------------------------------------#";
	firstLevel.MapSkeleton += "#--M-------M-------------------------------------------------------#";
	firstLevel.MapSkeleton += "#-#-#-#-#-#-#------------------------------------------------------#";
	firstLevel.MapSkeleton += "#P-----------------------------------------------------------------#";
	firstLevel.MapSkeleton += "####################################################################";
#elif 0

	firstLevel.Height = 15;
	firstLevel.Width = 15;
	firstLevel.CurrentTime = 200;
	firstLevel.Time = 200;
	firstLevel.MapSkeleton += "###############";
	firstLevel.MapSkeleton += "#------BBB----#";
	firstLevel.MapSkeleton += "#-#-#-#-#-#-#-#";
	firstLevel.MapSkeleton += "#----BBBB-----#";
	firstLevel.MapSkeleton += "#-#-#-#-#-#-#-#";
	firstLevel.MapSkeleton += "#--------B----#";
	firstLevel.MapSkeleton += "#-#-#-#-#-#B#-#";
	firstLevel.MapSkeleton += "#---BBBBB--BB-#";
	firstLevel.MapSkeleton += "#-#-#-#-#-#B#-#";
	firstLevel.MapSkeleton += "#------BBBB---#";
	firstLevel.MapSkeleton += "#-#-#-#-#-#-#-#";
	firstLevel.MapSkeleton += "#--M----------#";
	firstLevel.MapSkeleton += "#-#-#-#-#-#-#-#";
	firstLevel.MapSkeleton += "#P------------#";
	firstLevel.MapSkeleton += "###############";
#elif 1
	firstLevel.Height = 20;
	firstLevel.Width = 68;
	firstLevel.CurrentTime = 200;
	firstLevel.Time = 200;
	firstLevel.MapSkeleton += "####################################################################";
	firstLevel.MapSkeleton += "#------BBB---------------------------------------------------------#";
	firstLevel.MapSkeleton += "#-#-#-#-#-#-#------------------------------------------------------#";
	firstLevel.MapSkeleton += "#-#-#-#-#-#-#------------------------------------------------------#";
	firstLevel.MapSkeleton += "#-#-#-#-#-#-#------------------------------------------------------#";
	firstLevel.MapSkeleton += "#-#-#-#-#-#-#------------------------------------------------------#";
	firstLevel.MapSkeleton += "#-#-#-#-#-#-#------------------------------------------------------#";
	firstLevel.MapSkeleton += "#-#-#-#-#-#-#------------------------------------------------------#";
	firstLevel.MapSkeleton += "#----BBBB----------------------------------------------------------#";
	firstLevel.MapSkeleton += "#-#-#-#-#-#-#------------------------------------------------------#";
	firstLevel.MapSkeleton += "#--------B---------------------------------------------------------#";
	firstLevel.MapSkeleton += "#-#-#-#-#-#B#------------------------------------------------------#";
	firstLevel.MapSkeleton += "#---BBBBB--BB------------------------------------------------------#";
	firstLevel.MapSkeleton += "#-#-#-#-#-#B#------------------------------------------------------#";
	firstLevel.MapSkeleton += "#------BBBB--------------------------------------------------------#";
	firstLevel.MapSkeleton += "#-#-#-#-#-#-#------------------------------------------------------#";
	firstLevel.MapSkeleton += "#--M-------M-------------------------------------------------------#";
	firstLevel.MapSkeleton += "#-#-#-#-#-#-#------------------------------------------------------#";
	firstLevel.MapSkeleton += "#P-----------------------------------------------------------------#";
	firstLevel.MapSkeleton += "####################################################################";
#endif

	m_CurrentLevel = firstLevel;
}

