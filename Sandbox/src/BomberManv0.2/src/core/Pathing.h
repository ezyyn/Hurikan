#pragma once

#include <glm/glm.hpp>
#include <deque>

struct GridNode;

struct PathNode
{
	bool Visited = false;
	float GlobalGoal = 0.0f;
	float LocalGoal = 0.0f;
	GridNode* Handle = nullptr;
};

class PathFinder
{
public:
	PathFinder() = default;
	~PathFinder() = default;

	std::deque<glm::vec2> NewPath(GridNode* start, GridNode* target);
private:
};