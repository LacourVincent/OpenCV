#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;


void Umbralizacion()
{
	// Objects
	Mat img_src;
	long hist_vector[256];
	int threshold_value;

	// Load image from disk
	img_src = imread("ressource/lena.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!img_src.data) {
		cout << "error loading image" << endl;
	}

	// Calculate histogram
	Mat hist;
	// Establish the number of bins
	int histSize = 256;
	float range[] = { 0, 256 };
	const float* histRange = { range };

	calcHist(&img_src, 1, 0, Mat(), hist, 1, &histSize, &histRange, true, false);

	// Plot histogram
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	/// Normalize the result to [ 0, histImage.rows ]
	normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	/// Draw 
	hist_vector[0] = 0;
	for (int i = 1; i < histSize; i++)
	{
		hist_vector[i] = hist.at<float>(i);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
	}


	// Create image for thresholded image
	Mat im_thresholded(img_src.size(), CV_8UC1);
	
	//thresholding1.jpg = threshold_value = 127
	//thresholding2.jpg = threshold_value = 159
	//thresholding3.jpg = threshold_value = 164
	//lena.jpg = threshold_value = 
	threshold_value = 112;

	threshold(img_src, im_thresholded, threshold_value, 255, THRESH_BINARY);
	line(histImage, Point(bin_w*threshold_value, 0), Point(bin_w*threshold_value, 400), Scalar(0, 255, 0), 2, 8, 0);

	// Create window canvas to show image
	namedWindow("original", CV_WINDOW_AUTOSIZE);
	namedWindow("Histogram", CV_WINDOW_AUTOSIZE);
	namedWindow("Thresholded", CV_WINDOW_AUTOSIZE);

	// Show image in the name of the window
	imshow("original", img_src);
	imshow("Histogram", histImage);
	imshow("Thresholded", im_thresholded);
	// Function for show the image in ms.
	// 0 means wait until keyboard is pressed
	waitKey(0);

	// Free memory

	img_src.release();
	histImage.release();
	im_thresholded.release();
	destroyWindow("original");
	
}

void TransMorpho() {

	// Objects
	Mat src_img, erode_img, dilate_img, morph_img;
	Mat kernel;
	// Load image from disk
	src_img = imread("ressource/open_close.png");
	if (!src_img.data) {
		cout << "error loading image" << endl;
		system("pause");
	}

	// Create kernel of size 3x3. Try different sizes
	kernel = getStructuringElement(MORPH_RECT, Size(10, 10));
	//kernel = getStructuringElement(MORPH_CROSS, Size(10, 10));
	//kernel = getStructuringElement(MORPH_ELLIPSE, Size(10, 10));
	cout << kernel << endl;

	// Dilate
	dilate(src_img, dilate_img, kernel);

	// Erode
	erode(src_img, erode_img, kernel);

	// Morphologic transformation: could be:
	// - MORPH_OPEN 
	// - MORPH_CLOSE
	// - MORPH_GRADIENT
	// - MORPH_TOPHAT
	// - MORPH_BLACKHAT

	morphologyEx(src_img, morph_img, MORPH_OPEN, kernel);

	// Show image in the name of the window
	imshow("original", src_img);
	//imshow("dilate", dilate_img);
	//imshow("erode", erode_img);
	imshow("morphology", morph_img);

	waitKey(0);

	// Free memory
	src_img.release();
	dilate_img.release();
	erode_img.release();
	morph_img.release();
	destroyAllWindows();
}