#pragma once

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
    int         F, G, H;//Ÿ�� ���
    Tile*       P;      //���� ���Ž�Ų Ÿ��
    bool        isFind; //�˻������� �ִ°�?


    Int2        idx;    //Ÿ�� �ε���
    int         state;
    Vector2     Pos;

    void ClearCost();         //��� �ʱ�ȭ
    void ClacCost(Int2 DestIdx); //H����ض�
};
struct compare
{
    bool operator()(Tile* a, Tile* b)
    {
        return a->F > b->F;
    }
};
enum TileState
{
    TILE_NONE,
    TILE_WALL,
    TILE_DOOR,
    TILE_TRAP,
    TILE_WATER,
    TILE_SAND,
    //TILE_SAND1,
    //TILE_SAND2,
    TILE_SIZE
};
class ObTileMap : public GameObject
{
protected:
    VertexTile*             vertices;
    ID3D11Buffer*           vertexBuffer;
public:
    Int2                    tileSize;   //10x10 , 4x4 ���μ��� Ÿ���� ��ΰ�
    ObImage*                tileImages[4]; //Ÿ�ϸ� �ȿ��� ���� �̹���4��
    string                  file;    //����� �̸�
    vector<vector<Tile>>    Tiles;
public:
    ObTileMap();
    virtual ~ObTileMap();
    void    Render(class Camera* cam = app.maincam) override;
    //                  Ÿ����ġ       �̹�����ǥ  ����̹���?
    void    SetTile(Int2 TileIdx, Int2 FrameIdx, int ImgIdx = 0,
        int TileState = TILE_NONE, Color color = Color(0.5f, 0.5f, 0.5f, 0.5f));
    virtual void        ResizeTile(Int2 TileSize);
    //                  ������ǥ�� Ÿ����ǥ�� ��ȯ�ϴ� �Լ�
    virtual bool        WorldPosToTileIdx(Vector2 WPos, Int2& TileIdx);


    void UpdateBuffer(); 
    void    Save();
    virtual void    Load();
    virtual void    CreateTileCost();
    //                   Ÿ���ε���
    int     GetTileState(Int2 TileIdx);
    int     GetTileState(Vector2 WorldPos);
    bool    PathFinding(Int2 sour, Int2 dest, OUT vector<Tile*>& way);

    bool    PathFinding(Vector2 sour, Vector2 dest, OUT vector<Tile*>& way);
};

