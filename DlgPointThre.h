#pragma once


// CDlgPointThre �Ի���

class CDlgPointThre : public CDialog
{
	DECLARE_DYNAMIC(CDlgPointThre)

public:
	CDlgPointThre(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPointThre();

	CRect m_MouseRect;
	BOOL m_bisDraging;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PointThre };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillfocusEditThre();
	int m_bThre;
};
