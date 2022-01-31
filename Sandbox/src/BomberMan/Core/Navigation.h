#pragma once

#include <Hurikan/Core/Log.h>
#include <Hurikan/Scene/Entity.h>

#include <vector>
#include <chrono>
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

			//HU_INFO("{0}", duration.count());
		}
	};

	static inline std::vector<Entity> Discover(Entity& start)
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
			//HU_INFO("{0} ! {1} ", tr.x, tr.y);
		}

		return discovered;
	}

	static inline std::list<Entity> Navigate(Entity& start, Entity& target) // nice but costly
	{
		// Grid resets all grid nodes

		// Checks validation of @start and @target
		HU_CORE_ASSERT(start, "");
		HU_CORE_ASSERT(target, "");

		// Prints out specifications for this current path solving 
	//	HU_INFO("Enemy: {0}, {1} | Player: {2}, {3}", start.Transform().Translation.x, start.Transform().Translation.y,
	// target.Transform().Translation.x, target.Transform().Translation.y);

		// Initializing path
		std::list<Entity> path = {};

		if (target.GetComponent<GridNodeComponent>().Obstacle || start.GetComponent<GridNodeComponent>().Obstacle)
		{
			return path;
		}

		// Clears grid nodes from previous path findings
		Grid::ClearNodes();

		// Lambas to get the work done
		auto distance = [](Entity& a, Entity& b)
		{
			return glm::distance(a.Transform().Translation, b.Transform().Translation);
		};
		auto heuristic = [distance](Entity& a, Entity& b)
		{
			return /*distance(a, b)*/ 1;
		};

		// Algorithm starts on the position of the enemy
		Entity currentNode = start;
		{
			auto& currentNode_ = currentNode.GetComponent<GridNodeComponent>();
			currentNode_.LocalGoal = 0.0f;
			currentNode_.GlobalGoal = heuristic(start, target);
		}

		{
			Timer alg_performance;
			// Initializing list
			std::list<Entity> notTestedNodes = {};
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

		while (p.GetComponent<GridNodeComponent>().Parent)//
		{
			/*if (!p.HasComponent<SpriteRendererComponent>())
			{
				p.AddComponent<SpriteRendererComponent>().Color = { 1,1,1,0.5f }; // TODO: MINOR FIX
			}
			else
			{
				p.GetComponent<SpriteRendererComponent>().Color = { 1,1,1,0.5f };
			}*/

			// Storing each gridnode for enemy to follow each tile
			path.push_front(p);
			p = p.GetComponent<GridNodeComponent>().Parent;
		}

		//path.push_front({ start.Transform().Translation.x, start.Transform().Translation.y, 0 });

		//	HU_INFO("-------------");
		return path;
	}
	static inline std::list<Entity> RandomPath(Entity& start)
	{
		auto& discovered_path = Discover(start);
		int random_n = rand() % (discovered_path.size());
		return Navigate(start, discovered_path[random_n]);
	}
}