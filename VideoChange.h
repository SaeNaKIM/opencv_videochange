#include <iostream>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;


class VideoChange
{

private : 

	
	string filename;
	vector<Mat> sampledImgV;
	Mat background;
	Mat changeGraph;
	ofstream fs;

	 double dstFrameLoc;

	//file information
	float fps;
	float cols;
	float rows;
	double nframe;

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
	
	double detectChangeFrame(int begin, int end);

	void detectChangeFrame(int detectType, string outfilename, double pixelThreshold); // compute difference between background image and samplec img and Save the interesting image(.png)

	string computeTime(int curFrameLoc); // write time to image  

	void preprocess(Mat &src, Mat &dst);

	void controlVideo(int key, double curFrameLoc);

	bool isStop();

	void setSamplingRate(double samplingRate);

	void setFilename(string filename);

	double getFrameLoc();

	double getSampleNumber();

	void clearsampledImgV();
};