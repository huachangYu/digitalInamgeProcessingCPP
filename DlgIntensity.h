#if !defined(AFX_DLGINTENSITY_H__E4E1A0FF_79AC_4575_8C7E_411DE4E9168F__INCLUDED_)
#define AFX_DLGINTENSITY_H__E4E1A0FF_79AC_4575_8C7E_411DE4E9168F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CDlgIntensity : public CDialog
{
	DECLARE_DYNAMIC(CDlgIntensity)

public:
	CDlgIntensity(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgIntensity();

	// 当前鼠标拖动状态，0表示未拖动，1表示正在拖动下限，2表示正在拖动上限。
	BYTE* m_pPixel;
	int	m_iIsDraging;
	// 相应鼠标事件的矩形区域
	CRect m_MouseRect;
	// DIB的高度
	LONG m_lHeight;
	// DIB的宽度
	LONG m_nWidth;
	// 各个灰度值的计数
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
