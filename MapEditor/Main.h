#pragma once
class Main : public Scene
{
private:
	ObTileMap	tileMap;
	ObRect		LineX, LineY;
	float		camSpeed;
	float		ImageSize;


	int			brushImgIdx;
	Int2		brushFrame;
	int			brushState;
	Color		brushColor;

public:
	Main();
	~Main();
	virtual void Init() override;
	virtual void Release() override; //해제
	virtual void Update() override;
	virtual void LateUpdate() override;//갱신
	virtual void Render() override;
	virtual void ResizeScreen() override;
};
