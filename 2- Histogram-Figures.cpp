#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void DrawFigures() {

	//Object
	Mat img_original, img_modified;

	img_original = imread("ressource/lena.jpg");
	img_original.copyTo(img_modified);


	if (!img_original.data) {
		cout << "Error loading image" << endl;
		return ;
	}

	//Draw Circle
	Point center(img_modified.rows / 2, img_modified.cols / 2);
	int radius = 25;
	circle(img_modified, center, radius, Scalar(0, 255, 0));

	//Draw Line
	line(img_modified, center, Point(0, 0), Scalar(0, 255, 0));

	//Draw Rectangle
	rectangle(img_modified, center, Point(img_modified.rows, img_modified.cols), Scalar(0, 255, 0));

	//Create Windows Canvas
	namedWindow("Original", CV_WINDOW_AUTOSIZE);
	namedWindow("Modified", CV_WINDOW_AUTOSIZE);

	//Show Images
	imshow("Original", img_original);
	imshow("Modified", img_modified);

	waitKey(0);

	destroyWindow("Original");
	destroyWindow("Modified");
}

void DisplayHistogram() {

	//Object
	Mat img_original;

	img_original = imread("ressource/mandril.jpg", 0);

	if (!img_original.data) {
		cout << "Error loading image" << endl;
		return ;
	}

	//Create Windows Canvas
	namedWindow("Original", CV_WINDOW_AUTOSIZE);
	namedWindow("Histogram", CV_WINDOW_AUTOSIZE);

	//Initialize parameter
	int H_size = 256;
	float range[] = { 0, 255 };
	const float *ranges[] = { range };

	//Calculate Histogram
	Mat hist;
	calcHist(&img_original, 1, 0, Mat(), hist, 1, &H_size, ranges, true, false);

	//Show calculated hist in command windows
	double total;

	total = img_original.rows * img_original.cols;
	for (int h = 0; h < H_size; h++) {
		float binVal = hist.at<float>(h);
		cout << " " << binVal;
	}

	//Plot Histogram
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / H_size);

	Mat HistImage(hist_h, hist_w, CV_8UC1, Scalar(0, 0, 0));

	normalize(hist, hist, 0, HistImage.rows, NORM_MINMAX, -1, Mat());

	for (int i = 0; i < H_size; i++) {

		line(HistImage, Point(bin_w*(i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
	}

	//Show Images
	imshow("Original", img_original);
	imshow("Histogram", HistImage);

	waitKey(0);

	destroyAllWindows();
}