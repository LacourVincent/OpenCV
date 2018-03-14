#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void Correlacion_TM() {

	int i;

	// Nombre de la imagen que va a cargar
	char Nombre_Imagen[] = "ressource/corr_norm.tif";
	char Nombre_Modelo[] = "ressource/modelo.tif";

	//Cargamos las imagenes y se comprueba que lo ha hecho correctamente
	Mat src = imread(Nombre_Imagen);
	Mat templ = imread(Nombre_Modelo);

	if (!src.data || !templ.data) {
		cout << "Error al cargar las imagenes" << endl;
		exit(1);
	}

	Mat ftmp[6];
	int iwidth;
	int iheight;

	//Reservamos memoria para los diversos metodos
	iwidth = src.cols - templ.cols + 1;
	iheight = src.rows - templ.rows + 1;

	for (i = 0; i < 6; i++) {
		ftmp[i].create(iheight, iwidth, CV_32SC1);
	}

	//Correlacion
	for (i = 0; i < 6; i++) {
		matchTemplate(src, templ, ftmp[i], i);
		normalize(ftmp[i], ftmp[i], 1, 0, CV_MINMAX);
	}

	//Pantalla

	namedWindow("Modelo", CV_WINDOW_AUTOSIZE);
	imshow("Modelo", templ);

	namedWindow("Imagen", CV_WINDOW_AUTOSIZE);
	imshow("Imagen", src);

	namedWindow("SQDIFF", CV_WINDOW_AUTOSIZE);
	imshow("SQDIFF", ftmp[0]);

	namedWindow("SQDIFF_NORMED", CV_WINDOW_AUTOSIZE);
	imshow("SQDIFF_NORMED", ftmp[1]);

	namedWindow("CCORR", CV_WINDOW_AUTOSIZE);
	imshow("CCORR", ftmp[2]);

	namedWindow("CCORR_NORMED", CV_WINDOW_AUTOSIZE);
	imshow("CCORR_NORMED", ftmp[3]);

	namedWindow("COEFF", CV_WINDOW_AUTOSIZE);
	imshow("COEFF", ftmp[4]);

	namedWindow("COEFF_NORMED", CV_WINDOW_AUTOSIZE);
	imshow("COEFF_NORMED", ftmp[5]);



	//Esperar un pulsa de tecla
	cvWaitKey(0); //Any keys pressed
	destroyAllWindows();
}

void FindModel() {


	// Nombre de la imagen que va a cargar
	char Nombre_Imagen[] = "ressource/corr_norm.tif";
	char Nombre_Modelo[] = "ressource/modelo.tif";

	//Cargamos las imagenes y se comprueba que lo ha hecho correctamente
	Mat src = imread(Nombre_Imagen);
	Mat templ = imread(Nombre_Modelo);

	if (!src.data || !templ.data) {
		cout << "Error al cargar las imagenes" << endl;
		exit(1);
	}

	Mat dst;

	//Reservamos memoria para los diversos metodos
	int iwidth = src.cols - templ.cols + 1;
	int iheight = src.rows - templ.rows + 1;

	dst.create(iheight, iwidth, CV_32SC1);

	//int match_method = CV_TM_SQDIFF;
	//int match_method = CV_TM_SQDIFF_NORMED;
	//int match_method = CV_TM_CCORR;
	//int match_method = CV_TM_CCORR_NORMED;
	//int match_method = CV_TM_CCOEFF;
	int match_method = CV_TM_CCOEFF_NORMED;


	switch (match_method) {
	case 0:
		cout << "Match Method : CV_TM_SQDIFF : " << match_method << endl;
		break;
	case 1:
		cout << "Match Method : CV_TM_SQDIFF_NORMED : " << match_method << endl;
		break;
	case 2:
		cout << "Match Method : CV_TM_CCORR : " << match_method << endl;
		break;
	case 3:
		cout << "Match Method : CV_TM_CCORR_NORMED : " << match_method << endl;
		break;
	case 4:
		cout << "Match Method : CV_TM_CCOEFF : " << match_method << endl;
		break;
	case 5:
		cout << "Match Method : CV_TM_CCOEFF_NORMED : " << match_method << endl;
		break;
	}


	//Correlacion

	matchTemplate(src, templ, dst, CV_TM_CCOEFF_NORMED);
	normalize(dst, dst, 0, 1, NORM_MINMAX, -1, Mat());

	double minVal, maxVal;
	Point minLoc, maxLoc;
	Point matchLoc;

	minMaxLoc(dst, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED) {
		matchLoc = minLoc;
	}
	else {
		matchLoc = maxLoc;
	}

	rectangle(src, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar(255, 0, 0), 4, 8, 0);
	rectangle(dst, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 4, 8, 0);

	namedWindow("Source", CV_WINDOW_AUTOSIZE);
	imshow("Source", src);

	namedWindow("Result", CV_WINDOW_AUTOSIZE);
	imshow("Result", dst);

	namedWindow("Model", CV_WINDOW_AUTOSIZE);
	imshow("Model", templ);

	//Esperar un pulsa de tecla
	cvWaitKey(0); //Any keys pressed
	destroyAllWindows();
}