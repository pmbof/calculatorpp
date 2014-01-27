
// ParserDoc.h : interface of the CParserDoc class
//


#pragma once

#include "pmb_parser_debalgorithm.h"
//#include "pmb_parser_algorithm.cpp"

typedef pmb::parser::value<pmb::parser::number<double> > value;


class CParserDoc : public CDocument
{
public:
	static CParserDoc* getDocument(CWnd* pWnd);

protected: // create from serialization only
	CParserDoc();
	DECLARE_DYNCREATE(CParserDoc)

// Attributes
public:
	SYSTEMTIME m_time_ini;
	SYSTEMTIME m_time_end;
	int	m_countIterators;

	const char* m_expr;
	const pmb::parser::debug::auto_iterator<0, pmb::parser::debug::iterator, value>* m_iterators;
	const pmb::parser::debug::iterator* m_iterator;
	const pmb::parser::node<value>* getNewNode() const;
	const pmb::parser::node<value>* getNewNodeUnknow() const;
	const pmb::parser::tree<value>* getTree() const;
	const pmb::parser::tree<value>* getTree2() const;


	const pmb::parser::node<value>* getNextUnknowNode(const pmb::parser::node<value>* nd) const;

protected:
	pmb::parser::debug::algorithm m_parser;
	pmb::parser::algorithm<value> m_parser2;

// Operations
public:
	bool nextStep();

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CParserDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnIterationMoveNext();
	afx_msg void OnNextunknow();
};
