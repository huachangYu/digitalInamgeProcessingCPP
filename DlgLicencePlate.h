#pragma once
#include "opencv2/imgproc/imgproc.hpp"  
#include "opencv2/highgui/highgui.hpp"
#include "cv.h"

using namespace cv;

// CDlgLicencePlate �Ի���

class CDlgLicencePlate : public CDialog
{
	DECLARE_DYNAMIC(CDlgLicencePlate)

public:
	CDlgLicencePlate(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgLicencePlate();
	void  drawpic(IplImage * img, unsigned int id);

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgLicensePlate };
#endif

protected:
	CRect rect;
	CStatic* pStc; //��ʶͼ����ʾ��Picture�ؼ�  
	CDC* pDC; //��ʾ�ؼ��豸������  
	HDC hDC;  //��ʾ�ؼ��豸��� 

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	//void showPlate(Mat pic);
	void DrawcvMat(Mat m_cvImg, UINT ID);
	
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedLinsenseseg();
};
