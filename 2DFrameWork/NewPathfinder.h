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
	//���� ��带 �߰�
	//��带 Ű�� ���, ��ο� �Ÿ��� ������ neighbor�� �߰�
	void AddAdjacentNode(Node* adjNode, const vector<INTPAIR>& path, int distance)
	{
		neighbors[adjNode] = make_pair(path, distance);
	}
	//�ٸ� cluster�� ���ϴ� ���� ��� �߰�
	void AddIntraAdjacentNode(Node* adjNode)
	{
		vector<INTPAIR> path = { make_pair(adjNode->x, adjNode->y) };
		neighbors[adjNode] = make_pair(path, 10);
	}
	//free�� cluster������ ��带 �̾���
	void AddDirectNode(Node* adjNode)
	{
		vector<INTPAIR> path = { make_pair(adjNode->x, adjNode->y) };
		neighbors[adjNode] = make_pair(path, ApproximateDist(this, adjNode));
	}
	//key���� ���� ���, node�� �񱳸� ����
	bool operator<(const Node& other) const { return x > other.x; }
	//node�� ���� ��ȯ
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
	//�̹� �����ϴ� node�� �� node�� ��ȯ, �ƴϸ� ���ο� node �߰�
	Node* addNode(int x, int y);

	//��� entrace_node�� path�� ���.
	void FindInterPathOfAllNodes();

	//world(walk map �󿡼�) ��ǥ�� �޾� local ��ǥ�� ��ȯ�ѵ� ������ a* �˰��� ����.
	//��θ� �ٽ� world walk map���� ��ǥ�� ��ȯ �� ����. free �Ѱ�� ��� ����.
	//std::vector<std::pair<int, int>> FindInterPath(int world_start_x, int world_start_y, int world_end_x, int world_end_y)
	tuple<vector<INTPAIR>, int> FindInterPath(int worldStartX, int worldStartY, int worldEndX, int worldEndY);

public:
	int grid_x;
	int grid_y;
	vector<Node*> nodes;
	vector<vector<bool>> walkability;
	bool is_free;
};


