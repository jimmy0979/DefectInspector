#include "legendForm.h"

System::Void DefectInspector::legendForm::show_legend(cv::Mat* img)
{
	System::IntPtr ptr(img->ptr());
	System::Drawing::Bitmap^ res = gcnew System::Drawing::Bitmap(img->cols, img->rows, img->step, System::Drawing::Imaging::PixelFormat::Format24bppRgb, ptr);
	imgLegend->Image = res;
}
