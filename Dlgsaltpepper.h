#pragma once


// CDlgsaltpepper �Ի���

class CDlgsaltpepper : public CDialog
{
	DECLARE_DYNAMIC(CDlgsaltpepper)

public:
	CDlgsaltpepper(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgsaltpepper();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgSaltPepperNoise };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	float SNR;
};
