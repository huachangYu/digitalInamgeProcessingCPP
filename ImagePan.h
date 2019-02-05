#pragma once


// CImagePan 对话框

class CImagePan : public CDialog
{
	DECLARE_DYNAMIC(CImagePan)

public:
	CImagePan(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImagePan();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int pan_x;
	int pan_y;
};
