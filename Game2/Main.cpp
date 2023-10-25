#include "stdafx.h"
#include "Utility2.h"
#include "SystemManager.h"
#include "Unit.h"
#include "Brave.h"
#include "Boss.h"
#include "Main.h"
Main::Main()
{

}

Main::~Main()
{
}
void Main::Init()
{
	//map.file = "python.txt";
	map.Load();
	map.CreateTileCost();
	mainPlayer.SetWorldPos(Vector2(500, 500));
	boss.SetWorldPos(Vector2(500, 500));

	Utility2::InitImage(cursor, L"cursor/Idle.png",Vector2(0,0), 5);
	cursor.ChangeAnim(ANIMSTATE::LOOP, 0.1f);
	Utility2::InitImage(cursorDrag, L"cursor/Drag.png");

	int n = 0;
	Utility2::InitImage(cursorMoveScreen[n++], L"cursor/ScreenPullB.png",Vector2(),2);
	Utility2::InitImage(cursorMoveScreen[n++], L"cursor/ScreenPullLB.png", Vector2(), 2);
	Utility2::InitImage(cursorMoveScreen[n++], L"cursor/ScreenPullL.png", Vector2(), 2);
	Utility2::InitImage(cursorMoveScreen[n++], L"cursor/ScreenPullLT.png", Vector2(), 2);
	Utility2::InitImage(cursorMoveScreen[n++], L"cursor/ScreenPullT.png", Vector2(), 2);
	Utility2::InitImage(cursorMoveScreen[n++], L"cursor/ScreenPullRT.png", Vector2(), 2);
	Utility2::InitImage(cursorMoveScreen[n++], L"cursor/ScreenPullR_offset.png", Vector2(), 2);
	Utility2::InitImage(cursorMoveScreen[n++], L"cursor/ScreenPullRB.png", Vector2(), 2);
	n = 0;
	cursorMoveScreen[n++].SetPivot() = OFFSET_B;
	cursorMoveScreen[n++].SetPivot() = OFFSET_LB;
	cursorMoveScreen[n++].SetPivot() = OFFSET_L;
	cursorMoveScreen[n++].SetPivot() = OFFSET_LT;
	cursorMoveScreen[n++].SetPivot() = OFFSET_T;
	cursorMoveScreen[n++].SetPivot() = OFFSET_RT;
	cursorMoveScreen[n++].SetPivot() = OFFSET_R;
	cursorMoveScreen[n].SetPivot() = OFFSET_RB;

	for (size_t i = 0; i < 8; i++)
		cursorMoveScreen[i].ChangeAnim(ANIMSTATE::LOOP, FRAME(10));

	Utility2::InitImage(console, L"console/tconsole.png");
	Utility2::InitImage(mapImage, L"maps/map.bmp");
	mapImage.SetPivot() = OFFSET_LB;

	SelectArea.SetScale().x = 100.0f;
	SelectArea.SetScale().y = 100.0f;
	SelectArea.SetPivot() = OFFSET_LB;

	SelectAreaCol.SetScale().x = 100.0f;
	SelectAreaCol.SetScale().y = 100.0f;
	SelectAreaCol.SetPivot() = OFFSET_LB;

	app.maincam->viewport.x = 0;
	app.maincam->viewport.y = 0;
	app.maincam->viewport.width = app.GetWidth();
	app.maincam->viewport.height = app.GetHeight();
	app.maincam->SetWorldPos(Vector2(TILESCALE * 64, TILESCALE * 64) * IMGSCALE);

	cam2.viewport.x = 0;
	cam2.viewport.y = 0;
	cam2.viewport.width = app.GetWidth();
	cam2.viewport.height = app.GetHeight();
	cam2.UpdateMatrix();

	TestBox.SetWorldPos(Vector2(TILESCALE * 64, TILESCALE * 64) * IMGSCALE);
	TestBox.SetScale().x = 100.0f;
	TestBox.SetScale().y = 100.0f;

	SSYSTEM->TileMap = &map;
	SSYSTEM->UICam = &cam2;
}

void Main::Release()
{
}
UINT	pNumber = 0;
void Main::Update()
{
	if (INPUT->KeyDown(VK_F1))
		Utility2::ShowCollider = !Utility2::ShowCollider;
	if (INPUT->KeyDown(VK_F2))
		ShowCursor(showCursor = !showCursor);
	if (INPUT->KeyDown(VK_F3))
		showTileMap = !showTileMap;
	if (INPUT->KeyDown(VK_F4))
		app.maincam->SetWorldPos(Vector2(TILESCALE * 64, TILESCALE * 64) * IMGSCALE);
	if (INPUT->KeyDown(VK_F5))
	{
		Unit* temp = new Unit();
		temp->SetWorldPos(INPUT->GetWorldMousePos());
		temp->playerNum = pNumber++;
		SSYSTEM->UnitPool.push_back(temp);
	}

	if (INPUT->KeyPress(VK_LEFT))
	{
		app.maincam->MoveWorldPos(LEFT * 1200 * DELTA);
	}
	if (INPUT->KeyPress(VK_RIGHT))
	{
		app.maincam->MoveWorldPos(RIGHT * 1200 * DELTA);
	}
	if (INPUT->KeyPress(VK_UP))
	{
		app.maincam->MoveWorldPos(UP * 1200 * DELTA);
	}
	if (INPUT->KeyPress(VK_DOWN))
	{
		app.maincam->MoveWorldPos(DOWN * 1200 * DELTA);
	}
	SSYSTEM->Update();
}
float intervalTime = 0;
bool MoveLeftScreen = false, MoveRightScreen = false, MoveUpScreen = false, MoveDownScreen = false;
void Main::LateUpdate()
{
	if (INPUT->GetScreenMousePos().y < 690)
	{
		if (INPUT->KeyDown(VK_LBUTTON))
		{
			startDragPos = INPUT->GetScreenMousePos();
			SelectArea.SetWorldPos(Vector2(-app.GetHalfWidth() + startDragPos.x,
				app.GetHalfHeight() - startDragPos.y));
			SelectAreaCol.SetWorldPos(app.maincam->GetWorldPos() + SelectArea.GetWorldPos());
		}
		if (INPUT->KeyPress(VK_LBUTTON))
		{
			SelectArea.SetScale().x = -startDragPos.x + INPUT->GetScreenMousePos().x;
			SelectArea.SetScale().y = startDragPos.y - INPUT->GetScreenMousePos().y;

			SelectAreaCol.SetScale().x = -startDragPos.x + INPUT->GetScreenMousePos().x;
			SelectAreaCol.SetScale().y = startDragPos.y - INPUT->GetScreenMousePos().y;

			for (size_t i = 0; i < SSYSTEM->UnitPool.size(); i++)
			{
				if (SelectAreaCol.Intersect(&SSYSTEM->UnitPool[i]->col))
				{
					//SSYSTEM->UnitPool[i]->col.color = Color(0, 1, 0);
				}
				else
				{
					//SSYSTEM->UnitPool[i]->col.color = Color(1, 1, 1);
				}
			}
		}
		else
		{
			if (INPUT->GetScreenMousePos().y < 10)
			{
				MoveUpScreen = true;
				app.maincam->MoveWorldPos(UP * 1200 * DELTA);
			}
			else
			{
				MoveUpScreen = false;
			}
			if (INPUT->GetScreenMousePos().y > app.GetHeight() - 10)
			{
				MoveDownScreen = true;
				app.maincam->MoveWorldPos(DOWN * 1200 * DELTA);
			}
			else
			{
				MoveDownScreen = false;
			}
			if (INPUT->GetScreenMousePos().x < 10)
			{
				MoveLeftScreen = true;
				app.maincam->MoveWorldPos(LEFT * 1200 * DELTA);
			}
			else
			{
				MoveLeftScreen = false;
			}
			if (INPUT->GetScreenMousePos().x > app.GetWidth() - 10)
			{
				MoveRightScreen = true;
				app.maincam->MoveWorldPos(RIGHT * 1200 * DELTA);
			}
			else
			{
				MoveRightScreen = false;
			}
		}		
		if (INPUT->KeyUp(VK_LBUTTON) && abs(SelectAreaCol.GetScale().x) > 2 && abs(SelectAreaCol.GetScale().y) > 2)
		{			
			SSYSTEM->UnitPoolSelect.clear();
			for (size_t i = 0; i < SSYSTEM->UnitPool.size(); i++)
			{
				if (SelectAreaCol.Intersect(&SSYSTEM->UnitPool[i]->col))
				{
					if (SSYSTEM->UnitPoolSelect.size() < 12)
						SSYSTEM->UnitPoolSelect.push_back(SSYSTEM->UnitPool[i]);
				}
			}
			SSYSTEM->UpdateCmdIcons();
			startDragPos = Vector2(0, 0);
		}


		if (INPUT->KeyDown(VK_RBUTTON))
		{
			for (size_t i = 0; i < SSYSTEM->UnitPoolSelect.size(); i++)
			{
				SSYSTEM->UnitPoolSelect[i]->Move(INPUT->GetWorldMousePos());
			}
		}
	}
	for (int i = 0; i < way.size(); i++)
	{
		ImGui::Text("%d path  X : %f Y : %f", i, way[i]->Pos.x, way[i]->Pos.y);
	}
	if (map.GetTileState((mainPlayer.GetFoot())) == TILE_NONE)
	{
		ImGui::Text("NONE");
	}
	if (map.GetTileState((mainPlayer.GetFoot())) == TILE_WALL)
	{
		mainPlayer.StepBack();
	}
	ImGui::Text("AreaScale\n%f %f", SelectArea.GetScale().x, SelectArea.GetScale().y);
	ImGui::Text("AreaWorldPos\n%f %f", SelectArea.GetWorldPos().x, SelectArea.GetWorldPos().y);
	ImGui::Text("StartDragPos is Screen\n%f %f", startDragPos.x, startDragPos.y);
	ImGui::Text("ScreenMousePos\n%f %f", INPUT->GetScreenMousePos().x, INPUT->GetScreenMousePos().y);
	ImGui::Text("Unit Pool : %d", SSYSTEM->UnitPool.size());
	ImGui::Text("Unit Pool Selected : %d", SSYSTEM->UnitPoolSelect.size());
	ImGui::Text("TileCol Unit\n");
	for (size_t i = 0; i < SSYSTEM->TileMap->tileSize.y; i++)
	{
		for (size_t j = 0; j < SSYSTEM->TileMap->tileSize.x; j++)
		{
			if (SSYSTEM->TileMap->Tiles[j][i].tileColInfo == TileCol::UNIT)
				ImGui::Text("%d %d\n", j, i);
		}
	}
	
}
void Main::Render()
{
	mapImage.Render();
	if (showTileMap)
		map.Render();	
	console.Render(&cam2);
	SSYSTEM->Render();
	if (INPUT->KeyPress(VK_LBUTTON))
	{
		cursorDrag.SetWorldPos(INPUT->GetWorldMousePos());
		cursorDrag.Render();
		SelectArea.isFilled = true;
		SelectArea.color = Color(0, 1, 0, 0.1f);
		SelectArea.Render(&cam2);
		SelectArea.isFilled = false;
		SelectArea.color = Color(0, 1, 0, 0.5f);
		SelectArea.Render(&cam2);
	}
	else
	{
		if (MoveUpScreen)
		{
			if (MoveLeftScreen)
			{
				cursorMoveScreen[3].SetWorldPos(INPUT->GetWorldMousePos());
				cursorMoveScreen[3].Render();
			}
			else if (MoveRightScreen)
			{
				cursorMoveScreen[5].SetWorldPos(INPUT->GetWorldMousePos());
				cursorMoveScreen[5].Render();
			}
			else
			{
				cursorMoveScreen[4].SetWorldPos(INPUT->GetWorldMousePos());
				cursorMoveScreen[4].Render();
			}
		}
		else if (MoveDownScreen)
		{
			if (MoveLeftScreen)
			{
				cursorMoveScreen[1].SetWorldPos(INPUT->GetWorldMousePos());
				cursorMoveScreen[1].Render();
			}
			else if (MoveRightScreen)
			{
				cursorMoveScreen[7].SetWorldPos(INPUT->GetWorldMousePos());
				cursorMoveScreen[7].Render();
			}
			else
			{
				cursorMoveScreen[0].SetWorldPos(INPUT->GetWorldMousePos());
				cursorMoveScreen[0].Render();
			}
		}
		else if (MoveLeftScreen)
		{
			cursorMoveScreen[2].SetWorldPos(INPUT->GetWorldMousePos());
			cursorMoveScreen[2].Render();
		}
		else if (MoveRightScreen)
		{
			cursorMoveScreen[6].SetWorldPos(INPUT->GetWorldMousePos());
			cursorMoveScreen[6].Render();
		}
		else
		{
			cursor.SetWorldPos(INPUT->GetWorldMousePos());
			cursor.Render();
		}
	}
}

void Main::ResizeScreen()
{
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR param, int command)
{
	app.SetAppName(L"Game2");
	app.SetInstance(instance);
	app.InitWidthHeight(640.0f * IMGSCALE, 480.0f * IMGSCALE);
	WIN->Create();
	Main* main = new Main();
	int wParam = (int)WIN->Run(main);
	SafeDelete(main);
	WIN->Destroy();
	WIN->DeleteSingleton();

	return wParam;
}