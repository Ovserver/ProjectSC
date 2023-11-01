#pragma once

class PathFinder
{
public:
	static int ApproximateDistXY(int x1, int y1, int x2, int y2);
	static tuple<vector<INTPAIR>, int> aStarPathFind(const std::vector<std::vector<bool>>& walkability, int start_x, int start_y, int end_x, int end_y);
};
class Node
{
public:
	Node(int _x, int _y, Cluster* _cluster);
	//인접 노드를 추가
	//노드를 키로 삼고, 경로와 거리를 값으로 neighbor에 추가
	void AddAdjacentNode(Node* adjNode, const vector<INTPAIR>& path, int distance)
	{
		neighbors[adjNode] = make_pair(path, distance);
	}
	//다른 cluster에 속하는 인접 노드 추가
	void AddIntraAdjacentNode(Node* adjNode)
	{
		vector<INTPAIR> path = { make_pair(adjNode->x, adjNode->y) };
		neighbors[adjNode] = make_pair(path, 10);
	}
	//free한 cluster내에서 노드를 이어줌
	void AddDirectNode(Node* adjNode)
	{
		vector<INTPAIR> path = { make_pair(adjNode->x, adjNode->y) };
		neighbors[adjNode] = make_pair(path, ApproximateDist(this, adjNode));
	}
	//key값이 같을 경우, node간 비교를 실행
	bool operator<(const Node& other) const { return x > other.x; }
	//node간 간격 반환
	static int ApproximateDist(const Node* node1, const Node* node2)
	{
		return PathFinder::ApproximateDistXY(node1->x, node1->y, node2->x, node2->y);
	}
public:
	int x;
	int y;
	Cluster* cluster;
	map<Node*, pair<vector<INTPAIR>, int>> neighbors;
};

class Cluster
{
public:
	Cluster(int grid_x, int grid_y) : grid_x(grid_x), grid_y(grid_y), is_free(false)
	{
		for (int i = 0; i < CLUSTER_SCALE; ++i) {
			vector<bool> tempVec;
			walkability.push_back(tempVec);
			for (int j = 0; j < CLUSTER_SCALE; ++j) {

				walkability.back().push_back(true);
			}
		}
		is_free = false;
	}
	//이미 존재하는 node면 그 node를 반환, 아니면 새로운 node 추가
	Node* addNode(int x, int y);

	//모든 entrace_node의 path를 계산.
	void FindInterPathOfAllNodes();

	//world(walk map 상에서) 좌표를 받아 local 좌표로 변환한뒤 기존의 a* 알고리즘 적용.
	//경로를 다시 world walk map상의 좌표로 변환 후 리턴. free 한경우 즉시 리턴.
	//std::vector<std::pair<int, int>> FindInterPath(int world_start_x, int world_start_y, int world_end_x, int world_end_y)
	tuple<vector<INTPAIR>, int> FindInterPath(int worldStartX, int worldStartY, int worldEndX, int worldEndY);

public:
	int grid_x;
	int grid_y;
	vector<Node*> nodes;
	vector<vector<bool>> walkability;
	bool is_free;
};


