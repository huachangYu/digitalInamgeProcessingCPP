#pragma once


// CImagePan �Ի���

class CImagePan : public CDialog
{
	DECLARE_DYNAMIC(CImagePan)

public:
	CImagePan(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CImagePan();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int pan_x;
	int pan_y;
};
