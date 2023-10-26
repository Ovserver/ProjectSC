#pragma once
#define TILESETNUM 2
#define CLUSTER_SCALE 32
enum TileState
{
	TILE_NONE,
	TILE_WALL,
	TILE_DOOR,
	TILE_TRAP,
	TILE_WATER,
	TILE_SAND,
	TILE_SIZE
};
enum class TileCol
{
	NONE, UNIT, BUILDING, NEUTRAL
};
struct TileInt2
{
	Int2    idx;
	int     cost;
	TileInt2(Int2 _int2, int _cost)
	{
		idx = _int2; cost = _cost;
	}
};
class PathFinder
{
public:
public:
	static int approximateDistXY(int x1, int y1, int x2, int y2)
	{
		int dx = abs(x1 - x2);
		int dy = abs(y1 - y2);
		int e1 = abs(dx - dy);
		int e2 = min(dx, dy);
		return e1 * 10 + e2 * 14;
	}
	static vector<INTPAIR> aStarPathFind(const std::vector<std::vector<bool>>& walkability, int start_x, int start_y, int end_x, int end_y)
	{
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
				return path;
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
		return std::vector<std::pair<int, int>>();
	}
};
class Node
{
public:
	Node(int _x, int _y, Cluster _cluster) : x(_x), y(_y), cluster(_cluster) {}
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
	static int approximateDist(const Node* node1,const Node* node2)
	{
		return PathFinder::approximateDistXY(node1->x, node1->y, node2->x, node2->y);
	}
public:
	int x;
	int y;
	Cluster cluster;
	map<Node*, pair<vector<INTPAIR>, int>> neighbors;
};

class Cluster
{
public:
	int grid_x;
	int grid_y;
	vector<Node> nodes;
	vector<vector<bool>> walkability;
	bool is_free;

	Cluster(int grid_x, int grid_y) : grid_x(grid_x), grid_y(grid_y), is_free(false)
	{
		bool flag = false;
		for (int i = 0; i < CLUSTER_SCALE; ++i) {
			vector<bool> tempVec;
			walkability.push_back(tempVec);
			for (int j = 0; j < CLUSTER_SCALE; ++j) {
				// 이곳에 이동 가능 지역을 판별할 조건문을 주가하기
				bool temp = true;
				if (temp) flag = true;
				walkability.front().push_back(temp);
			}
		}
		//모든 칸이 walkable이면 false
		if (flag) is_free = false; else is_free = true;
	}
	//이미 존재하는 node면 그 node를 반환, 아니면 새로운 node 추가
	Node* addNode(int x, int y)
	{
		for (Node& node : nodes) {
			if (node.x == x && node.y == y) {
				return &node;
			}
		}
		Node newNode(10, 10, *this);
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
	//tuple<vector<INTPAIR>, int> findInterPath(int worldStartX, int worldStartY, int worldEndX, int worldEndY)
	std::vector<std::pair<int, int>> findInterPath(int world_start_x, int world_start_y, int world_end_x, int world_end_y)
	{
		int x_offset = grid_x * cluster_width;
		int y_offset = grid_y * cluster_height;

		if (is_free) {
			return { {world_end_x, world_end_y}, approximateDistXY(world_start_x, world_start_y, world_end_x, world_end_y) };
		}

		int local_start_x = world_start_x - x_offset;
		int local_start_y = world_start_y - y_offset;
		int local_end_x = world_end_x - x_offset;
		int local_end_y = world_end_y - y_offset;

		auto local_path = aStarPathFind(walkability, local_start_x, local_start_y, local_end_x, local_end_y);

		if (local_path.empty()) {
			return local_path;
		}

		std::vector<std::pair<int, int>> world_path;
		for (const auto& local_coord : local_path) {
			int local_x = local_coord.first;
			int local_y = local_coord.second;
			world_path.push_back({ local_x + x_offset, local_y + y_offset });
		}

		return world_path;
	}

	// Define approximateDistXY, aStarPathFind, and Node classes.
};

class Tile
{
public:
	int         F, G, H;//타일 비용
	Tile* P;      //나를 갱신시킨 타일
	bool        isFind; //검사한적이 있는가?

	TileCol     tileColInfo = TileCol::NONE;
	Int2        idx;    //타일 인덱스
	int         state;
	Vector2     Pos;

	void ClearCost();         //비용 초기화
	void CalcCost(Int2 DestIdx); //H계산해라
};
struct compare
{
	bool operator()(Tile* a, Tile* b)
	{
		return a->F > b->F;
	}
};

class ObTileMap : public GameObject
{
protected:
	VertexTile* vertices;
	ID3D11Buffer* vertexBuffer;
public:
	Int2                    tileSize;   //10x10 , 4x4 가로세로 타일이 몇개인가
	ObImage* tileImages[TILESETNUM]; //타일맵 안에서 사용될 이미지4장
	string                  file;    //저장될 이름
	vector<vector<Tile>>    Tiles;
public:
	ObTileMap();
	virtual ~ObTileMap();
	void    Render(class Camera* cam = app.maincam) override;
	//                  타일위치       이미지좌표  몇번이미지?
	void    SetTile(Int2 TileIdx, Int2 FrameIdx, int ImgIdx = 0,
		int TileState = TILE_NONE, Color color = Color(0.5f, 0.5f, 0.5f, 0.5f));
	virtual void        ResizeTile(Int2 TileSize);
	//                  월드좌표를 타일좌표로 변환하는 함수
	virtual bool        WorldPosToTileIdx(Vector2 WPos, Int2& TileIdx);


	void    UpdateBuffer();
	void    Save();
	virtual void    Load();
	virtual void    CreateTileCost();
	//                   타일인덱스
	int     GetTileState(Int2 TileIdx);
	int     GetTileState(Vector2 WorldPos);
	TileCol GetTileCol(Vector2 WorldPos);
	void    SetTileCol(TileCol setTileCol);
	bool    SetTileCol(Vector2 WorldPos, TileCol tileCol);

	bool    PathFinding(Int2 sour, Int2 dest, OUT vector<Tile*>& way);

	bool    PathFinding(Vector2 sour, Vector2 dest, OUT vector<Tile*>& way);
};

