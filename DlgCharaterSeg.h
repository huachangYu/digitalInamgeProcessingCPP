#pragma once

#include "opencv2/imgproc/imgproc.hpp"  
#include "opencv2/highgui/highgui.hpp"
#include "cv.h"
// CDlgCharaterSeg 对话框

class CDlgCharaterSeg : public CDialog
{
	DECLARE_DYNAMIC(CDlgCharaterSeg)

public:
	CDlgCharaterSeg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCharaterSeg();
	void drawpic(IplImage * img, unsigned int id);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CharacterSeg };
#endif

protected:
	CRect rect;
	CStatic* pStc; //标识图像显示的Picture控件  
	CDC* pDC; //显示控件设备上下文  
	HDC hDC;  //显示控件设备句柄 

	char *myfilename;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedBShowimg();
	afx_msg void OnBnClickedBCharaterseg();
};
