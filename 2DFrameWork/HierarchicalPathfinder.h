#pragma once
class HierarchicalPathfinder : public Singleton<HierarchicalPathfinder>
{
public:
	void initializeCluster(ObTileMap& gameMap);
	void createEntranceNodes(ObTileMap& gameMap);
	void calcInterPath(ObTileMap& gameMap);

	// ��� �׷����� �̿��� A* �˰���.
	// PathFinder�� A* �˰���� �ణ �ٸ�.
	tuple<vector<INTPAIR>, int> aStarAlgorithmOnNodeGraph(ObTileMap& gameMap, Node* startNode, Node* endNode);
	vector<INTPAIR> findPathInWalkTileGrid(ObTileMap& gameMap, int startWalkTileX, int startWalkTileY, int endWalkTileX, int endWalkTileY);
	vector<INTPAIR> findCompletePath(ObTileMap& gameMap, Vector2 start, Vector2 end);
};

