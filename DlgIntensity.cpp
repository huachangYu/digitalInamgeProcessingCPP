#include "stdafx.h"
#include "yuhuachang.h"
#include "DlgIntensity.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CDlgIntensity, CDialog)

CDlgIntensity::CDlgIntensity(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_Intensity, pParent)
{
	m_iLowGray = 0;
	m_iUpGray = 0;
}

CDlgIntensity::~CDlgIntensity()
{
}

void CDlgIntensity::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LowGray, m_iLowGray);
	DDV_MinMaxInt(pDX, m_iLowGray, 0, 255);
	DDX_Text(pDX, IDC_EDIT_UpGray, m_iUpGray);
	DDV_MinMaxInt(pDX, m_iUpGray, 0, 255);
}

BEGIN_MESSAGE_MAP(CDlgIntensity, CDialog)
	ON_EN_KILLFOCUS(IDC_EDIT_LowGray, &CDlgIntensity::OnKillfocusEDITLowGray)
	ON_EN_KILLFOCUS(IDC_EDIT_UpGray, &CDlgIntensity::OnKillfocusEDITUpGray)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CDlgIntensity::OnInitDialog()
{
	LONG i;
	LONG j;
	CDialog::OnInitDialog();
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	// 计算接受鼠标事件的有效区域
	pWnd->GetClientRect(m_MouseRect);
	pWnd->ClientToScreen(&m_MouseRect);
	CRect rect;
	GetClientRect(rect);
	ClientToScreen(&rect);
	m_MouseRect.top -= rect.top;
	m_MouseRect.left -= rect.left;
	// 设置接受鼠标事件的有效区域
	m_MouseRect.top += 25;
	m_MouseRect.left += 10;
	m_MouseRect.bottom = m_MouseRect.top + 255;
	m_MouseRect.right = m_MouseRect.left + 256;
	for (i = 0; i < 256; i++)
	{
		m_lCount[i] = 0;
	}
	for (i = 0; i < m_lHeight; i++)
	{
		for (j = 0; j < m_nWidth; j++)
		{
			int temp = m_pPixel[i*m_nWidth + j];
			m_lCount[temp]++;
		}
	}
	// 初始化拖动状态
	m_iIsDraging = 0;
	return TRUE;
}

void CDlgIntensity::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 判断是否在接受鼠标事件的有效区域中
	if (m_MouseRect.PtInRect(point))
	{
		if (point.x == (m_MouseRect.left + m_iLowGray))
		{
			// 设置拖动状态1，拖动下限
			m_iIsDraging = 1;
			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
		else if (point.x == (m_MouseRect.left + m_iUpGray))
		{

			// 设置拖动状态为2，拖动上限
			m_iIsDraging = 2;
			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}
	// 默认单击鼠标左键处理事件
	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgIntensity::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_iIsDraging != 0)
	{
		// 重置拖动状态
		m_iIsDraging = 0;
	}
	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgIntensity::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseRect.PtInRect(point))
	{
		if (m_iIsDraging != 0)
		{
			if (m_iIsDraging == 1)
			{
				if (point.x - m_MouseRect.left < m_iUpGray)
				{
					m_iLowGray = point.x - m_MouseRect.left;
				}
				else
				{
					m_iLowGray = m_iUpGray - 1;
					point.x = m_MouseRect.left + m_iUpGray - 1;
				}
			}
			else
			{
				if (point.x - m_MouseRect.left > m_iLowGray)
				{
					m_iUpGray = point.x - m_MouseRect.left;
				}
				else
				{
					m_iUpGray = m_iLowGray + 1;
					point.x = m_MouseRect.left + m_iLowGray + 1;
				}
			}
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			UpdateData(FALSE);
			InvalidateRect(m_MouseRect, TRUE);
		}
		else if (point.x == (m_MouseRect.left + m_iLowGray) || point.x == (m_MouseRect.left + m_iUpGray))
		{
			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CDlgIntensity::OnOK()
{
	if (m_iLowGray > m_iUpGray)
	{
		// 互换
		int iTemp = m_iLowGray;
		m_iLowGray = m_iUpGray;
		m_iUpGray = iTemp;
	}
	// 返回
	CDialog::OnOK();
}

void CDlgIntensity::OnPaint()
{
	CString str;
	LONG i;
	LONG lMaxCount = 0;
	CPaintDC dc(this);
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	// 指针
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();
	pDC->Rectangle(0, 0, 330, 300);
	CPen* pPenRed = new CPen;
	pPenRed->CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	CPen* pPenBlue = new CPen;
	pPenBlue->CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	CPen* pPenGreen = new CPen;
	pPenGreen->CreatePen(PS_DOT, 1, RGB(0, 255, 0));
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	pDC->MoveTo(10, 10);
	pDC->LineTo(10, 280);
	pDC->LineTo(320, 280);
	str.Format(_T("0"));
	pDC->TextOut(10, 283, str);
	str.Format(_T("50"));
	pDC->TextOut(60, 283, str);
	str.Format(_T("100"));
	pDC->TextOut(110, 283, str);
	str.Format(_T("150"));
	pDC->TextOut(160, 283, str);
	str.Format(_T("200"));
	pDC->TextOut(210, 283, str);
	str.Format(_T("255"));
	pDC->TextOut(265, 283, str);

	for (i = 0; i < 256; i += 5)
	{
		if ((i & 1) == 0)
		{
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 284);
		}
		else
		{
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 282);
		}
	}
	pDC->MoveTo(315, 275);
	pDC->LineTo(320, 280);
	pDC->LineTo(315, 285);
	pDC->MoveTo(10, 10);
	pDC->LineTo(5, 15);
	pDC->MoveTo(10, 10);
	pDC->LineTo(15, 15);
	for (i = m_iLowGray; i <= m_iUpGray; i++)
	{
		if (m_lCount[i] > lMaxCount)
		{
			lMaxCount = m_lCount[i];
		}
	}
	pDC->MoveTo(10, 25);
	pDC->LineTo(14, 25);
	str.Format(_T("%d"), lMaxCount);
	pDC->TextOut(11, 26, str);
	pDC->SelectObject(pPenGreen);
	pDC->MoveTo(m_iLowGray + 10, 25);
	pDC->LineTo(m_iLowGray + 10, 280);
	pDC->MoveTo(m_iUpGray + 10, 25);
	pDC->LineTo(m_iUpGray + 10, 280);
	pDC->SelectObject(pPenBlue);
	// 判断是否有计数
	if (lMaxCount > 0)
	{
		// 绘制直方图
		for (i = m_iLowGray; i <= m_iUpGray; i++)
		{
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 281 - (int)(m_lCount[i] * 256 / lMaxCount));
		}
	}
	// 恢复以前的画笔
	pDC->SelectObject(pOldPen);
	// 删除新的画笔
	delete pPenRed;
	delete pPenBlue;
	delete pPenGreen;
}


void CDlgIntensity::OnKillfocusEDITLowGray()
{
	UpdateData(TRUE);
	// 判断是否下限超过上限
	if (m_iLowGray > m_iUpGray)
	{
		// 互换
		int iTemp = m_iLowGray;
		m_iLowGray = m_iUpGray;
		m_iUpGray = iTemp;
		// 更新（成员变量中数值保存到控件中）
		UpdateData(FALSE);
	}
	// 重绘直方图
	InvalidateRect(m_MouseRect, TRUE);
}


void CDlgIntensity::OnKillfocusEDITUpGray()
{
	UpdateData(TRUE);
	// 判断是否下限超过上限
	if (m_iLowGray > m_iUpGray)
	{
		// 互换
		int iTemp = m_iLowGray;
		m_iLowGray = m_iUpGray;
		m_iUpGray = iTemp;
		// 更新
		UpdateData(FALSE);
	}
	// 重绘直方图
	InvalidateRect(m_MouseRect, TRUE);
}
