#include "stdafx.h"
#include "imageLicence.h"
ImageLicence::ImageLicence(void)
{
}

ImageLicence::~ImageLicence(void)
{
}

bool ImageLicence::verifySize(RotatedRect mr)  //检验体态比、面积是否符合要求
{
	float error = 0.5;

	float aspect = 3.142857;

	float min = 1 * aspect * 1; //最小区域
	float max = 2000 * aspect * 2000; //最大区域
									  
	float rmin = aspect - aspect*error;
	float rmax = aspect + aspect*error;

	int area = mr.size.height * mr.size.width;
	float r = (float)mr.size.width / (float)mr.size.height;
	if (r < 1)
	{
		r = (float)mr.size.height / (float)mr.size.width;
	}

	if ((area < min || area > max) || (r < rmin || r > rmax))
	{
		return false;
	}
	else
	{
		return true;
	}
}

Mat ImageLicence::licence_detect(String imagePath,bool showcar)
{
	int maxDif = 50;
	//找到蓝色区域
	int i = 0, j = 0;
	unsigned char * pPixel = NULL;
	unsigned char   pixelR = 0, pixelG = 0, pixelB = 0;
	unsigned char R = 28, G = 63, B = 138;

	Mat srcImage, image, bwImage, dstImage;

	double area, rectArea;
	double long2Short = 1.0;  //体态比
							 //计算边界序列的参数 长度 面积 矩形 最小矩形 
							 //并输出每个边界的参数
	double axisLong = 0.0, axisShort = 0.0;
	double temp;

	srcImage = imread(imagePath);
	if (!srcImage.data)
	{
		//return ;
	}
	image = srcImage.clone();

	GaussianBlur(image, image, Size(3, 3), 0, 0, BORDER_DEFAULT);

	bwImage = Mat::zeros(image.size(), 0);
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			pPixel = (unsigned char*)image.data + i*image.step + j * 3;
			pixelB = pPixel[0];
			pixelG = pPixel[1];
			pixelR = pPixel[2];
			if (abs(pixelB - B) < maxDif && abs(pixelG - G) < maxDif && abs(pixelR - R) < maxDif)
			{
				*((unsigned char*)bwImage.data + i*bwImage.step + j) = 255;
			}
			else {
				*((unsigned char*)bwImage.data + i*bwImage.step + j) = 0;
			}
		}
	}
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(bwImage, bwImage, element); //膨胀
	erode(bwImage, bwImage, element); //腐蚀

	vector<vector<Point>> contours;
	findContours(bwImage, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);  //找到轮廓

	Mat tempdstImage(bwImage.size(), CV_8U, Scalar(0));

	vector<vector<Point>>::const_iterator itc = contours.begin();
	while (itc != contours.end())
	{
		RotatedRect mr = minAreaRect(Mat(*itc));
		area = contourArea(*itc);
		if (!ImageLicence::verifySize(mr))
			itc = contours.erase(itc);
		else if (area < 1000 || area > 50000)
			itc = contours.erase(itc);
		else
			++itc;
	}
	dstImage = srcImage.clone();
	vector<RotatedRect> minRect(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		minRect[i] = minAreaRect(Mat(contours[i]));
		Point2f rect_points[4];
		minRect[i].points(rect_points);

		axisLong = sqrt(pow(rect_points[1].x - rect_points[0].x, 2) + pow(rect_points[1].y - rect_points[0].y, 2));
		axisShort = sqrt(pow(rect_points[2].x - rect_points[1].x, 2) + pow(rect_points[2].y - rect_points[1].y, 2));
		if (axisShort > axisLong)
		{
			temp = axisLong;
			axisLong = axisShort;
			axisShort = temp;
		}
		rectArea = axisLong*axisShort;
		long2Short = axisLong / axisShort;
		if (long2Short > 2.8 && long2Short < 3.8)
		{
			for (int j = 0; j < 4; j++)
			{
				line(dstImage, rect_points[j], rect_points[(j + 1) % 4], CV_RGB(255, 0, 0), 2);
			}
			if (!showcar)
			{
				return Mat(dstImage, Rect(rect_points[2].x, rect_points[2].y, axisLong, axisShort)); //返回车牌图片
			}
		}
	}

	return dstImage; //返回有画了红圈的图片
}

Mat ImageLicence::licence_seg(Mat licence, int threshold_flag)
{
	Mat matTmp;
	cvtColor(licence, matTmp, CV_BGR2GRAY);
	threshold(matTmp, matTmp, 100, 255, CV_THRESH_BINARY); //阈值化
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(matTmp, contours, hierarchy,
		CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE); //获取所有轮廓，轮廓存放在contours中，轮廓等级存放在hierarchy中
	float wh_scale = 0.00001;
	for (int i = 0; i < contours.size(); i++)
	{
		Rect rect = boundingRect(Mat(contours[i]));// 计算轮廓的垂直边界最小矩形，矩形是与图像上下边界平行的
		wh_scale = float(rect.width) / float(rect.height);
		if (rect.area() > 500 && wh_scale<1 && wh_scale>0.2) //
		{
			rectangle(licence, rect.tl(), rect.br(), Scalar(255, 0, 0)); //传入矩形参数来画矩形
		}
	}
	return licence;
}