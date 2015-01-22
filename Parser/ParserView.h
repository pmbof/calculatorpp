
// ParserView.h : interface of the CParserView class
//

#pragma once


class CParserView : public CView
{
protected: // create from serialization only
	CParserView();
	DECLARE_DYNCREATE(CParserView)

// Attributes
public:
	CParserDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

private:
	pmb::parser::node<transporter>* m_pNdUnk;
	pmb::parser::node<transporter>* m_pNd;

private:
	void drawNode(CDC* pDC, const pmb::parser::node<transporter>* nd, int x0, int y0, int height, const CString& expr);

// Implementation
public:
	virtual ~CParserView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnIterationMoveNext();
protected:
	afx_msg LRESULT OnSetnode(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnNextunknow();
};

#ifndef _DEBUG  // debug version in ParserView.cpp
inline CParserDoc* CParserView::GetDocument() const
   { return reinterpret_cast<CParserDoc*>(m_pDocument); }
#endif

