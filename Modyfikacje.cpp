#include "opencv\cv.h"
#include "opencv\highgui.h"
#include <iostream>

#include <stdio.h>
#include <stdlib.h>

using namespace std;


int main()
{
	cv::Mat frame;
	cv::VideoCapture capture;

	/* WYB”R è”D£A - KAMERA / FILM */
	if (!input_name || (isdigit(input_name[0]) && input_name[1] == '\0'))
		capture = cv::VideoCapture(0);
	else
		capture = cv::VideoCapture(input_name);

	if (!capture.isOpened())	return -1;
	if (!haar_cascade.load(cascade_name))
	{
		cout << "B≥πd ≥adowania pliku kaskady!";
	}
	capture.set(CV_CAP_PROP_FPS, 60);
	for (;;)
	{
		capture >> frame;
		if (!frame.empty())
		{
			show_edges(frame);
			detect_and_draw(frame);
		}
		else
		{
			cout << "B≥πd ≥adowania klatek filmu. Sprawdü po≥πczenie z kamerπ lub ürÛd≥o filmu.\n";
			break;
		}
		if (cv::waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "WciúniÍty Esc!" << endl;
			break;
		}
	}

	return 0;
}

void show_edges(cv::Mat frame)
{
	cv::Mat image, grey;
	cv::Mat grad_x, grad_y;
	cv::Mat edge;
	cv::Mat abs_grad_x, abs_grad_y;

	int scale = 1;
	int delta = 0;
	int depth = CV_16S;
	cv::namedWindow("Edge Detector", 1);

	cv::cvtColor(frame, grey, CV_BGR2GRAY); // Obraz w odcieniach szaroúci
	/*Poniøej zastosowanie filtru Sobela do wykrywania krawÍdzi*/
	Sobel(grey, grad_x, depth, 1, 0, 3, scale, delta, 2);
	Sobel(grey, grad_y, depth, 0, 1, 3, scale, delta, 2);
	convertScaleAbs(grad_x, abs_grad_x);
	convertScaleAbs(grad_y, abs_grad_y);
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, edge);

	imshow("Edge Detector", edge);
	cv::moveWindow("Edge Detector", 200, 440);
}

void detect_and_draw(cv::Mat frame)
{
	cv::Mat gray;
	cv::Mat small_img;
	cv::Mat src;
	/*
	cv::Point pt3;
	cv::Point pt4;
	*/
	cv::cvtColor(frame, gray, CV_BGR2GRAY);

	cv::namedWindow("Vehicle Detector", 1);
	imshow("Video", frame);
	if (&haar_cascade)
	{
		cv::vector<cv::Rect> wykryte;
		haar_cascade.detectMultiScale(gray, wykryte, 1.1, 3, 0);

		/*
		pt3.x = 0;
		pt3.y = 0;
		pt4.x = 300;
		pt4.y = 300;
	
		rectangle(frame, pt3, pt4, GREEN, 2, 8, 0);
		*/
		for (int i = 0; i < wykryte.size(); i++)
		{
			cv::Rect r;
			cv::Point pt1, pt2;
			
			/*Przypisanie wartoúci wspÛ≥rzÍdnych X i Y wykrytych obiektÛw.*/
			r.x = wykryte[i].x;
			r.y = wykryte[i].y;
			r.width = (wykryte[i].width);
			r.height = (wykryte[i].height);
			/*Dla wczeúniej wyznaczonych wartoúÊi X, Y i szerokoúci wykrytych obiektÛw ñ rysujÍ prostokπty o podanych wymiarach.*/
			pt1.x = r.x;
			pt2.x = (r.x + r.width);
			pt1.y = r.y;
			pt2.y = (r.y + r.height);
			rectangle(frame, pt1, pt2, GREEN, 2, 8, 0);
		
		}
	}
	imshow("Vehicle Detector", frame);
	cv::moveWindow("Vehicle Detector", 200, 50);
}