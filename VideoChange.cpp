#define  _CRT_SECURE_NO_WARNINGS
#include <iostream> 
#include <opencv2/opencv.hpp>
#include <vector>
#include <direct.h>
#include <thread>
#include <mutex>
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
	//GaussianBlur(resizeMat, dst, Size(3,3),0); //Blurring 
	
}
void VideoChange::samplingVideoFrame(Mat frame) {

	Mat tmp;
	tmp = frame.clone();
	sampledImgV.push_back(tmp); // 언제 clear 해줄건지 정해야함. * 벡터를 신중히 다룹시다..

}
void VideoChange::backgroundEstimation(Mat &background, int type) {

	Mat sum = Mat::zeros(sampledImgV[0].size(), CV_32FC1);
	Mat temp;
	Mat avg;
	Mat blur;

	double sampledImgL = sampledImgV.size();


	if (type == 1) { // mean 

		for (int i = 0; i < sampledImgV.size(); i++) {

			GaussianBlur(sampledImgV[i], sampledImgV[i], Size(3, 3), 0);
			accumulateWeighted(blur, sum, 1.0 / sampledImgL);
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
	return;
}
void VideoChange::threadForDetectChangeFrame1(int begin, int end, double pixelThreshold)
{
	int nonZeroCnt = 0;
	double changeRate = 0.0f;
	int imgSize = sampledImgV[begin].rows * sampledImgV[begin].cols;

	Mat hist_frame;
	Mat absImg;
	Mat thresholdImg;

	for (int i = begin; i < end; i++) {
	
		
		equalizeHist(sampledImgV[i], hist_frame);
		absImg = abs(hist_frame - this->background);
		threshold(absImg, thresholdImg, 50, 255, THRESH_BINARY);
		nonZeroCnt = countNonZero(thresholdImg);
		changeRate = (double)nonZeroCnt / imgSize;


		if (changeRate > pixelThreshold) {

			// store image and time of the frame 
			string filename = string(this->directory) + "\\frame_" + to_string(i*this->sample / this->fps) + ".png";
			imwrite(filename, sampledImgV[i]);

			//debugging    
			//imshow("change image", sampledImgV[i]);
			//imshow("abs image", absImg);
			//imshow("threshold image", thresholdImg);
			//waitKey(0);
		}

		changeRateArr[i] = to_string(i*this->sample / this->fps) + "," + to_string(changeRate) + "\n"; // n-th frame 
		
	}

	return ;
}
void VideoChange::threadForDetectChangeFrame2(Mat &src, Mat &dst, int i)
{
	int nonZeroCnt = 0;
	double changeRate = 0.0f;
	int imgSize = src.rows * src.cols;

	Mat hist_frame;
	Mat absImg;
	Mat thresholdImg;

	equalizeHist(src, hist_frame);
	absImg = abs(hist_frame - background);
	threshold(absImg, thresholdImg, 50, 255, THRESH_BINARY);
	nonZeroCnt = countNonZero(thresholdImg);
	changeRate = (double)nonZeroCnt / imgSize;


	if (changeRate > this->pixelThreshold) {

		// store image and time of the frame 
		string filename = string(this->directory) + "\\frame_" + to_string(i*this->sample / this->fps) + ".png";
		imwrite(filename, sampledImgV[i]);

		//debugging    
		//imshow("change image", sampledImgV[i]);
		//imshow("abs image", absImg);
		//imshow("threshold image", thresholdImg);
		//waitKey(0);
	}

	changeRateArr[i] = to_string(i*this->sample / this->fps) + "," + to_string(changeRate) + "\n"; // n-th frame 

																								   //}

	return ;
}
// compute difference between background image and sampled image. Save the interesting image(.png) and data(.csv)
void VideoChange::detectChangeFrame(int detectType, double pixelThreshold) {

	Mat absImg;
	int sampledImgVLength = sampledImgV.size() ;

	if (detectType == 1) { // pixel 

		equalizeHist(this->background, this->background);

		//thread t1{&VideoChange::threadForDetectChangeFrame, this, 0, sampledImgVLength/4, pixelThreshold };
		//thread t2{&VideoChange::threadForDetectChangeFrame, this, sampledImgVLength/4+1, sampledImgVLength/4, pixelThreshold};
		//thread t3{ &VideoChange::threadForDetectChangeFrame, this, 2*sampledImgVLength/4+1, 3*sampledImgVLength/4, pixelThreshold };
		//thread t4{ &VideoChange::threadForDetectChangeFrame, this, 3*sampledImgVLength/4+1, sampledImgVLength, pixelThreshold };

		//t1.join();
		//t2.join();
		//t3.join();
		//t4.join();

		cout << "thread is finished" << endl;

	}
	else if (detectType == 2) { //blob 

	}

	return;
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

void VideoChange::controlVideo(int key, double curFrameLoc)
{
	this->dstFrameLoc = curFrameLoc;

	if (key == 32) //space
	{
		//stop Toggle
		this->stopFlag = (this->stopFlag == true ? false : true);

	}
	else if (key == ']')
	{
		this->dstFrameLoc += 100;
	}
	else if (key == '[')
	{
		this->dstFrameLoc -= 100;
	}
	else if (key == '.')
	{
		this->dstFrameLoc += 10;
	}
	else if (key == ',')
	{
		this->dstFrameLoc -= 10;
	}

	return;
}

bool VideoChange::isStop()
{
	return stopFlag;
}
void VideoChange::setOutFilename(string filename)
{
	// csv file open
	this->outFile.open(filename + ".csv");
	strcpy(this->directory, filename.c_str());
	changeRateArr = new string[this->sampledImgV.size()];

	// make directory 
	int nResult = _mkdir(this->directory);
	
	if (nResult == 0)
	{
		cout << "directory creation sucess" << endl;
	}
	else { // directory already existed.

		cout << "directory creation failed " << endl;
	}

	return;
}
double VideoChange::getFrameLoc()
{
	return dstFrameLoc - 1;
}
void VideoChange::setSamplingRate(double samplingRate)
{
	this->samplingRate = samplingRate;
	this->sample = this->fps / this->samplingRate;
}
void VideoChange::clear()
{
	this->sampledImgV.clear();
	delete []changeRateArr;
	
}
double VideoChange::getSampleNumber()
{
	return this->sample;
}
void VideoChange::writeChangeRate() {

	for (int i = 0; i < sampledImgV.size(); i++) {
		
		outFile << changeRateArr[i];
	}	
}
void VideoChange::storeChangeRate(string storeValue) {

	this->outFile << storeValue;

}
