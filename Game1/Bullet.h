#pragma once
class Bullet : public ObRect
{
	float speed;
	Vector2 fireDir;
	ObImage bulletImg[7];

public:
	bool  active;
	Bullet();
	void Init(Vector2 FirePos, float rotation , int level);		//√ ±‚»≠
	void Update();
	void Render(class Camera* cam = app.maincam);
};



