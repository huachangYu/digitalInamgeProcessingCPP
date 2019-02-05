#include "stdafx.h"
#include "yuhuachang.h"
#include "DlgPointThre.h"
#include "afxdialogex.h"


// CDlgPointThre ¶Ô»°¿ò

IMPLEMENT_DYNAMIC(CDlgPointThre, CDialog)

CDlgPointThre::CDlgPointThre(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_PointThre, pParent)
	, m_bThre(0)
{

}

CDlgPointThre::~CDlgPointThre()
{
}

void CDlgPointThre::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Thre, m_bThre);
	DDV_MinMaxInt(pDX, m_bThre, 0, 255);
}


BEGIN_MESSAGE_MAP(CDlgPointThre, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_EN_KILLFOCUS(IDC_EDIT_Thre, &CDlgPointThre::OnKillfocusEditThre)
END_MESSAGE_MAP()


BOOL CDlgPointThre::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd* pWnd = GetDlgItem(IDC_COORD);
	pWnd->GetClientRect(m_MouseRect);
	pWnd->ClientToScreen(&m_MouseRect);

	CRect rect;
	GetClientRect(rect);
	ClientToScreen(&rect);

	m_MouseRect.top -= rect.top;
	m_MouseRect.left -= rect.left;

	m_MouseRect.top += 25;
	m_MouseRect.left -= 10;
	m_MouseRect.bottom = m_MouseRect.top + 25;
	m_MouseRect.right = m_MouseRect.left + 25;

	m_bisDraging = FALSE;

	return TRUE; 
}


void CDlgPointThre::OnPaint()
{
	CString str;
	CPaintDC dc(this);
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	CDC *pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();
	pDC->Rectangle(0, 0, 300, 300);
	CPen* pPenRed = new CPen;
	pPenRed->CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	CPen* pPenBlue = new CPen;
	pPenBlue->CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	CPen* pPenGreen = new CPen;
	pPenGreen->CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	pDC->MoveTo(10, 10);
	pDC->LineTo(10, 280);
	pDC->LineTo(320, 280);

	str.Format(_T("0"));
	pDC->TextOutW(10, 281, str);
	str.Format(_T("255"));
	pDC->TextOutW(265, 281, str);
	pDC->TextOutW(11, 25, str);
	pDC->LineTo(315, 275);
	pDC->MoveTo(320, 280);
	pDC->LineTo(315, 285);

	pDC->MoveTo(10, 10);
	pDC->LineTo(5, 15);

	pDC->MoveTo(10, 10);
	pDC->LineTo(15, 15);

	pDC->SelectObject(pPenGreen);
	pDC->MoveTo(m_bThre * 10, 25);
	pDC->MoveTo(m_bThre * 10, 280);

	pDC->SelectObject(pPenBlue);
	str.Format(_T("%d"), m_bThre);
	pDC->TextOutW(m_bThre + 10, 280,str);
	pDC->MoveTo(10, 280);
	pDC->LineTo(m_bThre+10, 280);
	pDC->LineTo(m_bThre + 10, 25);
	pDC->MoveTo(265, 25);

	pDC->SelectObject(pOldPen);

	pDC->MoveTo(10, 25);
	pDC->LineTo(265, 25);
	pDC->LineTo(265, 280);

	delete pPenRed;
	delete pPenBlue;
	delete pPenGreen;
}


void CDlgPointThre::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_MouseRect.PtInRect(point))
	{
		if (point.x == (m_MouseRect.left + m_bThre))
		{
			m_bisDraging = TRUE;

			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));


		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}


void CDlgPointThre::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bisDraging)
	{
		m_bisDraging = FALSE;
	}

	CDialog::OnLButtonUp(nFlags, point);
}


void CDlgPointThre::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_MouseRect.PtInRect(point))
	{
		m_bThre = (BYTE)(point.x - m_MouseRect.left);
		::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));

		UpdateData(FALSE);
		InvalidateRect(m_MouseRect, TRUE);
	}
	else if (point.x == (m_MouseRect.left + m_bThre))
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
	}

	CDialog::OnMouseMove(nFlags, point);
}


void CDlgPointThre::OnKillfocusEditThre()
{
	UpdateData(TRUE);

	InvalidateRect(m_MouseRect, TRUE);
}
