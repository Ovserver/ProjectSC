#include "stdafx.h"
#include "Brave.h"
#include "Boss.h"

Boss::Boss()
{
	//col.SetPivot() = OFFSET_B;
	col.isFilled = false;
	//range.SetPivot() = OFFSET_B;
	range.isFilled = false;
	range.SetParentRT(col);

	img.LoadFile(L"boss.bmp");
	//img.SetPivot() = OFFSET_B;
	img.maxFrame.y = 8;
	img.SetParentRT(col);
	img.SetScale().x = img.imageSize.x * 1.0f;
	img.SetScale().y = img.imageSize.y / 8.0f * 1.0f;
	col.SetScale() = img.GetScale();

	state = BossState::IDLE;

	dirFrameY[LB] = 6;
	dirFrameY[B] = 5;
	dirFrameY[RB] = 2;
	dirFrameY[R] = 0;
	dirFrameY[RT] = 7;
	dirFrameY[T] = 4;
	dirFrameY[LT] = 3;
	dirFrameY[L] = 1;

	speed = 100.0f;
}

void Boss::Update(Vector2 plPos)
{
	if (ModePathFind)
	{
		Vector2 playerDir = plPos - col.GetWorldPos();
		float pDistance = playerDir.Length();
		if (pDistance > 400.0f)
		{
			activeFind = false;
			if (TIMER->GetTick(chaseTime, 3.0f))
			{
				pathWay.clear();
				pathfinding = false;
			}
		}
		else
		{
			chaseTime = 0;
			activeFind = true;
		}
		if (pathfinding)
		{
			if (pathWay.empty())
			{
				pathfinding = false;
				pathWay.clear();
				img.ChangeAnim(ANIMSTATE::STOP, 0.1f);
				img.frame.x = 0;
				return;
			}
			dir = pathWay.back()->Pos - col.GetWorldPos();
			if (dir.Length() > 0.1f)
			{
				dir.Normalize();
				col.MoveWorldPos(dir * speed * DELTA);
				lookDir(dir);
			}
			else
			{
				col.SetWorldPos(pathWay.back()->Pos);
				pathWay.pop_back();
			}
		}

		if (state == BossState::IDLE)
		{
		}
		else if (state == BossState::MOVE)
		{
			img.frame.y = lookDir(dir);
			if (pDistance < 51.0f)
			{
				state = BossState::ATTACK;
			}
		}
		else if (state == BossState::ATTACK)
		{
			img.frame.y = lookDir(dir);
			static int stretch = 1; //0:가로로 늘어남 1:세로로 늘어남

			if (stretch == 0)
			{
				img.SetScale().x += 200.0f * DELTA;
				img.SetScale().y -= 200.0f * DELTA;
				if (img.SetScale().x > 200.0f)
				{
					stretch = 1;
				}
			}
			else
			{
				img.SetScale().x -= 200.0f * DELTA;
				img.SetScale().y += 200.0f * DELTA;
				if (img.SetScale().y > 200.0f)
				{
					stretch = 0;
				}
			}
			//attack -> move
			if (pDistance > 51.0f)
			{
				state = BossState::MOVE;
				stretch = 0;
				img.SetScale().x = img.imageSize.x * 1.0f;
				img.SetScale().y = img.imageSize.y / 8.0f * 1.0f;
			}
		}
	}
	else 
	{
		dir = plPos - col.GetWorldPos();
		distance = dir.Length();
		dir.Normalize();
		if (state == BossState::IDLE)
		{

			//idle -> look
			if (distance < 400.0f)
			{
				state = BossState::LOOK;
			}
		}
		else if (state == BossState::LOOK)
		{

			img.frame.y = lookDir(dir);


			//look -> move
			if (distance < 200.0f)
			{
				state = BossState::MOVE;
			}
			//Look -> Idle
			if (distance > 400.0f)
			{
				state = BossState::MOVE;
			}
		}
		else if (state == BossState::MOVE)
		{
			img.frame.y = lookDir(dir);
			col.MoveWorldPos(dir * 100.0f * DELTA);

			//move -> attack
			if (distance < 100.0f)
			{
				state = BossState::ATTACK;
			}
			//move -> Look
			if (distance > 200.0f)
			{
				state = BossState::LOOK;
			}
		}
		else if (state == BossState::ATTACK)
		{
			static int stretch = 1; //0:가로로 늘어남 1:세로로 늘어남

			if (stretch == 0)
			{
				img.SetScale().x += 200.0f * DELTA;
				img.SetScale().y -= 200.0f * DELTA;
				if (img.SetScale().x > 200.0f)
				{
					stretch = 1;
				}
			}
			else
			{
				img.SetScale().x -= 200.0f * DELTA;
				img.SetScale().y += 200.0f * DELTA;
				if (img.SetScale().y > 200.0f)
				{
					stretch = 0;
				}
			}


			//attack -> move
			if (distance > 100.0f)
			{
				state = BossState::MOVE;
				stretch = 0;
				img.SetScale().x = img.imageSize.x * 1.0f;
				img.SetScale().y = img.imageSize.y / 8.0f * 1.0f;
			}
		}
	}
}

void Boss::Render()
{
	col.Render();
	img.Render();

	//거리400이내 주시
	range.SetScale() = Vector2(800, 800);
	range.Render();

	//거리 200이내 따라가기
	range.SetScale() = Vector2(400, 400);
	range.Render();

	//거리 100이내 공격
	range.SetScale() = Vector2(200, 200);
	range.Render();
}

void Boss::InitPath(vector<Tile*> way)
{
	pathWay = way;
	pathfinding = true;

	state = BossState::MOVE;
	img.ChangeAnim(ANIMSTATE::LOOP, 0.1f);
}
