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
	int samplingRate = 1;
	double sample = 0;



public :

	//constructor 
	VideoChange(string filename,VideoCapture cap);

	void videoInfoPrint();

	void samplingVideoFrame(Mat  Frame, double curFrameLoc);  

	void backgroundEstimation( Mat &background, int type );
	
	void detectChangeFrame(int detectType, string outfilename, double pixelThreshold); // compute difference between background image and samplec img and Save the interesting image(.png)

	string computeTime(int curFrameLoc); // write time to image  

	void drawChangeGraph(int sampleNum, double changeRate); // parameter�� ��� ���� ���ΰ� ���� �ؾ���...

	void setFilename(string filename);

	void preprocess(Mat &src, Mat &dst);

	void videoControl(int key, double curFrameLoc);

	bool isStop();

	double getFrameLoc();

	void setSamplingRate(int samplingRate);

	void clearsampledImgV();

	void setChangeGraph(int width);
};