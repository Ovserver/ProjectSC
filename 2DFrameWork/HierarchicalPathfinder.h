#pragma once
class HierarchicalPathfinder : public Singleton<HierarchicalPathfinder>
{
public:
	void InitializeCluster(ObTileMap& gameMap);
	void CreateEntranceNodes(ObTileMap& gameMap);
	void CalcInterPath(ObTileMap& gameMap);

	struct CompareNodes {
		bool operator()(const pair<int, Node*>& a, const pair<int, Node*>& b)
		{
			return a.first > b.first; // 작은 값 우선
		}
	};
	// 노드 그래프를 이용한 A* 알고리즘.
	// PathFinder의 A* 알고리즘과 약간 다름.
	tuple<vector<Node*>, int> aStarAlgorithmOnNodeGraph(ObTileMap& gameMap, Node* startNode, Node* endNode);
	vector<INTPAIR> FindPathInWalkTileGrid(ObTileMap& gameMap, int startWalkTileX, int startWalkTileY, int endWalkTileX, int endWalkTileY);
	vector<INTPAIR> FindCompletePath(ObTileMap& gameMap, Vector2 start, Vector2 end);
	vector<INTPAIR> RefinePath(ObTileMap& gameMap, Node* startNode, Node* endNode, vector<Node*> nodePath)
	{
		int refine_distance = CONSTANT_DISTANCE;

		// Start point
		int distFromStartNode = 0;
		Node* currentNode1 = startNode;
		Node* nextNode1 = nullptr;
		int n = nodePath.size();
		int index1 = 0;

		// Find the nearest node that is at least refine_distance away from the starting point.
		while (distFromStartNode < refine_distance && currentNode1 != endNode) {
			nextNode1 = nodePath[n - 1 - index1];
			int dist_to_next_node = currentNode1->neighbors[nextNode1].second;
			distFromStartNode += dist_to_next_node;
			currentNode1 = nextNode1;
			index1 += 1;
		}
		index1 -= 1;

		// Case 1: If the distance between the starting and ending points is less than or equal to refine_distance,
		// just apply A*.
		if (currentNode1 == endNode) {
			vector<INTPAIR> intPair;
			int returnInt;
			tie(intPair, returnInt) = PathFinder::aStarPathFind(gameMap.walkableTiles, startNode->x, startNode->y, endNode->x, endNode->y);
			return intPair;
		}

		// End point
		int distFronEndNode = 0;
		Node* currentNode2 = endNode;
		Node* nextNode2 = nullptr;
		int index2 = 1;

		// Find the nearest node that is at least refine_distance away from the ending point.
		while (distFronEndNode < refine_distance && index2 < n) {
			nextNode2 = nodePath[index2];
			int dist_to_next_node = nextNode2->neighbors[currentNode2].second;
			distFronEndNode += dist_to_next_node;
			currentNode2 = nextNode2;
			index2 += 1;
		}
		index2 -= 1;

		// Case 2: If the sum of index1 and index2 is greater than or equal to n, apply A*.
		if (index1 + index2 >= n) {
			vector<INTPAIR> intPair;
			int returnInt;
			tie(intPair, returnInt) = PathFinder::aStarPathFind(gameMap.walkableTiles, startNode->x, startNode->y, endNode->x, endNode->y);
			return intPair;
		}

		// Case 3: If the path is long enough, apply A* from the start and end points and combine the middle path.
		vector<INTPAIR> path1;
		vector<INTPAIR> path2;

		// If the startNode and currentNode1 are neighbors, no need to apply A* again.
		if (currentNode1->cluster == startNode->cluster) {
			cout << "start node does not require A*" << endl;
			path1 = startNode->neighbors[currentNode1].first;
		}
		else {
			vector<INTPAIR> intPair;
			int returnInt;
			tie(intPair, returnInt) = PathFinder::aStarPathFind(gameMap.walkableTiles, startNode->x, startNode->y, endNode->x, endNode->y);
			path1 = intPair;
		}

		// If the currentNode2 and endNode are neighbors, no need to apply A* again.
		if (currentNode2->cluster == endNode->cluster) {
			cout << "end node does not require A*" << endl;
			path2 = currentNode2->neighbors[endNode].first;
		}
		else {
			vector<INTPAIR> intPair;
			int returnInt;
			tie(intPair, returnInt) = PathFinder::aStarPathFind(gameMap.walkableTiles, startNode->x, startNode->y, endNode->x, endNode->y);
			path2 = intPair;
		}

		// Final refined path
		vector<INTPAIR> refinedPath;
		refinedPath.insert(refinedPath.end(), path2.begin(), path2.end());

		Node* curNode = nodePath[index2];
		Node* nextNode = nullptr;
		for (int i = 0; i < n - index1 - index2 - 1; i++) {
			nextNode = nodePath[index2 + 1 + i];
			vector<INTPAIR> pathToNextNode = nextNode->neighbors[curNode].first;
			refinedPath.insert(refinedPath.end(), pathToNextNode.begin(), pathToNextNode.end());
			curNode = nextNode;
		}

		refinedPath.insert(refinedPath.end(), path1.begin(), path1.end());

		return refinedPath;
	}
};

