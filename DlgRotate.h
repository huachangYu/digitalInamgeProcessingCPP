#pragma once


// CDlgRotate �Ի���

class CDlgRotate : public CDialog
{
	DECLARE_DYNAMIC(CDlgRotate)

public:
	CDlgRotate(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgRotate();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Rotate };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double angle;
};
