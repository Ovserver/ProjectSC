#include "framework.h"

void HierarchicalPathfinder::initializeCluster(ObTileMap& gameMap)
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

void HierarchicalPathfinder::createEntranceNodes(ObTileMap& gameMap)
{
	int clusterWidthNumber = gameMap.tileSize.x / CLUSTER_SCALE;
	int clusterHeightNumber = gameMap.tileSize.y / CLUSTER_SCALE;

	const vector<vector<bool>>& walkability = gameMap.walkableTiles;
	vector<vector<Cluster*>>& clusters = gameMap.cluster;
	int entranceCount = 0;

	// ���� ���� entrance
	for (int i = 0; i < clusterWidthNumber - 1; ++i) {
		for (int j = 0; j < clusterHeightNumber; ++j) {
			int offset_x = i * CLUSTER_SCALE + CLUSTER_SCALE - 1;
			int offset_y = j * CLUSTER_SCALE;
			int start = -1;

			Cluster* leftCluster = clusters[i][j];
			Cluster* rightCluster = clusters[i + 1][j];

			for (int k = 0; k < CLUSTER_SCALE; ++k) {
				if (walkability[offset_x][offset_y + k] && walkability[offset_x + 1][offset_y + k]) {
					if (start == -1) {
						start = k;
					}
					if (k < CLUSTER_SCALE - 1) {
						continue;
					}
					// ������ĭ�� walkable�� ��� ��� ó������.
					if (start != -1) {
						int mid = (start + k - 1) / 2;

						Node* node_00 = leftCluster->addNode(offset_x, offset_y + mid);
						Node* node_10 = rightCluster->addNode(offset_x + 1, offset_y + mid);

						node_00->addIntraAdjacentNode(node_10);
						node_10->addIntraAdjacentNode(node_00);

						start = -1;
					}
				}
			}
		}
	}
	// ���� ���� entrance
	for (int i = 0; i < clusterWidthNumber; ++i) {
		for (int j = 0; j < clusterHeightNumber - 1; ++j) {
			int offset_x = i * CLUSTER_SCALE;
			int offset_y = j * CLUSTER_SCALE + CLUSTER_SCALE - 1;
			int start = -1;

			Cluster* topCluster = clusters[i][j];
			Cluster* bottomCluster = clusters[i][j + 1];

			for (int k = 0; k < CLUSTER_SCALE; ++k) {
				if (walkability[offset_x + k][offset_y] && walkability[offset_x + k][offset_y + 1]) {
					if (start == -1) {
						start = k;
					}
					if (k < CLUSTER_SCALE - 1) {
						continue;
					}
				}

				if (start != -1) {
					int mid = (start + k - 1) / 2;

					Node* node_00 = topCluster->addNode(offset_x + mid, offset_y);
					Node* node_10 = bottomCluster->addNode(offset_x + mid, offset_y + 1);

					node_00->addIntraAdjacentNode(node_10);
					node_10->addIntraAdjacentNode(node_00);
					start = -1;
				}
			}
		}
	}
}

void HierarchicalPathfinder::calcInterPath(ObTileMap& gameMap)
{
	vector<vector<Cluster*>>& clusters = gameMap.cluster;
	int interPathCount = 0;
	int entranceCount = 0;
	time_t startTime = time(nullptr);
	int maxEntranceNodeNumber = 0;

	for (auto& clusterRow : clusters) {
		for (Cluster*& cluster : clusterRow) {
			cluster->findInterPathOfAllNodes();

			int n = cluster->nodes.size();
			if (n > maxEntranceNodeNumber) {
				maxEntranceNodeNumber = n;
			}
			entranceCount += n;
			interPathCount += ((n * (n - 1)) / 2);
		}
	}

	std::cout << "max entrance number: " << maxEntranceNodeNumber << std::endl;
	std::cout << "total_entrance: " << entranceCount << std::endl;
	std::cout << "total_inter_path: " << interPathCount << std::endl;

	std::time_t endTime = std::time(nullptr);
	std::cout << "inter path calc time: " << (endTime - startTime) << " seconds" << std::endl;
}

tuple<vector<INTPAIR>, int> HierarchicalPathfinder::aStarAlgorithmOnNodeGraph(ObTileMap& gameMap, Node* startNode, Node* endNode)
{
	priority_queue<pair<int, Node*>> openNodes;
	unordered_map<Node*, int> fromStartDistMap;
	unordered_map<Node*, Node*> parentMap;

	int dist = Node::approximateDist(startNode, endNode);
	fromStartDistMap[startNode] = 0;
	openNodes.push({ dist, startNode });

	while (!openNodes.empty()) {
		auto minNode = openNodes.top();
		openNodes.pop();

		int priority = minNode.first;
		Node* curNode = minNode.second;

		int destDist = Node::approximateDist(curNode, endNode);
		int fromStartDist = priority - destDist;

		if (curNode == endNode) {
			std::vector<INTPAIR> path;
			Node* nNode = endNode;
			while (nNode != startNode) {
				Node* parent = parentMap[nNode];
				vector<INTPAIR> toParentPath = parent->neighbors[nNode].first;
				path.insert(path.end(), toParentPath.rbegin(), toParentPath.rend());
				nNode = parent;
			}
			return { path, fromStartDist };
		}

		for (auto neighbor : curNode->neighbors) {
			Node* neighborNode = neighbor.first;
			int dist = neighbor.second.second;
			int destDist = Node::approximateDist(neighborNode, endNode);
			int newFromStartDist = fromStartDist + dist;

			auto p = fromStartDistMap.find(neighborNode);

			if (p == fromStartDistMap.end() || p->second > newFromStartDist) {
				openNodes.push({ newFromStartDist + destDist, neighborNode });
				fromStartDistMap[neighborNode] = newFromStartDist;
				parentMap[neighborNode] = curNode;
			}
		}
	}
	return { vector<INTPAIR>(), -1 };
}

vector<INTPAIR> HierarchicalPathfinder::findPathInWalkTileGrid(ObTileMap& gameMap, int startWalkTileX, int startWalkTileY, int endWalkTileX, int endWalkTileY)
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

	// case 1. ���� Cluster ���� ���� ���
	if (startGridX == endGridX && startGridY == endGridY) {
		tie(path, cost) = startCluster.findInterPath(startWalkTileX, startWalkTileY, endWalkTileX, endWalkTileY);

		// ��� �����ϸ� �ٷ� ����. ���� ��� �� ������ Ž���� �ؾ���.
		if (cost != -1) {
			auto t2 = std::chrono::high_resolution_clock::now();
			std::cout << "At same cluster! Path found at " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " microseconds" << std::endl;
			return path;
		}
	}
	// case 2. �ٸ� Cluster�� ��� 

	// (1)�ӽ÷� start, end node �����
	// �ӽ� node �̹Ƿ� cluster�� nodes(node list)�� �߰������� ��������.
	Node startNode(startWalkTileX, startWalkTileY, &startCluster);
	Node endNode(endWalkTileX, endWalkTileY, &endCluster);


	// (2-1) start_node �� start_cluster�� entrance node����� path ã��
	// �ӽ� node �̹Ƿ� cluster�� nodes(node list)�� �߰����� ����
	for (Node& n : startCluster.nodes) {
		tie(path, cost) = startCluster.findInterPath(startWalkTileX, startWalkTileY, n.x, n.y);
		if (cost != -1) {
			// start_node���� entrance node�� �� ���̹Ƿ�
			// entrance node�� �̿����� start_node�� �߰������� ����.
			startNode.addAdjacentNode(&n, path, cost);
		}
	}

	// (2-2) end_node �� end_cluster�� entrance node����� path ã��
	for (Node& n : endCluster.nodes) {
		tie(path, cost) = endCluster.findInterPath(n.x, n.y, endWalkTileX, endWalkTileY);
		if (cost != -1) {
			// entrance node���� end_node�� �� ���̹Ƿ�
			// end_node�� �̿����� entrance node�� �߰������� ����.
			n.addAdjacentNode(&endNode, path, cost);
		}
	}

	tie(path, cost) = aStarAlgorithmOnNodeGraph(gameMap, &startNode, &endNode);

	// (3) start_node�� entrance node�� �̿����� start_node�� �߰�������
	// �ʾ����Ƿ� ó�� ���ص� ��. �ݸ� end_node�� ���� �����.
	for (Node& n : endCluster.nodes) {
		n.neighbors.erase(&endNode); //��Ȯ���� ���� ���� ����
	}

	auto t2 = chrono::high_resolution_clock::now();
	std::cout << "Path found at: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " microseconds" << std::endl;
	return path;
}

vector<INTPAIR> HierarchicalPathfinder::findCompletePath(ObTileMap& gameMap, Vector2 start, Vector2 end)
{
	int cluster_width = CLUSTER_SCALE;
	int cluster_height = CLUSTER_SCALE;
	int m = 8 * IMGSCALE;

	int start_walk_tile_x = (int)start.x / m;
	int start_walk_tile_y = (int)start.y / m;
	int end_walk_tile_x = (int)end.x / m;
	int end_walk_tile_y = (int)end.y / m;

	vector<INTPAIR> path = findPathInWalkTileGrid(gameMap, start_walk_tile_x, start_walk_tile_y, end_walk_tile_x, end_walk_tile_y);

	if (path.empty()) {
		return std::vector<std::pair<int, int>>();
	}
	else {
		for (size_t i = 0; i < path.size(); ++i) {
			int x = path[i].first * m;
			int y = path[i].second * m;
			path[i] = std::make_pair(x, y);
		}
		return path;
	}
}

