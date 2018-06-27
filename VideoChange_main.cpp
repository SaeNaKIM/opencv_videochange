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
	int SampleRate = 1;
	double curFrameLoc = 0.0;
	
	Mat frame;
	Mat grayFrame;
	Mat background;

	clock_t begin, end1, end2;

	string videoFilename = "C:\\Users\\Dev3Team\\Documents\\2.h264";
	//string videoFilename = "C:\\Users\\Dev3Team\\Desktop\\Video_1.mp4";
	VideoCapture cap(videoFilename);
	VideoChange VC(videoFilename, cap);

	if (!cap.isOpened()) {
		cerr << "Video File Open Error" << endl;
		exit(1);
	}


	VC.videoInfoPrint(); 
	VC.setSamplingRate(SampleRate);

	begin = clock();

	//video play
	while (1)
	{
		if (!VC.isStop())
		{
			if (!cap.read(frame))
				break;

			//curFrameLoc = cap.get(CAP_PROP_POS_FRAMES); //왜 안되냐 빡치게...
			//cout << cap.get(CAP_PROP_POS_MSEC) << endl;
			curFrameLoc++;
			VC.preprocess(frame, grayFrame);	
			VC.samplingVideoFrame(grayFrame, curFrameLoc);		
			//imshow("current Frame", grayFrame);
		}

		
		//key = cv::waitKey(10);
		if (key == 27) {
			cout << "terminated during video execution" << endl;
			break;
		}
		else {
			VC.videoControl(key, curFrameLoc);
			cap.set(CV_CAP_PROP_POS_FRAMES, VC.getFrameLoc());
		}

	}

	end1 = clock();

	VC.backgroundEstimation(background, BG_MEAN);
	//imshow("background", background);
	//waitKey(0);

 	VC.detectChangeFrame(DT_PIXEL, "video_change.csv", 0.06 );

	end2 = clock();

	cout << "total frame: " << curFrameLoc << endl;
	cout << "sampling execution time:" << (end1 - begin) / CLOCKS_PER_SEC << endl;
	cout << "total execution time:" << (end2 - begin) / CLOCKS_PER_SEC << endl;
	system("pause");

	//release resource
	destroyAllWindows();
	VC.clearsampledImgV();
	cap.release();
	
	return 0;
}

