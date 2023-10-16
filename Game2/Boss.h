#pragma once

enum class BossState
{
	IDLE,
	LOOK,
	MOVE,
	ATTACK
};

class Boss : public Character
{
public:
	bool			activeFind = false;
	bool			ModePathFind = true;
	bool			pathfinding = false;
	vector<Tile*>	pathWay;
public:
	Boss();
	void Update(Vector2 plPos);
	void Render();
	void SetPlayerPointer(Brave* pl) { this->pl = pl; }
	void InitPath(vector<Tile*> way);
	Vector2 GetWorldPos() { return col.GetWorldPos(); }
	void SetWorldPos(Vector2 pos) { col.SetWorldPos(pos); }
private:
	float			chaseTime = 0;
	float			distance;
	float			speed;
	BossState		state;
	ObImage			img;
	ObCircle		range;
	Vector2         dir;

	//참조하여 만든다
	Brave* pl;
	//값복사하여 만든다
	//Vector3* pos;
};

