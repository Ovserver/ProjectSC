#pragma once


class Main : public Scene
{

public:
	Main();
	~Main();
	virtual void Init() override;
	virtual void Release() override; //해제
	virtual void Update() override;
	virtual void LateUpdate() override;//갱신
	virtual void Render() override;
	virtual void ResizeScreen() override;
public:
	bool			BuildingMode = false;
	bool			showCursor = false;
	bool			showTileMap = true;
private:
	//타입을 몰라도 포인터는 선언가능
	Brave mainPlayer;
	Boss boss;
	ObTileMap map;
	ObTileMap* dynamicMap;
	//플레이어가 찾아가야될 경로
	vector<Tile*> way;
	ObImage	cursor;
	ObImage cursorDrag;
	//아래부터 시작 시계방향
	ObImage cursorMoveScreen[8];

	Camera	cam2;
	ObRect  SelectArea;
	ObRect	SelectAreaCol;
	Vector2	startDragPos;
	ObImage	mapImage;
	ObRect	TestBox;
	ObImage	buildingImage;
	ObRect	buildingImageBox[12];
};
