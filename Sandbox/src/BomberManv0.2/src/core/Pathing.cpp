#include "Pathing.h"

#include "../sceneobjects/Grid.h"

extern Grid* g_GameGrid;

// maybe add this as a helper function

std::deque<glm::vec2> PathFinder::NewPath(GridNode* start, GridNode* target)
{
	std::deque<glm::vec2> path;


	g_GameGrid->Each([](GridNode* node)
		{
			if (!node->Obstacle)
				node->Handle.GetComponent<SpriteRendererComponent>().Color = glm::vec4(0.0f);

			node->Visited = false;
			node->GlobalGoal = INFINITY;
			node->LocalGoal = INFINITY;
			node->Parent = nullptr;

			return false;
		});


	auto distance = [](GridNode* a, GridNode* b)
	{
		return glm::distance(a->Position, b->Position);
	};
	auto heuristic = [distance](GridNode* a, GridNode* b)
	{
		return distance(a, b);
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

			float possiblyLowerGoal = currentNode->LocalGoal + distance(currentNode, nodeNeighbour);

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
			p->Parent->Handle.GetComponent<SpriteRendererComponent>().Color = { 1,1,1, 0.5f };

			// Storing each gridnode for enemy to follow each tile
			path.push_front({ p->Position.x, p->Position.y * (-1) });
			p = p->Parent;
		}

		path.push_front({ start->Position.x, start->Position.y * (-1) });

		//	HU_INFO("-------------");
	}


	return path;
}

