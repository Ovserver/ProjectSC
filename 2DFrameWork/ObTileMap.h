#pragma once
#define TILESETNUM 2
#define CLUSTER_SCALE 32

class Cluster;

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
	vector<vector<bool>>	walkableTiles;
	vector<vector<bool>>	buildingTiles;
	vector<vector<Cluster*>> cluster;
public:
	ObTileMap(int tileSizeX = 4 * 128, int tileSizeY = 4 * 128, Vector2 tileScale = Vector2(TILESCALE, TILESCALE) * IMGSCALE / 4.0f);
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
	//타일의 지형 정보 enum 갱신 (Load이후 수동 갱신)
	virtual void    CreateTileState();
	void	ClusterResize();
	//                   타일인덱스
	int     GetTileState(Int2 TileIdx);
	int     GetTileState(Vector2 WorldPos);
	bool	GetTileWalkable(Vector2 WorldPos);	
	TileCol GetTileCol(Vector2 WorldPos);
	void    SetTileCol(TileCol setTileCol);
	bool    SetTileCol(Vector2 WorldPos, TileCol tileCol);
	// 건물 건설 후, 충돌체 설정 후처리
	void	UpdateBuildingState(ObTileMap* dynamicMap, bool isActiveCol);

	bool    PathFinding(Int2 sour, Int2 dest, OUT vector<Tile*>& way);

	bool    PathFinding(Vector2 sour, Vector2 dest, OUT vector<Tile*>& way);
private:
	void	SetBuildingState(UINT x, UINT y, bool isActiveCol);
};

