#pragma once


class Main : public Scene
{

private:
	//Ÿ���� ���� �����ʹ� ���𰡴�
	Brave mainPlayer;
	Boss boss;
	ObTileMap map;
	//�÷��̾ ã�ư��ߵ� ���
	vector<Tile*> way;
	ObImage	cursor;
	ObImage	console;
	Camera	cam2;
	ObRect  SelectArea;
	Vector2	startDragPos;
public:
	Main();
	~Main();
	virtual void Init() override;
	virtual void Release() override; //����
	virtual void Update() override;
	virtual void LateUpdate() override;//����
	virtual void Render() override;
	virtual void ResizeScreen() override;
};
