#include "stdafx.h"
#include "Bullet.h"
#include "Player.h"

void Player::Init()
{

	SetPivot() = OFFSET_T;
	SetScale().x = 70.0f;
	SetScale().y = 70.0f;
	isFilled = false;

	LR.LoadFile(L"LeftRight.png");
	LR.maxFrame.x = 7;
	LR.SetScale().x = LR.imageSize.x * 2.0f / 7.0f;
	LR.SetScale().y = LR.imageSize.y * 2.0f;
	LR.SetPivot() = OFFSET_T;
	LR.SetParentRT(*this);
	LR.frame.x = 4;

	Boost.LoadFile(L"Start.png");
	Boost.maxFrame.x = 11;
	Boost.SetScale().x = Boost.imageSize.x * 2.0f / 11.0f;
	Boost.SetScale().y = Boost.imageSize.y * 2.0f;
	Boost.SetPivot() = OFFSET_T;
	Boost.isVisible = false;
	Boost.SetParentRT(*this);

	state = PlayerState::NONE;
	speed = 200.0f;

	horizon = 0.0f;
}

void Player::Update()
{
	if (INPUT->KeyDown('Z'))
	{
		for (int i = 0; i < 30; i++)
		{
			if (not bullet[i].active)
			{
				bullet[i].Init(GetWorldPos(), 90 * ToRadian , 3);
				break;
			}
		}
	}


	if (INPUT->KeyPress(VK_LEFT))
	{
		MoveWorldPos(LEFT * speed * DELTA);
		horizon -= DELTA * 2.0f;
		if (horizon < -1.0f)horizon = -1.0f;
	}
	else if (INPUT->KeyPress(VK_RIGHT))
	{
		MoveWorldPos(RIGHT * speed * DELTA);

		horizon += DELTA * 2.0f;
		if (horizon > 1.0f)horizon = 1.0f;
	}
	else
	{
		//0이 되도록 감산, 가산
		if (horizon < 0)
		{
			horizon += DELTA * 2.0f;
		}
		else if (horizon > 0)
		{
			horizon -= DELTA * 2.0f;
		}

	}

	if (INPUT->KeyPress(VK_UP))
	{
		MoveWorldPos(UP * speed * DELTA);
	}
	else if (INPUT->KeyPress(VK_DOWN))
	{
		MoveWorldPos(DOWN * speed * DELTA);
	}


	if (state == PlayerState::NONE)
	{
		
		//(int)    =  float
		LR.frame.x = round(horizon * 3.0f + 3.0f);


		//none -> boost
		if (INPUT->KeyPress(VK_SPACE))
		{
			state = PlayerState::BOOST;
			app.deltaScale = 2.0f;
			Boost.isVisible = true;
			LR.isVisible = false;
			Boost.ChangeAnim(ANIMSTATE::LOOP, 0.1f);
		}
	}
	else if (state == PlayerState::BOOST)
	{

		//boost -> none
		if (INPUT->KeyUp(VK_SPACE))
		{
			state = PlayerState::NONE;
			app.deltaScale = 1.0f;
			Boost.isVisible = false;
			LR.isVisible = true;
			LR.frame.x = 3;
		}

	}
	


	for (int i = 0; i < 30; i++)
	{
		bullet[i].Update();
	}

}

void Player::Render(Camera* cam)
{
	ObRect::Render(cam);
	Boost.Render(cam);
	LR.Render(cam);
	for (int i = 0; i < 30; i++)
	{
		bullet[i].Render(cam);
	}
}
