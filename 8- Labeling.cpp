#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;


void labeling()
{
	// Objects
	Mat original_img;
	// Load image from disk

	original_img = imread("ressource/binary_image.png", CV_LOAD_IMAGE_GRAYSCALE);
	if (!original_img.data) {
		cout << "error loading image" << endl;
		system("pause");
	}

	int number_of_labels;
	// number_of_labels - Is included the background as label 0.
	Mat labels;
	// destination labeled image (not equalized)

	Mat stats;
	// STATS:
	// CC_STAT_LEFT The leftmost (x) coordinate which is the inclusive start of the bounding box.
	// CC_STAT_TOP The topmost (y) coordinate which is the inclusive start of the bounding box.
	// CC_STAT_WIDTH The horizontal size of the bounding box
	// CC_STAT_HEIGHT The vertical size of the bounding box
	// CC_STAT_AREA The total area (in pixels) of the connected component

	// In the bounding box is included the shape of the binary object.

	Mat centroids;
	// Centroids – floating point centroid (x,y) output for each label, including the background label

	int connectivity = 8;
	// Connectivity - Determine the neighbors of the image 
	// 4 connectivity  -  8 connectivity
	//	0 1 0				1 1 1
	//	1 X 1				1 X 1
	//	0 1 0				1 1 1

	number_of_labels = connectedComponentsWithStats(original_img, labels, stats, centroids, connectivity);

	cout << "number_of_labels: " << number_of_labels << endl;
	cout << "stats:" << endl << stats << endl;
	cout << "centroids:" << endl << centroids << endl;

	// Draw bounding boxes	
	Mat bounding_boxes_image;
	original_img.copyTo(bounding_boxes_image);
	cvtColor(bounding_boxes_image, bounding_boxes_image, CV_GRAY2BGR);

	// For random colors (Random Number Generator)
	RNG rng;

	for (int i = 1; i < stats.rows; i++)
	{
		Point cornerTopLeft(stats.at<int>(i, 0), stats.at<int>(i, 1));
		Point cornerBottomRight(stats.at<int>(i, 0) + stats.at<int>(i, 2), stats.at<int>(i, 1) + stats.at<int>(i, 3));
		Scalar color(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		rectangle(bounding_boxes_image, cornerTopLeft, cornerBottomRight, color, 2);
	}

	// Create image with only label X shape.
	Mat only_label_X;
	compare(labels, 1, only_label_X, CMP_EQ);

	// Show image in the name of the window
	imshow("original", original_img);
	imshow("bounding boxes", bounding_boxes_image);
	imshow("only label X", only_label_X);

	// Function for show the image in ms.
	waitKey(0);
	// Free memory
	destroyAllWindows();
	original_img.release();
	// End of the program
}


void coffee_beans_counter()
{
	// Objects
	Mat original_img;
	// Load image from disk
	original_img = imread("ressource/cafe.tif", CV_LOAD_IMAGE_GRAYSCALE);
	if (!original_img.data) {
		cout << "error loading image" << endl;
		system("pause");
	}

	// Create image for thresholded image
	Mat inverse_binary(original_img.size(), CV_8UC1);
	// Convert to inverse binary
	int threshold_value = 100;
	threshold(original_img, inverse_binary, threshold_value, 255, THRESH_BINARY_INV);

	// It is necessary to erode several times the image in order to separate each grain
	Mat eroded_image;
	int iterations = 3;
	Mat kernel = getStructuringElement(CV_SHAPE_RECT, Size(5, 5));
	erode(inverse_binary, eroded_image, kernel, Point(-1, -1), iterations);

	// Find all closed shapes in the binary image. The image change at the end of the algorithm.
	vector<vector<Point> > contours;
	findContours(eroded_image.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	cout << "Number of countours " << contours.size() << endl;
	// Draw contours
	Mat img_contours(inverse_binary.size(), CV_8UC3, Scalar(0, 0, 0));
	for (size_t k = 0; k < contours.size(); k++) {
		Scalar color(rand() & 255, rand() & 255, rand() & 255);
		drawContours(img_contours, contours, k, color);
	}

	// Label all the objects
	Mat labels, stats, centroids;
	int connectivity = 8;
	int coffee_beans = connectedComponentsWithStats(eroded_image, labels, stats, centroids, connectivity, CV_32S);
	cout << "number of grains: " << coffee_beans << endl;
	// Draw the centroids
	Mat img_centroids(inverse_binary.size(), CV_8UC3, Scalar(0, 0, 0));
	for (int k = 0; k < centroids.rows; k++) {
		Scalar color(rand() & 255, rand() & 255, rand() & 255);
		Point centroid((int)centroids.at<double>(k, 0), (int)centroids.at<double>(k, 1));
		circle(img_centroids, centroid, 3, color, -1, 8);
	}
	// Show images
	imshow("original", original_img);
	imshow("binary_inverse", inverse_binary);
	imshow("erosion", eroded_image);
	imshow("contours", img_contours);
	imshow("crentroids", img_centroids);
	waitKey(0);
	// Free memory
	destroyAllWindows();
	original_img.release();
	inverse_binary.release();
	eroded_image.release();
	img_centroids.release();
	img_contours.release();
	// End of the program
}

