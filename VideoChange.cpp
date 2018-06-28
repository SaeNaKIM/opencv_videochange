#define  _CRT_SECURE_NO_WARNINGS
#include <iostream> 
#include <opencv2/opencv.hpp>
#include <vector>
#include <direct.h>
#include <thread>
#include "VideoChange.h"


VideoChange::VideoChange(string filename, VideoCapture cap) {

	setFilename(filename);
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
void VideoChange::samplingVideoFrame(Mat frame) {

	Mat tmp;
	tmp = frame.clone();
	sampledImgV.push_back(tmp); // 언제 clear 해줄건지 정해야함. * 벡터를 신중히 다룹시다..

}
void VideoChange::backgroundEstimation(Mat &background, int type) {

	Mat sum = Mat::zeros(sampledImgV[0].size(), CV_32FC1);
	Mat temp;
	Mat avg;

	double sampledImgL = sampledImgV.size();


	if (type == 1) { // mean 

		for (int i = 0; i < sampledImgV.size(); i++) {


			accumulateWeighted(sampledImgV[i], sum, 1.0 / sampledImgL);
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

	//imshow("background in function", this->background); waitKey(0); //debug
	background = this->background.clone();
	return;
}
// compute difference between background image and sampled image. Save the interesting image(.png) and data(.csv)
void VideoChange::detectChangeFrame(int detectType, string outfilename, double pixelThreshold) {


	int nonZeroCnt = 0;
	double changeRate = 0.0f;
	int imgSize = sampledImgV[0].rows * sampledImgV[0].cols;

	Mat absImg;
	Mat thresholdImg;
	Mat hist_frame;

	ofstream outFile(outfilename + ".csv");
	char directory[] = {"video_change"};
	
	int nResult = _mkdir(directory);

	if (nResult == 0)
	{
		cout << "directory creation sucess" << endl;
	}
	else { // directory already existed.

		cout << "directory creation failed " << endl;
	}

	if (detectType == 1) { // pixel 

		equalizeHist(this->background, this->background);

		for (int i = 0; i < sampledImgV.size(); i++) {

			//cout << "channel: " << sampledImgV[i].channels() << this->background.channels() << endl; // debug
			equalizeHist(sampledImgV[i], hist_frame);
			absImg = abs(hist_frame - this->background);
			
			threshold(absImg, thresholdImg, 50, 255, THRESH_BINARY);
			nonZeroCnt = countNonZero(thresholdImg);
			changeRate = (double)nonZeroCnt / imgSize;
			//cout << "change rate: " << changeRate << "\n";
		
			if (changeRate > pixelThreshold) {

				// store image and time of the frame 
				string filename = string(directory) + "\\frame_" + to_string(i*this->sample/ this->fps) + ".png";
				imwrite(filename, sampledImgV[i]);

				//debugging    
				//imshow("change image", sampledImgV[i]);
				//imshow("abs image", absImg);
				//imshow("threshold image", thresholdImg);
				//waitKey(0);
			}
			outFile << to_string(i*this->sample / this->fps) << "," << changeRate << "\n"; // n-th frame 
		}

	}
	else if (detectType == 2) { //blob - threshold 필요


	}

	outFile.close();
	return;
}
string VideoChange::computeTime(int curFrameLoc)
{
	//compute time 
	int min = (int)curFrameLoc * sample / 60;
	int sec = ((double)curFrameLoc * sample / 60.0 - min) * 60.0;
	string current_time = to_string(min) + "." + to_string(sec);

	return current_time;
}
void VideoChange::setFilename(string filename) {

	this->filename = filename;
}

void VideoChange::controlVideo(int key, double curFrameLoc)
{
	this->dstFrameLoc = curFrameLoc;


	if (key == 32)
	{
		//stop Toggle
		stopFlag = (stopFlag == true ? false : true);

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

double VideoChange::getFrameLoc()
{
	return dstFrameLoc - 1;
}

void VideoChange::setSamplingRate(double samplingRate)
{
	this->samplingRate = samplingRate;
	this->sample = this->fps / this->samplingRate;
}

void VideoChange::clearsampledImgV()
{
	this->sampledImgV.clear();
}
double VideoChange::getSampleNumber()
{
	return this->sample;
}
