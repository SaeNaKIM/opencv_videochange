//
//#pragma once
//#include <opencv2/opencv.hpp>
//#include <iostream> 
//
//
//using namespace cv;
//using namespace std;
//
//class VideoChange {
//
//	private:
//
//		Ptr<BackgroundSubtractor> pMOG2;
//		Mat cur_frame;
//		Mat diff_frame;
//		Mat background;
//
//
//
//	public:
//
//
//		
//		int dst_frame;
//		bool isStop = false;
//
//		VideoChange() {
//
//
//
//			pMOG2 = createBackgroundSubtractorMOG2(); //MOG2 approach
//
//			
//		}
//
//		void preprocess(Mat &src, Mat &dst) {
//
//			cvtColor(src, src, CV_BGR2GRAY);
//			resize(src, src, Size(src.cols / 2, src.rows / 2));
//			GaussianBlur(src, dst, Size(3, 3), 0);
//
//		}
//
//		void diffFrame(Mat &src1, Mat &src2, Mat &diff) {
//
//			Mat diff_gray;
//
//
//			diff_gray = abs(src1 - src2);
//			imshow("temp", diff_gray);
//
//			return;
//		}
//
//		
//		void diffFrame(Mat &src1, Mat &src2, Mat &src3, Mat &diff) { //motion mask _Q.���� �������� ��..
//
//			Mat diff_gray1;
//			Mat diff_gray2;
//
//
//			diff_gray1 = abs(src1 - src2);
//			diff_gray2 = abs(src2 - src3);
//
//
//			diff = abs(diff_gray1 - diff_gray2);
//			imshow("diff img for adjacent three", diff);
//			return;
//		}
//
//		void thresholdForChange(Mat & src, Mat &dst )
//		{
//
//			Scalar mean, stddev;
//			Mat diff_gray;
//			meanStdDev(src, mean, stddev);
//			threshold(src, dst, (double)(mean[0] + stddev[0]), 255, THRESH_BINARY);
//
//		}
//		void backgroundSubtration(string videoFilename) {
//
//			VideoCapture cap(videoFilename);
//			Mat frame; //current frame
//			Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
//			Mat pre_frame;
//			Mat ppre_frame;
//			Mat diff;
//			int key = 0;
//			double frame_cnt = 0;
//
//			if (!cap.isOpened()) {
//				//error in opening the video input
//				cerr << "Unable to open video file: " << videoFilename << endl;
//				exit(EXIT_FAILURE);
//			}
//
//			while (1) {
//				
//				if (!cap.read(frame)) {
//					cerr << "next frame is not valid" << endl;
//					break;
//				}
//
//				//update the background model
//				resize(frame, frame, Size(frame.cols / 2, frame.rows / 2));
//				//resize(fgMaskMOG2, fgMaskMOG2, Size(fgMaskMOG2.cols / 2, fgMaskMOG2.rows / 2));
//				pMOG2->apply(frame, fgMaskMOG2);
//				
//				//get the frame number and write it on the current frame
//				stringstream ss;
//				rectangle(frame, cv::Point(10, 2), cv::Point(100, 20), cv::Scalar(255, 255, 255), -1);
//				
//				frame_cnt = cap.get(CAP_PROP_POS_FRAMES);
//				ss << frame_cnt;
//				
//				string frameNumberString = ss.str();
//				putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
//					FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
//				
//				//show the current frame and the fg masks
//					
//				medianBlur(fgMaskMOG2, fgMaskMOG2, 3);
//
//				imshow("Frame", frame);
//				imshow("FG Mask MOG 2", fgMaskMOG2);
//				
//				ppre_frame = pre_frame;
//				pre_frame = frame;
//
//				//preprocess(frame, frame);
//				//preprocess(pre_frame, pre_frame);
//				//preprocess(ppre_frame, ppre_frame);
//				diffFrame(frame, pre_frame, ppre_frame, diff);
//				
//				//get the input from the keyboard
//				key = waitKey(20);
//				/*
//				if (!videoControl(key, frame_cnt)) {
//					break;
//				}
//				else {
//					cap.set(CV_CAP_PROP_POS_FRAMES, dst_frame - 1);
//				}
//				*/
//				
//			}
//			
//
//	
//
//			cap.release();
//			return;
//
//		}
//
//		bool videoControl(int key, int cur_frame_cnt) {
//
//
//			dst_frame = cur_frame_cnt;
//
//			if (key == 27)
//			{
//				//break;
//				return false;
//			}
//			else if (key == 32)
//			{
//				if (isStop == true)
//					isStop = false;
//				else if (isStop == false)
//					isStop = true;
//			}
//			else if (key == ']')
//			{
//				dst_frame += 100;
//			}
//			else if (key == '[')
//			{
//				dst_frame -= 100;
//			}
//			else if (key == '.')
//			{
//				dst_frame += 10;
//			}
//			else if (key == ',')
//			{
//				dst_frame -= 10;
//			}
//
//			return true;
//		}
//
//
//};
//
//int main() {
//
//	string videoFilename = "C:\\Users\\Dev3Team\\Documents\\2.h264";
//	VideoCapture cap(videoFilename);
//	
//	Mat frame;
//	Mat resize;
//	Mat diff; 
//	Mat pre_frame;
//	VideoChange vc;
//
//	char key;
//	bool stop = false;
//	double frame_cnt = 0;
//	double dst_frame_cnt;
//
//	//background subtraction
//	vc.backgroundSubtration("C:\\Users\\Dev3Team\\Documents\\2.h264");
//	return 0;
//	//video information
//	float fps = cap.get(CV_CAP_PROP_FPS);
//
//	cout << "fps: " << fps << endl;
//	
//	if (!cap.isOpened()) {
//		cerr << "Video File Open Error" << endl;
//		exit(1);
//	}
//	while (1)
//	{
//		if (!vc.isStop)
//		{
//			if (!cap.read(frame))
//				break;															
//			frame_cnt = cap.get(CV_CAP_PROP_POS_FRAMES);
//			
//			
//			vc.preprocess(frame, frame);
//			//imshow("cur_frame", frame);
//			vc.diffFrame(frame, pre_frame, diff);
//			imshow("diff_frame", diff);
//			
//		}
//
//		pre_frame = frame;
//		
//		
//		//video control
//		//key = waitKey(1000 / fps); //fps  : 15 -> �� �̷��� ������ �𸣰ڳ�..
//		key = waitKey(60);
//		if (!vc.videoControl(key, frame_cnt)) {
//			break;
//		}
//		else {
//			cap.set(CV_CAP_PROP_POS_FRAMES, vc.dst_frame - 1);
//		}
//			
//	}
//
//
//	destroyAllWindows();
//	waitKey(0);
//	cap.release();
//	
//
//	return 0;
//}
//
//