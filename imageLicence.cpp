#include "stdafx.h"
#include "imageLicence.h"
ImageLicence::ImageLicence(void)
{
}

ImageLicence::~ImageLicence(void)
{
}

bool ImageLicence::verifySize(RotatedRect mr)  //������̬�ȡ�����Ƿ����Ҫ��
{
	float error = 0.5;

	float aspect = 3.142857;

	float min = 1 * aspect * 1; //��С����
	float max = 2000 * aspect * 2000; //�������
									  
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
	//�ҵ���ɫ����
	int i = 0, j = 0;
	unsigned char * pPixel = NULL;
	unsigned char   pixelR = 0, pixelG = 0, pixelB = 0;
	unsigned char R = 28, G = 63, B = 138;

	Mat srcImage, image, bwImage, dstImage;

	double area, rectArea;
	double long2Short = 1.0;  //��̬��
							 //����߽����еĲ��� ���� ��� ���� ��С���� 
							 //�����ÿ���߽�Ĳ���
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
	dilate(bwImage, bwImage, element); //����
	erode(bwImage, bwImage, element); //��ʴ

	vector<vector<Point>> contours;
	findContours(bwImage, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);  //�ҵ�����

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
				return Mat(dstImage, Rect(rect_points[2].x, rect_points[2].y, axisLong, axisShort)); //���س���ͼƬ
			}
		}
	}

	return dstImage; //�����л��˺�Ȧ��ͼƬ
}

Mat ImageLicence::licence_seg(Mat licence, int threshold_flag)
{
	Mat matTmp;
	cvtColor(licence, matTmp, CV_BGR2GRAY);
	threshold(matTmp, matTmp, 100, 255, CV_THRESH_BINARY); //��ֵ��
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(matTmp, contours, hierarchy,
		CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE); //��ȡ�������������������contours�У������ȼ������hierarchy��
	float wh_scale = 0.00001;
	for (int i = 0; i < contours.size(); i++)
	{
		Rect rect = boundingRect(Mat(contours[i]));// ���������Ĵ�ֱ�߽���С���Σ���������ͼ�����±߽�ƽ�е�
		wh_scale = float(rect.width) / float(rect.height);
		if (rect.area() > 500 && wh_scale<1 && wh_scale>0.2) //
		{
			rectangle(licence, rect.tl(), rect.br(), Scalar(255, 0, 0)); //������β�����������
		}
	}
	return licence;
}