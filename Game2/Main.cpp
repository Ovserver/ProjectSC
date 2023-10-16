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
	map.file = "map1.txt";
	map.Load();
	map.CreateTileCost();
	mainPlayer.SetWorldPos(Vector2(500, 500));
	boss.SetWorldPos(Vector2(500, 500));
	cursor.LoadFile(L"cursor.png");
	cursor.maxFrame.x = 5;
	cursor.SetScale().x = cursor.imageSize.x / 5.0f * IMGSCALE;
	cursor.SetScale().y = cursor.imageSize.y * IMGSCALE;
	cursor.ChangeAnim(ANIMSTATE::LOOP, 0.1f);
	console.LoadFile(L"tconsole.png");
	console.SetScale().x = console.imageSize.x * IMGSCALE;
	console.SetScale().y = console.imageSize.y * IMGSCALE;
	//console.SetPivot() = OFFSET_B;
	//console.SetWorldPos(Vector2(0, -app.GetHalfHeight()));

	SelectArea.SetPivot() = OFFSET_LB;
	SelectArea.SetScale().x = 100.0f;
	SelectArea.SetScale().y = 100.0f;
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

void Main::Update()
{
	mainPlayer.Update();
	boss.Update(mainPlayer.GetWorldPos());
	if (INPUT->KeyPress('J'))
	{
		app.maincam->SetScale().x += DELTA;
		cout << app.maincam->GetScale().y << endl;
	}
	if (INPUT->KeyPress('K'))
	{
		app.maincam->SetScale().x -= DELTA;
		cout << app.maincam->GetScale().y << endl;
	}
	if (INPUT->KeyPress('N'))
	{
		app.maincam->MoveWorldPos(LEFT * 100 * DELTA);
		cout << app.maincam->GetScale().y << endl;
	}
	if (INPUT->KeyPress('M'))
	{
		app.maincam->MoveWorldPos(RIGHT * 100 * DELTA);
		cout << app.maincam->GetScale().y << endl;
	}
}
float intervalTime = 0;
void Main::LateUpdate()
{
	if (INPUT->KeyDown(VK_LBUTTON))
	{
		startDragPos = INPUT->GetScreenMousePos();
		SelectArea.SetWorldPos(Vector2(-app.GetHalfWidth() + startDragPos.x,
			app.GetHalfHeight() - startDragPos.y));
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
	cursor.SetWorldPos(INPUT->GetWorldMousePos());
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
	map.Render();
	mainPlayer.Render();
	boss.Render();
	console.Render(&cam2);
	cursor.Render();
	if (INPUT->KeyPress(VK_LBUTTON))
	{
		SelectArea.isFilled = true;
		SelectArea.color = Color(0, 1, 0, 0.1f);
		SelectArea.Render(&cam2);
		SelectArea.isFilled = false;
		SelectArea.color = Color(0, 1, 0, 0.5f);
		SelectArea.Render(&cam2);
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