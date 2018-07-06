//#pragma omp parallel for
#define  _CRT_SECURE_NO_WARNINGS
#include <iostream> 
#include <opencv2/opencv.hpp>
#include <vector>
#include <direct.h>
#include <thread>
#include <mutex>
#include <map>
#include "VideoChange.h"


using namespace std;
using namespace cv;

VideoChange::VideoChange(string filename, VideoCapture cap) {

	setInFilename(filename);
	this->fps = cap.get(CV_CAP_PROP_FPS);
	this->cols = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	this->rows = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	this->nframe = cap.get(CV_CAP_PROP_FRAME_COUNT);

}
void VideoChange::videoInfoPrint() {


	/*-------------Output video information-------------------*/
	cout << endl;
	cout << "Input Video File Name : " << this->filename << endl;
	cout << "Frame Per Seconds : " << this->fps << endl;
	cout << "Frame Size : " << this->cols << " x " << rows << endl;
	cout << "Frame Count : " << this->nframe << endl;
	/*--------------------------------------------------------*/

}
void VideoChange::preprocess(Mat &src, Mat &dst) {


	Mat gray;
	Mat resizeMat;

	cvtColor(src, gray, CV_BGR2GRAY); // convert channel 3 to 1 
	resize(gray, resizeMat, Size(gray.cols / 4, gray.rows / 4));
	GaussianBlur(resizeMat, dst, Size(3, 3), 0); //Blurring 

}
void VideoChange::samplingVideoFrame(VideoCapture cap1, VideoCapture cap2) {

	
	int nframe = this->nframe; 
	

	thread t1(&VideoChange::threadsamplingVideoFrame, this, cap1, 0, nframe /2, "cap1");
	thread t2(&VideoChange::threadsamplingVideoFrame, this, cap2, nframe/2 + 1, nframe, "cap2");

	t1.join();
	t2.join();

	cout << "sampling thread is completed\n";
	return;

}
void VideoChange::threadsamplingVideoFrame(VideoCapture cap, int begin, int end, string windowname)
{
	int key;

	for (int i = begin; i < end; i+= this->sample) {
		
		
		Mat frame, grayFrame;

		cap.set(CV_CAP_PROP_POS_FRAMES, i );
		cap.read(frame);
		preprocess(frame, grayFrame); // preprocess

		mutex_lock.lock();
		this->sampledImgM.insert(make_pair(i ,grayFrame));
		imshow(windowname, grayFrame);
		waitKey(1);
		mutex_lock.unlock();	

	}
}
 void VideoChange::backgroundEstimation(Mat &background, int type) {

	clock_t begin, end;
	begin = clock();

	//Mat sum = Mat::zeros(sampledImgM[0].size(), CV_32FC1);
	Mat sum = Mat::zeros(sampledImgM[0].size(), CV_32FC1);
	Mat temp;
	Mat avg;
	Mat blur;

	map<int, Mat>::iterator iter;
	double sampledImgL = sampledImgM.size();


	if (type == 1) { // mean 

		//for(int i = 0; i < sampledImgV.size(); i++){
		for (iter = sampledImgM.begin(); iter != sampledImgM.end(); iter++) {
			
			accumulateWeighted(iter->second, sum, 1.0 / sampledImgL);
			
		}
		convertScaleAbs(sum, this->background);
	}
	else if (type == 2) { // median of median 




	}
	else if (type == 3) { // gaussian of mixture 



	}
	else {

		cout << "wrong type" << endl;
		return;
	}


	background = this->background.clone();
	end = clock();
	cout << "background time in func: " << end - begin << " mileseconds"<< endl;
	return;
}
 void VideoChange::detectChangeFrame(int detectType, double pixelThreshold) {

	 Mat absImg;
	 int sampledImgVLength = this->sampledImgM.size();

	 if (detectType == 1) { // pixel 

		 equalizeHist(this->background, this->background);

		 thread t1{ &VideoChange::threadForDetectChangeFrame1, this, 0, this->nframe / 2, pixelThreshold };
		 thread t2{ &VideoChange::threadForDetectChangeFrame2, this,  0, this->nframe / 2, pixelThreshold };
		

		 t1.join();
		 t2.join();
		

		 cout << "thread is finished" << endl;

	 }
	 else if (detectType == 2) { //blob 

	 }
	 return;
 }
void VideoChange::threadForDetectChangeFrame1(int begin, int end, double pixelThreshold)
{
	int imgSize = sampledImgM[begin * this->sample].rows * sampledImgM[begin * this->sample].cols;

	int i = 0;
	for (auto it = sampledImgM.cbegin(); it->first < end; ++it, i++){

		Mat curImg = it->second.clone();
		Mat hist_frame;
		Mat absImg;
		Mat thresholdImg;
		int nonZeroCnt = 0;
		double changeRate = 0.0f;

		// change detection - compute  difference between samplied frame and bacgkround 
		equalizeHist(curImg, hist_frame);
		absImg = abs(hist_frame - this->background);
		threshold(absImg, thresholdImg, 50, 255, THRESH_BINARY);
		nonZeroCnt = countNonZero(thresholdImg);
		changeRate = (double)nonZeroCnt / imgSize;
		

		if (changeRate > pixelThreshold) {

			// store interest image with time(second) of the frame 
			string filename = string(this->directory) + "\\frame_" + to_string((int)it->first / this->fps) + ".png";
			imwrite(filename, curImg);

		}

		//save data
		mutex_lock.lock();
		changeRateArr[i] = to_string(i * this->sample / this->fps) + "," + to_string(changeRate) + "\n"; // n-th frame 
		mutex_lock.unlock();	

	}

	return;
}
void VideoChange::threadForDetectChangeFrame2(int begin, int end, double pixelThreshold)
{

	int imgSize = sampledImgM[begin * this->sample].rows * sampledImgM[begin * this->sample].cols;

	int i = sampledImgM.size() - 1 ;
	for (auto it = sampledImgM.crbegin(); it->first >= end; ++it, i--) {

		Mat curImg = it->second.clone();
		Mat hist_frame;
		Mat absImg;
		Mat thresholdImg;
		int nonZeroCnt = 0;
		double changeRate = 0.0f;


		// change detection - compute  difference between samplied frame and bacgkround 
		equalizeHist(curImg, hist_frame);
		absImg = abs(hist_frame - this->background);
		threshold(absImg, thresholdImg, 50, 255, THRESH_BINARY);
		nonZeroCnt = countNonZero(thresholdImg);
		changeRate = (double)nonZeroCnt / imgSize;


		if (changeRate > pixelThreshold) {

			// store interest image with time(second) of the frame 
			string filename = string(this->directory) + "\\frame_" + to_string((int)it->first / this->fps) + ".png";
			imwrite(filename, curImg);

		}

		//save data
		mutex_lock.lock();
		changeRateArr[i] = to_string(i * this->sample / this->fps) + "," + to_string(changeRate) + "\n"; // n-th frame 
		mutex_lock.unlock();	
	}
}
string VideoChange::computeTime(int curFrameLoc)
{
	//compute time 
	double t = 60;
	int min = (int)curFrameLoc * sample / t;
	int sec = ((double)curFrameLoc * sample / t - min) * t;

	return to_string(min) + "." + to_string(sec);
}
void VideoChange::setInFilename(string filename) {

	this->filename = filename;
}
void VideoChange::setOutFilename(string filename)
{
	// csv file open
	this->outFile.open(filename + ".csv");
	strcpy(this->directory, filename.c_str());
	changeRateArr = new string[this->sampledImgM.size()];

	// make directory 
	cout << (_mkdir(this->directory) ? "directory creation sucess\n" : "directory creation failed\n ");

	//int nResult = _mkdir(this->directory);

	//if (nResult == 0)
	//{
	//	cout << "directory creation sucess" << endl;
	//}
	//else { // directory already existed.

	//	cout << "directory creation failed " << endl;
	//}

	return;
}
void VideoChange::setSamplingRate(double samplingRate)
{
	this->samplingRate = samplingRate;
	this->sample = this->fps / this->samplingRate;
}
void VideoChange::clear()
{
	this->sampledImgM.clear();
	delete[]changeRateArr;

}
double VideoChange::getSampleNumber()
{
	return this->sample;
}
void VideoChange::writeChangeRate() {

	for (int i = 0; i < changeRateArr->length(); i++) {

		outFile << changeRateArr[i];
	}
}

