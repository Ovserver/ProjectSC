#include "stdafx.h"
#include "Utility2.h"
#include "Brave.h"

Brave::Brave()

{
	col.SetPivot() = OFFSET_B;
	col.isFilled = false;

	walk.LoadFile(L"player_walk.png");
	walk.SetPivot() = OFFSET_B;
	walk.maxFrame.x = 6;
	walk.maxFrame.y = 8;
	walk.SetParentRT(col);
	walk.SetScale().x = walk.imageSize.x / 6.0f * 4.0f;
	walk.SetScale().y = walk.imageSize.y / 8.0f * 4.0f;
	col.SetScale() = walk.GetScale();




	roll.LoadFile(L"player_roll.png");
	roll.SetPivot() = OFFSET_B;
	roll.maxFrame.x = 6;
	roll.maxFrame.y = 8;
	roll.SetParentRT(col);
	roll.SetScale().x = roll.imageSize.x / 6.0f * 4.0f;
	roll.SetScale().y = roll.imageSize.y / 8.0f * 4.0f;
	roll.isVisible = false;
	state = BraveState::IDLE;

	for (int i = 0; i < TRAILMAX; i++)
	{
		trail[i].LoadFile(L"player_roll.png");
		trail[i].SetPivot() = OFFSET_B;
		trail[i].maxFrame.x = 6;
		trail[i].maxFrame.y = 8;
		trail[i].SetScale().x = roll.imageSize.x / 6.0f * 4.0f;
		trail[i].SetScale().y = roll.imageSize.y / 8.0f * 4.0f;
		trail[i].isVisible = false;
	}


	speed = 200.0f;
	trailTime = 0.0f;
}
int contain = 0;
Vector2 dir;
void Brave::Update()
{
	ImGui::ColorEdit3("color", (float*)&walk.color);
	if (ModePathFind)
	{
		if (pathfinding)
		{
			if (pathWay.empty())
			{
				pathfinding = false;
				state = BraveState::IDLE;
				walk.ChangeAnim(ANIMSTATE::STOP, 0.1f);
				walk.frame.x = 0;
				return;
			}
			dir = pathWay.back()->Pos - col.GetWorldPos();
			if (dir.Length() > speed * DELTA)
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
		if (state == BraveState::IDLE)
		{
			walk.frame.y = lookDir(dir);
		}
		else if (state == BraveState::WALK)
		{
			walk.frame.y = lookDir(dir);
		}
	}
	else
	{
		for (int i = 0; i < TRAILMAX; i++)
		{
			if (trail[i].isVisible)
			{
				trail[i].animTime -= DELTA;
				//                   1초동안 1줄기 
				//trail[i].color.w -= DELTA * 0.8333f;

				trail[i].color.x += DELTA * 0.6f;
				trail[i].color.y += DELTA * 0.6f;
				trail[i].color.z += DELTA * 0.6f;

				if (trail[i].animTime < 0.0f)
				{
					trail[i].isVisible = false;
				}
			}
		}

		if (state == BraveState::IDLE)
		{
			walk.frame.y = lookDir(INPUT->GetWorldMousePos() - col.GetWorldPos());
			// idle -> spriteMove
			if (INPUT->KeyPress('W') or
				INPUT->KeyPress('A') or
				INPUT->KeyPress('S') or
				INPUT->KeyPress('D'))
			{
				state = BraveState::WALK;
				walk.ChangeAnim(ANIMSTATE::LOOP, 0.1f);
			}

		}
		else if (state == BraveState::WALK)
		{
			walk.frame.y = lookDir(INPUT->GetWorldMousePos() - col.GetWorldPos());

			keyDir = Vector2(0, 0);
			if (INPUT->KeyPress('W'))
			{
				keyDir.y = 1.0f;
			}
			else if (INPUT->KeyPress('S'))
			{
				keyDir.y = -1.0f;
			}
			if (INPUT->KeyPress('A'))
			{
				keyDir.x = -1.0f;
			}
			else if (INPUT->KeyPress('D'))
			{
				keyDir.x = 1.0f;
			}
			keyDir.Normalize();
			col.MoveWorldPos(keyDir * speed * DELTA);


			// spriteMove -> spriteAttack
			if (INPUT->KeyPress(VK_SPACE))
			{
				state = BraveState::ROLL;
				walk.isVisible = false;
				roll.isVisible = true;
				roll.ChangeAnim(ANIMSTATE::ONCE, 0.1f);
				speed = 200.0f;
				trailTime = 0.0f;
			}


			// spriteMove -> idle
			if (not INPUT->KeyPress('W') and
				not INPUT->KeyPress('A') and
				not INPUT->KeyPress('S') and
				not INPUT->KeyPress('D'))
			{
				state = BraveState::IDLE;
				walk.ChangeAnim(ANIMSTATE::STOP, 0.1f);
				walk.frame.x = 0;
			}
		}
		else if (state == BraveState::ROLL)
		{
			if (TIMER->GetTick(trailTime, 0.05f))
			{
				for (int i = 0; i < TRAILMAX; i++)
				{
					if (not trail[i].isVisible)
					{
						trail[i].isVisible = true;
						trail[i].SetWorldPos(walk.GetWorldPos());
						trail[i].frame = roll.frame;
						trail[i].animTime = 0.6f;
						trail[i].color.x = 81.0f / 255.0f;
						trail[i].color.y = 106.0f / 255.0f;
						trail[i].color.z = 200.0f / 255.0f;
						trail[i].color.w = 0.5f;
						break;
					}
				}
			}

			//구르기
			col.MoveWorldPos(keyDir * speed * DELTA);

			roll.frame.y = lookDir(keyDir);

			if (roll.animState == ANIMSTATE::STOP)
			{
				walk.isVisible = true;
				roll.isVisible = false;
				speed = 100.0f;
				// spriteAttack -> spriteMove
				if (INPUT->KeyPress('W') or
					INPUT->KeyPress('A') or
					INPUT->KeyPress('S') or
					INPUT->KeyPress('D'))
				{
					state = BraveState::WALK;
					walk.ChangeAnim(ANIMSTATE::LOOP, 0.1f);
				}
				// spriteAttack -> idle
				else
				{
					state = BraveState::IDLE;
					walk.ChangeAnim(ANIMSTATE::STOP, 0.1f);
				}
			}
		}
	}
}

void Brave::Render()
{
	for (int i = 0; i < TRAILMAX; i++)
	{
		trail[i].Render();
	}
	walk.Render();
	roll.Render();
	col.Render();
}

void Brave::InitPath(vector<Tile*> way)
{
	pathWay = way;
	pathfinding = true;
	pathWay.pop_back();

	state = BraveState::WALK;
	walk.ChangeAnim(ANIMSTATE::LOOP, 0.1f);
}

void Brave::StepBack()
{
	col.MoveWorldPos(-keyDir * speed * DELTA);
}

int Character::lookDir(Vector2 dir)
{
	float seta = atan2f(dir.y, dir.x);
	seta /= ToRadian;

	for (int i = 0; i < 7; i++)
	{
		if (-157.5f + 45.0f * i < seta and seta <= -157.5f + 45.0f * (i + 1))
		{
			return dirFrameX[i];
		}
	}
}
