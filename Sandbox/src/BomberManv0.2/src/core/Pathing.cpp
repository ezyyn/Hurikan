#include "Pathing.h"

#include "../sceneobjects/Grid.h"

extern Grid* g_GameGrid;

// maybe add this as a helper function

std::deque<glm::vec2> PathFinder::NewPath(GridNode* start, GridNode* target)
{
	std::deque<glm::vec2> path;

	g_GameGrid->Each([](GridNode* node)
		{
			node->Visited = false;
			node->GlobalGoal = INFINITY;
			node->LocalGoal = INFINITY;
			node->Parent = nullptr;

			return false;
		});


	auto heuristic = [](GridNode* a, GridNode* b)
	{
		return glm::distance(glm::vec2{ a->IndexX, a->IndexY }, glm::vec2{ b->IndexX, b->IndexY });
	};

	GridNode* currentNode = start;
	currentNode->LocalGoal = 0.0f;
	currentNode->GlobalGoal = heuristic(start, target);

	std::list<GridNode*> notTestedNodes;
	notTestedNodes.push_back(start);

	while (!notTestedNodes.empty() && currentNode != target)
	{
		notTestedNodes.sort([](const GridNode* a, const GridNode* b) { return a->GlobalGoal < b->GlobalGoal; });

		while (!notTestedNodes.empty() && notTestedNodes.front()->Visited)
			notTestedNodes.pop_front();

		if (notTestedNodes.empty())
			break;

		currentNode = notTestedNodes.front();
		currentNode->Visited = true;

		for (auto nodeNeighbour : currentNode->Neighbours)
		{
			if (!nodeNeighbour->Visited && !nodeNeighbour->Obstacle)
				notTestedNodes.push_back(nodeNeighbour);

			float possiblyLowerGoal = currentNode->LocalGoal + heuristic(currentNode, nodeNeighbour);

			if (possiblyLowerGoal < nodeNeighbour->LocalGoal)
			{
				nodeNeighbour->Parent = currentNode;
				nodeNeighbour->LocalGoal = possiblyLowerGoal;
				nodeNeighbour->GlobalGoal = nodeNeighbour->LocalGoal + heuristic(nodeNeighbour, target);
			}
		}
	}

	if (target != nullptr)
	{
		GridNode* p = target;

		while (p->Parent != nullptr)
		{
			// Storing each gridnode for enemy to follow each tile
			path.push_front({ p->Handle.Transform().Translation.x, p->Handle.Transform().Translation.y });
			p = p->Parent;
		}

		path.push_front({ start->Handle.Transform().Translation.x, start->Handle.Transform().Translation.y });

		//	HU_INFO("-------------");
	}

	g_GameGrid->Each([](GridNode* node)
		{
			node->Visited = false;
			node->GlobalGoal = INFINITY;
			node->LocalGoal = INFINITY;
			node->Parent = nullptr;

			return false;
		});

	return path;
}

