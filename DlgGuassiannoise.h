#pragma once


// CDlgGuassiannoise 对话框

class CDlgGuassiannoise : public CDialog
{
	DECLARE_DYNAMIC(CDlgGuassiannoise)

public:
	CDlgGuassiannoise(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgGuassiannoise();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GuassianNoise };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	float mean_value;
	float variance;
};
