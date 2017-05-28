#include "DeltaFromBorder.h"

DeltaFromBorder::DeltaFromBorder()
{
}

DeltaFromBorder::DeltaFromBorder(Mask *mCreator, int window)
{
	this->mCreator = mCreator;
	this->window = window;
}

DeltaFromBorder::~DeltaFromBorder()
{
}

Point2i DeltaFromBorder::get(Mat a, Mat b)
{
	Point2i delta(0, 0);
	if (b.empty())
	{
		return delta;
	}
	Mat mask, c, bestC;
	Rect2i roiA, roiB;
	a = mCreator->get(a, a.rows, a.cols);
	b = mCreator->get(b, b.rows, b.cols);

	double maxRate = 0, rate;
	for (int i = -MAX_OFFSET.x; i < MAX_OFFSET.x; i++)
	{
		for (int j = -MAX_OFFSET.y; j < MAX_OFFSET.y; j++)
		{
			roiA.x = p(i);
			roiA.y = p(j);
			roiA.width = a.cols + n(i) - roiA.x;
			roiA.height = a.rows + n(j) - roiA.y;

			roiB.x = -n(i);
			roiB.y = -n(j);
			roiB.width = b.cols - p(i) - roiB.x;
			roiB.height = b.rows - p(j) - roiB.y;

			c = (a(roiA) + b(roiB));
			rate = (double)count(c, 2) / (double)(c.cols * c.rows);
			if (rate > maxRate)
			{
				maxRate = rate;
				delta.x = i;
				delta.y = j;
				bestC = c;
			}
		}
	}
	if (DF)
	{
		Mat buf;
		resize(bestC, buf, Size(0, 0), 2, 2);
		imshow("w", buf * 127);
		waitKey(1); 
	}
	return delta;
}

int DeltaFromBorder::count(Mat a, int n)
{
	int k = 0;
	int nRows = a.rows;
	int nCols = a.cols;
	uchar* p = a.data;

	for (unsigned int i = 0; i < nCols*nRows; ++i)
	{
		if (*p++ == n) k++;
	}
	return k;
}