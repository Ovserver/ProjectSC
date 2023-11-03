#include "stdafx.h"
#include "Main.h"

Main::Main()
{
	

}

Main::~Main()
{

}
ObImage mapImage;
void Main::Init()
{
	ShowCursor(true);

	LineX.color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	LineX.SetPivot() = OFFSET_N;
	LineX.SetScale().x = 20000.0f;
	LineX.SetScale().y = 5.0f;

	LineY.color = Color(0.0f, 1.0f, 0.0f, 1.0f);
	LineY.SetPivot() = OFFSET_N;
	LineY.SetScale().x = 5.0f;
	LineY.SetScale().y = 20000.0f;

	brushImgIdx = 0;
	camSpeed = 1200.0f;
	ImageSize = 300.0f;
	brushColor = Color(0.5f, 0.5f, 0.5f, 0.5f);
	brushState = 0;

	//
	/*tileMap.SetTile(Int2(0, 0), Int2(6, 3));
	tileMap.SetTile(Int2(0, 1), Int2(6, 3));
	tileMap.SetTile(Int2(1, 0), Int2(6, 3));
	tileMap.UpdateBuffer();*/

	mapImage.LoadFile(L"maps/map.bmp");
	mapImage.SetScale().x = mapImage.imageSize.x * IMGSCALE;
	mapImage.SetScale().y = mapImage.imageSize.y * IMGSCALE;
	mapImage.SetPivot() = OFFSET_LB;
}

void Main::Release()
{
   
}
bool showCursor = true;
void Main::Update()
{	
	if (INPUT->KeyDown(VK_F2))
		ShowCursor(showCursor = !showCursor);
	ImGui::Begin("TileMap Editor");

	Vector2 Pos = tileMap.GetWorldPos();
	if (ImGui::InputFloat2("TileMapPos", (float*)&Pos))
	{
		tileMap.SetWorldPos(Pos);
	}

	Vector2 Scale1 = tileMap.GetScale();
	if (ImGui::InputFloat2("TileMapScale", (float*)&Scale1))
	{
		tileMap.SetScale() = Scale1;
	}
	//TileSize
	Int2 tileSize = tileMap.tileSize;
	if (ImGui::SliderInt2("TileSize", (int*)&tileSize, 1, 100))
	{
		tileMap.ResizeTile(tileSize);
	}

	ImGui::SliderFloat("CamSpeed", &camSpeed, 0.0f, 500.0f);
	//캠값 받아오기
	Vector2 Scale = app.maincam->GetScale();
	if (ImGui::InputFloat2("CamScale",(float*)&Scale))
	{
		//슬라이더를 건들였을때
		app.maincam->SetScale() = Scale;
	}

	for (int i = 0; i < 2; i++)
	{
		string str = "Texture" + to_string(i);
		if (GUI->FileImGui(str.c_str(), str.c_str(),
			".jpg,.png,.bmp,.dds,.tga", "../Contents/Images"))
		{
			string path = ImGuiFileDialog::Instance()->GetCurrentPath();
			Utility::Replace(&path, "\\", "/");

			//현재경로가 Images 안에 없을떄
			if (path.find("/Images/") != -1)
			{

				size_t tok = path.find("/Images/") + 8;
				path = path.substr(tok, path.length())
					+ "/" + ImGuiFileDialog::Instance()->GetCurrentFileName();
			}
			//현재경로가 image일때
			else
			{
				path = ImGuiFileDialog::Instance()->GetCurrentFileName();
			}
			SafeDelete(tileMap.tileImages[i]);
			wstring wImgFile = L"";
			wImgFile.assign(path.begin(), path.end());
			tileMap.tileImages[i] = new ObImage();
			tileMap.tileImages[i]->LoadFile(wImgFile);
			break;
		}
		if (i < 1)
		{
			ImGui::SameLine();
		}
	}

	if (ImGui::InputInt("ImgIdx", &brushImgIdx))
	{
		brushImgIdx = Utility::Saturate(brushImgIdx, 0, 3);

		if (not tileMap.tileImages[brushImgIdx])
		{
			brushImgIdx = 0;
		}
	}

	ImGui::InputInt2("maxFrame", (int*)&tileMap.tileImages[brushImgIdx]->maxFrame);

	ImGui::SliderFloat("viewImgSize", &ImageSize, 0.0f, 1000.0f);
	Int2 MF = tileMap.tileImages[brushImgIdx]->maxFrame;
	ImVec2 size;
	size.x = ImageSize / (float)MF.x;
	size.y = ImageSize / (float)MF.y;
	ImVec2 LT, RB;
	int index = 0;
	for (UINT i = 0; i < MF.y; i++)
	{
		for (UINT j = 0; j < MF.x; j++)
		{
			if (j != 0)
			{
				//같은줄에 배치
				ImGui::SameLine();
			}
			//텍스쳐 좌표
			LT.x = 1.0f / MF.x * j;
			LT.y = 1.0f / MF.y * i;
			RB.x = 1.0f / MF.x * (j + 1);
			RB.y = 1.0f / MF.y * (i + 1);

			ImGui::PushID(index);
			if (ImGui::ImageButton((void*)tileMap.tileImages[brushImgIdx]->GetSRV()
				, size, LT, RB))
			{
				brushFrame.x = j;
				brushFrame.y = i;
			}
			index++;
			ImGui::PopID();
		}
	}

	//TileState
	ImGui::SliderInt("TileState", &brushState, TILE_NONE, TILE_SIZE - 1);
	//TileColor
	ImGui::ColorEdit4("TileColor", (float*)&brushColor, ImGuiColorEditFlags_PickerHueWheel);

	{
		string str = "SAVE";
		if (GUI->FileImGui(str.c_str(), str.c_str(),
			".txt", "../Contents/TileMap"))
		{
			string path = ImGuiFileDialog::Instance()->GetCurrentPath();
			Utility::Replace(&path, "\\", "/");
			//현재경로가 Images 안에 없을떄
			if (path.find("/TileMap/") != -1)
			{

				size_t tok = path.find("/TileMap/") + 9;
				path = path.substr(tok, path.length())
					+ "/" + ImGuiFileDialog::Instance()->GetCurrentFileName();
			}
			//현재경로가 image일때
			else
			{
				path = ImGuiFileDialog::Instance()->GetCurrentFileName();
			}
			tileMap.file = path;
			tileMap.Save();
		}
	}
	{
		string str = "LOAD";
		if (GUI->FileImGui(str.c_str(), str.c_str(),
			".txt", "../Contents/TileMap"))
		{
			string path = ImGuiFileDialog::Instance()->GetCurrentPath();
			Utility::Replace(&path, "\\", "/");
			//현재경로가 Images 안에 없을떄
			if (path.find("/TileMap/") != -1)
			{

				size_t tok = path.find("/TileMap/") + 9;
				path = path.substr(tok, path.length())
					+ "/" + ImGuiFileDialog::Instance()->GetCurrentFileName();
			}
			//현재경로가 image일때
			else
			{
				path = ImGuiFileDialog::Instance()->GetCurrentFileName();
			}
			tileMap.file = path;
			tileMap.Load();
		}
	}


	ImGui::End();

	if (INPUT->KeyPress('W') || INPUT->KeyPress(VK_UP))
	{
		app.maincam->MoveWorldPos( UP * camSpeed * DELTA);
	}
	if (INPUT->KeyPress('S') || INPUT->KeyPress(VK_DOWN))
	{
		app.maincam->MoveWorldPos(DOWN * camSpeed * DELTA);
	}
	if (INPUT->KeyPress('A') || INPUT->KeyPress(VK_LEFT))
	{
		app.maincam->MoveWorldPos(LEFT * camSpeed * DELTA);
	}
	if (INPUT->KeyPress('D') || INPUT->KeyPress(VK_RIGHT))
	{
		app.maincam->MoveWorldPos(RIGHT * camSpeed * DELTA);
	}

	

}

void Main::LateUpdate()
{
	//브러시 클릭할때
	if (INPUT->KeyPress(VK_RBUTTON))
	{
		
		//tileMap.

		//마우스가 현재 타일맵 위에 있으면
		Int2 tileIdx;
		if (tileMap.WorldPosToTileIdx(INPUT->GetWorldMousePos(), tileIdx))
		{
			tileMap.SetTile(tileIdx, brushFrame, brushImgIdx,brushState,brushColor);
			tileMap.UpdateBuffer();
		}

		
	}

	
}

void Main::Render()
{
	mapImage.Render();
	LineX.Render();
	LineY.Render();
	tileMap.Render();
}

void Main::ResizeScreen()
{

}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR param, int command)
{
    //창,프로그램 이름
	app.SetAppName(L"MapEditor");
	//등록된 인스턴스
    app.SetInstance(instance);
	//창 크기
	app.InitWidthHeight(1200,800.0f);
	//배경색
    app.background = Color(0.3, 0.3, 0.3);
	WIN->Create();
	Main* main = new Main();
	//Loop
	int wParam = (int)WIN->Run(main);
	//Loop End
	SafeDelete(main);
	WIN->Destroy();
	WIN->DeleteSingleton();
	
	return wParam;
}