#pragma once


// CDlgsaltpepper 对话框

class CDlgsaltpepper : public CDialog
{
	DECLARE_DYNAMIC(CDlgsaltpepper)

public:
	CDlgsaltpepper(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgsaltpepper();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgSaltPepperNoise };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	float SNR;
};
