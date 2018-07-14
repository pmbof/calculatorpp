
// ParserDoc.h : interface of the CParserDoc class
//


#pragma once

#include "pmb_parser_debalgorithm.h"
#include "pmb_calculator.h"
//#include "pmb_parser_transporter.h"

#include "pmb_parser_calc_build_in_function.hpp"
#include "pmb_parser_calc_block.hpp"
#include "pmb_parser_calc_transporter.hpp"

#include "pmb_calculate_number.h"
#include "pmb_calculate_magnitude.h"

//typedef pmb::parser::transporter<pmb::parser::number<double> > transporter;

typedef pmb::calculate::units::magnitude<double, unsigned short, char> number_double;
//typedef pmb::calculate::number<double> number_double;
typedef pmb::parser::calc::transporter<number_double, unsigned short> transporter;
typedef pmb::parser::calc::transporter_args<transporter, unsigned char> transporter_args;


typedef pmb::parser::calc::build_in_function<transporter_args, char, unsigned char> build_in_function;
typedef pmb::parser::calc::build_in_function_table<build_in_function, unsigned short> build_in_function_table;


typedef pmb::parser::ndtype ndtype;
typedef pmb::parser::item<char, unsigned short> item;
typedef pmb::parser::tree<item, ndtype> tree;
typedef tree::cnode tnode;
typedef pmb::parser::calc::iterator<transporter_args, tree> citerator;

typedef pmb::parser::operation<transporter_args> operation;
typedef pmb::parser::operation_table<operation, tnode> operation_table;

typedef pmb::parser::calc::block<citerator, build_in_function_table> block;
typedef block::symbol symbol;



class CParserDoc : public CDocument
{
	typedef build_in_function::transporter_args transporter_args;

	static const  operation _operation[];
	operation_table _operation_table;

	static void opr_positive(transporter_args& args);
	static void opr_negative(transporter_args& args);
	static void opr_factorial(transporter_args& args);
	static void opr_exponentiation(transporter_args& args);
	static void opr_root(transporter_args& args);
	static void opr_multiplication(transporter_args& args);
	static void opr_division(transporter_args& args);
	static void opr_modulo(transporter_args& args);
	static void opr_addition(transporter_args& args);
	static void opr_subtraction(transporter_args& args);
	static void opr_assignation(transporter_args& args);
	static void opr_result(transporter_args& args);


	static const build_in_function _build_in_function[];
	build_in_function_table _build_in_fnc_table;

	static void binf_test(transporter_args& args);
	static void binf_abs(transporter_args& args);
	static void binf_sgn(transporter_args& args);
	static void binf_rnd(transporter_args& args);
	static void binf_ceil(transporter_args& args);
	static void binf_floor(transporter_args& args);
	static void binf_round(transporter_args& args);
	static void binf_lg(transporter_args& args);
	static void binf_ln(transporter_args& args);
	static void binf_log(transporter_args& args);
	static void binf_exp(transporter_args& args);
	static void binf_sin(transporter_args& args);
	static void binf_cos(transporter_args& args);
	static void binf_tg(transporter_args& args);
	static void binf_sec(transporter_args& args);
	static void binf_cosec(transporter_args& args);
	static void binf_cotg(transporter_args& args);
	static void binf_asin(transporter_args& args);
	static void binf_acos(transporter_args& args);
	static void binf_atg(transporter_args& args);
	static void binf_atg2(transporter_args& args);
	static void binf_asec(transporter_args& args);
	static void binf_acosec(transporter_args& args);
	static void binf_acotg(transporter_args& args);
	static void binf_if(transporter_args& args);

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
	const pmb::parser::debug::auto_iterator<0, pmb::parser::debug::iterator, item, ndtype>* m_iterators;
	const pmb::parser::debug::iterator* m_iterator;
	const tnode* getNewNode() const;
	const tnode* getNewNodeUnknow() const;
	const tree* getTree() const;
	const tnode* getNodeRoot() const;
	const tree* getTree2() const;


	const tnode* getNextUnknowNode(const tnode* nd) const;

protected:
	symbol m_symbols;
	pmb::parser::debug::algorithm m_parser;
	pmb::calculator<block, operation_table> m_calculator;
	block _block;

public:
	pmb::parser::exception<item> m_error;

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
