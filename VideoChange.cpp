#define  _CRT_SECURE_NO_WARNINGS
#include <iostream> 
#include <opencv2/opencv.hpp>
#include <vector>
#include <direct.h>
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
void VideoChange:: preprocess(Mat &src, Mat &dst) {

	cvtColor(src, src, CV_BGR2GRAY); // convert channel 3 to 1 
	resize(src, src, Size(src.cols / 2, src.rows / 2));
	GaussianBlur(src, dst, Size(3, 3), 0); //Blurring 

}
// sampling rate, videofile path,  output - sampledImg
void VideoChange::samplingVideoFrame(Mat frame ,double curFrameLoc) {

	Mat tmp; 
	tmp = frame.clone();
	if (((int)curFrameLoc % (int)this->sample) == 0) {
			
		sampledImgV.push_back(tmp); // 언제 clear 해줄건지 정해야함. * 벡터를 신중히 다룹시다..
	
	}

} 
void VideoChange::backgroundEstimation(Mat &background, int type ) {
	
	Mat sum = Mat::zeros(sampledImgV[0].size(), CV_32FC1);
	Mat temp;
	Mat avg;

	double sampledImgL = sampledImgV.size();


	if (type == 1) { // mean 

		for (int i = 0; i < sampledImgV.size(); i++) {

		
			accumulateWeighted(sampledImgV[i], sum, 1.0 / sampledImgL);
			//sampledImgV[i].convertTo(temp, CV_32FC1);
			//sum += temp;
			//temp = temp / sampledImgL;
			//accumulate(sampledImgV[i], sum);
			//imshow("sum", sum);
			//waitKey(30);

		}

		convertScaleAbs(sum, this->background);
		//this->background = sum / sampledImgL;
		//sum.convertTo(avg, CV_8UC1, 1.0 / sampledImgL);
		//this->background = sum / sampledImgL;
		//imshow("background in function", avg);
		//waitKey(0);
		//cout << "background channgel" << this->background.channels() << "type: " << this->background.type() << "\n";


	}
	else if (type == 2) { // median of median 
	
		
		
	
	}
	else if (type == 3) { // gaussian of mixture 

	

	}
	else {

		cout << "wrong type" << endl;
		return;
	}

	//imshow("background in function", this->background); //debug
	background = this->background.clone();
	//background = avg.clone();
	return;
}

// compute difference between background image and sampled image. Save the interesting image(.png) and data(.csv)
void VideoChange::detectChangeFrame(int detectType, string outfilename){


	int nonZeroCnt = 0;
	double changeRate = 0.0f;
	double pixelThreshold = 0.10;
	int imgSize = sampledImgV[0].rows * sampledImgV[0].cols;
	
	Mat absImg;
	Mat norm_frame;
	Mat norm_bg;

	ofstream outFile(outfilename);
	char directory[] = { "C:\\Users\\Dev3Team\\source\\repos\\opencv_test1\\opencv_test1\\video_change" };
	int nResult = _mkdir(directory);
	
	if (nResult == 0)
	{
		cout << "directory creation sucess" << endl; 
	}
	else { // directory already existed.

		cout << "directory creation failed " << endl;
	}

	if (detectType == 1) { // pixel 
		
		setChangeGraph(sampledImgV.size()); // initialize chage graph
		normalize(this->background, norm_bg, 1.0, 0.0, NORM_MINMAX);

		for (int i = 0; i < sampledImgV.size(); i++) {

			//cout << "channel: " << sampledImgV[i].channels() << this->background.channels() << endl; // debug
			normalize(sampledImgV[i], norm_frame, 1.0, 0.0, NORM_MINMAX); // normalize data
			absImg = abs(norm_frame - norm_bg);
			nonZeroCnt = countNonZero(absImg);
			changeRate = (double)nonZeroCnt / imgSize;
			cout << "change rate: " << changeRate << "\n";

			if (changeRate > pixelThreshold) {
				
				// store image and time of the frame 
				string filename = string(directory) + "\\frame_" + to_string(i / samplingRate ) + ".png";
				imwrite(filename,sampledImgV[i]);

				//debugging    
				imshow("change image", sampledImgV[i]);
				imshow("abs image", absImg);
				waitKey(0);

			}


			drawChangeGraph(i,changeRate);
			outFile << i * sample << "," << changeRate << "\n"; // n-th frame 
		}

	}
	else if (detectType == 2) { //blob - threshold 필요
		
		
	}

	imshow("change graph", changeGraph);
	outFile.close();
	return;
}
void VideoChange::writeTime(Mat &frame, string timeContent) {



}
void VideoChange::writeTime(ofstream fs, string timeContent){


}

// parameter를 어떻게 만들 것인가 생각 해야함 
void VideoChange::drawChangeGraph(int sampleNum, double changeRate) {

	circle(this->changeGraph, Point2f(sampleNum, changeRate * 100), 1, Scalar(255, 0, 0));
}

void VideoChange::setFilename(string filename) {

	this->filename = filename;
}

void VideoChange::setFimeStream() {



}
void VideoChange::videoControl(int key, double curFrameLoc)
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

	return ;
}

bool VideoChange::isStop()
{
	return stopFlag;
}

double VideoChange::getFrameLoc()
{
	return dstFrameLoc - 1;
}

void VideoChange::setSamplingRate(int samplingRate)
{
	this->samplingRate = samplingRate;
	this->sample = this->fps / this->samplingRate;
}

void VideoChange::clearsampledImgV()
{
	this->sampledImgV.clear();
}

void VideoChange::setChangeGraph(int width)
{
	this->changeGraph = Mat::zeros(100, width, CV_8UC3);
}
