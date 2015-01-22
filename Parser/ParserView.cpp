
// ParserView.cpp : implementation of the CParserView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Parser.h"
#endif

#include "ParserDoc.h"
#include "ParserView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CParserView

IMPLEMENT_DYNCREATE(CParserView, CView)

BEGIN_MESSAGE_MAP(CParserView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CParserView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_ITERATION_MOVE_NEXT, &CParserView::OnIterationMoveNext)
	ON_MESSAGE(MM_SETNODE, &CParserView::OnSetnode)
	ON_COMMAND(ID_NEXTUNKNOW, &CParserView::OnNextunknow)
END_MESSAGE_MAP()

// CParserView construction/destruction

CParserView::CParserView()
	: m_pNd(NULL), m_pNdUnk(NULL)
{
	// TODO: add construction code here

}

CParserView::~CParserView()
{
}

BOOL CParserView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CParserView drawing

void CParserView::OnDraw(CDC* pDC)
{
	CParserDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CFont f;
	f.CreatePointFont(120, L"Bookman Old Style", pDC);
	pDC->SelectObject(&f);

	CString expr(pDoc->m_expr);
	int ini = pDoc->m_iterator->_i0,
		end = pDoc->m_iterator->_i;
	CRect wr;
	GetWindowRect(&wr);

	const int x0 = 10,
			  y0 = 10;
	int height;
	{
		CString str(expr, ini);
		CRect r(x0, y0, wr.right, 40);
		pDC->DrawText(str, &r, DT_CALCRECT | DT_SINGLELINE | DT_LEFT);
		height = r.Height();
		int xini = r.Width();
		
		r.right = wr.right;
		str = CString(expr, end);
		pDC->DrawText(str, &r, DT_CALCRECT | DT_SINGLELINE | DT_LEFT);
		int xend = r.Width();

		str = CString(expr, end < expr.GetLength() ? end + 1: end);
		pDC->DrawText(str, &r, DT_CALCRECT | DT_SINGLELINE | DT_LEFT);
		int xend1 = r.Width();

		r.right = wr.right;
		pDC->DrawText(expr, &r, DT_SINGLELINE | DT_LEFT);

		r.left = x0 + xend;
		r.right = x0 + xend1;
		CPen pen(PS_SOLID , 1, RGB(0xFF, 0x00, 0x00)),
			* oldPen = pDC->SelectObject(&pen);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(&r);

		pDC->SelectObject(oldPen);
		r.left = x0 + xini;
		r.right = x0 + xend;
		r.InflateRect(0, 2);
		pDC->DrawFocusRect(&r);

		drawNode(pDC, m_pNdUnk, x0, y0, height, expr);
		drawNode(pDC, m_pNd, x0, y0, height, expr);
	}

}

void CParserView::drawNode(CDC* pDC, const pmb::parser::node<transporter>* nd, int x0, int y0, int height, const CString& expr)
{
	if(nd)
	{
		CRect r(x0, y0, x0 + 100, y0 + height);
		CString str(expr, nd->getIni());
		pDC->DrawText(str, &r, DT_CALCRECT | DT_SINGLELINE | DT_LEFT);
		int xini = r.Width();

		str = CString(expr, nd->getEnd());
		pDC->DrawText(str, &r, DT_CALCRECT | DT_SINGLELINE | DT_LEFT);
		int xend = r.Width();
		r.top = y0;
		r.bottom = y0 + height;
		r.left = x0 + xini;
		r.right = x0 + xend;
		if(nd == m_pNd)
			r.InflateRect(1, 2);
		else
			r.InflateRect(2, 3);

		CPen pen(PS_SOLID , 1, nd == m_pNd ? RGB(0x00, 0x00, 0x80): RGB(0x80, 0x00, 0x00));
		CPen* oldPen = pDC->SelectObject(&pen);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(&r);

		pDC->SelectObject(oldPen);
	}
}


// CParserView printing


void CParserView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CParserView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CParserView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CParserView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CParserView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CParserView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CParserView diagnostics

#ifdef _DEBUG
void CParserView::AssertValid() const
{
	CView::AssertValid();
}

void CParserView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CParserDoc* CParserView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CParserDoc)));
	return (CParserDoc*)m_pDocument;
}
#endif //_DEBUG


// CParserView message handlers


void CParserView::OnIterationMoveNext()
{
	GetDocument()->nextStep();
	RedrawWindow();
	AfxGetMainWnd()->SendMessageToDescendants(MM_REDRAWTREE);
}


afx_msg LRESULT CParserView::OnSetnode(WPARAM wParam, LPARAM lParam)
{
	if(lParam)
		m_pNdUnk = reinterpret_cast<pmb::parser::node<transporter>*>(lParam);
	else
		m_pNd = reinterpret_cast<pmb::parser::node<transporter>*>(wParam);
	RedrawWindow();
	return 0;
}


void CParserView::OnNextunknow()
{
	AfxGetMainWnd()->SendMessageToDescendants(MM_NEXTUNKNOW);
}
