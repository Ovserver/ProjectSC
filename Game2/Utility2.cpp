#include "stdafx.h"
#include "Utility2.h"

bool Utility2::ShowCollider = true;

void Utility2::InitImage(ObImage& image, wstring fName, int xFrame, int yFrame, bool isRender)
{
	image.LoadFile(fName);
	image.SetPivot() = OFFSET_B;
	image.maxFrame.x = xFrame;
	image.maxFrame.y = yFrame;
	image.SetScale().x = image.imageSize.x / xFrame * IMGSCALE;
	image.SetScale().y = image.imageSize.y / yFrame * IMGSCALE;
	image.isVisible = isRender;
}
