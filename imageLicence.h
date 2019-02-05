#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;
class ImageLicence
{

private:
	Mat img1;
	Mat img2;
	Mat img3;
	Mat img4;
	Mat img5;
	Mat img6;
	Mat img7;


public:
	ImageLicence(void);
	~ImageLicence(void);
	static bool verifySize(RotatedRect mr);
	static Mat licence_detect(String imagePath,bool showcar);
	static Mat licence_seg(Mat licence,int threshold);
};