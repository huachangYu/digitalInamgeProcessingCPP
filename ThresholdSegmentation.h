#pragma once

class CThresholdSegmentation : public CDialog
{
	DECLARE_DYNAMIC(CThresholdSegmentation)

public:
	CThresholdSegmentation(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CThresholdSegmentation();
	

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_THRESHOLD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int val_threshold;
};
