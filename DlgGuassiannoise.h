#pragma once


// CDlgGuassiannoise �Ի���

class CDlgGuassiannoise : public CDialog
{
	DECLARE_DYNAMIC(CDlgGuassiannoise)

public:
	CDlgGuassiannoise(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgGuassiannoise();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GuassianNoise };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	float mean_value;
	float variance;
};
