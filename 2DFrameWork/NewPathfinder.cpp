#include "framework.h"

int PathFinder::ApproximateDistXY(int x1, int y1, int x2, int y2)
{
	int dx = abs(x1 - x2);
	int dy = abs(y1 - y2);
	int e1 = abs(dx - dy);
	int e2 = min(dx, dy);
	return e1 * 10 + e2 * 14;
}

tuple<vector<INTPAIR>, int> PathFinder::aStarPathFind(const vector<vector<bool>>& walkability, int start_x, int start_y, int end_x, int end_y, bool pathCorrection)
{
	if (start_x == end_x && start_y == end_y)
	{
		cout << "start end point equal\n";
		return { vector<INTPAIR>(), -1 };
	}
	if (!walkability[end_x][end_y])
	{
		cout << "not walkable terrain\n";
		return { vector<INTPAIR>(), -1 };
	}
	vector<int> direction_x = { 0, 1, 1, 1, 0, -1, -1, -1 };
	vector<int> direction_y = { 1, 1, 0, -1, -1, -1, 0, 1 };
	chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();

	int grid_width = walkability.size();
	int grid_height = walkability[0].size();

	vector<vector<int>> from_start_distance_map(grid_width, vector<int>(grid_height, 100000));
	vector<vector<int>> direction_map(grid_width, vector<int>(grid_height, 8));

	priority_queue<INTPAIR, vector<INTPAIR>, greater<INTPAIR>> open_nodes;

	int start_end_dist = ApproximateDistXY(start_x, start_y, end_x, end_y);
	open_nodes.push({ start_end_dist, start_x * grid_height + start_y });
	from_start_distance_map[start_x][start_y] = 0;

	while (!open_nodes.empty()) {
		auto min_node = open_nodes.top();
		open_nodes.pop();

		int x = min_node.second / grid_height;
		int y = min_node.second % grid_height;

		int from_start_dist = from_start_distance_map[x][y];

		if (x == end_x && y == end_y) {
			int nx = end_x;
			int ny = end_y;
			int current_direction = 8;
			std::vector<std::pair<int, int>> path;
			while (!(nx == start_x && ny == start_y)) {
				int next_direction = direction_map[nx][ny];
				if (current_direction != next_direction) {
					path.emplace_back(nx, ny);
				}
				nx += direction_x[next_direction];
				ny += direction_y[next_direction];
				current_direction = next_direction;
			}

			auto t2 = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1);
			cout << "Path found in " << duration.count() << " microseconds" << endl;
			return make_tuple(path, from_start_dist);
		}

		for (int direction = 0; direction < 8; ++direction) {
			int nx = x + direction_x[direction];
			int ny = y + direction_y[direction];

			if (nx < 0 || nx >= grid_width || ny < 0 || ny >= grid_height) {
				continue;
			}

			if (!walkability[nx][ny]) {
				continue;
			}

			int distance = 10;

			if (direction % 2 == 1) {
				distance += 4;
				if (!walkability[x][ny] || !walkability[nx][y]) {
					continue;
				}
			}

			int from_end_dist = ApproximateDistXY(nx, ny, end_x, end_y);
			int from_start_dist_new = from_start_dist + distance;

			if (from_start_dist_new < from_start_distance_map[nx][ny]) {
				from_start_distance_map[nx][ny] = from_start_dist_new;
				open_nodes.push({ from_end_dist + from_start_dist_new, nx * grid_height + ny });
				direction_map[nx][ny] = (direction + 4) % 8;
			}
		}
	}
	auto t2 = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1);
	cout << "No path found in " << duration.count() << " microseconds" << endl;
	return make_tuple(vector<INTPAIR>(), -1);
}

Node::Node(int _x, int _y, Cluster* _cluster)
{
	x = _x;
	y = _y;
	cluster = _cluster;
}

Node* Cluster::addNode(int x, int y)
{
	for (Node*& node : nodes) {
		if (node->x == x && node->y == y) {
			return node;
		}
	}
	Node* newNode = new Node(x, y, this);
	nodes.push_back(newNode);
	return nodes.back();
}

void Cluster::FindInterPathOfAllNodes()
{
	for (size_t i = 0; i < nodes.size(); ++i) {
		Node* startNode = nodes[i];
		for (size_t j = 0; j < i; ++j) {
			Node* endNode = nodes[j];
			vector<INTPAIR> path;
			int cost;
			tie(path, cost) = FindInterPath(startNode->x, startNode->y, endNode->x, endNode->y);
			if (cost != -1) {
				startNode->AddAdjacentNode(endNode, path, cost);

				//path 거꾸로 해서 end_node에 start_node를 이웃으로 추가.

				tie(path, cost) = FindInterPath(endNode->x, endNode->y, startNode->x, startNode->y);
				endNode->AddAdjacentNode(startNode, path, cost);
			}
		}
	}
}

tuple<vector<INTPAIR>, int> Cluster::FindInterPath(int worldStartX, int worldStartY, int worldEndX, int worldEndY, bool pathCorrection)
{
	int x_offset = grid_x * CLUSTER_SCALE;
	int y_offset = grid_y * CLUSTER_SCALE;

	if (is_free) {
		return make_tuple(vector<INTPAIR> { { worldEndX, worldEndY }},
			PathFinder::ApproximateDistXY(worldStartX, worldStartY, worldEndX, worldEndY));
	}

	int local_start_x = worldStartX - x_offset;
	int local_start_y = worldStartY - y_offset;
	int local_end_x = worldEndX - x_offset;
	int local_end_y = worldEndY - y_offset;

	int cost;
	vector<INTPAIR> local_path;
	tie(local_path, cost) = PathFinder::aStarPathFind(walkability, local_start_x, local_start_y, local_end_x, local_end_y, pathCorrection);

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
