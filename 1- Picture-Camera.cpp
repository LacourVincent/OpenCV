#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

#define ESCAPE 27


void DiplayPicture() {
	Mat image;
	image = imread("ressource/mandril.jpg"); // Read the file

	if (!image.data) {// Check for invalid input
		cout << "Could not open or find the image" << endl;
		return ;
	}
	namedWindow("Original", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Original", image); // Show our image inside it.

	waitKey(0); // Wait for a keystroke in the window
	destroyWindow("Original");
}

void DisplayPictureFromVideo() {

	char* name = "ressource/honda-asimo.avi";
	VideoCapture capture;
	Mat frame;
	char keypressed = 0;
	bool sucess = true;
	capture.open(name);

	if (!capture.isOpened()) {
		cout << "Error cannot open the video" << endl;
		return ;
	}
	namedWindow("Video", CV_WINDOW_AUTOSIZE);

	while (keypressed != ESCAPE) {
		sucess = capture.read(frame);

		if (sucess == false) {
			cout << "Cannot read the frame from file" << endl;
			return ;
		}
		imshow("Video", frame);
		keypressed = waitKey(0);
	}

	destroyWindow("Video");
	capture.release();
}

void DisplayPictureFromCamera() {

	VideoCapture capture(0);

	if (!capture.isOpened()) {
		cout << "Impossible to access to camera" << endl;
		return ;
	}

	Mat frame;
	char keypressed = 0;
	bool sucess = true;

	namedWindow("Video", CV_WINDOW_AUTOSIZE);

	while (keypressed != ESCAPE) {
		sucess = capture.read(frame);

		if (sucess == false) {
			cout << "Cannot read the frame from file" << endl;
			return ;
		}
		imshow("Video", frame);
		keypressed = waitKey(0);
	}
	destroyWindow("Video");
	capture.release();
}