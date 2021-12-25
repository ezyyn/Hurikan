#pragma once

#include <glm/glm.hpp>
#include <deque>

struct GridNode;

class PathFinder
{
public:
	PathFinder() = default;
	~PathFinder() = default;

	std::deque<glm::vec2> NewPath(GridNode* start, GridNode* target);
private:
};