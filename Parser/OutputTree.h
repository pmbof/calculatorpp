#pragma once


#include "pmb_parser_debalgorithm.h"
#include <list>
#include "ParserDoc.h"

// COutputTree

class COutputTree : public CWnd
{
	DECLARE_DYNAMIC(COutputTree)

public:
	COutputTree(bool bDebug);
	virtual ~COutputTree();

private:
	struct node: CRect
	{
		node(const CRect& r, const pmb::parser::node<item, ndtype>* nd)
			: CRect(r), pNd(nd), bSelected(false)
		{}

		const pmb::parser::node<item, ndtype>* pNd;
		bool bSelected;
	};

	bool m_bDebug;
	typedef std::list<node> lrect;
	lrect m_cnd;
	const pmb::parser::node<item, ndtype>* m_pNd;
	const pmb::parser::node<item, ndtype>* m_pNdUnknow;
	int m_cx;
	int m_cy;

protected:
	struct graph_node
	{
		struct scolor {
			char type;
			COLORREF color;
		};
		static const scolor color[];

		COutputTree* parent;
		CDC* pDC;
		const CString& expr;
		const pmb::parser::node<item, ndtype>* nd;
		int x0;
		int x;
		int y;
		int h;

		COLORREF getColor() const;
		int draw();
	};

protected:
	int drawIterators(CDC* pDC, CParserDoc* pDoc, int x0, int y0, int height, const CString& expr);
	void setScrolls(int cx, int cy);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
protected:
	afx_msg LRESULT OnRedrawtree(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
//	afx_msg void OnNextunknow();
protected:
	afx_msg LRESULT OnNextunknow(WPARAM wParam, LPARAM lParam);
};


