#define  _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <opencv2/opencv.hpp>
#include <time.h>
#include <map>
#include "VideoChange.h"


using namespace std;
using namespace cv;

// backgrund subtraction algorithm option
const int BG_MEAN = 1;
const int BG_MEDIAN = 2;
const int BG_MOG = 3;

//change notification parameter option
const int DT_PIXEL =  1;
const int DT_BLOB = 2;

int main() {


	double SampleRate = 1;
	int frameCount = 0.0;
	double sum_read = 0;
	double sum_pre = 0;
	double sum_push = 0;

	// for computing time 
	clock_t begin, read_begin, read_end, sampling_end, total_end, 
		preprocess_begin, preprocess_end, push_begin, push_end, background_end  ;

	string videoFilename = "C:\\Users\\Dev3Team\\Documents\\2.mp4";
	VideoCapture capStream1(videoFilename);
	VideoCapture capStream2(videoFilename);
	//VideoCapture capStream3(videoFilename);
	//VideoCapture capStream4(videoFilename);

	Mat frame;
	Mat grayFrame = Mat::zeros(Size(495,270),CV_8UC1);
	Mat background;
	
	//video open check
	if (!capStream1.isOpened() || !capStream2.isOpened() /*|| !capStream3.isOpened() || !capStream4.isOpened()*/) {
		cerr << "Video File Open Error" << endl;
		exit(1);
	}

	// video change setting 
	VideoChange VC(videoFilename, capStream1);
	VC.videoInfoPrint(); 
	VC.setSamplingRate(SampleRate);
	
	// 1. video sampling
	begin = clock();
	VC.samplingVideoFrame(capStream1, capStream2);
	//VC.samplingVideoFrame(capStream1, capStream2, capStream3, capStream4);
	sampling_end = clock();

	// 2. background estimatjion - compute average image of sampled images
	VC.backgroundEstimation(background, BG_MEAN);
	background_end = clock();

	//debugging - background result
	/*imshow("background", background);
	waitKey(0);*/

	// 3. change detection 
	VC.setOutFilename("video_change");
	VC.detectChangeFrame(DT_PIXEL, 0.06);

	// 4. save data (.csv)
	VC.writeChangeRate();
	total_end = clock();

	//release 
	destroyAllWindows();
	VC.clear();
	capStream1.release();
	capStream2.release();

	// result
	cout << "sampling execution time:" << (sampling_end - begin) / CLOCKS_PER_SEC << endl;
	cout << "background estimation execution time:" << (background_end - sampling_end) << " mileseconds"<< endl;
	cout << "change detect execution time:" << (total_end - background_end) / CLOCKS_PER_SEC << endl;
	cout << "total execution time:" << (total_end - begin) / CLOCKS_PER_SEC << endl;
	system("pause");
	
	return 0;
}

