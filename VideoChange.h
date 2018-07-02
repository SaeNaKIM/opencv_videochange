#include <iostream>
#include <opencv2/opencv.hpp>
#include <mutex>


using namespace std;
using namespace cv;


class VideoChange
{

private : 
	
	string filename;
	vector<Mat> sampledImgV;
	string *changeRateArr;
	Mat background;
	ofstream outFile;

	double dstFrameLoc;

	//input video file information
	float fps;
	float cols;
	float rows;
	double nframe;
	char directory[50];

	//video control
	bool stopFlag = false;
	double samplingRate = 1;
	double sample = 0;

	//thread
	mutex mutex_lock;

public :

	//constructor 
	VideoChange(string filename,VideoCapture cap);

	void videoInfoPrint();

	void samplingVideoFrame(Mat Frame);  

	void backgroundEstimation( Mat &background, int type );
	
	double threadForDetectChangeFrame(int begin, int end, double pixelThreshold);

	void detectChangeFrame(int detectType,  double pixelThreshold); // compute difference between background image and samplec img and Save the interesting image(.png)

	string computeTime(int curFrameLoc); // write time to image  

	void writeChangeRate();

	void storeChangeRate(string storeValue);

	void preprocess(Mat &src, Mat &dst);

	void controlVideo(int key, double curFrameLoc);

	bool isStop();

	void setSamplingRate(double samplingRate);

	void setInFilename(string filename);

	void setOutFilename(string filename);

	double getFrameLoc();

	double getSampleNumber();

	void clear();

};