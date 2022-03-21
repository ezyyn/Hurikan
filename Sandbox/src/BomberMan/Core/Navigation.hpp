#pragma once

#include <Hurikan/Core/Log.h>
#include <Hurikan/Scene/Entity.h>

#include "BomberMan/Game/Grid.hpp"

#include <vector>
#include <chrono>
#include <random>
#include <time.h>

namespace Navigation
{
	struct Timer
	{
		std::chrono::time_point<std::chrono::steady_clock> start, end;

		std::chrono::duration<float> duration;

		Timer()
		{
			start = std::chrono::high_resolution_clock::now();
		}

		~Timer()
		{
			end = std::chrono::high_resolution_clock::now();

			duration = end - start;
		}
	};

	/*static inline std::vector<Entity> Discover(Entity& start)
	{
		HU_CORE_ASSERT(start, "");

		Grid::ClearNodes();

		std::vector<Entity> discovered = {};
		std::list<Entity> notTestedNodes = {};
		notTestedNodes.push_back(start);

		Entity currentNode = start;

		while (!notTestedNodes.empty())
		{
			if (!notTestedNodes.empty() && currentNode.GetComponent<GridNodeComponent>().Visited)
				notTestedNodes.pop_front();

			if (notTestedNodes.empty())
				break;

			currentNode = notTestedNodes.front();
			currentNode.GetComponent<GridNodeComponent>().Visited = true;
			discovered.push_back(currentNode);

			for (auto& neighbour : currentNode.GetComponent<GridNodeComponent>().Neighbours)
			{
				if (!neighbour.GetComponent<GridNodeComponent>().Visited && !neighbour.GetComponent<GridNodeComponent>().Obstacle)
				{
					notTestedNodes.push_back(neighbour);
				}
			}
		}

		for (auto& p : discovered)
		{
			if (p.GetComponent<GridNodeComponent>().Visited)
				continue;

			auto& tr = p.Transform().Translation;

			if (!p.HasComponent<SpriteRendererComponent>())
			{
				p.AddComponent<SpriteRendererComponent>().Color = { 1, 0, 0, 0.5f }; // TODO: MINOR FIX
			}
			else
			{
				p.GetComponent<SpriteRendererComponent>().Color = { 1, 0, 0, 0.5f };
			}
		}

		return discovered;
	}*/

	static inline std::list<Entity> Navigate(Entity& start, Entity& target)
	{
		// Checks validation of @start and @target
		HU_CORE_ASSERT(start, "");
		HU_CORE_ASSERT(target, "");

		// Initializing path
		std::list<Entity> path = {};

		if (target.GetComponent<GridNodeComponent>().Obstacle || start.GetComponent<GridNodeComponent>().Obstacle)
			return path;

		// Clears grid nodes from previous path findings
		Grid::ClearNodes();

		// Lambas to get the work done
		auto distance = [](Entity& a, Entity& b)
		{
			return glm::distance(a.Transform().Translation, b.Transform().Translation);
		};
		auto heuristic = [distance](Entity& a, Entity& b)
		{
			return /*distance(a, b)*/ 1.0f;
		};

		// Implementace A-Star algoritmu
		Entity currentNode = start; // 1.
		auto& currentNode_ = currentNode.GetComponent<GridNodeComponent>();
		currentNode_.LocalGoal = 0.0f;
		currentNode_.GlobalGoal = heuristic(start, target); // 2.
		{
			Timer alg_performance;
			std::list<Entity> notTestedNodes = {}; // 3.
			notTestedNodes.push_back(start);
			while (!notTestedNodes.empty())
			{
				if (currentNode == target)
					break;

				notTestedNodes.sort([](Entity& a, Entity& b)
					{
						return a.GetComponent<GridNodeComponent>().GlobalGoal < b.GetComponent<GridNodeComponent>().GlobalGoal;
					});

				while (!notTestedNodes.empty() && notTestedNodes.front().GetComponent<GridNodeComponent>().Visited)
					notTestedNodes.pop_front();

				if (notTestedNodes.empty())
					break;

				currentNode = notTestedNodes.front();
				auto& currentNode_gnc = currentNode.GetComponent<GridNodeComponent>();
				currentNode_gnc.Visited = true;

				for (auto& node_neighbour : currentNode_gnc.Neighbours)
				{
					HU_CORE_ASSERT(node_neighbour, "");

					auto& node_neightbour_gnc = node_neighbour.GetComponent<GridNodeComponent>();

					if (!node_neightbour_gnc.Visited && !node_neightbour_gnc.Obstacle)
						notTestedNodes.push_back(node_neighbour);

					float possiblyLowerGoal = currentNode_gnc.LocalGoal + distance(currentNode, node_neighbour);

					if (possiblyLowerGoal < node_neightbour_gnc.LocalGoal)
					{
						node_neightbour_gnc.Parent = currentNode;
						node_neightbour_gnc.LocalGoal = possiblyLowerGoal;
						node_neightbour_gnc.GlobalGoal = node_neightbour_gnc.LocalGoal + heuristic(node_neighbour, target);
					}
				}
			}
		}

		Entity p = target;

		while (p.GetComponent<GridNodeComponent>().Parent)
		{
			// Storing each gridnode for enemy to follow each tile
			path.push_front(p);
			p = p.GetComponent<GridNodeComponent>().Parent;
		}
		
		HU_INFO("Recalculated!");

		return path;
	}

	enum class Direction
	{
		UP = 0,
		DOWN,
		LEFT,
		RIGHT
	};


	static inline std::list<Entity> RandomPath(Entity& start)
	{
		HU_CORE_ASSERT(start, "");

		std::vector<Entity> full_path;
		std::list<Entity> random_path;

		Entity tested = start;
		// Available directions
		std::vector<Direction> dir_available;

		for (auto& neighbour : start.GetComponent<GridNodeComponent>().Neighbours)
		{
			if (neighbour.GetComponent<GridNodeComponent>().Obstacle)
				continue;

			auto& start_pos = start.Transform().Translation;
			auto& neightbour_pos = neighbour.Transform().Translation;

			if (neightbour_pos.y > start_pos.y)// UP
			{
				dir_available.push_back(Direction::UP);
			}
			else if (neightbour_pos.y < start_pos.y) // DOWN
			{
				dir_available.push_back(Direction::DOWN);
			}
			else if (neightbour_pos.x > start_pos.x) // RIGHT
			{
				dir_available.push_back(Direction::RIGHT);
			}
			else if(neightbour_pos.x < start_pos.x) // LEFT
			{
				dir_available.push_back(Direction::LEFT);
			}
		}

		if (dir_available.size() == 0)
			return {};

		int random = Utils::Random(0, (int)(dir_available.size() - 1));

		auto& random_direction = dir_available[random];

		while(!tested.GetComponent<GridNodeComponent>().Obstacle)
		{
			auto& tested_pos = tested.Transform().Translation;

			for (auto& neighbour : tested.GetComponent<GridNodeComponent>().Neighbours)
			{
				auto& neightbour_pos = neighbour.Transform().Translation;

				if (random_direction == Direction::UP && neightbour_pos.y > tested_pos.y)// UP
				{
					full_path.push_back(neighbour);
					tested = neighbour;
					break;
				}
				else if (random_direction == Direction::DOWN && neightbour_pos.y < tested_pos.y) // DOWN
				{
					full_path.push_back(neighbour);
					tested = neighbour;
					break;
				}
				else if (random_direction == Direction::RIGHT && neightbour_pos.x > tested_pos.x) // RIGHT
				{
					full_path.push_back(neighbour);
					tested = neighbour;
					break;
				}
				else if (random_direction == Direction::LEFT && neightbour_pos.x < tested_pos.x) // LEFT
				{
					full_path.push_back(neighbour);
					tested = neighbour;
					break;
				}
			}
		}
		
		if (full_path.empty())
		{
			return {};
		}

		int random_length = Utils::Random(1, (int)full_path.size());

		for (int i = 0; i < random_length; ++i)
		{
			random_path.push_back(full_path[i]);
		}

		return random_path;
	}

	static inline std::list<Entity> DirPath(int dir, int length, const Entity& start)
	{
		std::list<Entity> full_path;

		Entity tested = start;

		while (!tested.GetComponent<GridNodeComponent>().Obstacle)
		{
			auto& tested_pos = tested.Transform().Translation;

			for (auto& neighbour : tested.GetComponent<GridNodeComponent>().Neighbours)
			{
				auto& neightbour_pos = neighbour.Transform().Translation;

				if ((Direction)dir == Direction::UP && neightbour_pos.y > tested_pos.y)// UP
				{
					full_path.push_back(neighbour);
					tested = neighbour;
					break;
				}
				else if ((Direction)dir == Direction::DOWN && neightbour_pos.y < tested_pos.y) // DOWN
				{
					full_path.push_back(neighbour);
					tested = neighbour;
					break;
				}
				else if ((Direction)dir == Direction::RIGHT && neightbour_pos.x > tested_pos.x) // RIGHT
				{
					full_path.push_back(neighbour);
					tested = neighbour;
					break;
				}
				else if ((Direction)dir == Direction::LEFT && neightbour_pos.x < tested_pos.x) // LEFT
				{
					full_path.push_back(neighbour);
					tested = neighbour;
					break;
				}
			}
		}

		return full_path;
	}
}