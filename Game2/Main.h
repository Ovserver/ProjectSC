#pragma once


class Main : public Scene
{

public:
	Main();
	~Main();
	virtual void Init() override;
	virtual void Release() override; //����
	virtual void Update() override;
	virtual void LateUpdate() override;//����
	virtual void Render() override;
	virtual void ResizeScreen() override;
public:
	bool			BuildingMode = false;
	bool			showCursor = false;
	bool			showTileMap = true;
private:
	//Ÿ���� ���� �����ʹ� ���𰡴�
	Brave mainPlayer;
	Boss boss;
	ObTileMap map;
	ObTileMap* dynamicMap;
	//�÷��̾ ã�ư��ߵ� ���
	vector<Tile*> way;
	ObImage	cursor;
	ObImage cursorDrag;
	//�Ʒ����� ���� �ð����
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
