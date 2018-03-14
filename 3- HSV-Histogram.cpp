#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void NumeroCanal () {
	//Object
	Mat gray_img, color_img;

	//Load gray picture
	gray_img = imread("ressource/mandril.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!gray_img.data) {
		cout << "Error loading image" << endl;
		return ;
	}

	//Load color picture
	color_img = imread("ressource/mandril_c.jpg", CV_LOAD_IMAGE_COLOR);
	if (!color_img.data) {
		cout << "Error loading image" << endl;
		return ;
	}

	//Display informations channels
	cout << "gray channels : " << gray_img.channels() << endl;
	cout << "color channels : " << color_img.channels() << endl;

	//Create Windows
	namedWindow("gray", CV_WINDOW_AUTOSIZE);
	namedWindow("color", CV_WINDOW_AUTOSIZE);

	//Display Picture
	imshow("gray", gray_img);
	imshow("color", color_img);

	waitKey(0); //Any keys pressed
	destroyAllWindows();
}

void SplitImageColor() {

	//Object
	Mat  color_img;

	//Load color picture
	color_img = imread("ressource/mandril_c.jpg", CV_LOAD_IMAGE_COLOR);
	if (!color_img.data) {
		cout << "Error loading image" << endl;
		return ;
	}

	//Change from bgr to rgb
	cvtColor(color_img, color_img, CV_BGR2RGB);

	//Split color image - required containers
	Mat r_channel(color_img.size(), CV_8UC1);
	Mat g_channel(color_img.size(), CV_8UC1);
	Mat b_channel(color_img.size(), CV_8UC1);

	Mat arraychannels[] = { r_channel, g_channel, b_channel };
	split(color_img, arraychannels);

	//Create Windows
	namedWindow("color", CV_WINDOW_AUTOSIZE);
	namedWindow("R", CV_WINDOW_AUTOSIZE);
	namedWindow("G", CV_WINDOW_AUTOSIZE);
	namedWindow("B", CV_WINDOW_AUTOSIZE);

	//Display Picture
	imshow("color", color_img);
	imshow("R", r_channel);
	imshow("G", g_channel);
	imshow("B", b_channel);

	waitKey(0); //Any keys pressed
	destroyAllWindows();
}

void DisplayHSVHistogram() {

	//Object
	Mat color_img, hsv_img;

	color_img = imread("ressource/mandril_c.jpg", CV_LOAD_IMAGE_COLOR);
	if (!color_img.data) {
		cout << "Error loading image" << endl;
		return ;
	}

	//Change from bgr to rgb
	cvtColor(color_img, hsv_img, CV_BGR2RGB);



	//Split color image - required containers
	Mat h_channel(hsv_img.size(), CV_8UC1);
	Mat s_channel(hsv_img.size(), CV_8UC1);
	Mat v_channel(hsv_img.size(), CV_8UC1);

	Mat arraychannels[] = { h_channel, s_channel, v_channel };
	split(hsv_img, arraychannels);

	//Initialize parameter
	int H_size = 256;
	float range[] = { 0, 255 };
	const float *ranges[] = { range };

	//Calculate Histogram
	Mat h_hist, s_hist, v_hist;
	calcHist(&h_channel, 1, 0, Mat(), h_hist, 1, &H_size, ranges, true, false);
	calcHist(&s_channel, 1, 0, Mat(), s_hist, 1, &H_size, ranges, true, false);
	calcHist(&v_channel, 1, 0, Mat(), v_hist, 1, &H_size, ranges, true, false);

	//Plot Histogram
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / H_size);
	Mat HistImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	//Normalize the result [0 Image.rows]
	normalize(h_hist, h_hist, 0, HistImage.rows, NORM_MINMAX, -1, Mat());
	normalize(s_hist, s_hist, 0, HistImage.rows, NORM_MINMAX, -1, Mat());
	normalize(v_hist, v_hist, 0, HistImage.rows, NORM_MINMAX, -1, Mat());

	for (int i = 0; i < H_size; i++) {
		line(HistImage, Point(bin_w*(i - 1), hist_h - cvRound(h_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(h_hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);

		line(HistImage, Point(bin_w*(i - 1), hist_h - cvRound(s_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(s_hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);

		line(HistImage, Point(bin_w*(i - 1), hist_h - cvRound(v_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(v_hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
	}


	//Create Windows Canvas
	namedWindow("Color", CV_WINDOW_AUTOSIZE);
	namedWindow("H", CV_WINDOW_AUTOSIZE);
	namedWindow("S", CV_WINDOW_AUTOSIZE);
	namedWindow("V", CV_WINDOW_AUTOSIZE);
	namedWindow("Histogram - Red = H - Green = S - Blue = V", CV_WINDOW_AUTOSIZE);

	//Show Images
	imshow("Color", color_img);
	imshow("H", h_channel);
	imshow("S", s_channel);
	imshow("V", v_channel);
	imshow("Histogram - Red = H - Green = S - Blue = V", HistImage);

	waitKey(0);
	destroyAllWindows();
}
