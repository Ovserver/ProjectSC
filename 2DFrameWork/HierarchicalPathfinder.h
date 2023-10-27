#pragma once
class HierarchicalPathfinder : public Singleton<HierarchicalPathfinder>
{
public:
	void initializeCluster(ObTileMap& gameMap);
	void createEntranceNodes(ObTileMap& gameMap);
	void calcInterPath(ObTileMap& gameMap);

	// 노드 그래프를 이용한 A* 알고리즘.
	// PathFinder의 A* 알고리즘과 약간 다름.
	tuple<vector<INTPAIR>, int> aStarAlgorithmOnNodeGraph(ObTileMap& gameMap, Node* startNode, Node* endNode);
	vector<INTPAIR> findPathInWalkTileGrid(ObTileMap& gameMap, int startWalkTileX, int startWalkTileY, int endWalkTileX, int endWalkTileY);
	vector<INTPAIR> findCompletePath(ObTileMap& gameMap, Vector2 start, Vector2 end);
};

