#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void MinMaxLoc() {

	Mat original_img = imread("ressource/rayosx.tif", CV_LOAD_IMAGE_GRAYSCALE);

	if (!original_img.data) {
		cout << "error loading image" << endl;
	}

	//Initialize histogram parameters
	//Establish the number of bins
	int HistSize = 256;
	//Set the range GryScale 0-255
	float range[] = { 0 , 256 };
	const float* histRange = { range };
	//Set histogram image
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / HistSize);
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	//Calculate histogram
	Mat original_hist, normalized_hist;
	calcHist(&original_img, 1, 0, Mat(), original_hist, 1, &HistSize, &histRange, true, false);

	//Print the values of the original histogram on console
	cout << "Original histogram" << endl;
	for (int h = 0; h < HistSize; h++) {
		float binVal = original_hist.at<float>(h);
		cout << " " << binVal;
	}
	cout << endl;

	//MinMaxLocation
	Point min_point, max_point;
	double min, max;
	minMaxLoc(original_hist, &min, &max, &min_point, &max_point, Mat());

	//Print min and max value and its location
	cout << "minimum value : " << min << "at index : " << min_point.y << endl;
	cout << "maximum value : " << max << "at index : " << max_point.y << endl;

	//Normalize the result to [0 , histImage.Rows ]
	normalize(original_hist, normalized_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	//Print the values on console
	cout << "Normalized histogram" << endl;
	for (int h = 0; h < HistSize; h++) {
		float binVal = normalized_hist.at<float>(h);
		cout << " " << binVal;
	}
	cout << endl;

	minMaxLoc(normalized_hist, &min, &max, &min_point, &max_point, Mat());

	//Print min and max value and its location
	cout << "minimum value : " << min << "at index : " << min_point.y << endl;
	cout << "maximum value : " << max << "at index : " << max_point.y << endl;

	//Plot Hist
	for (int i = 1; i<HistSize; i++) {

		//Line of width 2(bin_w =512 width /256 gray scale value 9 filled
		line(histImage,
			Point(bin_w*(i), hist_w),
			Point(bin_w*(i), hist_h - cvRound(normalized_hist.at<float>(i))),
			Scalar(255, 0, 0), bin_w, 8, 0);
	}

	namedWindow("Original picture", CV_WINDOW_AUTOSIZE);
	namedWindow("Histogram", CV_WINDOW_AUTOSIZE);

	//Show image in the name of the windows
	imshow("Original picture", original_img);
	imshow("Histogram", histImage);



	waitKey(0);
	histImage.release();
	original_hist.release();
	destroyAllWindows();


}

void HistogramEqualization() {


	Mat original_img, equalized_img;

	original_img = imread("ressource/rayosx.tif", CV_LOAD_IMAGE_GRAYSCALE);

	if (!original_img.data) {
		cout << "error loading image" << endl;
	}

	//Initialize histogram parameters
	//Establish the number of bins
	int HistSize = 256;
	//Set the range GryScale 0-255
	float range[] = { 0 , 256 };
	const float* histRange = { range };
	//Set histogram image
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / HistSize);

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
	Mat equalizedHistImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
	//Calculate histogram
	Mat original_hist, normalized_hist, equalized_hist, equalized_normalized_hist;
	calcHist(&original_img, 1, 0, Mat(), original_hist, 1, &HistSize, &histRange, true, false);

	//Print the values of the original histogram on console
	cout << "Original histogram" << endl;
	for (int h = 0; h < HistSize; h++) {
		float binVal = original_hist.at<float>(h);
		cout << " " << binVal;
	}
	cout << endl;

	//Normalize the result to [0 , histImage.Rows ]
	normalize(original_hist, normalized_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	//Print the values on console
	cout << "Normalized histogram" << endl;
	for (int h = 0; h < HistSize; h++) {
		float binVal = normalized_hist.at<float>(h);
		cout << " " << binVal;
	}
	cout << endl;

	//Equalize histogram from a grayscale image
	equalizeHist(original_img, equalized_img);
	calcHist(&equalized_img, 1, 0, Mat(), equalized_hist, 1, &HistSize, &histRange, true, false);

	//Print the values on console
	cout << "Equalized histogram" << endl;
	for (int h = 0; h < HistSize; h++) {
		float binVal = equalized_hist.at<float>(h);
		cout << " " << binVal;
	}
	cout << endl;

	normalize(equalized_hist, equalized_normalized_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	//Print the values on console
	cout << "Equalized normalized histogram" << endl;
	for (int h = 0; h < HistSize; h++) {
		float binVal = equalized_normalized_hist.at<float>(h);
		cout << " " << binVal;
	}
	cout << endl;

	//Plot Hist
	for (int i = 1; i < HistSize; i++) {

		line(histImage,
			Point(bin_w*(i), hist_w),
			Point(bin_w*(i), hist_h - cvRound(normalized_hist.at<float>(i))),
			Scalar(255, 0, 0), bin_w, 8, 0);

		line(equalizedHistImage,
			Point(bin_w*(i), hist_w),
			Point(bin_w*(i), hist_h - cvRound(equalized_normalized_hist.at<float>(i))),
			Scalar(255, 0, 0), bin_w, 8, 0);
	}

	//Windows for all the image
	namedWindow("Original Picture", CV_WINDOW_AUTOSIZE);
	namedWindow("Equalized Picture", CV_WINDOW_AUTOSIZE);
	namedWindow("Original Histogram", CV_WINDOW_AUTOSIZE);
	namedWindow("Equalized Histogram", CV_WINDOW_AUTOSIZE);

	imshow("Original Picture", original_img);
	imshow("Equalized Picture", equalized_img);
	imshow("Original Histogram", histImage);
	imshow("Equalized Histogram", equalizedHistImage);

	waitKey(0);
	//free Memory
	original_img.release();
	equalized_img.release();
	histImage.release();
	equalized_img.release();
	original_hist.release();
	normalized_hist.release();
	equalized_normalized_hist.release();
	destroyAllWindows();
}