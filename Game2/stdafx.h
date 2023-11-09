#pragma once
#include "../2DFrameWork/framework.h"
#pragma comment (lib,"../Build/2DFrameWork/2DFrameWork.lib")
#define	SSYSTEM SystemManager::GetInstance()
#define PFINDER HierarchicalPathfinder::GetInstance()
enum class UnitName
{
	UNUSED, ZEALOT, MARINE, DEVOURER, COMMANDCENTER
};
enum class UnitType
{
	UNUSED, GROUNDUNIT, AIRUNIT, BUILDING, NEUTRAL, RESOURCE, EFFECT
};
enum class UnitCmd
{
	NONE, STOP, MOVE, ATTACK, PATROL, HOLD, GATHER
};
enum class UnitState
{
	IDLE, MOVE, ATTACK, GATHER
};