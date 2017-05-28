#include "DeltaBruteForce.h"

#include <opencv2\highgui\highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
DeltaBruteForce::DeltaBruteForce()
{
}

DeltaBruteForce::DeltaBruteForce(Mask *mCreator, int window)
{
	this->mCreator = mCreator;
	this->window = window;
}

DeltaBruteForce::~DeltaBruteForce()
{
}

Point2i DeltaBruteForce::get(Mat a, Mat b)
{
	Point2i delta(0, 0);
	if (b.empty())
	{
		return delta;
	}
	Mat mask, c;
	Rect2i roiA, roiB;
	mask = mCreator->get(a, a.rows, a.cols);
	if (DF)
	{
		Mat buf;
		resize(mask, buf, Size(0, 0), 2, 2);
		imshow("w", buf * 255);
		waitKey(1); 
	}
	double minRate = 2, rate;
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

			absdiff(a(roiA), b(roiB), c);

			rate = mean(c, mask(roiA))[0];
			if (rate < minRate)
			{
				minRate = rate;
				delta.x = i;
				delta.y = j;
			}
		}
	}
	return delta;
}