#pragma once


class Main : public Scene
{
public:
	bool			showCursor = false;
	bool			showTileMap = true;
	vector<Unit*>	UnitPool;
	vector<Unit*>	UnitPoolSelect;
public:
	Main();
	~Main();
	virtual void Init() override;
	virtual void Release() override; //����
	virtual void Update() override;
	virtual void LateUpdate() override;//����
	virtual void Render() override;
	virtual void ResizeScreen() override;
private:
	//Ÿ���� ���� �����ʹ� ���𰡴�
	Brave mainPlayer;
	Boss boss;
	ObTileMap map;
	//�÷��̾ ã�ư��ߵ� ���
	vector<Tile*> way;
	ObImage	cursor;
	ObImage cursorDrag;
	ObImage cursorMoveScreen[8];
	ObImage	console;
	Camera	cam2;
	ObRect  SelectArea;
	ObRect	SelectAreaCol;
	Vector2	startDragPos;
	ObImage	mapImage;
	ObRect	TestBox;
};
