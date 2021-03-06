#define _CRT_SECURE_NO_WARNINGS

#pragma once
#include <iostream> 
#include <opencv2/opencv.hpp>


/*
#ifdef _DEBUG               
#pragma comment(lib, "opencv_core320d.lib")       
#pragma comment(lib, "opencv_highgui320d.lib")    
#pragma comment(lib, "opencv_imgproc320d.lib")  
#pragma comment(lib, "opencv_videoio320d.lib")    
#pragma comment(lib, "opencv_bgsegm320d.lib")    
#pragma comment(lib, "opencv_video320d.lib")    
#else       
#pragma comment(lib, "opencv_core320.lib")       
#pragma comment(lib, "opencv_highgui320.lib")    
#pragma comment(lib, "opencv_imgproc320.lib")  
#pragma comment(lib, "opencv_videoio320.lib")    
#pragma comment(lib, "opencv_bgsegm320.lib")    
#pragma comment(lib, "opencv_video320.lib")    
#endif   
*/


using namespace cv;
using namespace std;


int main() {

	string videoFilename = "C:\\Users\\Dev3Team\\Documents\\2.h264";
	VideoCapture cap(videoFilename);



	Ptr<BackgroundSubtractor> pMOG2;
	Ptr<BackgroundSubtractor> pGMG;
	Mat cur_frame;
	Mat diff_frame;
	Mat background;


	Mat frame; //current frame
	Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
	Mat fgMaskGMG;
	Mat pre_frame;
	Mat ppre_frame;
	Mat diff;
	int key = 0;
	double frame_cnt = 0;
	bool isStop = false;

	if (!cap.isOpened()) {
		//error in opening the video input
		cerr << "Unable to open video file: " << videoFilename << endl;
		exit(EXIT_FAILURE);
	}

	pMOG2 = createBackgroundSubtractorMOG2();

	while (1) {

		if (!cap.read(frame)) {
			cerr << "next frame is not valid" << endl;
			break;
		}

		if (isStop) {

			//update the background model
			resize(frame, frame, Size(frame.cols / 2, frame.rows / 2));
			pMOG2->apply(frame, fgMaskMOG2);

			//get the frame number and write it on the current frame
			stringstream ss;
			rectangle(frame, cv::Point(10, 2), cv::Point(100, 20), cv::Scalar(255, 255, 255), -1);

			frame_cnt = cap.get(CAP_PROP_POS_FRAMES);
			ss << frame_cnt;

			string frameNumberString = ss.str();
			putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
				FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));

			//show the current frame and the fg masks
			resize(frame, frame, Size(frame.cols / 2, frame.rows / 2));
			resize(fgMaskMOG2, fgMaskMOG2, Size(fgMaskMOG2.cols / 2, fgMaskMOG2.rows / 2));
			medianBlur(fgMaskMOG2, fgMaskMOG2, 3);


			
			imshow("Frame", frame);
			imshow("FG Mask MOG 2", fgMaskMOG2);

			ppre_frame = pre_frame;
			pre_frame = frame;

			//preprocess(frame, frame);
			//preprocess(pre_frame, pre_frame);
			//preprocess(ppre_frame, ppre_frame);
			//diffFrame(frame, pre_frame, ppre_frame, diff);

			//get the input from the keyboard
		}

		key = waitKey(20);
		if (key == 27)
		{
			//break;
			return false;
		}
		else if (key == 32)
		{
			if (isStop == true)
				isStop = false;
			else if (isStop == false)
				isStop = true;
		}


		return 0;
	}
}