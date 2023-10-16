#pragma once

enum PlayerState
{
	NONE,
	BOOST,
	BOMB
};

struct Player : public ObRect
{
	Bullet	bullet[30];
	PlayerState state;
	ObImage LR;
	ObImage Boost;
	float   speed;
	float   horizon;
public:
	void Init();
	void Update();
	void Render(class Camera* cam = app.maincam);
};
