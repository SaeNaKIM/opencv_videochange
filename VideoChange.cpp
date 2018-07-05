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

	
	/*Mat tempDst1;
	Mat tempDst2;

	Mat tempDst3;
	Mat tempDst4;
	Mat tempDst5;
	Mat tempDst6;
	Mat tempDst7;
	Mat tempDst8;*/

	/*
	thread t1(&VideoChange::threadforpreprocess, this, src(Rect(0, 0, src.cols / 2, src.rows )),std::ref(tempDst1));
	thread t2(&VideoChange::threadforpreprocess, this, src(Rect(src.cols / 2 + 1, 0, src.cols / 2, src.rows )), std::ref(tempDst2));
	thread t3(&VideoChange::threadforpreprocess, this, src(Rect(2 * src.cols / 4 + 1, 0, src.cols / 4, src.rows / 2)), dst(Rect(2 * dst.cols / 4 + 1, 0, dst.cols / 4, dst.rows / 2)));
	thread t4(&VideoChange::threadforpreprocess, this, src(Rect(3 * src.cols / 4 + 1, 0, src.cols / 4 - 1, src.rows / 2)), dst(Rect(3 * dst.cols / 4 + 1, 0, dst.cols / 4 - 1, dst.rows / 2)));

	thread t5(&VideoChange::threadforpreprocess, this, src(Rect(0, src.rows / 2, src.cols / 4, src.rows / 2)), dst(Rect(0, dst.rows / 2, dst.cols / 4, dst.rows / 2)));
	thread t6(&VideoChange::threadforpreprocess, this, src(Rect(src.cols / 4 + 1, src.rows / 2, src.cols / 4, src.rows / 2)), dst(Rect(dst.cols / 4 + 1, dst.rows / 2, dst.cols / 4, dst.rows / 2)));
	thread t7(&VideoChange::threadforpreprocess, this, src(Rect(2 * src.cols / 4 + 1, src.rows / 2, src.cols / 4, src.rows / 2)), dst(Rect(2 * dst.cols / 4 + 1, dst.rows / 2, dst.cols / 4, dst.rows / 2)));
	thread t8(&VideoChange::threadforpreprocess, this, src(Rect(3 * src.cols / 4 + 1, src.rows / 2, src.cols / 4 - 1, src.rows / 2)), dst(Rect(3 * dst.cols / 4 + 1, dst.rows / 2, dst.cols / 4 - 1, dst.rows / 2)));
	*/

	/*
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();
	t8.join();
	*/

	//tempDst1.copyTo(dst(Rect(0, 0, dst.cols / 4, dst.rows / 2))) ;
	//tempDst2.copyTo(dst(Rect(dst.cols / 4 + 1, 0, dst.cols / 4, dst.rows / 2)));


	/*
	mutex_lock.lock();
	imshow("dst", dst);
	imshow("dst1", tempDst1);
	imshow("dst2", tempDst2);
	waitKey(0);
	mutex_lock.unlock();
	*/

	//dst(Rect(0, 0, tempDstL.cols, tempDstL.rows)) = tempDstL.clone();
	//dst(Rect(tempDstR.cols, 0, tempDstR.cols, tempDstR.rows)) = tempDstR.clone();
}
void VideoChange::threadforpreprocess(Mat src, Mat &dst) {

	
	Mat gray;
	Mat resizeMat;

	cvtColor(src, gray, CV_BGR2GRAY); // convert channel 3 to 1 
	resize(gray, resizeMat, Size(gray.cols / 4, gray.rows / 4));
	GaussianBlur(resizeMat, dst, Size(3, 3), 0); //Blurring 


}
void VideoChange::samplingVideoFrame(VideoCapture cap1, VideoCapture cap2) {

	
	int nframe = this->nframe; 
	

	thread t1(&VideoChange::threadsamplingVideoFrame, this, cap1, 0, nframe/2, "cap1");
	thread t2(&VideoChange::threadsamplingVideoFrame, this, cap2, nframe/2 + 1, nframe, "cap2");

	t1.join();
	t2.join();

	cout << "sampling thread is completed\n";
	return;

}
void VideoChange::samplingVideoFrame(VideoCapture cap1, VideoCapture cap2, VideoCapture cap3, VideoCapture cap4)
{
	int nframe = this->nframe;


	thread t1(&VideoChange::threadsamplingVideoFrame, this, cap1, 0, nframe / 4, "cap1");
	thread t2(&VideoChange::threadsamplingVideoFrame, this, cap2, nframe / 4 + 1, 2 * nframe /4, "cap2");
	thread t3(&VideoChange::threadsamplingVideoFrame, this, cap3,  2 * nframe / 4 + 1, 3 * nframe /4 , "cap3");
	thread t4(&VideoChange::threadsamplingVideoFrame, this, cap4, 3 * nframe / 4 + 1,nframe, "cap4");

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	cout << "sampling thread is completed\n";
	return;
}
void VideoChange::threadsamplingVideoFrame(VideoCapture cap, int begin, int end, string windowname)
{
	int key;

	for (int i = begin; i < end; i += this->sample) {
		
		
		Mat frame, grayFrame;

		cap.set(CV_CAP_PROP_POS_FRAMES, i);
		cap.read(frame);
		preprocess(frame, grayFrame);

		mutex_lock.lock();
		this->sampledImgM.insert(make_pair(i,grayFrame));
		imshow(windowname, grayFrame);
		waitKey(1);
		mutex_lock.unlock();	
		
		//vector debog
		//mutex_lock.lock();
		//this->sampledImgV.push_back(grayFrame);
		//imshow("sampling Frame", grayFrame);
		//waitKey(1);
		////cout << to_string(i) << "th frame\n";
		//mutex_lock.unlock();

		// thread 안에서 해결 가능한지 모름.
		//if (key == 27) { //ESC
		//	cout << "terminated during sampling video" << endl;
		//	break;
		//}

	}
}
void VideoChange::samplingVideoFrame(Mat frame) {

	Mat tmp;
	tmp = frame.clone(); //왜 복사해서 넣었지...?
	sampledImgV.push_back(tmp); // 언제 clear 해줄건지 정해야함. * 벡터를 신중히 다룹시다..

}
 void VideoChange::backgroundEstimation(Mat &background, int type) {

	clock_t begin, end;
	begin = clock();

	Mat sum = Mat::zeros(sampledImgM[0].size(), CV_32FC1);
	Mat temp;
	Mat avg;
	Mat blur;

	map<int, Mat>::iterator iter;
	double sampledImgL = sampledImgM.size();


	if (type == 1) { // mean 

		for (iter = sampledImgM.begin(); iter != sampledImgM.end(); iter++) {

			//debug
			/*imshow("frame push check", iter->second);
			waitKey(0);*/
			
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
void VideoChange::threadForDetectChangeFrame1(int begin, int end, double pixelThreshold)
{
	int nonZeroCnt = 0;
	double changeRate = 0.0f;
	int imgSize = sampledImgM[begin].rows * sampledImgM[begin].cols;

	Mat hist_frame;
	Mat absImg;
	Mat thresholdImg;

	map<int, Mat>::iterator iter;
	int i ;
	for (iter = sampledImgM.begin(), i = begin; iter != sampledImgM.find(end * this->sample); iter++, i++){

		equalizeHist(iter->second, hist_frame);
		absImg = abs(hist_frame - this->background);
		
		mutex_lock.lock();
		imshow("hist_frame", hist_frame);
		imshow("absImg", absImg);
		waitKey(0);
		mutex_lock.unlock();
		
		
		threshold(absImg, thresholdImg, 50, 255, THRESH_BINARY);
		nonZeroCnt = countNonZero(thresholdImg);
		changeRate = (double)nonZeroCnt / imgSize;


		if (changeRate > pixelThreshold) {

			// store image and time of the frame 
			string filename = string(this->directory) + "\\frame_" + to_string(i*this->sample / this->fps) + ".png";
			imwrite(filename, sampledImgM[i]);

			//debugging    
			//imshow("change image", sampledImgV[i]);
			//imshow("abs image", absImg);
			//imshow("threshold image", thresholdImg);
			//waitKey(0);
		}

		changeRateArr[i] = to_string(i * this->sample/ this->fps) + "," + to_string(changeRate) + "\n"; // n-th frame 
		//i++;

	}

	return;
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

	//changeRateArr[i] = to_string(i*this->sample / this->fps) + "," + to_string(changeRate) + "\n"; // n-th frame 
	mutex_lock.lock();
	changeRateArrN[i] += changeRate;
	mutex_lock.unlock();																		  


	return;
}
// compute difference between background image and sampled image. Save the interesting image(.png) and data(.csv)
void VideoChange::detectChangeFrame(int detectType, double pixelThreshold) {

	clock_t begin, end;
	begin = clock();

	Mat absImg;
	int sampledImgVLength = this->sampledImgM.size();

   if (detectType == 1) { // pixel 

		equalizeHist(this->background, this->background);

		thread t1{&VideoChange::threadForDetectChangeFrame1, this, 0, sampledImgVLength/2, pixelThreshold };
		thread t2{&VideoChange::threadForDetectChangeFrame1, this,  sampledImgVLength/2+1, sampledImgVLength, pixelThreshold};
		/*thread t3{ &VideoChange::threadForDetectChangeFrame1, this, 2*sampledImgVLength/8+1, 3*sampledImgVLength/8, pixelThreshold };
		thread t4{ &VideoChange::threadForDetectChangeFrame1, this, 3*sampledImgVLength/8+1, 4*sampledImgVLength / 8, pixelThreshold };
		thread t5{ &VideoChange::threadForDetectChangeFrame1, this, 4 * sampledImgVLength / 8 + 1, 5*sampledImgVLength / 8, pixelThreshold };
		thread t6{ &VideoChange::threadForDetectChangeFrame1, this, 5*sampledImgVLength / 8 + 1, 6 * sampledImgVLength / 8, pixelThreshold };
		thread t7{ &VideoChange::threadForDetectChangeFrame1, this, 6 * sampledImgVLength / 8 + 1, 7 * sampledImgVLength / 8, pixelThreshold };
		thread t8{ &VideoChange::threadForDetectChangeFrame1, this, 7 * sampledImgVLength / 8 + 1, sampledImgVLength, pixelThreshold };
		*/

		t1.join();
		t2.join();
		/*t3.join();
		t4.join();
		t5.join();
		t6.join();
		t7.join();
		t8.join();*/


		cout << "thread is finished" << endl;

	}
	else if (detectType == 2) { //blob 

	}

	end = clock();
	cout << "change detection time in func: " << end - begin << endl;

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
	//changeRateArrN = new int[this->sampledImgV.size()];

	/*for (int i = 0; i < this->sampledImgV.size(); i++) {
		
		changeRateArrN[i] = 0;
	
	}*/

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
	this->sampledImgM.clear();
	delete[]changeRateArr;

}
double VideoChange::getSampleNumber()
{
	return this->sample;
}
void VideoChange::writeChangeRate() {

	for (int i = 0; i < sampledImgV.size(); i++) {

		//changeRateArr[i] = to_string(i*this->sample / this->fps) + "," + to_string( changeRateArrN[i]) + "\n"; // n-th frame 
		//outFile << to_string(i*this->sample / this->fps) + "," + to_string(changeRateArrN[i]) + "\n"; // n-th frame 
		outFile << changeRateArr[i];
	}
}
void VideoChange::storeChangeRate(string storeValue) {

	this->outFile << storeValue;

}
