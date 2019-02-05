#pragma once
#include "opencv2/imgproc/imgproc.hpp"  
#include "opencv2/highgui/highgui.hpp"
#include "cv.h"

using namespace cv;

// CDlgLicencePlate 对话框

class CDlgLicencePlate : public CDialog
{
	DECLARE_DYNAMIC(CDlgLicencePlate)

public:
	CDlgLicencePlate(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLicencePlate();
	void  drawpic(IplImage * img, unsigned int id);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgLicensePlate };
#endif

protected:
	CRect rect;
	CStatic* pStc; //标识图像显示的Picture控件  
	CDC* pDC; //显示控件设备上下文  
	HDC hDC;  //显示控件设备句柄 

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	//void showPlate(Mat pic);
	void DrawcvMat(Mat m_cvImg, UINT ID);
	
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedLinsenseseg();
};
