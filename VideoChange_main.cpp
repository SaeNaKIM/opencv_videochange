#define  _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <opencv2/opencv.hpp>
#include <time.h>
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
	double sum = 0;

	Mat frame;
	Mat grayFrame;
	Mat background;

	clock_t begin, read_begin, read_end, sampling_end, total_end;

	string videoFilename = "C:\\Users\\Dev3Team\\Documents\\2.mp4";
	VideoCapture cap(videoFilename);
	
	if (!cap.isOpened()) {
		cerr << "Video File Open Error" << endl;
		exit(1);
	}

	VideoChange VC(videoFilename, cap);
	VC.videoInfoPrint(); 
	VC.setSamplingRate(SampleRate);
	sample = VC.getSampleNumber();
	begin = clock();

	//video sampling
	while (1)
	{
		read_begin = clock();

		cap.set(CV_CAP_PROP_POS_FRAMES, frameCount * sample); 
		frameCount++;
		if (!cap.read(frame))
			break;

		read_end = clock();
		sum += read_end - read_begin;
		
		VC.preprocess(frame, grayFrame);
		VC.samplingVideoFrame(grayFrame); 
			
		imshow("sampling Frame", grayFrame);
		key = waitKey(1);

		if (key == 27) { //ESC
			cout << "terminated during video sampling" << endl;
			break;
		}

	}

	sampling_end = clock();
	VC.backgroundEstimation(background, BG_MEAN);
	
	//debugging background result
	//imshow("background", background);
	//waitKey(0);

 	VC.detectChangeFrame(DT_PIXEL, "video_change", 0.06 );
	total_end = clock();

	//release resource
	destroyAllWindows();
	VC.clearsampledImgV();
	cap.release();

	// image processing result 
	cout << "sampling frame: " << frameCount << endl;
	cout << "image read execution time:" << sum / CLOCKS_PER_SEC  << endl;
	cout << "average read execution time per frame:" << sum / CLOCKS_PER_SEC / frameCount << endl;
	cout << "sampling execution time:" << (sampling_end - begin) / CLOCKS_PER_SEC << endl;
	cout << "total execution time:" << (total_end - begin) / CLOCKS_PER_SEC << endl;
	system("pause");
	
	return 0;
}

