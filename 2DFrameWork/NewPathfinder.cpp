#include "framework.h"

int PathFinder::approximateDistXY(int x1, int y1, int x2, int y2)
{
	int dx = abs(x1 - x2);
	int dy = abs(y1 - y2);
	int e1 = abs(dx - dy);
	int e2 = min(dx, dy);
	return e1 * 10 + e2 * 14;
}

tuple<vector<INTPAIR>, int> PathFinder::aStarPathFind(const std::vector<std::vector<bool>>& walkability, int start_x, int start_y, int end_x, int end_y)
{
	if (start_x == end_x && start_y == end_y)
	{
		cout << "start end point equal\n";
		return { vector<INTPAIR>(), 0 };
	}
	if (!walkability[end_x][end_y])
	{
		cout << "not walkable terrain\n";
		return { vector<INTPAIR>(), 0 };
	}
	std::vector<int> direction_x = { 0, 1, 1, 1, 0, -1, -1, -1 };
	std::vector<int> direction_y = { 1, 1, 0, -1, -1, -1, 0, 1 };
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

	int grid_width = walkability.size();
	int grid_height = walkability[0].size();

	std::vector<std::vector<int>> from_start_distance_map(grid_width, std::vector<int>(grid_height, 100000));
	std::vector<std::vector<int>> direction_map(grid_width, std::vector<int>(grid_height, 8));

	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> open_nodes;

	int start_end_dist = approximateDistXY(start_x, start_y, end_x, end_y);
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

			auto t2 = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
			std::cout << "Path found in " << duration.count() << " microseconds" << std::endl;
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

			int from_end_dist = approximateDistXY(nx, ny, end_x, end_y);
			int from_start_dist_new = from_start_dist + distance;

			if (from_start_dist_new < from_start_distance_map[nx][ny]) {
				from_start_distance_map[nx][ny] = from_start_dist_new;
				open_nodes.push({ from_end_dist + from_start_dist_new, nx * grid_height + ny });
				direction_map[nx][ny] = (direction + 4) % 8;
			}
		}
	}
	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
	std::cout << "No path found in " << duration.count() << " microseconds" << std::endl;
	return make_tuple(vector<INTPAIR>(), -1);

}

Node::Node(int _x, int _y, Cluster* _cluster)
{
	x = _x;
	y = _y;
	cluster = _cluster;
}
