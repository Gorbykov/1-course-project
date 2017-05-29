#include "Mask.h"
#include "Const.h"

Mask::Mask(int type)
{
	this->type = type;
}

Mask::~Mask()
{
}
Mat Mask::get(Mat img, int rows, int cols)
{
	switch (type)
	{
	case FULL:
		return Mat::ones(rows, cols, CV_8U);
		break;
	case ROBERTS:
		x = (Mat_<int>(2, 2) << -1, 0, 0, 1);
		y = (Mat_<int>(2, 2) << 0, -1, 1, 0);
		break;
	case SOBEL:
		x = (Mat_<int>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
		y = (Mat_<int>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
		break;
	case PREVIT:
		x = (Mat_<int>(3, 3) << -1, 0, 1, -1, 0, 1, -1, 0, 1);
		y = (Mat_<int>(3, 3) << -1, -1, -1, 0, 0, 0, 1, 1, 1);
		break;
	}
	Mat mask, i, iu;

	filter2D(img, x, -1, x);
	x = abs(x);

	filter2D(img, y, -1, y);
	y = abs(y);

	addWeighted(x, 0.5, y, 0.5, 0, i);

	i.convertTo(iu, CV_8U, 255);
	adaptiveThreshold(iu, mask, 1, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, blockSize, C);
	
	return mask;
}

void Mask::setParametrs(int C, int blockSize)
{
	this->C = C;
	this->blockSize = blockSize;
}