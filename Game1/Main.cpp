#include "stdafx.h"
#include "Bullet.h"
#include "Player.h"
#include "Main.h"

Main::Main()
{
	

}

Main::~Main()
{

}

void Main::Init()
{
	bg.LoadFile(L"bg.png");
	bg.SetPivot() = OFFSET_B;

	SOUND->AddSound("GameOver.wav","GameOver",true);
	SOUND->AddSound("Coin.wav","Coin");
	SOUND->AddSound("FireBall.wav","FireBall");
	//SOUND->AddSound("FireBall.wav","FireBall2");
	
	bg.SetWorldPosY(-400.0f);
	bg.SetScale().x = 448;
	bg.SetScale().y = 2144.0f * 2.0f;

	vol1 = 1.0f;
	vol2 = 1.0f;
	mainPlayer.Init();
}

void Main::Release()
{
   
}

void Main::Update()
{
	if (ImGui::SliderFloat("MASTERVOL", &app.soundScale, 0.0f, 1.0f))
	{
		SOUND->SetMasterVolume();
	}
	if (ImGui::SliderFloat("BGMVOL", &vol1, 0.0f, 1.0f))
	{
		SOUND->SetVolume("GameOver", vol1);
	}


	if (ImGui::Button("BGMPLAY"))
	{
		SOUND->Play("GameOver");
	}
	ImGui::SameLine();
	if (ImGui::Button("BGMSTOP"))
	{
		SOUND->Stop("GameOver");
	}
	ImGui::SameLine();
	if (ImGui::Button("BGMPAUSE"))
	{
		SOUND->Pause("GameOver");
	}
	ImGui::SameLine();
	if (ImGui::Button("BGMRESUME"))
	{
		SOUND->Resume("GameOver");
	}
	if (ImGui::SliderFloat("FIREVOL", &vol2, 0.0f, 1.0f))
	{
		SOUND->SetVolume("FireBall", vol2);
	}

	if (ImGui::Button("FIREPLAY"))
	{
		SOUND->Stop("FireBall");
		SOUND->Play("FireBall");
	}




	bg.uv.y -= DELTA/ bg.GetScale().y * 200.0f;
	bg.uv.w -= DELTA/ bg.GetScale().y * 200.0f;

	mainPlayer.Update();

}

void Main::LateUpdate()
{

	
}

void Main::Render()
{
	bg.Render();
	mainPlayer.Render();
}

void Main::ResizeScreen()
{

}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR param, int command)
{
    //창,프로그램 이름
	app.SetAppName(L"Game12");
	//등록된 인스턴스
    app.SetInstance(instance);
	//창 크기
	app.InitWidthHeight(448,800.0f);
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