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
	map.file = "python.txt";
	map.Load();
	map.CreateTileState();
	map.ClusterResize();
	for (size_t i = 0; i < map.tileSize.x / 4; i++)
	{
		map.buildingTiles.push_back(vector<bool>());
		for (size_t j = 0; j < map.tileSize.y / 4; j++)
			map.buildingTiles[i].push_back(false);
	}

	dynamicMap = new ObTileMap(128, 128, Vector2(TILESCALE, TILESCALE) * IMGSCALE);
	dynamicMap->Load();
	dynamicMap->CreateTileState();
	Utility2::InitDynamicMap(&map, dynamicMap);

	//init pathfinding
	PFINDER->InitializeCluster(map);
	PFINDER->CreateEntranceNodes(map);
	PFINDER->CalcInterPath(map);

	Unit::InitWireframes();
	Unit::GameMap = &map;

	for (size_t i = 0; i < map.cluster.size(); i++)
	{
		for (size_t j = 0; j < map.cluster[i].size(); j++)
		{
			cout << "cluster " << i << " " << j << " / size : " << map.cluster[i][j]->nodes.size() << map.cluster[i][j] << endl;
			for (size_t k = 0; k < map.cluster[i][j]->nodes.size(); k++)
			{
				cout << "x : " <<
					map.cluster[i][j]->nodes[k]->x << "/ y : " <<
					map.cluster[i][j]->nodes[k]->y << "/ neighber : ";
				cout << map.cluster[i][j]->nodes[k]->neighbors.size() << " / ";
				for (auto neighbor : map.cluster[i][j]->nodes[k]->neighbors)
					cout << neighbor.first->x << " " << neighbor.first->y << " / ";
				cout << endl;
			}

		}
	}

	mainPlayer.SetWorldPos(Vector2(500, 500));
	boss.SetWorldPos(Vector2(500, 500));

	Utility2::InitImage(cursor, L"cursor/Idle.png", Vector2(0, 0), 5);
	cursor.ChangeAnim(ANIMSTATE::LOOP, 0.1f);
	Utility2::InitImage(cursorDrag, L"cursor/Drag.png");

	int n = 0;
	Utility2::InitImage(cursorMoveScreen[n++], L"cursor/ScreenPullB.png", Vector2(), 2);
	Utility2::InitImage(cursorMoveScreen[n++], L"cursor/ScreenPullLB.png", Vector2(), 2);
	Utility2::InitImage(cursorMoveScreen[n++], L"cursor/ScreenPullL.png", Vector2(), 2);
	Utility2::InitImage(cursorMoveScreen[n++], L"cursor/ScreenPullLT.png", Vector2(), 2);
	Utility2::InitImage(cursorMoveScreen[n++], L"cursor/ScreenPullT.png", Vector2(), 2);
	Utility2::InitImage(cursorMoveScreen[n++], L"cursor/ScreenPullRT.png", Vector2(), 2);
	Utility2::InitImage(cursorMoveScreen[n++], L"cursor/ScreenPullR.png", Vector2(), 2);
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
	TestBox.SetScale().x = 10.0f;
	TestBox.SetScale().y = 10.0f;

	Utility2::InitImage(buildingImage, L"building/cmdCenter_test.png");
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			buildingImageBox[i * 3 + j].SetParentT(buildingImage);
			buildingImageBox[i * 3 + j].UpdateMatrix();
			buildingImageBox[i * 3 + j].SetScale().x = 32 * IMGSCALE;
			buildingImageBox[i * 3 + j].SetScale().y = 32 * IMGSCALE;
			buildingImageBox[i * 3 + j].SetLocalPos(Vector2(-48 * IMGSCALE + i * 32 * IMGSCALE, 32 * IMGSCALE - j * 32 * IMGSCALE));
			buildingImageBox[i * 3 + j].color = Color(0, 1, 0, 0.25f);
		}
	}
	Utility2::InitImage(HelpBox, L"Help.png");
	HelpBox.SetScale().x /= IMGSCALE;
	HelpBox.SetScale().y /= IMGSCALE;
	SSYSTEM->GameMap = &map;
	SSYSTEM->DynamicGameMap = dynamicMap;
	SSYSTEM->UICam = &cam2;
}

void Main::Release()
{
}
Vector2 startPos;
vector<INTPAIR> pathway;
UINT	pNumber = 0;
Int2	int2tmp;
//F1: Show Help
//F2 + 1 : Show Unit Collider
//F2 + 2 : Show Grid Point
//F2 + 3 : Show Cursor
//F3 : Show Terrain Map
//F4 + NUM : Cam Position Set
//F5 + 1 : Create Zealot Player1
//F5 + 2 : Create Devourer Player1
//F6 + 1 : Create Zealot Player2
//F6 + 2 : Create Devourer Player2
//F7 : Set Virtual Start Point to Pathfinding
//F8 : Set Virtual Destination Point to Pathfinding
//Q : Create Building CommandCenter Player1
void Main::Update()
{
	if (INPUT->KeyDown(VK_F1))
		showHelp = !showHelp;
	if (INPUT->KeyPress(VK_F2))
	{
		if (INPUT->KeyDown('1'))
			Utility2::ShowCollider = !Utility2::ShowCollider;
		if (INPUT->KeyDown('2'))
			showGridMap = !showGridMap;
		if (INPUT->KeyDown('3'))
			ShowCursor(showCursor = !showCursor);
	}
	if (INPUT->KeyDown(VK_F3))
		showTileMap = !showTileMap;
	if (INPUT->KeyPress(VK_F4))
	{
		if (INPUT->KeyDown('1'))
			app.maincam->SetWorldPos(Vector2(TILESCALE * 64, TILESCALE * 64) * IMGSCALE);
		if (INPUT->KeyDown('2'))
			app.maincam->SetWorldPos(Vector2(TILESCALE * 16, TILESCALE * 16) * IMGSCALE);
		if (INPUT->KeyDown('3'))
			app.maincam->SetWorldPos(Vector2(TILESCALE * 16, TILESCALE * 112) * IMGSCALE);
		if (INPUT->KeyDown('4'))
			app.maincam->SetWorldPos(Vector2(TILESCALE * 112, TILESCALE * 16) * IMGSCALE);
		if (INPUT->KeyDown('5'))
			app.maincam->SetWorldPos(Vector2(TILESCALE * 112, TILESCALE * 112) * IMGSCALE);
	}
	if (INPUT->KeyPress(VK_F5))
	{
		if (INPUT->KeyDown('1'))
		{
			Unit* temp = new Unit();
			temp->SetWorldPos(INPUT->GetWorldMousePos());
			temp->playerNum = 0;
			SSYSTEM->UnitPool.push_back(temp);
		}
		if (INPUT->KeyDown('2'))
		{
			Unit* temp = new Unit(UnitType::AIRUNIT, UnitName::DEVOURER);
			temp->SetWorldPos(INPUT->GetWorldMousePos());
			temp->playerNum = 0;
			SSYSTEM->UnitPool.push_back(temp);
		}
	}
	if (INPUT->KeyPress(VK_F6))
	{
		if (INPUT->KeyDown('1'))
		{
			Unit* temp = new Unit();
			temp->SetWorldPos(INPUT->GetWorldMousePos());
			temp->playerNum = 1;
			SSYSTEM->UnitPool.push_back(temp);
		}
		if (INPUT->KeyDown('2'))
		{
			Unit* temp = new Unit(UnitType::AIRUNIT, UnitName::DEVOURER);
			temp->SetWorldPos(INPUT->GetWorldMousePos());
			temp->playerNum = 1;
			SSYSTEM->UnitPool.push_back(temp);
		}
	}
	if (INPUT->KeyDown(VK_F7))
	{
		startPos = INPUT->GetWorldMousePos();
	}
	if (INPUT->KeyDown(VK_F8))
	{
		Vector2 ttmp = INPUT->GetWorldMousePos();
		pathway = PFINDER->FindCompletePath(map, startPos, ttmp);
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
	if (INPUT->KeyDown('Q'))
	{
		if (!BuildingMode)
			BuildingMode = true;
	}
	if (BuildingMode)
	{
		buildingImage.UpdateMatrix();
		dynamicMap->WorldPosToTileIdx(INPUT->GetWorldMousePos(), int2tmp);
		buildingImage.SetWorldPos(dynamicMap->Tiles[int2tmp.x][int2tmp.y].Pos - Vector2(32, 0));
	}
	SSYSTEM->Update();
}
float intervalTime = 0;
bool MoveLeftScreen = false, MoveRightScreen = false, MoveUpScreen = false, MoveDownScreen = false;

void Main::LateUpdate()
{
	if (BuildingMode)
	{
		if (INPUT->KeyDown(VK_LBUTTON))
		{
			if (SSYSTEM->CreateBuilding(UnitType::BUILDING, UnitName::COMMANDCENTER,
				buildingImageBox, 4, 3))
				BuildingMode = false;
		}
	}
	//if (INPUT->GetScreenMousePos().y < 690)
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
		if (INPUT->KeyUp(VK_LBUTTON) && abs(SelectAreaCol.GetScale().x) > 0 && abs(SelectAreaCol.GetScale().y) > 0)
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
	//ImGui::Text("AreaScale\n%f %f", SelectArea.GetScale().x, SelectArea.GetScale().y);
	//ImGui::Text("AreaWorldPos\n%f %f", SelectArea.GetWorldPos().x, SelectArea.GetWorldPos().y);
	//ImGui::Text("StartDragPos is Screen\n%f %f", startDragPos.x, startDragPos.y);
	ImGui::Text("ScreenMousePos\n%f %f", INPUT->GetScreenMousePos().x, INPUT->GetScreenMousePos().y);
	//ImGui::Text("Unit Pool : %d", SSYSTEM->UnitPool.size());
	//ImGui::Text("Unit Pool Selected : %d", SSYSTEM->UnitPoolSelect.size());
	//ImGui::Text("TileCol Unit\n");
	//for (size_t i = 0; i < SSYSTEM->TileMap->tileSize.y; i++)
	//{
	//	for (size_t j = 0; j < SSYSTEM->TileMap->tileSize.x; j++)
	//	{
	//		if (SSYSTEM->TileMap->Tiles[j][i].tileColInfo == TileCol::UNIT)
	//			ImGui::Text("%d %d\n", j, i);
	//	}
	//}

}
void Main::Render()
{
	mapImage.Render();
	if (showTileMap)
		map.Render();
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
				cursorMoveScreen[3].SetWorldPos(
					Vector2(app.maincam->GetWorldPos().x - app.GetHalfWidth(), app.maincam->GetWorldPos().y + app.GetHalfHeight()));
				cursorMoveScreen[3].Render();
			}
			else if (MoveRightScreen)
			{
				cursorMoveScreen[5].SetWorldPos(
					Vector2(app.maincam->GetWorldPos().x + app.GetHalfWidth(), app.maincam->GetWorldPos().y + app.GetHalfHeight()));
				cursorMoveScreen[5].Render();
			}
			else
			{
				cursorMoveScreen[4].SetWorldPos(
					Vector2(INPUT->GetWorldMousePos().x, app.maincam->GetWorldPos().y + app.GetHalfHeight()));
				cursorMoveScreen[4].Render();
			}
		}
		else if (MoveDownScreen)
		{
			if (MoveLeftScreen)
			{
				cursorMoveScreen[1].SetWorldPos(
					Vector2(app.maincam->GetWorldPos().x - app.GetHalfWidth(), app.maincam->GetWorldPos().y - app.GetHalfHeight()));
				cursorMoveScreen[1].Render();
			}
			else if (MoveRightScreen)
			{
				cursorMoveScreen[7].SetWorldPos(
					Vector2(app.maincam->GetWorldPos().x + app.GetHalfWidth(), app.maincam->GetWorldPos().y - app.GetHalfHeight()));
				cursorMoveScreen[7].Render();
			}
			else
			{
				cursorMoveScreen[0].SetWorldPos(
					Vector2(INPUT->GetWorldMousePos().x, app.maincam->GetWorldPos().y - app.GetHalfHeight()));
				cursorMoveScreen[0].Render();
			}
		}
		else if (MoveLeftScreen)
		{
			cursorMoveScreen[2].SetWorldPos(
				Vector2(app.maincam->GetWorldPos().x - app.GetHalfWidth(), INPUT->GetWorldMousePos().y));
			cursorMoveScreen[2].Render();
		}
		else if (MoveRightScreen)
		{
			cursorMoveScreen[6].SetWorldPos(
				Vector2(app.maincam->GetWorldPos().x + app.GetHalfWidth(), INPUT->GetWorldMousePos().y));
			cursorMoveScreen[6].Render();
		}
		else
		{
			cursor.SetWorldPos(INPUT->GetWorldMousePos());
			cursor.Render();
		}
	}

	if (showGridMap)
	{
		TestBox.color = Color(1, 0, 0);
		for (size_t i = 0; i < map.cluster.size(); i++)
		{
			for (size_t j = 0; j < map.cluster[i].size(); j++)
			{
				for (size_t k = 0; k < map.cluster[i][j]->nodes.size(); k++)
				{
					Vector2 vectors;
					vectors.x = map.cluster[i][j]->nodes[k]->x;
					vectors.y = map.cluster[i][j]->nodes[k]->y;
					TestBox.SetWorldPos(vectors * 16 + Vector2(8, 8));
					TestBox.Render();
				}
			}
		}
		TestBox.color = Color(0, 1, 0);
		TestBox.SetWorldPos(startPos);
		TestBox.Render();
		for (size_t i = 0; i < pathway.size(); i++)
		{
			TestBox.color = Color((float)i / (float)(pathway.size()), (float)i / (float)(pathway.size()), 0);
			TestBox.SetWorldPos(Vector2(pathway[i].first, pathway[i].second));
			TestBox.Render();
		}
		TestBox.color = Color(0, 1, 0);
		TestBox.SetWorldPos(INPUT->GetWorldMousePos());
		TestBox.Render();
	}
	if (BuildingMode)
	{
		buildingImage.Render();
		for (size_t i = 0; i < 12; i++)
		{
			if (!dynamicMap->GetTileWalkable(buildingImageBox[i].GetWorldPos()))
				buildingImageBox[i].color = Color(1, 0, 0, 0.25f);
			else
				buildingImageBox[i].color = Color(0, 1, 0, 0.25f);
			buildingImageBox[i].Render();
		}
	}
	if (showHelp)
	{
		HelpBox.SetWorldPos(app.maincam->GetWorldPos() + Vector2(-300, 225));
		HelpBox.Render();
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