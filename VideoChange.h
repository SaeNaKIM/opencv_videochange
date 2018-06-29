#include <iostream>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;


class VideoChange
{

private : 

	
	string filename;
	vector<Mat> sampledImgV;
	vector<string> changeRateV;
	Mat background;
	Mat changeGraph;
	ofstream outFile;

	 double dstFrameLoc;

	//file information
	float fps;
	float cols;
	float rows;
	double nframe;
	char directory[10];

	//video control
	bool stopFlag = false;
	double samplingRate = 1;
	double sample = 0;



public :

	//constructor 
	VideoChange(string filename,VideoCapture cap);

	void videoInfoPrint();

	void samplingVideoFrame(Mat Frame);  

	void backgroundEstimation( Mat &background, int type );
	
	double detectChangeFrame(int begin, int end, double pixelThreshold);

	void detectChangeFrame(int detectType, string outfilename, double pixelThreshold); // compute difference between background image and samplec img and Save the interesting image(.png)

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

	void thread_exercise();
	
	void thread_1(int i );
};