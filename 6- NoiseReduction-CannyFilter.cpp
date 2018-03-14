#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void Noise_Reduction() {

	Mat original_img, filtered_img;

	original_img = imread("ressource/moon_gaussian_noise.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!original_img.data) {
		cout << "error loading image" << endl;
	}

	//Noise reduction by average of neightbours determined by the mask
	//blur(original_img, filtered_img, Size(3, 3));

	//Gaussian sigma values. The mask dimension musk be squared(3x3 ,9x9...)
	int sigma_x = 0;
	int sigma_y = 0;
	GaussianBlur(original_img, filtered_img, Size(9, 9), sigma_x, sigma_y);

	//Median Size parameter must be odd and positive (1,3,5..)
	//int size = 5;
	//medianBlur(original_img, filtered_img, size);

	//Windows for all Imagen
	namedWindow("Original picture", CV_WINDOW_AUTOSIZE);
	namedWindow("Filtered picture", CV_WINDOW_AUTOSIZE);

	//Show image in the name of the windows
	imshow("Original picture", original_img);
	imshow("Filtered picture", filtered_img);

	waitKey(0);
	original_img.release();
	filtered_img.release();
	destroyAllWindows();
}


void Sober_Detection_Borde() {

	Mat original_img = imread("ressource/lena.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	if (!original_img.data) {
		cout << "error loading image" << endl;
	}

	Mat filtered_img(original_img.size(), CV_16SC1);
	Mat show_filtered_img_X(original_img.size(), CV_8UC1);
	Mat show_filtered_img_Y(original_img.size(), CV_8UC1);

	//Sobel edge Detector
	//Gx first orden
	//Gy None
	// Mask 3x3
	Sobel(original_img, filtered_img, CV_16SC1, 1, 0, 3);
	filtered_img.convertTo(show_filtered_img_X, CV_8UC1);

	//Solbel edge detector
	//Gx first orden
	//Gy None
	// Mask 3x3
	Sobel(original_img, filtered_img, CV_16SC1, 0, 1, 3);
	filtered_img.convertTo(show_filtered_img_Y, CV_8UC1);

	//Windows for all Imagen
	namedWindow("Original picture", CV_WINDOW_AUTOSIZE);
	namedWindow("Filtered picture X", CV_WINDOW_AUTOSIZE);
	namedWindow("Filtered picture Y", CV_WINDOW_AUTOSIZE);

	//Show image in the name of the windows
	imshow("Original picture", original_img);
	imshow("Filtered picture X", show_filtered_img_X);
	imshow("Filtered picture Y", show_filtered_img_Y);

	waitKey(0);
	original_img.release();
	show_filtered_img_X.release();
	show_filtered_img_Y.release();
	destroyAllWindows();
}

void Canny_Detection_Borde() {

	Mat original_img;
	Mat filtered_img(original_img.size(), original_img.type());

	original_img = imread("ressource/googleCar.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if (!original_img.data) {
		cout << "error loading image" << endl;
	}

	//Reduce noise with a kernel 3x3
	blur(original_img, original_img, Size(3, 3));

	//Canny 2:1 : 3x3
	int low_threshold = 50;
	int high_threshold = low_threshold * 2;
	int kernel_size = 3;
	
	Canny(original_img, filtered_img, low_threshold, high_threshold, kernel_size);

	//Windows for all Imagen
	namedWindow("Original picture", CV_WINDOW_AUTOSIZE);
	namedWindow("Filtered picture", CV_WINDOW_AUTOSIZE);


	//Show image in the name of the windows
	imshow("Original picture", original_img);
	imshow("Filtered picture", filtered_img);

	waitKey(0);
	original_img.release();
	filtered_img.release();
	destroyAllWindows();

}
