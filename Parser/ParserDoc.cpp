
// ParserDoc.cpp : implementation of the CParserDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Parser.h"
#endif

#include "ParserDoc.h"
#include "pmb_parser_algorithm.cpp"
#include "pmb_parser_operation.cpp"

#include "pmb_parser_tree.cpp"
#include "pmb_parser_nodes_calc.cpp"
#include "pmb_parser_nodes_parentheses.cpp"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CParserDoc

IMPLEMENT_DYNCREATE(CParserDoc, CDocument)

BEGIN_MESSAGE_MAP(CParserDoc, CDocument)
END_MESSAGE_MAP()


// CParserDoc construction/destruction

CParserDoc::CParserDoc()
	: m_parser(),
		m_parser2()
{

}

CParserDoc::~CParserDoc()
{
}


CParserDoc* CParserDoc::getDocument(CWnd* pWnd)
{
	CFrameWnd* pMainFrame = pWnd->GetParentFrame();
	if(!pMainFrame)
		return NULL;
	CFrameWnd* pAcFrame = pMainFrame->GetActiveFrame();
	if(!pAcFrame)
		return NULL;
	CParserDoc* pDoc = static_cast<CParserDoc*>(pAcFrame->GetActiveDocument());
	if(!pDoc || !pDoc->IsKindOf(RUNTIME_CLASS(CParserDoc)))
		return NULL;
	return pDoc;
}


BOOL CParserDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	m_parser.initialize();
	m_parser2.initialize();

	m_iterators = m_parser.getIterators();
	m_iterator = m_parser.getIterator();
	m_countIterators = 1;

	m_expr = "a (sin(b + 3, zl), 4 ^ 7 * z^i * 2^3 + 1*2 + 3 + 4 +5, beta(c-3)alpha,  (8*(9*(h + 5))^(j-k)) )(2c) = - a*b+c";
	m_expr = "a = 2^3";
//	m_expr = "a acos(6x, -3y, z) = -5k";
//	m_expr = "a acos (";

	m_parser.parser(m_expr);
	GetSystemTime(&m_time_ini);
	m_parser2.parser(m_expr);
	m_expr = "b = 2 * a";
	m_parser2.parser(m_expr);
	m_expr = "c = (a + b)/2";
	m_parser2.parser(m_expr);
	GetSystemTime(&m_time_end);
	AfxGetMainWnd()->PostMessage(MM_CHARGENEWDOC, WPARAM(&m_parser2.getVariables()));
	return TRUE;
}


bool CParserDoc::nextStep()
{
	return m_parser.nextStep();
}

const pmb::parser::tree<value>* CParserDoc::getTree() const
{
	return m_parser.getTree();
}

const pmb::parser::node<value>* CParserDoc::getNewNode() const
{
	return m_parser.getNewNode();
}

const pmb::parser::node<value>* CParserDoc::getNewNodeUnknow() const
{
	return m_parser.getNewNodeUnknow();
}


const pmb::parser::node<value>* CParserDoc::getNextUnknowNode(const pmb::parser::node<value>* nd) const
{
	if(!nd)
	{
		nd = m_parser2.getTree()->getRootNode();
		if(nd && nd->getType() == pmb::parser::ndUnknow)
			//nd = nd->getFirstUnknowNode();
			nd = ((pmb::parser::nodes::unknow<value>*)nd)->nextCalc();
	}
	else if(nd->getType() == pmb::parser::ndUnknow)
//		nd = nd->getNextUnknowNode();
		nd = ((pmb::parser::nodes::unknow<value>*)nd)->nextCalc();
	if(nd && nd->getType() == pmb::parser::ndUnknow)
		((pmb::parser::nodes::unknow<value>*)nd)->setCalculated();
	return nd;
}



const pmb::parser::tree<value>* CParserDoc::getTree2() const
{
	return m_parser2.getTree();
}

// CParserDoc serialization

void CParserDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CParserDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CParserDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CParserDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CParserDoc diagnostics

#ifdef _DEBUG
void CParserDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CParserDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CParserDoc commands




