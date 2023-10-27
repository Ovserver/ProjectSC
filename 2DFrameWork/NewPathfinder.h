#pragma once

class PathFinder
{
public:
	static int approximateDistXY(int x1, int y1, int x2, int y2);
	static tuple<vector<INTPAIR>, int> aStarPathFind(const std::vector<std::vector<bool>>& walkability, int start_x, int start_y, int end_x, int end_y);	
};
class Node
{
public:
	Node(int _x, int _y, Cluster* _cluster);
	//인접 노드를 추가
	void addAdjacentNode(Node* adjNode, const vector<INTPAIR>& path, int distance)
	{
		neighbors[adjNode] = make_pair(path, distance);
	}
	//다른 cluster에 속하는 인접 노드 추가
	void addIntraAdjacentNode(Node* adjNode)
	{
		vector<INTPAIR> path = { make_pair(adjNode->x, adjNode->y) };
		neighbors[adjNode] = make_pair(path, 10);
	}
	//free한 cluster내에서 노드를 이어줌
	void addDirectNode(Node* adjNode)
	{
		vector<INTPAIR> path = { make_pair(adjNode->x, adjNode->y) };
		neighbors[adjNode] = make_pair(path, approximateDist(this, adjNode));
	}
	//key값이 같을 경우, node간 비교를 실행
	bool operator<(const Node& other) const
	{
		return x > other.x;
	}
	//node간 간격 반환
	static int approximateDist(const Node* node1, const Node* node2)
	{
		return PathFinder::approximateDistXY(node1->x, node1->y, node2->x, node2->y);
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
	Node* addNode(int x, int y)
	{
		for (Node& node : nodes) {
			if (node.x == x && node.y == y) {
				return &node;
			}
		}
		Node newNode(x, y, this);
		nodes.push_back(newNode);
		return &nodes.back();
	}
	//모든 entrace_node의 path를 계산.
	void findInterPathOfAllNodes()
	{
		for (size_t i = 0; i < nodes.size(); ++i) {
			Node* startNode = &nodes[i];
			for (size_t j = 0; j < i; ++j) {
				Node* endNode = &nodes[j];
				vector<INTPAIR> path;
				int cost;
				tie(path, cost) = findInterPath(startNode->x, startNode->y, endNode->x, endNode->y);
				if (cost != -1) {
					startNode->addAdjacentNode(endNode, path, cost);

					//path 거꾸로 해서 end_node에 start_node를 이웃으로 추가.
					std::vector<INTPAIR> path2;
					for (int k = static_cast<int>(path.size()) - 1; k >= 0; --k) {
						path2.push_back(make_pair(path[k].first, path[k].second));
					}
					endNode->addAdjacentNode(startNode, path2, cost);
				}
			}
		}
	}
	//world(walk map 상에서) 좌표를 받아 local 좌표로 변환한뒤 기존의 a* 알고리즘 적용.
	//경로를 다시 world walk map상의 좌표로 변환 후 리턴. free 한경우 즉시 리턴.
	//std::vector<std::pair<int, int>> findInterPath(int world_start_x, int world_start_y, int world_end_x, int world_end_y)
	tuple<vector<INTPAIR>, int> findInterPath(int worldStartX, int worldStartY, int worldEndX, int worldEndY)
	{
		int x_offset = grid_x * CLUSTER_SCALE;
		int y_offset = grid_y * CLUSTER_SCALE;

		if (is_free) {
			return make_tuple(vector<INTPAIR> { { worldEndX, worldEndY }},
				PathFinder::approximateDistXY(worldStartX, worldStartY, worldEndX, worldEndY));
		}

		int local_start_x = worldStartX - x_offset;
		int local_start_y = worldStartY - y_offset;
		int local_end_x = worldEndX - x_offset;
		int local_end_y = worldEndY - y_offset;

		int cost;
		vector<INTPAIR> local_path;
		tie(local_path, cost) = PathFinder::aStarPathFind(walkability, local_start_x, local_start_y, local_end_x, local_end_y);

		if (local_path.empty()) {
			return make_tuple(local_path, cost);
		}

		vector<INTPAIR> world_path;
		for (const auto& local_coord : local_path) {
			int local_x = local_coord.first;
			int local_y = local_coord.second;
			world_path.push_back({ local_x + x_offset, local_y + y_offset });
		}

		return make_tuple(world_path, cost);
	}
public:
	int grid_x;
	int grid_y;
	vector<Node> nodes;
	vector<vector<bool>> walkability;
	bool is_free;
};


