#include "stdafx.h"
#include "Utility2.h"
#include "SystemManager.h"
#include "Unit.h"

ObTileMap* Unit::GameMap = nullptr;
map<UnitName, ObImage*> Unit::UnitWireframe;
void Unit::InitWireframes()
{
	UnitWireframe[UnitName::UNUSED] = new ObImage();
	Utility2::InitImage(*UnitWireframe[UnitName::UNUSED], L"unit/wireframe/wireUnused.png", Vector2());
	UnitWireframe[UnitName::ZEALOT] = new ObImage();
	Utility2::InitImage(*UnitWireframe[UnitName::ZEALOT], L"unit/wireframe/wireZealot.png", Vector2());
}
Unit::Unit(UnitType _unitType, UnitName _unitName)
{
	unitName = _unitName;
	unitType = _unitType;
	unitState = UnitState::IDLE;

	maxHp = 100;
	hp = maxHp;
	atkDamage = 20;

	playerNum = 0;
	col.isFilled = false;
	targetCmdUnit = nullptr;

	InitUnitImage();

	dirFrameX[A0000] = 0;
	dirFrameX[A0225] = 1;
	dirFrameX[A0450] = 2;
	dirFrameX[A0675] = 3;
	dirFrameX[A0900] = 4;
	dirFrameX[A1125] = 5;
	dirFrameX[A1350] = 6;
	dirFrameX[A1575] = 7;
	dirFrameX[A1800] = 8;


	col.SetScale() = spriteIdle.GetScale();

	tickAttackCooldown = attackCooldown;
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

	unitCmd = UnitCmd::STOP;
}

Unit::~Unit()
{
	
}

void Unit::Release()
{
	if (unitType == UnitType::BUILDING)
	{
		for (size_t i = 0; i < buildingColGrid.size(); i++)
		{
			SSYSTEM->DynamicGameMap->walkableTiles[buildingColGrid[i].x][buildingColGrid[i].y] = true;
		}
		GameMap->UpdateBuildingState(SSYSTEM->DynamicGameMap, false);
	}
}

void Unit::Update()
{
	if (hp <= 0 && deathTime >= 0)
		deathTime -= DELTA;
	if (deathTime < 0)
	{
		deathTime = 100;
		SSYSTEM->UnitPoolDelete.push_back(this);
		return;
	}
	if (unitState == UnitState::MOVE)
	{
		if (TIMER->GetTick(tickPathUpdateTime, PathUpdateTime))
		{
			vector<Tile*> temp;
			Vector2	commandPostemp;

			if (unitCmd == UnitCmd::ATTACK)
			{
				if (targetCmdUnit)
					commandPostemp = targetCmdUnit->GetWorldPos();
				else
					commandPostemp = cmdPos;
			}
			else if (unitCmd == UnitCmd::MOVE) commandPostemp = cmdPos;
			// pathfinding을 특정 이벤트 발생 시 갱신하도록 함수화 하여 수정하기
			pathWay2 = PFINDER->FindCompletePath(*GameMap, GetWorldPos(), commandPostemp);
			if (pathWay2.empty())
			{
				Stop2();
				cout << "pathClear\n";
			}
			else
			{
				InitPath2(pathWay2);
			}
		}
	}
	if (pathfinding)
	{
		if (unitState == UnitState::MOVE)
		{
			if (pathWay2.empty())
			{
				Stop2();
				return;
			}
			moveDir = Vector2(pathWay2.back().first, pathWay2.back().second) - col.GetWorldPos();
			if (moveDir.Length() > moveSpeed * DELTA)
			{
				moveDir.Normalize();
				col.MoveWorldPos(moveDir * moveSpeed * DELTA);
				lookDir(moveDir);
			}
			else
			{
				col.SetWorldPos(Vector2(pathWay2.back().first, pathWay2.back().second));
				pathWay2.pop_back();
			}
		}
		else if (unitState == UnitState::IDLE)
		{
			Stop2();
		}
	}
	if ((unitCmd == UnitCmd::STOP || unitCmd == UnitCmd::ATTACK || unitCmd == UnitCmd::PATROL || unitCmd == UnitCmd::HOLD))
	{
		for (size_t i = 0; i < SSYSTEM->UnitPool.size(); i++)
		{
			Vector2 temp = SSYSTEM->UnitPool[i]->GetWorldPos() - GetWorldPos();
			if (playerNum != SSYSTEM->UnitPool[i]->playerNum && temp.Length() < sightRange * TILESCALE)
			{
				if (!targetCmdUnit)
					targetCmdUnit = SSYSTEM->UnitPool[i];
				if (unitCmd == UnitCmd::STOP)
				{
					pathfinding = true;
					unitCmd = UnitCmd::ATTACK;
				}
				unitState = UnitState::MOVE;
				originPos = GetWorldPos();
				break;
			}
		}
	}
	if (unitCmd == UnitCmd::ATTACK)
	{
		if (targetCmdUnit)
		{
			if (targetCmdUnit->hp > 0)
			{
				Vector2 temp = targetCmdUnit->GetWorldPos() - GetWorldPos();
				if (temp.Length() < attackRange * TILESCALE)
				{
					unitState = UnitState::ATTACK;
				}
				else if (temp.Length() < sightRange * TILESCALE)
				{
					unitState = UnitState::MOVE;
				}
			}
			else
			{
				targetCmdUnit = nullptr;
				if (cmdPos != Vector2())
					unitState = UnitState::MOVE;
				else
					unitState = UnitState::IDLE;
			}
		}
	}
	if (unitCmd == UnitCmd::HOLD)
	{
		if (targetCmdUnit)
		{
			if (targetCmdUnit->hp > 0)
			{
				Vector2 temp = targetCmdUnit->GetWorldPos() - GetWorldPos();
				if (temp.Length() < attackRange * TILESCALE)
				{
					unitState = UnitState::ATTACK;
				}
				else
				{
					unitState = UnitState::IDLE;
				}
			}
			else
			{
				targetCmdUnit = nullptr;
			}
		}
	}
	if (unitState == UnitState::ATTACK)
	{
		if (targetCmdUnit && targetCmdUnit->hp > 0 && TIMER->GetTick(tickAttackCooldown, attackCooldown))
		{
			targetCmdUnit->Damage(atkDamage);
			spriteAttack.ChangeAnim(ANIMSTATE::ONCE, FRAME(18), false);
			ImGui::Text("attack State");
		}
		else
		{
			if (unitCmd == UnitCmd::ATTACK)
				unitState == UnitState::MOVE;
			else
				unitState == UnitState::IDLE;
		}
	}
	spriteMove.frame.x = lookDir(moveDir);
	spriteIdle.frame.x = lookDir(moveDir);
	spriteAttack.frame.x = lookDir(moveDir);

	ImGui::Text("unitState %d", unitState);
	ImGui::Text("unitcmd %d", unitCmd);
	ImGui::Text("targetcmdunit %d", (bool)targetCmdUnit);
}

void Unit::Render()
{
	if (hp > 0)
	{
		if (flipImage)
		{
			spriteIdle.SetRotation().y = 180 * ToRadian;
			spriteMove.SetRotation().y = 180 * ToRadian;
			spriteAttack.SetRotation().y = 180 * ToRadian;
		}
		else
		{
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
	else
	{
		spriteDeath.Render();
	}
}

void Unit::InitPath(vector<Tile*> way)
{
	pathWay = way;
	pathWay.pop_back();

	unitState = UnitState::MOVE;
	spriteMove.ChangeAnim(ANIMSTATE::LOOP, FRAME(18), false);
}

void Unit::InitPath2(vector<INTPAIR> way)
{
	pathWay2 = way;

	unitState = UnitState::MOVE;
	spriteMove.ChangeAnim(ANIMSTATE::LOOP, FRAME(18), false);
}

void Unit::Death()
{

	spriteDeath.isVisible = true;
	cout << "death" << endl;
	return;

}

void Unit::Move(Vector2 CommandPos)
{
	pathfinding = true;
	unitCmd = UnitCmd::MOVE;
	unitState = UnitState::MOVE;
	cmdPos = CommandPos;
	tickPathUpdateTime = PathUpdateTime;
	targetCmdUnit = nullptr;
	cout << "Command Move " << endl;
}

void Unit::Attack(Vector2 CommandPos)
{
	pathfinding = true;
	unitCmd = UnitCmd::ATTACK;
	unitState = UnitState::MOVE;
	cmdPos = CommandPos;
	tickPathUpdateTime = PathUpdateTime;
	targetCmdUnit = nullptr;
	cout << "Command Attack " << endl;
}

void Unit::Hold()
{
	pathWay.clear();
	pathfinding = false;
	unitCmd = UnitCmd::HOLD;
	unitState = UnitState::IDLE;
	spriteMove.ChangeAnim(ANIMSTATE::STOP, 0.1f, false);
	spriteMove.frame.y = 0;
	tickPathUpdateTime = PathUpdateTime;
	targetCmdUnit = nullptr;
	cout << "Command Hold " << endl;
}

void Unit::Stop()
{
	pathWay.clear();
	pathfinding = false;
	unitCmd = UnitCmd::STOP;
	unitState = UnitState::IDLE;
	spriteMove.ChangeAnim(ANIMSTATE::STOP, 0.1f, false);
	spriteMove.frame.y = 0;
	tickPathUpdateTime = PathUpdateTime;
	targetCmdUnit = nullptr;
	cout << "Command Stop " << endl;
}
void Unit::Stop2()
{
	pathWay2.clear();
	pathfinding = false;
	unitCmd = UnitCmd::STOP;
	unitState = UnitState::IDLE;
	spriteMove.ChangeAnim(ANIMSTATE::STOP, 0.1f, false);
	spriteMove.frame.y = 0;
	tickPathUpdateTime = PathUpdateTime;
	targetCmdUnit = nullptr;
	cout << "Command Stop " << endl;
}

void Unit::InitUnitImage()
{
	switch (unitName)
	{
	case UnitName::UNUSED:
		break;
	case UnitName::ZEALOT:
		attackRange = 3;
		sightRange = 6;

		Utility2::InitImage(spriteIdle, L"unit/zealotMove.png", Vector2(), 9, 8);
		spriteIdle.SetParentRT(col);

		Utility2::InitImage(spriteMove, L"unit/zealotMove.png", Vector2(), 9, 8);
		spriteMove.SetParentRT(col);

		Utility2::InitImage(spriteAttack, L"unit/zealotAttack.png", Vector2(), 9, 5);
		spriteAttack.SetParentRT(col);

		Utility2::InitImage(spriteDeath, L"unit/zealotDeath.png", Vector2(), 7);

		spriteDeath.ChangeAnim(ANIMSTATE::ONCE, FRAME(18));
		spriteDeath.SetParentRT(col);

		deathTime = 7 * FRAME(18);

		moveSpeed = 200;
		break;
	case UnitName::MARINE:
		break;
	case UnitName::COMMANDCENTER:
		attackRange = 0;
		sightRange = 0;

		Utility2::InitImage(spriteIdle, L"building/cmdCenter_test.png", Vector2());
		spriteIdle.SetParentRT(col);

		Utility2::InitImage(spriteMove, L"building/cmdCenter_test.png", Vector2());
		spriteMove.SetParentRT(col);

		Utility2::InitImage(spriteAttack, L"building/cmdCenter_test.png", Vector2());
		spriteAttack.SetParentRT(col);

		Utility2::InitImage(spriteDeath, L"building/cmdCenter_test.png", Vector2());

		spriteDeath.ChangeAnim(ANIMSTATE::ONCE, FRAME(18));
		spriteDeath.SetParentRT(col);

		deathTime = 7 * FRAME(18);

		moveSpeed = 0;
		break;
	default:
		break;
	}
}

int Unit::lookDir(Vector2 dir)
{
	float seta = atan2f(dir.y, dir.x);
	seta /= ToRadian;
	seta -= 90.0f;
	if (seta < 0)
		seta += 360.0f;
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