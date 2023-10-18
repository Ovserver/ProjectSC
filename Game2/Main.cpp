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

	cursor.LoadFile(L"cursor/Idle.png");
	cursor.maxFrame.x = 5;
	cursor.SetScale().x = cursor.imageSize.x / 5.0f * IMGSCALE;
	cursor.SetScale().y = cursor.imageSize.y * IMGSCALE;
	cursor.ChangeAnim(ANIMSTATE::LOOP, 0.1f);

	//cursor.LoadFile(L"cursor/SelectMine.png");
	//cursor.maxFrame.x = 14;
	//cursor.SetScale().x = cursor.imageSize.x / 14.0f * IMGSCALE;
	//cursor.SetScale().y = cursor.imageSize.y * IMGSCALE;
	//cursor.ChangeAnim(ANIMSTATE::LOOP, 0.1f);

	cursorDrag.LoadFile(L"cursor/Drag.png");
	cursorDrag.SetScale().x = cursorDrag.imageSize.x * IMGSCALE;
	cursorDrag.SetScale().y = cursorDrag.imageSize.y * IMGSCALE;

	int n = 0;
	cursorMoveScreen[n++].LoadFile(L"cursor/ScreenPullB.png");
	cursorMoveScreen[n++].LoadFile(L"cursor/ScreenPullLB.png");
	cursorMoveScreen[n++].LoadFile(L"cursor/ScreenPullL.png");
	cursorMoveScreen[n++].LoadFile(L"cursor/ScreenPullLT.png");
	cursorMoveScreen[n++].LoadFile(L"cursor/ScreenPullT.png");
	cursorMoveScreen[n++].LoadFile(L"cursor/ScreenPullRT.png");
	cursorMoveScreen[n++].LoadFile(L"cursor/ScreenPullR_offset.png");
	cursorMoveScreen[n].LoadFile(L"cursor/ScreenPullRB.png");
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
	{
		cursorMoveScreen[i].maxFrame.x = 2;
		cursorMoveScreen[i].SetScale().x = cursorMoveScreen[i].imageSize.x / 2.0f * IMGSCALE;
		cursorMoveScreen[i].SetScale().y = cursorMoveScreen[i].imageSize.y * IMGSCALE;
		cursorMoveScreen[i].ChangeAnim(ANIMSTATE::LOOP, 0.1f);
	}

	console.LoadFile(L"console/tconsole.png");
	console.SetScale().x = console.imageSize.x * IMGSCALE;
	console.SetScale().y = console.imageSize.y * IMGSCALE;
	//console.SetPivot() = OFFSET_B;
	//console.SetWorldPos(Vector2(0, -app.GetHalfHeight()));
	mapImage.LoadFile(L"maps/map.bmp");
	mapImage.SetScale().x = mapImage.imageSize.x * IMGSCALE;
	mapImage.SetScale().y = mapImage.imageSize.y * IMGSCALE;
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
	app.maincam->SetWorldPos(Vector2(32 * 64, 32 * 64) * IMGSCALE);

	cam2.viewport.x = 0;
	cam2.viewport.y = 0;
	cam2.viewport.width = app.GetWidth();
	cam2.viewport.height = app.GetHeight();
	cam2.UpdateMatrix();

	TestBox.SetWorldPos(Vector2(32 * 64, 32 * 64) * IMGSCALE);
	TestBox.SetScale().x = 100.0f;
	TestBox.SetScale().y = 100.0f;

	SSYSTEM->TileMap = &map;

	IMOVE.LoadFile(L"icons/cmdiconsMove.png");
	IMOVE.SetScale().x = IMOVE.imageSize.x * IMGSCALE;
	IMOVE.SetScale().y = IMOVE.imageSize.y * IMGSCALE;
	IMOVE.SetWorldPos(Vector2(-app.GetHalfWidth() + 1048, app.GetHalfHeight() - 750));
	ISTOP.LoadFile(L"icons/cmdiconsStop.png");
	ISTOP.SetScale().x = ISTOP.imageSize.x * IMGSCALE;
	ISTOP.SetScale().y = ISTOP.imageSize.y * IMGSCALE;
	ISTOP.SetWorldPos(Vector2(-app.GetHalfWidth() + 1140, app.GetHalfHeight() - 750));
	IATTACK.LoadFile(L"icons/cmdiconsAttack.png");
	IATTACK.SetScale().x = IATTACK.imageSize.x * IMGSCALE;
	IATTACK.SetScale().y = IATTACK.imageSize.y * IMGSCALE;
	IATTACK.SetWorldPos(Vector2(-app.GetHalfWidth() + 1232, app.GetHalfHeight() - 750));
	IPATROL.LoadFile(L"icons/cmdiconsPatrol.png");
	IPATROL.SetScale().x = IPATROL.imageSize.x * IMGSCALE;
	IPATROL.SetScale().y = IPATROL.imageSize.y * IMGSCALE;
	IPATROL.SetWorldPos(Vector2(-app.GetHalfWidth() + 1048, app.GetHalfHeight() - 830));
	IHOLD.LoadFile(L"icons/cmdiconsHold.png");
	IHOLD.SetScale().x = IHOLD.imageSize.x * IMGSCALE;
	IHOLD.SetScale().y = IHOLD.imageSize.y * IMGSCALE;
	IHOLD.SetWorldPos(Vector2(-app.GetHalfWidth() + 1140, app.GetHalfHeight() - 830));
}

void Main::Release()
{
}
void Main::Update()
{
	if (INPUT->KeyDown(VK_F1))
		Utility2::ShowCollider = !Utility2::ShowCollider;
	if (INPUT->KeyDown(VK_F2))
		ShowCursor(showCursor = !showCursor);
	if (INPUT->KeyDown(VK_F3))
		showTileMap = !showTileMap;
	if (INPUT->KeyDown(VK_F5))
	{
		Unit* temp = new Unit();
		temp->SetWorldPos(INPUT->GetWorldMousePos());
		SSYSTEM->UnitPool.push_back(temp);
	}
	mainPlayer.Update();
	for (size_t i = 0; i < SSYSTEM->UnitPool.size(); i++)
	{
		SSYSTEM->UnitPool[i]->Update();
	}
	boss.Update(mainPlayer.GetWorldPos());

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
}
float intervalTime = 0;
bool MoveLeftScreen = false, MoveRightScreen = false, MoveUpScreen = false, MoveDownScreen = false;
void Main::LateUpdate()
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

		if (SelectAreaCol.Intersect(&TestBox))
		{
			cout << "ccooooooollll" << endl;
		}
		for (size_t i = 0; i < SSYSTEM->UnitPool.size(); i++)
		{
			if (SelectAreaCol.Intersect(&SSYSTEM->UnitPool[i]->col))
			{
				cout << "ccooooooollll" << endl;
				SSYSTEM->UnitPool[i]->col.color = Color(0, 1, 0);
			}
			else
			{
				SSYSTEM->UnitPool[i]->col.color = Color(1, 1, 1);
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
	if (INPUT->KeyUp(VK_LBUTTON))
	{
		startDragPos = Vector2(0, 0);
		SSYSTEM->UnitPoolSelect.clear();
		for (size_t i = 0; i < SSYSTEM->UnitPool.size(); i++)
		{
			if (SelectAreaCol.Intersect(&SSYSTEM->UnitPool[i]->col))
			{
				if (SSYSTEM->UnitPoolSelect.size() < 12)
					SSYSTEM->UnitPoolSelect.push_back(SSYSTEM->UnitPool[i]);
				else
					SSYSTEM->UnitPool[i]->col.color = Color(1, 1, 1);
			}
			else
			{
				SSYSTEM->UnitPool[i]->col.color = Color(1, 1, 1);
			}
		}
	}


	if (INPUT->KeyDown(VK_RBUTTON))
	{
		for (size_t i = 0; i < SSYSTEM->UnitPoolSelect.size(); i++)
		{
			SSYSTEM->UnitPoolSelect[i]->Move(INPUT->GetWorldMousePos());
			//if (map.PathFinding(SSYSTEM->UnitPoolSelect[i]->col.GetWorldPos(), INPUT->GetWorldMousePos(), way))
			//{
			//	SSYSTEM->UnitPoolSelect[i]->InitPath(way);
			//}
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
	ImGui::Text("Unit Pool Selected\n%d", SSYSTEM->UnitPoolSelect.size());
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
	for (size_t i = 0; i < SSYSTEM->UnitPool.size(); i++)
	{
		SSYSTEM->UnitPool[i]->Render();
	}
	mainPlayer.Render();
	boss.Render();

	TestBox.Render();

	console.Render(&cam2);
	IMOVE.Render(&cam2);
	IATTACK.Render(&cam2);
	ISTOP.Render(&cam2);
	IHOLD.Render(&cam2);
	IPATROL.Render(&cam2);
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