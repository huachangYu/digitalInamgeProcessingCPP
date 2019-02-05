#if !defined(AFX_DLGINTENSITY_H__E4E1A0FF_79AC_4575_8C7E_411DE4E9168F__INCLUDED_)
#define AFX_DLGINTENSITY_H__E4E1A0FF_79AC_4575_8C7E_411DE4E9168F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CDlgIntensity : public CDialog
{
	DECLARE_DYNAMIC(CDlgIntensity)

public:
	CDlgIntensity(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgIntensity();

	// ��ǰ����϶�״̬��0��ʾδ�϶���1��ʾ�����϶����ޣ�2��ʾ�����϶����ޡ�
	BYTE* m_pPixel;
	int	m_iIsDraging;
	// ��Ӧ����¼��ľ�������
	CRect m_MouseRect;
	// DIB�ĸ߶�
	LONG m_lHeight;
	// DIB�Ŀ��
	LONG m_nWidth;
	// �����Ҷ�ֵ�ļ���
	LONG m_lCount[256];

	enum { IDD = IDD_DLG_Intensity };
	int	m_iLowGray;
	int	m_iUpGray;

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

	virtual BOOL OnInitDialog();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnOK();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillfocusEDITLowGray();
	afx_msg void OnKillfocusEDITUpGray();
};
#endif
