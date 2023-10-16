#pragma once

#define TRAILMAX 12

//오른아래쪽부터 반시계방향
enum DirState
{
	LB,B,RB,
	R,RT,T,
	LT,L
};

enum class BraveState
{
	IDLE,
	WALK,
	ROLL
};

class Character
{
protected:
	ObRect			col;
	int				dirFrameY[8];
public:
	int  lookDir(Vector2 dir);
};

class Brave : public Character
{
public:
	bool			ModePathFind = true;
	bool			pathfinding = false;
	vector<Tile*>	pathWay;

private:
	BraveState		state;
	Vector2         keyDir;
	float			speed;
	ObImage			walk;
	ObImage			roll;

	ObImage			trail[TRAILMAX];

	float			trailTime;

public:
	Brave();
	void Update();
	void Render();
	Vector2 GetWorldPos() { return col.GetWorldPos(); }
	void SetWorldPos(Vector2 pos) { col.SetWorldPos(pos); }
	Vector2 GetFoot() { return col.GetWorldPos() + Vector2(0,10) + keyDir* 10.0f; }
	void InitPath(vector<Tile*> way);
	void StepBack();
};