#include "Rendering.h"

void Rendering::BlendPictures(Bitmap^ bg, Bitmap^ front, int deltaX, int deltaY) {
	if (bg == nullptr || front == nullptr)	return;
	
	for (int y = 0; y < front->Height; y++) {
		for (int x = 0; x < front->Width; x++) {
			if (front->GetPixel(x, y).A < 255) {
				Color newColor = bg->GetPixel(x + deltaX, y + deltaY);
				front->SetPixel(x, y, newColor);
			}
		}
	}
}

void Rendering::BlendPictures(PictureBox^ back, PictureBox^ front) {
	int leftDiifference = abs(back->Left - front->Left);
	int topDifference = abs(back->Top - front->Top);

	BlendPictures((Bitmap^)back->Image, (Bitmap^)front->Image, leftDiifference, topDifference);
}