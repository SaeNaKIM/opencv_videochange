#define  _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <opencv2/opencv.hpp>
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


const int SampleRate = 1;
double curFrameLoc = 0.0;

int main() {

	string videoFilename = "C:\\Users\\Dev3Team\\Documents\\2.h264";
	VideoCapture cap(videoFilename);	
	VideoChange VC(videoFilename,cap);
	Mat frame;
	Mat background;


	int key;

	
	if (!cap.isOpened()) {
		cerr << "Video File Open Error" << endl;
		exit(1);
	}


	VC.videoInfoPrint();
	VC.setSamplingRate(SampleRate);

	while (1)
	{
		if (!VC.isStop())
		{
			if (!cap.read(frame))
				break;

			//curFrameLoc = cap.get(CAP_PROP_POS_FRAMES); //왜 안되냐 빡치게...
			//cout << (double)cap.get(CAP_PROP_POS_MSEC) << endl;
			curFrameLoc++;
			VC.preprocess(frame, frame);
			
			VC.samplingVideoFrame(frame, curFrameLoc);
			imshow("current Frame", frame);
		}

		
		key = waitKey(60);
		if (key == 27) {
			cout << "terminated during execution" << endl;
			break;
		}
		else {
			VC.videoControl(key, curFrameLoc);
			cap.set(CV_CAP_PROP_POS_FRAMES, VC.getFrameLoc());
		}

	}

	VC.backgroundEstimation(background,BG_MEAN);
	imshow("background", background);
	VC.detectChangeFrame(DT_PIXEL, "video_change.txt");

	waitKey(0);
	destroyAllWindows();
	VC.clearsampledImgV();
	cap.release();
	
	return 0;
}

