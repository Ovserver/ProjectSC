#include "stdafx.h"
#include "Bullet.h"

Bullet::Bullet()
{
	active = false;
	SetPivot() = OFFSET_T;
	isFilled = false;
	SetScale().x = 60.0f;
	SetScale().y = 70.0f;

	for (int i = 0; i < 7; i++)
	{
		bulletImg[i].LoadFile(L"PlayeBullet.png");
		bulletImg[i].maxFrame.x = 4;
		bulletImg[i].SetScale().x = bulletImg[i].imageSize.x * 2.0f / 4.0f;
		bulletImg[i].SetScale().y = bulletImg[i].imageSize.y * 2.0f;
		bulletImg[i].SetPivot() = OFFSET_T;
		bulletImg[i].SetParentRT(*this);
		bulletImg[i].ChangeAnim(ANIMSTATE::LOOP, 0.1f);


	}
	bulletImg[0].SetLocalPosX(-30);
	
	bulletImg[1].SetLocalPosX(-20);
	bulletImg[2].SetLocalPosX(-10);

	bulletImg[4].SetLocalPosX(10);
	bulletImg[5].SetLocalPosX(20);
	bulletImg[6].SetLocalPosX(30);

	bulletImg[0].SetLocalPosY(-30+40);
	bulletImg[1].SetLocalPosY(-20+40);
	bulletImg[2].SetLocalPosY(-10+40);
	bulletImg[3].SetLocalPosY(0  +40);
	bulletImg[4].SetLocalPosY(-10+40);
	bulletImg[5].SetLocalPosY(-20+40);
	bulletImg[6].SetLocalPosY(-30+40);

}

void Bullet::Init(Vector2 FirePos, float rotation, int level)
{
	if (active)return;

	SetWorldPos(FirePos);
	fireDir.x = cos(rotation);
	fireDir.y = sin(rotation);
	speed = 100.0f;
	active = true;
	if (level == 1)
	{
		bulletImg[0].isVisible = false;
		bulletImg[1].isVisible = false;
		bulletImg[5].isVisible = false;
		bulletImg[6].isVisible = false;
	}
	if (level == 2)
	{
		bulletImg[0].isVisible = false;
		bulletImg[1].isVisible = true;
		bulletImg[5].isVisible = true;
		bulletImg[6].isVisible = false;
	}
	if (level == 3)
	{
		bulletImg[0].isVisible = true;
		bulletImg[1].isVisible = true;
		bulletImg[5].isVisible = true;
		bulletImg[6].isVisible = true;
	}
}

void Bullet::Update()
{
	if (not active)return;

	MoveWorldPos(fireDir * speed * DELTA);
}

void Bullet::Render(Camera* cam )
{
	if (not active)return;

	ObRect::Render(cam);
	for (int i = 0; i < 7; i++)
	{
		bulletImg[i].Render(cam);
	}
}
