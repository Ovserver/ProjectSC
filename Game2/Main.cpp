#include "stdafx.h"
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

	cursorMoveScreen[0].LoadFile(L"cursor/ScreenPullB.png");
	cursorMoveScreen[1].LoadFile(L"cursor/ScreenPullLB.png");
	cursorMoveScreen[2].LoadFile(L"cursor/ScreenPullL.png");
	cursorMoveScreen[3].LoadFile(L"cursor/ScreenPullLT.png");
	cursorMoveScreen[4].LoadFile(L"cursor/ScreenPullT.png");
	cursorMoveScreen[5].LoadFile(L"cursor/ScreenPullRT.png");
	cursorMoveScreen[6].LoadFile(L"cursor/ScreenPullR_offset.png");
	cursorMoveScreen[7].LoadFile(L"cursor/ScreenPullRB.png");
	cursorMoveScreen[0].SetPivot() = OFFSET_B;
	cursorMoveScreen[1].SetPivot() = OFFSET_LB;
	cursorMoveScreen[2].SetPivot() = OFFSET_L;
	cursorMoveScreen[3].SetPivot() = OFFSET_LT;
	cursorMoveScreen[4].SetPivot() = OFFSET_T;
	cursorMoveScreen[5].SetPivot() = OFFSET_RT;
	cursorMoveScreen[6].SetPivot() = OFFSET_R;
	cursorMoveScreen[7].SetPivot() = OFFSET_RB;

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
	app.maincam->viewport.x = 0;
	app.maincam->viewport.y = 0;
	app.maincam->viewport.width = app.GetWidth();
	app.maincam->viewport.height = app.GetHeight();
	cam2.viewport.x = 0;
	cam2.viewport.y = 0;
	cam2.viewport.width = app.GetWidth();
	cam2.viewport.height = app.GetHeight();
	cam2.UpdateMatrix();
}

void Main::Release()
{
}
bool showCursor = false;
bool showTileMap = true;
void Main::Update()
{
	if (INPUT->KeyDown(VK_F2))
		ShowCursor(showCursor = !showCursor);
	if (INPUT->KeyDown(VK_F3))
		showTileMap = !showTileMap;
	mainPlayer.Update();
	boss.Update(mainPlayer.GetWorldPos());
	if (INPUT->KeyPress(VK_LEFT))
	{
		app.maincam->MoveWorldPos(LEFT * 1000 * DELTA);
	}
	if (INPUT->KeyPress(VK_RIGHT))
	{
		app.maincam->MoveWorldPos(RIGHT * 1000 * DELTA);
	}
	if (INPUT->KeyPress(VK_UP))
	{
		app.maincam->MoveWorldPos(UP * 1000 * DELTA);
	}
	if (INPUT->KeyPress(VK_DOWN))
	{
		app.maincam->MoveWorldPos(DOWN * 1000 * DELTA);
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
	}
	else
	{
		if (INPUT->GetScreenMousePos().y < 10)
		{
			MoveUpScreen = true;
			app.maincam->MoveWorldPos(UP * 1000 * DELTA);
		}
		else
		{
			MoveUpScreen = false;
		}
		if (INPUT->GetScreenMousePos().y > app.GetHeight() - 10)
		{
			MoveDownScreen = true;
			app.maincam->MoveWorldPos(DOWN * 1000 * DELTA);
		}
		else 
		{
			MoveDownScreen = false;
		}
		if (INPUT->GetScreenMousePos().x < 10)
		{
			MoveLeftScreen = true;
			app.maincam->MoveWorldPos(LEFT * 1000 * DELTA);
		}
		else 
		{
			MoveLeftScreen = false;
		}
		if (INPUT->GetScreenMousePos().x > app.GetWidth() - 10)
		{
			MoveRightScreen = true;
			app.maincam->MoveWorldPos(RIGHT * 1000 * DELTA);
		}
		else 
		{
			MoveRightScreen = false;
		}
	}
	if (INPUT->KeyPress(VK_LBUTTON))
	{
		SelectArea.SetScale().x = -startDragPos.x + INPUT->GetScreenMousePos().x;
		SelectArea.SetScale().y = startDragPos.y - INPUT->GetScreenMousePos().y;
	}
	if (INPUT->KeyUp(VK_LBUTTON))
	{
		startDragPos = Vector2(0, 0);
	}
	if (map.GetTileState((mainPlayer.GetFoot())) == TILE_NONE)
	{
		ImGui::Text("NONE");
	}
	if (map.GetTileState((mainPlayer.GetFoot())) == TILE_WALL)
	{
		mainPlayer.StepBack();
	}
	ImGui::Text("%f %f", startDragPos.x, startDragPos.y);
	ImGui::Text("%f %f", INPUT->GetScreenMousePos().x, INPUT->GetScreenMousePos().y);

	if (INPUT->KeyDown(VK_RBUTTON))
	{
		if (map.PathFinding(mainPlayer.GetWorldPos(), INPUT->GetWorldMousePos(), way))
		{
			mainPlayer.InitPath(way);
		}
	}
	if (boss.activeFind)
	{
		if (TIMER->GetTick(intervalTime, 1.5f))
		{
			if (map.PathFinding(boss.GetWorldPos(), mainPlayer.GetWorldPos(), way))
			{
				boss.InitPath(way);
			}
		}
	}
	for (int i = 0; i < way.size(); i++)
	{
		ImGui::Text("%d path  X : %f Y : %f", i, way[i]->Pos.x, way[i]->Pos.y);
	}
}
void Main::Render()
{
	mapImage.Render();
	if (showTileMap)
		map.Render();
	mainPlayer.Render();
	boss.Render();
	console.Render(&cam2);
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