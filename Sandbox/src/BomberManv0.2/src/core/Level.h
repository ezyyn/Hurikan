#pragma once

#include <string>

struct Level
{
	bool BossFight = false;
	std::string MapSkeleton;
	int Width, Height;
	// -1  => Unlimited time
	int Time = 0;
};
