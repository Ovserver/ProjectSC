#include "stdafx.h"
#include "Utility2.h"
#include "SystemManager.h"
#include "Unit.h"

Unit::Unit()
{
	//col.SetPivot() = OFFSET_B;
	col.isFilled = false;

	Utility2::InitImage(spriteIdle, L"unit/zealotMove.png", Vector2(), 9, 8);
	spriteIdle.SetParentRT(col);

	Utility2::InitImage(spriteMove, L"unit/zealotMove.png", Vector2(), 9, 8);
	spriteMove.SetParentRT(col);

	Utility2::InitImage(spriteAttack, L"player_roll.png", Vector2(), 6, 8);
	spriteAttack.SetParentRT(col);

	dirFrameX[A0000] = 0;
	dirFrameX[A0225] = 1;
	dirFrameX[A0450] = 2;
	dirFrameX[A0675] = 3;
	dirFrameX[A0900] = 4;
	dirFrameX[A1125] = 5;
	dirFrameX[A1350] = 6;
	dirFrameX[A1575] = 7;
	dirFrameX[A1800] = 8;

	unitType = UnitType::UNUSED;
	unitState = UnitState::IDLE;
	col.SetScale() = spriteMove.GetScale();
	moveSpeed = 200;

	tickPathUpdateTime = PathUpdateTime;

	int i = 0;
	IconPool.iconList[i++] = CmdIconList::MOVE;
	IconPool.iconList[i++] = CmdIconList::STOP;
	IconPool.iconList[i++] = CmdIconList::ATTACK;
	IconPool.iconList[i++] = CmdIconList::PATROL;
	IconPool.iconList[i++] = CmdIconList::HOLD;
	IconPool.iconList[i++] = CmdIconList::NONE;
	IconPool.iconList[i++] = CmdIconList::NONE;
	IconPool.iconList[i++] = CmdIconList::NONE;
	IconPool.iconList[i] = CmdIconList::NONE;
}

void Unit::Update()
{
	if (INPUT->KeyPress('N'))
		spriteMove.SetRotation().y += DELTA * 2;
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
				Stop();
				cout << "pathClear\n";
			}
		}
	}
	if (pathfinding)
	{
		if (pathWay.empty())
		{
			Stop();
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
	spriteMove.frame.x = lookDir(moveDir);
	spriteIdle.frame.x = lookDir(moveDir);
}

void Unit::Render()
{
	ImGui::Text("rotation : %f", spriteMove.GetRotation().y);	
	if (flipImage)
	{
	ImGui::Text("rotation : true");	
		spriteIdle.SetRotation().y = 180 * ToRadian;
		spriteMove.SetRotation().y = 180 * ToRadian;
		spriteAttack.SetRotation().y = 180 * ToRadian;

	}
	else {
	ImGui::Text("rotation : false");	
		spriteIdle.SetRotation().y = 0;
		spriteMove.SetRotation().y = 0;
		spriteAttack.SetRotation().y = 0;
	}
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
	spriteMove.ChangeAnim(ANIMSTATE::LOOP, 1.0f / 18, false);
}

void Unit::Move(Vector2 CommandPos)
{
	pathfinding = true;
	unitState = UnitState::MOVE;
	cmdPos = CommandPos;
	tickPathUpdateTime = PathUpdateTime;
	cout << "Command Move " << endl;
}

void Unit::Attack()
{
}

void Unit::Stop()
{
	pathWay.clear();
	pathfinding = false;
	unitState = UnitState::IDLE;
	spriteMove.ChangeAnim(ANIMSTATE::STOP, 0.1f, false);
	spriteMove.frame.y = 0;
	tickPathUpdateTime = PathUpdateTime;
}

int Unit::lookDir(Vector2 dir)
{
	float seta = atan2f(dir.y, dir.x);
	seta /= ToRadian;
	seta -= 90.0f;
	if (seta < 0)
		seta += 360.0f;
	cout << seta << endl;
	for (int i = 0; i < 8; i++)
	{
		if (360 - 20.0f * i > seta and seta >= 360 - 20.0f * (i + 1))
		{
			flipImage = false;
			return dirFrameX[i];
		}
	}
	for (int i = 0; i < 8; i++)
	{
		if (0 + 20.0f * i <= seta and seta < 0 + 20.0f * (i + 1))
		{
			flipImage = true;
			return dirFrameX[i];
		}
	}
	flipImage = false;
	return dirFrameX[A1800];
}