#include "framework.h"

void HierarchicalPathfinder::InitializeCluster(ObTileMap& gameMap)
{
	int clusterWidthNumber = gameMap.cluster[0].size();
	int clusterHeightNumber = gameMap.cluster.size();

	for (int i = 0; i < clusterWidthNumber; ++i) {
		for (int j = 0; j < clusterHeightNumber; ++j) {
			Cluster* cluster = new Cluster(i, j);
			int x = i * CLUSTER_SCALE;
			int y = j * CLUSTER_SCALE;
			bool isFree = true;

			for (int k = 0; k < CLUSTER_SCALE; ++k) {
				for (int l = 0; l < CLUSTER_SCALE; ++l) {
					bool temp = gameMap.walkableTiles[x + k][y + l];
					cluster->walkability[k][l] = temp;
					if (!temp) {
						isFree = false;
					}
				}
			}
			cluster->is_free = isFree;
			gameMap.cluster[i][j] = cluster;
		}
	}
}

void HierarchicalPathfinder::CreateEntranceNodes(ObTileMap& gameMap)
{
	int clusterWidthNumber = gameMap.tileSize.x / CLUSTER_SCALE;
	int clusterHeightNumber = gameMap.tileSize.y / CLUSTER_SCALE;

	const vector<vector<bool>> walkability = gameMap.walkableTiles;
	vector<vector<Cluster*>>& clusters = gameMap.cluster;
	int entranceCount = 0;

	// 세로 방향 entrance
	for (int i = 0; i < clusterWidthNumber - 1; ++i) {
		for (int j = 0; j < clusterHeightNumber; ++j) {
			int offset_x = i * CLUSTER_SCALE + CLUSTER_SCALE - 1;
			int offset_y = j * CLUSTER_SCALE;
			int start = -1;

			Cluster* leftCluster = clusters[i][j];
			Cluster* rightCluster = clusters[i + 1][j];

			int n = 0;
			int k;
			for (k = 0; k < CLUSTER_SCALE; ++k) {
				if (walkability[offset_x][offset_y + k] && walkability[offset_x + 1][offset_y + k]) {
					++n;
				}
				else {
					if (n > 0) {
						int mid = n > 1 ? k - (n / 2 + 1) : k - 1;
						Node* node_00 = leftCluster->addNode(offset_x, offset_y + mid);
						Node* node_10 = rightCluster->addNode(offset_x + 1, offset_y + mid);
						node_00->AddIntraAdjacentNode(node_10);
						node_10->AddIntraAdjacentNode(node_00);
						n = 0;
					}
				}
			}
			if (n != 0) {
				int mid = n > 1 ? k - (n / 2 + 1) : k - 1;
				Node* node_00 = leftCluster->addNode(offset_x, offset_y + mid);
				Node* node_10 = rightCluster->addNode(offset_x + 1, offset_y + mid);
				node_00->AddIntraAdjacentNode(node_10);
				node_10->AddIntraAdjacentNode(node_00);
			}
		}
	}
	// 가로 방향 entrance
	for (int i = 0; i < clusterWidthNumber; ++i) {
		for (int j = 0; j < clusterHeightNumber - 1; ++j) {
			int offset_x = i * CLUSTER_SCALE;
			int offset_y = j * CLUSTER_SCALE + CLUSTER_SCALE - 1;
			int start = -1;

			Cluster* topCluster = clusters[i][j];
			Cluster* bottomCluster = clusters[i][j + 1];

			int n = 0;
			int k;
			for (k = 0; k < CLUSTER_SCALE; ++k) {
				if (walkability[offset_x + k][offset_y] && walkability[offset_x + k][offset_y + 1]) {
					++n;
				}
				else {
					if (n > 0)
					{
						int mid = n > 1 ? k - (n / 2 + 1) : k - 1;
						Node* node_00 = topCluster->addNode(offset_x + mid, offset_y);
						Node* node_10 = bottomCluster->addNode(offset_x + mid, offset_y + 1);
						node_00->AddIntraAdjacentNode(node_10);
						node_10->AddIntraAdjacentNode(node_00);
						n = 0;
					}
				}
			}
			if (n != 0) {
				int mid = n > 1 ? k - (n / 2 + 1) : k - 1;
				Node* node_00 = topCluster->addNode(offset_x + mid, offset_y);
				Node* node_10 = bottomCluster->addNode(offset_x + mid, offset_y + 1);
				node_00->AddIntraAdjacentNode(node_10);
				node_10->AddIntraAdjacentNode(node_00);
			}
		}
	}
}

void HierarchicalPathfinder::CalcInterPath(ObTileMap& gameMap)
{
	vector<vector<Cluster*>>& clusters = gameMap.cluster;
	int interPathCount = 0;
	int entranceCount = 0;
	time_t startTime = time(nullptr);
	int maxEntranceNodeNumber = 0;

	for (auto& clusterRow : clusters) {
		for (Cluster*& cluster : clusterRow) {
			cluster->FindInterPathOfAllNodes();

			int n = cluster->nodes.size();
			if (n > maxEntranceNodeNumber) {
				maxEntranceNodeNumber = n;
			}
			entranceCount += n;
			interPathCount += ((n * (n - 1)) / 2);
		}
	}

	cout << "max entrance number: " << maxEntranceNodeNumber << std::endl;
	cout << "total_entrance: " << entranceCount << std::endl;
	cout << "total_inter_path: " << interPathCount << std::endl;

	time_t endTime = std::time(nullptr);
	cout << "inter path calc time: " << (endTime - startTime) << " seconds" << std::endl;
}

tuple<vector<Node*>, int> HierarchicalPathfinder::aStarAlgorithmOnNodeGraph(ObTileMap& gameMap, Node* startNode, Node* endNode)
{
	priority_queue<pair<int, Node*>, vector<pair<int, Node*>>, CompareNodes> openNodes;
	unordered_map<Node*, int> fromStartDistMap;
	unordered_map<Node*, Node*> parentMap;

	int dist = Node::ApproximateDist(startNode, endNode);
	fromStartDistMap[startNode] = 0;
	openNodes.push({ dist, startNode });

	while (!openNodes.empty()) {
		auto minNode = openNodes.top();
		openNodes.pop();

		int priority = minNode.first;
		Node* curNode = minNode.second;

		int destDist = Node::ApproximateDist(curNode, endNode);
		int fromStartDist = priority - destDist;

		//목표지점 도달 시
		if (curNode == endNode) {
			vector<Node*> path;
			Node* nNode = endNode;
			while (nNode != startNode) {
				path.push_back(nNode);
				Node* parent = parentMap[nNode];
				nNode = parent;
				//Node* parent = parentMap[nNode];
				//vector<INTPAIR> toParentPath = parent->neighbors[nNode].first;
				//path.insert(path.end(), toParentPath.rbegin(), toParentPath.rend());
				//nNode = parent;
			}
			return { path, fromStartDist };
		}
		for (pair<Node*, pair<vector<INTPAIR>, int>> neighbor : curNode->neighbors) {
			Node* neighborNode = neighbor.first;
			int dist = neighbor.second.second;
			int destDist = Node::ApproximateDist(neighborNode, endNode);
			int newFromStartDist = fromStartDist + dist;

			auto p = fromStartDistMap.find(neighborNode);

			if (p == fromStartDistMap.end() || p->second > newFromStartDist) {
				openNodes.push({ newFromStartDist + destDist, neighborNode });
				fromStartDistMap[neighborNode] = newFromStartDist;
				parentMap[neighborNode] = curNode;
			}
		}
	}
	return { vector<Node*>(), -1 };
}

vector<INTPAIR> HierarchicalPathfinder::FindPathInWalkTileGrid(ObTileMap& gameMap, int startWalkTileX, int startWalkTileY, int endWalkTileX, int endWalkTileY)
{
	auto t1 = std::chrono::high_resolution_clock::now();
	int startGridX = static_cast<int>(startWalkTileX / CLUSTER_SCALE);
	int startGridY = static_cast<int>(startWalkTileY / CLUSTER_SCALE);
	int endGridX = static_cast<int>(endWalkTileX / CLUSTER_SCALE);
	int endGridY = static_cast<int>(endWalkTileY / CLUSTER_SCALE);

	Cluster startCluster = *gameMap.cluster[startGridX][startGridY];
	Cluster endCluster = *gameMap.cluster[endGridX][endGridY];

	vector<INTPAIR> path;
	int cost;

	// case 1. 같은 Cluster 내에 있을 경우
	if (startGridX == endGridX && startGridY == endGridY) {
		tie(path, cost) = startCluster.FindInterPath(startWalkTileX, startWalkTileY, endWalkTileX, endWalkTileY);

		// 경로 존재하면 바로 리턴. 없을 경우 더 복잡한 탐색을 해야함.
		if (cost != -1) {
			auto t2 = std::chrono::high_resolution_clock::now();
			std::cout << "At same cluster! Path found at " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " microseconds" << std::endl;
			return path;
		}
	}
	// case 2. 다른 Cluster일 경우 

	// (1)임시로 start, end node 만들기
	// 임시 node 이므로 cluster의 nodes(node list)에 추가하지는 않을거임.

	Node startNode(startWalkTileX, startWalkTileY, &startCluster);
	Node endNode(endWalkTileX, endWalkTileY, &endCluster);


	// (2-1) start_node 와 start_cluster의 entrance node들과의 path 찾기
	// 임시 node 이므로 cluster의 nodes(node list)에 추가하지 않음

	for (Node*& n : startCluster.nodes) {
		tie(path, cost) = startCluster.FindInterPath(startWalkTileX, startWalkTileY, n->x, n->y);
		if (cost != -1) {
			// start_node에서 entrance node로 갈 뿐이므로
			// entrance node의 이웃으로 start_node를 추가하지는 않음.
			startNode.AddAdjacentNode(n, path, cost);
		}
	}

	// (2-2) end_node 와 end_cluster의 entrance node들과의 path 찾기

	for (Node*& n : endCluster.nodes) {
		tie(path, cost) = endCluster.FindInterPath(n->x, n->y, endWalkTileX, endWalkTileY);
		if (cost != -1) {
			// entrance node에서 end_node로 갈 뿐이므로
			// end_node의 이웃으로 entrance node를 추가하지는 않음.
			n->AddAdjacentNode(&endNode, path, cost);
		}
	}

	vector<Node*> nodePath;
	vector<INTPAIR> refinedPath;
	tie(nodePath, cost) = aStarAlgorithmOnNodeGraph(gameMap, &startNode, &endNode);

	if (!path.empty()) {
		clock_t t3 = clock();
		refinedPath = RefinePath(gameMap, &startNode, &endNode, nodePath);
		clock_t t4 = clock();
		double elapsed_time = (t4 - t3) / (double)CLOCKS_PER_SEC;
		std::cout << "refine based on distance: " << elapsed_time << " seconds" << std::endl;
	}

	// (3) start_node는 entrance node의 이웃으로 start_node를 추가하지는
	// 않았으므로 처리 안해도 됨. 반면 end_node는 없애 줘야함.

	for (Node*& n : endCluster.nodes) {
		n->neighbors.erase(&endNode); //정확하지 않은 번역 유의
	}

	auto t2 = chrono::high_resolution_clock::now();
	std::cout << "Path found at: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " microseconds" << std::endl;
	return refinedPath;
}

vector<INTPAIR> HierarchicalPathfinder::FindCompletePath(ObTileMap& gameMap, Vector2 start, Vector2 end)
{
	int cluster_width = CLUSTER_SCALE;
	int cluster_height = CLUSTER_SCALE;
	int m = 8 * IMGSCALE;

	int start_walk_tile_x = (int)start.x / m;
	int start_walk_tile_y = (int)start.y / m;
	int end_walk_tile_x = (int)end.x / m;
	int end_walk_tile_y = (int)end.y / m;

	vector<INTPAIR> path = FindPathInWalkTileGrid(gameMap, start_walk_tile_x, start_walk_tile_y, end_walk_tile_x, end_walk_tile_y);

	if (path.empty()) {
		return std::vector<std::pair<int, int>>();
	}
	else {
		for (size_t i = 0; i < path.size(); ++i) {
			int x = path[i].first * m + (m / 2);
			int y = path[i].second * m + (m / 2);
			path[i] = std::make_pair(x, y);
		}
		return path;
	}
}

