#pragma once

#include "opencv2/imgproc/imgproc.hpp"  
#include "opencv2/highgui/highgui.hpp"
#include "cv.h"
// CDlgCharaterSeg �Ի���

class CDlgCharaterSeg : public CDialog
{
	DECLARE_DYNAMIC(CDlgCharaterSeg)

public:
	CDlgCharaterSeg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCharaterSeg();
	void drawpic(IplImage * img, unsigned int id);

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CharacterSeg };
#endif

protected:
	CRect rect;
	CStatic* pStc; //��ʶͼ����ʾ��Picture�ؼ�  
	CDC* pDC; //��ʾ�ؼ��豸������  
	HDC hDC;  //��ʾ�ؼ��豸��� 

	char *myfilename;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedBShowimg();
	afx_msg void OnBnClickedBCharaterseg();
};
