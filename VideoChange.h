#include <iostream>
#include <opencv2/opencv.hpp>
#include <mutex>
#include <map>


using namespace std;
using namespace cv;


class VideoChange
{

private : 
	
	string filename;
	vector<Mat> sampledImgV;
	map<int,Mat> sampledImgM;
	string *changeRateArr;
	Mat background;
	ofstream outFile;
	VideoCapture cap1;
	VideoCapture cap2;

	//input video file information
	float fps;
	float cols;
	float rows;
	double nframe;
	char directory[64];

	//video control
	bool stopFlag = false;
	double samplingRate = 1;
	double sample = 0;
	double pixelThreshold = 0.06;

	//thread
	mutex mutex_lock;

public :

	//constructor 
	VideoChange(string filename,VideoCapture cap);

	void videoInfoPrint();

	//sampling

	void preprocess(Mat &src, Mat &dst);

	void threadsamplingVideoFrame(VideoCapture cap, int begin, int end, string windowname);

	void samplingVideoFrame(VideoCapture cap, VideoCapture cap2);

	
	//background
	
	void backgroundEstimation( Mat &background, int type );
	
	
	// change detect

	void detectChangeFrame(int detectType, double pixelThreshold); // compute difference between background image and samplec img and Save the interesting image(.png)
	
	void threadForDetectChangeFrameFront(int begin, int end, double pixelThreshold);

	void threadForDetectChangeFrameBack(int begin, int end, double pixelThreshold);
	
	// etc
	
	string computeTime(int curFrameLoc); // write time to image  

	void writeChangeRate();

	void setSamplingRate(double samplingRate);

	void setInFilename(string filename);

	void setOutFilename(string filename);

	double getSampleNumber();

	void clear();

};
