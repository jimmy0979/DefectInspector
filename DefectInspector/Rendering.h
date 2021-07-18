#pragma once

#include <math.h>

using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

class Rendering
{
private:
public:
	static void BlendPictures(Bitmap^ bg, Bitmap^ front, int deltaX, int deltaY);
	static void BlendPictures(PictureBox^ back, PictureBox^ front);
};
