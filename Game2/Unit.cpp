#include "stdafx.h"
#include "Utility2.h"
#include "SystemManager.h"
#include "Unit.h"

Unit::Unit()
{
	//col.SetPivot() = OFFSET_B;
	col.isFilled = false;

	Utility2::InitImage(spriteIdle, L"player_walk.png",OFFSET_B, 6, 8);
	spriteMove.SetParentRT(col);

	Utility2::InitImage(spriteMove, L"player_walk.png", OFFSET_B, 6, 8);
	spriteMove.SetParentRT(col);

	Utility2::InitImage(spriteAttack, L"player_roll.png", OFFSET_B, 6, 8);
	spriteAttack.SetParentRT(col);

	dirFrameY[LB] = 5;
	dirFrameY[B] = 3;
	dirFrameY[RB] = 4;
	dirFrameY[R] = 0;
	dirFrameY[RT] = 7;
	dirFrameY[T] = 1;
	dirFrameY[LT] = 6;
	dirFrameY[L] = 2;

	unitType = UnitType::UNUSED;
	unitState = UnitState::IDLE;
	col.SetScale() = spriteMove.GetScale();
	moveSpeed = 200;

	tickPathUpdateTime = PathUpdateTime;
}

void Unit::Update()
{
	if (unitState == UnitState::MOVE)
	{
		if (TIMER->GetTick(tickPathUpdateTime, PathUpdateTime))
		{
			vector<Tile*> temp;
			SSYSTEM->UpdateTileCol();
			if (SSYSTEM->TileMap->PathFinding(col.GetWorldPos(), cmdPos, pathWay))
			{
				InitPath(pathWay);
			}
			else
			{
				pathWay.clear();
				cout << "pathClear\n";
			}
		}
	}
	if (pathfinding)
	{
		if (pathWay.empty())
		{
			pathfinding = false;
			unitState = UnitState::IDLE;
			spriteMove.ChangeAnim(ANIMSTATE::STOP, 0.1f);
			spriteMove.frame.x = 0;
			tickPathUpdateTime = PathUpdateTime;
			return;
		}
		moveDir = pathWay.back()->Pos - col.GetWorldPos();
		if (moveDir.Length() > moveSpeed * DELTA)
		{
			moveDir.Normalize();
			col.MoveWorldPos(moveDir * moveSpeed * DELTA);
			lookDir(moveDir);
		}
		else
		{
			col.SetWorldPos(pathWay.back()->Pos);
			pathWay.pop_back();
		}
	}
	if (unitState == UnitState::IDLE)
	{
		spriteMove.frame.y = lookDir(moveDir);
	}
	else if (unitState == UnitState::MOVE)
	{
		spriteMove.frame.y = lookDir(moveDir);
	}
}

void Unit::Render()
{
	if (unitState == UnitState::IDLE)
		spriteIdle.Render();
	if (unitState == UnitState::MOVE)
		spriteMove.Render();
	if (unitState == UnitState::ATTACK)
		spriteAttack.Render();
	if (Utility2::ShowCollider)
		col.Render();
}

void Unit::InitPath(vector<Tile*> way)
{
	pathWay = way;
	pathWay.pop_back();

	unitState = UnitState::MOVE;
	spriteMove.ChangeAnim(ANIMSTATE::LOOP, 0.1f);
}

void Unit::Move(Vector2 CommandPos)
{
	pathfinding = true;
	unitState = UnitState::MOVE;
	cmdPos = CommandPos;
	tickPathUpdateTime = PathUpdateTime;
	cout << "Command Move " << endl;
}

int Unit::lookDir(Vector2 dir)
{
	float seta = atan2f(dir.y, dir.x);
	seta /= ToRadian;

	for (int i = 0; i < 7; i++)
	{
		if (-157.5f + 45.0f * i < seta and seta <= -157.5f + 45.0f * (i + 1))
		{
			return dirFrameY[i];
		}
	}
	return dirFrameY[L];
}