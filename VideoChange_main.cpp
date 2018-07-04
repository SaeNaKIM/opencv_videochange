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

	int key = 0 ;
	double SampleRate = 1;
	int frameCount = 0.0;
	int sample = 0;
	double sum_read = 0;
	double sum_pre = 0;
	double sum_push = 0;

	clock_t begin, read_begin, read_end, sampling_end, total_end, 
		preprocess_begin, preprocess_end, push_begin, push_end, background_end  ;

	string videoFilename = "C:\\Users\\Dev3Team\\Documents\\2.mp4";
	VideoCapture capStream1(videoFilename);
	//VideoCapture capStream2(videoFilename);

	Mat frame;
	//Mat frame2;
	Mat grayFrame = Mat::zeros(Size(495,270),CV_8UC1);
	Mat background;
	
	
	if (!capStream1.isOpened()) {
		cerr << "Video File Open Error" << endl;
		exit(1);
	}

	VideoChange VC(videoFilename, capStream1);
	VC.videoInfoPrint(); 
	VC.setSamplingRate(SampleRate);
	sample = VC.getSampleNumber();
	
	// video sampling
	begin = clock();
	VC.samplingVideoFrame(capStream1);
	sampling_end = clock();

	//video sampling
	//while (1)
	//{
	//	read_begin = clock();
	//	capStream1.set(CV_CAP_PROP_POS_FRAMES, frameCount * sample);
	//	//capStream2.set(CV_CAP_PROP_POS_FRAMES, frameCount * sample);
	//	
	//	if (!capStream1.read(frame) || !capStream2.read(frame2))
	//		break;
	//	
	//	frameCount++;
	//	read_end = clock();
	//	sum_read += read_end - read_begin;
	//	
	//	preprocess_begin = clock();
	//	VC.preprocess(frame, grayFrame); // thread 적용지점 
	//	preprocess_end = clock();
	//	sum_pre += preprocess_end - preprocess_begin;

	//	VC.samplingVideoFrame(grayFrame); 

	//	imshow("sampling Frame2", frame2);
	//	imshow("sampling Frame", grayFrame);
	//	key = waitKey(1);

	//	if (key == 27) { //ESC
	//		cout << "terminated during sampling video" << endl;
	//		break;
	//	}
	//}

	VC.backgroundEstimation(background, BG_MEAN);
	background_end = clock();
	

	//debugging - background result
	imshow("background", background);
	waitKey(0);

	VC.setOutFilename("video_change");
	VC.detectChangeFrame(DT_PIXEL, 0.06);
	VC.writeChangeRate();
	total_end = clock();


	//release resource
	destroyAllWindows();
	VC.clear();
	capStream1.release();

	// image processing result 
	//cout << "sampling frame: " << frameCount << endl;
	//cout << "image read execution time:" << sum_read / CLOCKS_PER_SEC  << endl;
	//cout << "image preprocess execution time:" << sum_pre / CLOCKS_PER_SEC << endl;
	//cout << "average read execution time per frame:" << sum_read / CLOCKS_PER_SEC / frameCount << endl;
	cout << "sampling execution time:" << (sampling_end - begin) / CLOCKS_PER_SEC << endl;
	cout << "background estimation execution time:" << (background_end - sampling_end) << "mileseconds"<< endl;
	cout << "change detect execution time:" << (total_end - background_end) / CLOCKS_PER_SEC << endl;
	cout << "total execution time:" << (total_end - begin) / CLOCKS_PER_SEC << endl;
	system("pause");
	
	return 0;
}

