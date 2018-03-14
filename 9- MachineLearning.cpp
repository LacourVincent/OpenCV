#include "stdafx.h"
#include <opencv/cv.hpp>
#include <opencv/ml.h>
#include <iostream>
#include <vector>

// Machine Learning

// --Classification
#define NCLASS1 3
#define NCLASS2 4
#define NCLASS3 13
#define TRAIN_SAMPLES (NCLASS1+NCLASS2+NCLASS3)

using namespace std;
using namespace cv;


static Ptr<ml::TrainData>
//Convertir los datos de una matriz al tipo TrainData
prepare_train_data(const Mat& data, const Mat& responses, int ntrain_samples)
{
	Mat sample_idx = Mat::zeros(1, data.rows, CV_8U);
	Mat train_samples = sample_idx.colRange(0, ntrain_samples);
	train_samples.setTo(Scalar::all(1));

	int nvars = data.cols;
	Mat var_type(nvars + 1, 1, CV_8U);
	var_type.setTo(Scalar::all(ml::VAR_ORDERED));
	var_type.at<uchar>(nvars) = ml::VAR_CATEGORICAL;

	return ml::TrainData::create(data, ml::ROW_SAMPLE, responses,
		noArray(), sample_idx, noArray(), var_type);
}

void Clasificador_Bayes()
{
	Mat image, binary_img;
	vector<vector<Point> > contours;

	int n_contours, k = 0;
	double perimeter, area;

	char* classes_names[3] = { "Tornillo", "Tuerca", "Tachuela" };
	char* imgs_names[3] = { "Clase1.tif", "Clase2.tif", "Clase3.tif" };

	// Auxiliary matrixes
	Mat train_features(TRAIN_SAMPLES, 2, CV_32FC1);
	Mat train_labels(TRAIN_SAMPLES, 1, CV_32FC1);

	// Prepare Training
	cout << "Training samples" << endl;
	for (int class_id = 0; class_id < 3; class_id++) {
		image = imread("ressource/" + std::string(imgs_names[class_id]), IMREAD_GRAYSCALE);
		if (!image.data) {
			cerr << "Error loading the image" << endl;
		}

		binary_img = Mat(image.size(), CV_8UC1);
		threshold(image, binary_img, 128, 255, THRESH_BINARY);

		imshow("Training", binary_img);
		waitKey(0);

		findContours(binary_img.clone(), contours, RETR_LIST, CHAIN_APPROX_NONE);
		n_contours = contours.size();

		cout << "Total number of " << imgs_names[class_id] << ": " << n_contours << endl;

		for (size_t idx = 0; idx < n_contours; idx++, k++) {
			area = contourArea(contours[idx], false);
			perimeter = arcLength(contours[idx], true);

			train_features.at<float>(k, 0) = perimeter;
			train_features.at<float>(k, 1) = area;
			train_labels.at<float>(k) = class_id;
		}
	}

	// Create Bayes classifier and train (save)
	int ntrain_samples = (int)(TRAIN_SAMPLES);
	Ptr<ml::NormalBayesClassifier> bayes = ml::NormalBayesClassifier::create();
	Ptr<ml::TrainData> tdata = prepare_train_data(train_features, train_labels, ntrain_samples);
	bayes->train(tdata);
	bayes->save("ressource/bayes_smv_model.xml");

	// Load Bayes classifier
	//bayes->load("ressource/bayes_smv_model.xml");

	// Test the model
	Mat test_features(1, 2, CV_32FC1);
	image = imread("ressource/test.tif", IMREAD_GRAYSCALE);
	if (!image.data) {
		cout << "Error loading the test image" << endl;
	}
	binary_img = Mat(image.size(), CV_8UC1);
	threshold(image, binary_img, 128, 255, THRESH_BINARY);

	Mat color_img(image.size(), CV_8UC3);
	cvtColor(image, color_img, CV_GRAY2BGR);

	findContours(binary_img.clone(), contours, RETR_LIST, CHAIN_APPROX_NONE);
	n_contours = contours.size();

	cout << "Total number of pieces: " << n_contours << endl;
	Mat test_prediction(n_contours, 1, CV_32FC1);

	for (size_t idx = 0; idx < n_contours; idx++) {
		area = contourArea(contours[idx], false);
		perimeter = arcLength(contours[idx], true);

		test_features.at<float>(0, 0) = perimeter;
		test_features.at<float>(0, 1) = area;

		test_prediction.at<float>(idx) = bayes->predict(test_features);

		cvtColor(image, color_img, CV_GRAY2BGR);
		Scalar color(0, 0, 255);
		drawContours(color_img, contours, idx, color, 2);
		imshow("Test", color_img);

		cout << "Highlighted piece is a " << classes_names[(int)test_prediction.at<float>(idx)] << endl;
		waitKey(0);
	}

	// Free memory
	image.release();
	binary_img.release();
	color_img.release();
	train_features.release();
	train_labels.release();
	test_features.release();
	test_prediction.release();
	bayes->clear();
}