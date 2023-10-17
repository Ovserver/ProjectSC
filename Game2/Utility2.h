#pragma once
enum DirState
{
	LB, B, RB,
	R, RT, T,
	LT, L
};
class Utility2
{
public:
	static bool ShowCollider;
public:
	static void InitImage(ObImage& image, wstring fName, int xFrame = 1, int yFrame = 1, bool isRender = true);
};

