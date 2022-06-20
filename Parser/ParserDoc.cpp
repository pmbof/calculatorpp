
// ParserDoc.cpp : implementation of the CParserDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Parser.h"
#endif

#include "ParserDoc.h"
#include "ParserView.h"

#include "pmb_parser_algorithm.cpp"
#include "pmb_calculator.cpp"

#include "pmb_parser_operation.cpp"
#include "pmb_parser_symbol.cpp"

#include "pmb_parser_tree.cpp"
#include "pmb_parser_nodes_calc.cpp"
#include "pmb_parser_nodes_parentheses.cpp"

#include "pmb_configuration_file.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CParserDoc
const operation* CParserDoc::_operation[] = {
	/*	 1- Symb        = Symbol
		 2- prec        = precedence
		 3- L2R         = left to right association
		 4- bin         = is binary
		 5- name
		 6- description
		 7- *function   = pointer to function
		 8- CCFunc      = can call function
		 9- CCLvar      = can create left variable
		10- CCRvar      = can create right variable
	*/
	///                                 Symb, prec,   L2R,   bin, name,					description,                                           *funcion,                         CCFunc, CCLvar, CCRvar
	new CParserView::opr_plus(           "+",  250, false, false, "positive",              "positive",                                            &CParserDoc::opr_positive),
	new CParserView::opr_minus(          "-",  250, false, false, "negative",              "negative",                                            &CParserDoc::opr_negative),
	new CParserView::operation(          "!",  250, true,  false, "factorial",             "factorial",                                           &CParserDoc::opr_factorial),
	new CParserView::opr_root(           "\\", 210, false, false, "square root",           "square root",                                         &CParserDoc::opr_sqrroot),
	new CParserView::opr_power(          "^",  200, false,  true, "power",                 "exponentiation",                                      &CParserDoc::opr_exponentiation),
	new CParserView::opr_root(           "\\", 200, true,   true, "root",                  "root",                                                &CParserDoc::opr_root),
	new CParserView::opr_product(        "*",  110, true,   true, "product",               "multiplication",                                      &CParserDoc::opr_multiplication),
	new CParserView::opr_division_inline("/.", 110, true,   true, "inline cocient",        "division",                                            &CParserDoc::opr_division),
	new CParserView::opr_division(       "/",  110, true,   true, "cocient",               "division",                                            &CParserDoc::opr_division),
	new CParserView::operation(          "/!", 110, true,   true, "com",                   "combinatory",                                         &CParserDoc::opr_division),
	new CParserView::operation(          "",   100, true,   true, "product implicit",      "multiplication implicit or call function",            &CParserDoc::opr_multiplication,  true),
	new CParserView::operation(          " ",  100, true,   true, "product space",         "multiplication space or call function",               &CParserDoc::opr_multiplication,  true),
	new CParserView::operation(          " ",  100, false,  true, "product space inverse", "multiplication space or call function right to left", &CParserDoc::opr_multiplication,  true),
	new CParserView::operation(          "&",  100, true,   true, "modulo",                "congruence relation",                                 &CParserDoc::opr_modulo),
	new CParserView::opr_minus(          "-",   50, true,   true, "substract",             "substraction",                                        &CParserDoc::opr_subtraction),
	new CParserView::opr_plus(           "+",   50, true,   true, "add",                   "addition",                                            &CParserDoc::opr_addition),
	new CParserView::opr_equal(          "=",   10, false,  true, "assignation",           "assignation",                                         &CParserDoc::opr_assignation,     false,  true,   false),
	new CParserView::opr_equal(          "=",    0, true,  false, "result",                "result",                                              &CParserDoc::opr_result),
	new CParserView::opr_result(         "=.",   0, true,   true, "result modify",         "result modify",                                       &CParserDoc::opr_result_modify)
};



const build_in_function CParserDoc::_build_in_function[] = {
	/*	1- name        = function name
		2- description
		3- *function   = pointer to function
		4- nArgs       = amount of arguments
	*/
	///               name,     description,            *function,               nArgs
	build_in_function("test",   "for test",             &CParserDoc::binf_test,   4),
	build_in_function("abs",    "absolute value",       &CParserDoc::binf_abs),
	build_in_function("sgn",    "sign",                 &CParserDoc::binf_sgn),
	build_in_function("rnd",    "random",               &CParserDoc::binf_rnd),
	build_in_function("ceil",   "ceil",                 &CParserDoc::binf_ceil),
	build_in_function("floor",  "sign",                 &CParserDoc::binf_floor),
	build_in_function("round",  "round",                &CParserDoc::binf_round),

	build_in_function("lg",     "decimal logarithm",    &CParserDoc::binf_lg),
	build_in_function("ln",     "neperian logarithm",   &CParserDoc::binf_ln),
	build_in_function("log",    "n-esim logarithm",     &CParserDoc::binf_log,    2),
	build_in_function("exp",    "exponential",          &CParserDoc::binf_exp),

	build_in_function("sin",    "sine",                 &CParserDoc::binf_sin),
	build_in_function("cos",    "cosine",               &CParserDoc::binf_cos),
	build_in_function("tg",     "tangent",              &CParserDoc::binf_tg),
	build_in_function("sec",    "secant",               &CParserDoc::binf_sec),
	build_in_function("cosec",  "cosecant",             &CParserDoc::binf_cosec),
	build_in_function("cotg",   "cotangent",            &CParserDoc::binf_cotg),
	build_in_function("asin",   "arc sine",             &CParserDoc::binf_asin),
	build_in_function("acos",   "arc cosine",           &CParserDoc::binf_acos),
	build_in_function("atg",    "arc tangent",          &CParserDoc::binf_atg),
	build_in_function("atg",    "tangent",              &CParserDoc::binf_atg2,   2),
	build_in_function("asec",   "arc secant",           &CParserDoc::binf_asec),
	build_in_function("acosec", "arc cosecant",         &CParserDoc::binf_acosec),
	build_in_function("acotg",  "arc cotangent",        &CParserDoc::binf_acotg),

	build_in_function("if",     "conditional function", &CParserDoc::binf_if,     3)
};



IMPLEMENT_DYNCREATE(CParserDoc, CDocument)

BEGIN_MESSAGE_MAP(CParserDoc, CDocument)
END_MESSAGE_MAP()


// CParserDoc construction/destruction

CParserDoc::CParserDoc()
	: _operation_table(_operation, sizeof(_operation) / sizeof(*_operation)),
		_build_in_fnc_table(_build_in_function, sizeof(_build_in_function) / sizeof(*_build_in_function)),
		_block(&_build_in_fnc_table, &m_symbols),
		m_calculator(&_operation_table, &_block)
{
}


CParserDoc::~CParserDoc()
{
	m_error.reset();
}



void CParserDoc::destroy()
{
	for (int i = 0; i < sizeof(_operation) / sizeof(*_operation); ++i)
		delete static_cast<const CParserView::operation*>(_operation[i]);
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


// funcion recursiva para calcular el determinante de una matriz de orden 
int determinante_recursiva(int orden, const int* matriz, int f, int* cols)
{
	int det;
	int c, cs, col;
	int k, mult;
	int fc, fo;

	if (2 < orden - f) // matriz de orden > 2
	{
		det = 0;
		col = f; // para saber el signo
		for (c = 0; c < orden; ++c)
		{
			fc = FALSE;
			for (cs = 0; cs < f && !fc; ++cs)
				fc = cols[cs] == c;
			if (fc)
				continue;
			cols[f] = c; // columna anulada
			// si la suma de los indices es par, se suma, y si es impar se resta: ((f + c) % 2 ? -1 : 1) 
			// acá hace la recursividad al llamrse a si misma:
			for (k = 0; k < f; ++k)
				*pmb::log::instance() << "\t";
			*pmb::log::instance() << (((f + col) % 2 ? "-" : "")) << f + 1 << c + 1 << " * |";
			//                                    [   fila  ][columna]
			det += ((f + col) % 2 ? -1 : 1) * matriz[f * orden + c] * determinante_recursiva(orden, matriz, f + 1, cols);
			*pmb::log::instance() << "|[= " << det << "]\n";
			++col;
		}
	}
	else if (orden - f == 2) // matriz de orden 2:
	{
		// matriz[fila * orden + columna] es el elemento (fila, columna) de la matriz, con los indices comenzando desde 0
		// fila es la fila que estámos pivoteando, tenemos que calcular a partir de fila + 1
		// col es la columna que estámos pivoteando (anulando)
		// entonce:
		// |11 12|
		// |21 22|
		// en las filas no me paso del orden: fila + 2 < orden
		// pero en las columnas si me paso porque nos desplazamos sobre col + 1, entonces tenemos que usar el modulo (%) para que sea ciclica:
		// (col + 1) % orden y (col + 2) % orden para ubicar la columna
		det = 0;
		*pmb::log::instance() << "(";
		fo = f;
		for (k = 0; k < 2; ++k)
		{
			mult = 1;
			for (c = 0; c < orden; ++c)
			{
				fc = FALSE;
				for (cs = 0; cs < orden - 2 && !fc; ++cs)
					fc = cols[cs] == c;
				if (!fc) // saltea las columnas que estan en cols
				{
					*pmb::log::instance() << f + 1 << c + 1 << " ";
					///printf(f == fila + 1 ? "%d%d * " : k ? "%d%d" : "%d%d - ", f + 1, c + 1);
					mult *=
						//    [  fila   ][columna]
						matriz[f * orden + c];
					if (k)
						--f;
					else
						++f;
				}
			}
			if (!k)
			{
				*pmb::log::instance() << " - ";
				det += mult;
				f = fo + 1;
			}
			else
				det -= mult;
		}
		*pmb::log::instance() << ")[= " << det << "]";
		///printf(")");
	}
	else // matriz de orden 1: escalar
	{
		for (c = 0; c < orden; ++c)
		{
			fc = FALSE;
			for (cs = 0; cs < orden - 2 && !fc; ++cs)
				fc = cols[cs] == c;
			if (!fc)
			{
				*pmb::log::instance() << "[" << f + 1 << c + 1 << "] ";
				//          [  fila   ][columna]
				det = matriz[f * orden + c];
				break;
			}
		}
	}
	return det;
}


int determinante(int orden, const int* matriz)
{
	int* cols; // este es un vector, con en numero de la columna que estamos anulando
	int det;

	cols = 2 < orden ? (int*)malloc(orden - 2) : NULL;
	det = determinante_recursiva(orden, matriz, 0, cols);

//	if (cols)
//		free(cols);
	return det;
}




BOOL CParserDoc::OnNewDocument()
{
	pmb::log* log = pmb::log::beginFunction(pmb::logDebug, __FUNCTION__);
	m_expr.clear();
	if (!CDocument::OnNewDocument())
	{
		log->endFunction();
		return FALSE;
	}

	if (!load_configuration())
	{
		AfxMessageBox(L"Error in configuration file!\r\nPlease check log file", MB_OK | MB_ICONEXCLAMATION);
	}
	m_calculator.clear();

	m_symbols.select_search("Variables");
	AfxGetMainWnd()->PostMessage(MM_CHARGENEWDOC, WPARAM(m_symbols.get()));

	bool bMoveTitle = false;
	CString title = GetTitle();
	for (int i = 0; true; ++i)
	{
		CString nf;
		nf.Format(CALCULATOR_PROFILE_DOCUMENT_BACKUP_OPEN_PATTERN, i);
		if (bMoveTitle)
		{
			for (int i = title.GetLength() - 1; 0 < i; --i)
			{
				if (title[i] < L'0' || L'9' < title[i])
				{
					CString stitle = title.Mid(0, ++i);
					int ntitle = atoi(CStringA(title.Mid(i))) + 1;
					title.Format(_T("%s%d"), (LPCTSTR)stitle, ntitle);
					SetTitle(title);
					break;
				}
			}
			bMoveTitle = false;
		}
		CString filename = theApp.GetProfileString(CALCULATOR_PROFILE_DOCUMENT_SECTION_BACKUP, nf);
		if (filename.IsEmpty() || filename == title)
		{
			POSITION pos = theApp.GetFirstDocTemplatePosition();
			CDocTemplate* pDT = theApp.GetNextDocTemplate(pos);
			for (pos = pDT->GetFirstDocPosition(); pos; )
			{
				CParserDoc* pDoc = static_cast<CParserDoc*>(pDT->GetNextDoc(pos));
				if (!pDoc->m_profile_back_entry.IsEmpty() && pDoc->GetTitle() == title)
				{
					bMoveTitle = true;
					break;
				}
			}
			if (!bMoveTitle)
			{
				m_profile_back_entry = nf;
				int fp = nf.Find(L'.');
				nf = nf.Mid(0, fp);
				int pidx = theApp.GetProfileInt(CALCULATOR_PROFILE_DOCUMENT_SECTION_BACKUP, nf, 0);
				if (pidx <= i)
					theApp.WriteProfileInt(CALCULATOR_PROFILE_DOCUMENT_SECTION_BACKUP, nf, i + 1);
				theApp.WriteProfileString(CALCULATOR_PROFILE_DOCUMENT_SECTION_BACKUP, m_profile_back_entry, title);
				SaveModified();
				break;
			}
		}
	}

	backup();
	log->endFunction();
	return true;



	if (check_operation_table())
		print_operation_table();


	const char* symb[] = { "c", "g", "G", "h", "e" };
	number_double val;
	std::string sunit;
	for (int i = 0; i < sizeof(symb) / sizeof(*symb); ++i)
	{
		bool bFound = m_symbols.value(nullptr, symb[i], val, sunit);
		if (!bFound)
			log->trace(pmb::logError, "%d. symbol %s not found!\n", i, symb[i]);
		else
			log->trace(pmb::logDebug, "%d. %s = %f%s\n", i, symb[i], val, sunit.c_str());
	}

	m_countIterators = 1;

	m_expr = "a (sin(b + 3, zl), 4 ^ 7 * z^i * 2^3 + 1*2 + 3 + 4 +5, beta(c-3)alpha,  (8*(9*(h + 5))^(j-k)) )(2c) = - a*b+c";
	m_expr = "a = 2^3";
//	m_expr = "a acos(6x, -3y, z) = -5k";
//	m_expr = "a acos (";


	typedef std::tuple<const char*, bool, number_double> tuple;
	typedef std::vector<tuple> vector;

	const double pi = 4 * atan(1);
	vector test;
	test.push_back(tuple("M.Earth = 5.972 10^24kg", true, 8));
	test.push_back(tuple("R.Earth = 6371 km", true, 8));
	test.push_back(tuple("v.Escape = \\(2G M.Earth / R.Earth)", true, 11185.8));
	test.push_back(tuple("f(x, y, z) = z + 4 x^2 + y", false, 0));
	test.push_back(tuple("f(2, 3, 1)", true, 1 + 4 * 2 * 2 + 3));
	test.push_back(tuple("f(x) = 1 + 4 x^2 + x", false, 0));
	test.push_back(tuple("a0 = f(5) - f(2)", true, 1 + 4 * 5 * 5 + 5 - (1 + 4 * 2 * 2 + 2)));
	test.push_back(tuple("k2 = (2 * 3) ^ (1 + 1) / (5 + 4) + 8 * (((1 + 1)(5 + 7)(2 + 1)) / (6 + 6)) / 12", true, 8));
	test.push_back(tuple("a1 = 5 test(9 - 8, 1 + 1, 6 - 3, 2 * 2)", true, 50));
	test.push_back(tuple("a2 = 5 test(1, 2, 3, 4)", true, 50));
	//test.push_back(tuple("10 - 10 * 10 + 10", true, 10 - 10 * 10 + 10)); // Memory leaks!!!
	test.push_back(tuple("pi = 4 atg 1", true, pi));
	test.push_back(tuple("a3 = sin(3.141592/6)", true, sin(3.141592 / 6)));
	test.push_back(tuple("a4 = sin pi/6", true, sin(pi / 6)));
	test.push_back(tuple("a5 = 5 test(9 - 8, 1 + 1, 6 - 3, 2 * 2)", true, 50));
	test.push_back(tuple("k1 = (2 * 3) ^ (1 + 1) / (5 + 4) + 8 * (((1 + 1)(5 + 7)(2 + 1)) / (6 + 6)) / 12", true, 8));
	test.push_back(tuple("a6 = sin pi/6", true, sin(pi / 6)));
	test.push_back(tuple("g(x) = 1 + 4 x^2 + x", false, 0));
	test.push_back(tuple("a7 = f(5) - f(-1)", true, 1 + 4 * 5 * 5 + 5 - (1 + 4 - 1)));
	test.push_back(tuple("f(1)", true, 1 + 4 + 1));
	test.push_back(tuple("k1 = (2 * 3) ^ (1 + 1) / (5 + 4) + 8 * (((1 + 1)(5 + 7)(2 + 1)) / (6 + 6)) / 12", true, 8));
	test.push_back(tuple("Lata_choclo = 2.1lb", true, 11185.8));
	test.push_back(tuple("2Lata_choclo + 5kg + 250g", true, 11185.8));
	test.push_back(tuple("2 1024 1024 1024B", true, 11185.8));
	test.push_back(tuple("2^ \\4^1", true, 4));
	test.push_back(tuple("v.Escape = (2 +0/1)\\(2G M.Earth / R.Earth)", true, 11185.8));

	if (check_operation_table())
		print_operation_table();

	int errors = 0;
	m_symbols.add_set_variable("test");
	m_symbols.select_search("Test");
	for (int i = 0; i < test.size(); ++i)
	{
		bool bResult = false;
		if (i == 2)
			bResult = false;
		try
		{
			m_calculator.calculate((m_expr = std::get<0>(test[i])).c_str());
			result();
			bResult = true;
			if (_block.nresult() != std::get<2>(test[i]) || !std::get<1>(test[i]))
			{
				std::stringstream sr;
				_block.result().stringstream(sr);
				if (std::get<1>(test[i]))
					log->trace(pmb::logError, "%s = %f != %f\n", m_expr, _block.result(), std::get<2>(test[i]));
				else
					log->trace(pmb::logError, "%s = %f != <NULL>\n", m_expr, _block.result());
				++errors;
			}
			else
				log->trace(pmb::logWarning, "%s == %f\n", m_expr, _block.result());
		}
		catch (pmb::parser::exception<item>& ex)
		{
			m_result.clear();
			if (!bResult || std::get<1>(test[i]))
			{
				log->trace(pmb::logError, "Exception \"%s\": %s = <NULL> =! %f\n", ex.message(m_expr.c_str()).c_str(), m_expr, std::get<2>(test[i]));
				m_error = ex;
				++errors;
			}
			else
				log->trace(pmb::logWarning, "%s == <NULL>\n", m_expr);
		}
		if (!check_operation_table())
			throw "Bad operation table";
	}
	if (errors)
		log->trace(pmb::logError, "%d/%d errors found\n", errors, test.size());
	else
		log->trace(pmb::logWarning, "%d tests successful!!!\n", test.size());

	AfxGetMainWnd()->PostMessage(MM_CHARGENEWDOC, WPARAM(m_symbols.get()));
	return true;
	try
	{
		m_calculator.parser((m_expr = "f(x) = 4 x").c_str());
		m_calculator.parser((m_expr = "a = 3 f(2)").c_str());
		m_calculator.parser((m_expr = "e = 2.71828182").c_str());
	}
	catch (pmb::parser::exception<item>& ex)
	{
		pmb::log::instance()->trace(pmb::logError, "Error: %s\n", ex.message(m_expr.c_str()).c_str());
		m_error = ex;
		AfxGetMainWnd()->PostMessage(MM_CHARGENEWDOC, WPARAM(m_symbols.get()));
		return true;
	}
	m_symbols.select_search("Variables");


	GetSystemTime(&m_time_ini);
//	m_calculator.parser(m_expr);
//	m_expr = "b = 2 * pi";
//	m_calculator.parser(m_expr);
//	m_expr = "c = (a + b)/2";
//	m_calculator.parser(m_expr);
//	m_expr = "fac = 5!";
//	m_calculator.parser(m_expr);
//	m_expr = "pi4 = sin pi/4";
//	m_calculator.parser(m_expr);
//	m_expr = "x = 1";
//	m_calculator.parser(m_expr);
	try
	{
		m_expr = "f(x, y, z) = 2 pi + 4 * x sin y * z";
		m_calculator.parser(m_expr.c_str());
	}
	catch (pmb::parser::exception<item>& ex)
	{
		pmb::log::instance()->trace(pmb::logError, "Error: %s\n", ex.message(m_expr.c_str()).c_str());
	}
//	m_expr = "a = y(pi)";
//	m_calculator.parser(m_expr);
	GetSystemTime(&m_time_end);

	AfxGetMainWnd()->PostMessage(MM_CHARGENEWDOC, WPARAM(m_symbols.get()));
	log->endFunction(pmb::logDebug);
	return TRUE;
}



BOOL CParserDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	LPCTSTR pattern = CALCULATOR_PROFILE_DOCUMENT_BACKUP_OPEN_PATTERN;

	int iheader = 0;
	for (int i = 0; lpszPathName[i] && lpszPathName[i] == pattern[i]; ++i)
	{
		if (lpszPathName[i] == '.' && lpszPathName[i + 1])
		{
			int i0 = ++i;
			for (; lpszPathName[i] && L'0' <= lpszPathName[i] && lpszPathName[i] <= L'9'; ++i)
				;
			if (i0 < i && lpszPathName[i] == L'|' && lpszPathName[i + 1])
			{
				iheader = i + 1;
				CString path = theApp.getAppDataPath(L"");

				int j;
				for (j = 0; j < path.GetLength() && lpszPathName[iheader + j] == path[j]; ++j)
					;
				if (j == path.GetLength() && lpszPathName[iheader + j])
				{
					int je;
					for (je = j + 1; lpszPathName[iheader + je] && lpszPathName[iheader + je] != L'\\'; ++je)
						;
					if (j + 1 < je && !lpszPathName[iheader + je])
					{
						CString title(lpszPathName + iheader + j);
						SetTitle(title);
						m_profile_back_entry = CString(lpszPathName, i);
					}
					else
					{
						return false;
					}
				}
			}
		}
	}

	if (!CDocument::OnOpenDocument(lpszPathName + iheader))
		return FALSE;

	return TRUE;
}



void CParserDoc::OnCloseDocument()
{
	if (!theApp.m_bClosing && !m_profile_back_entry.IsEmpty())
	{
		if (!m_expr.empty())
		{
			POSITION pos = GetFirstViewPosition();
			if (pos)
			{
				CView* pVw = GetNextView(pos);
				if (pVw && pVw->MessageBox(L"You lost change, do you want to continue?", nullptr, MB_ICONQUESTION | MB_YESNO) == IDNO)
				{
					return;
				}
			}
		}
		int pf = m_profile_back_entry.Find(L'.');
		CString nf = m_profile_back_entry.Mid(pf + 1);
		int i = _ttoi(nf);
		nf = m_profile_back_entry.Mid(0, pf);
		int pidx = theApp.GetProfileInt(CALCULATOR_PROFILE_DOCUMENT_SECTION_BACKUP, nf, 0);
		if (pidx <= i + 1)
			theApp.WriteProfileInt(CALCULATOR_PROFILE_DOCUMENT_SECTION_BACKUP, nf, i);
		theApp.WriteProfileString(CALCULATOR_PROFILE_DOCUMENT_SECTION_BACKUP, m_profile_back_entry, _T(""));
	}
	CDocument::OnCloseDocument();
}



bool CParserDoc::nextStep()
{
	return false;// m_parser.nextStep();
}



void CParserDoc::backup()
{
	CString title = GetTitle();
	CString sfn = theApp.getAppDataPath(title);
	CFile file;
	file.Open(sfn, CFile::modeCreate | CFile::typeBinary | CFile::modeReadWrite);
	file.Write(m_expr.c_str(), m_expr.size());
	file.Close();
}



void CParserDoc::update()
{
	bool bResult = false;

	if (!m_profile_back_entry.IsEmpty())
		backup();
	else
		m_bModified = true;


	try
	{
		m_error.reset();
		m_calculator.calculate(m_expr.c_str());
		bResult = true;
		result();
	}
	catch (pmb::parser::exception<item>& ex)
	{
		m_result.clear();
		if (!bResult)
		{
			pmb::log::instance()->trace(pmb::logError, "Exception: \"%s\"\n", ex.message(m_expr.c_str()).c_str());
			m_error = ex;
		}
	}
	AfxGetMainWnd()->PostMessage(MM_CHANGEEXPRESSION, WPARAM(m_symbols.get()));
}


const tree* CParserDoc::getTree() const
{
	return false; // m_parser.getTree();
}

const tnode* CParserDoc::getNodeRoot() const
{
	return m_calculator.getTree() ? m_calculator.getTree()->getRootNode() : nullptr;
}

const tnode* CParserDoc::getNodeResult() const
{
	return m_calculator.getResultNode() ? m_calculator.getResultNode()->getRootNode() : nullptr;
}

const tnode* CParserDoc::getNewNode() const
{
	return false; // m_parser.getNewNode();
}

const tnode* CParserDoc::getNewNodeUnknow() const
{
	return false; // m_parser.getNewNodeUnknow();
}


const tnode* CParserDoc::getNextUnknowNode(const tnode* nd) const
{
	if(!nd)
	{
		nd = m_calculator.getTree()->getRootNode();
		if(nd && nd->getType() == pmb::parser::ndUnknown)
			//nd = nd->getFirstUnknowNode();
			nd = ((pmb::parser::nodes::unknown<item, ndtype>*)nd)->nextCalc();
	}
	else if(nd->getType() == pmb::parser::ndUnknown)
//		nd = nd->getNextUnknowNode();
		nd = ((pmb::parser::nodes::unknown<item, ndtype>*)nd)->nextCalc();
///	if(nd && nd->getType() == pmb::parser::ndUnknown)
///		((pmb::parser::nodes::unknown<item, ndtype>*)nd)->setCalculated();
	return nd;
}



void CParserDoc::print_operation_table() const
{
	pmb::log* lg = pmb::log::instance();
	lg->trace(pmb::logDebug, "Operation table:\n");
	for (int i = 0; i < sizeof(_operation) / sizeof(*_operation); ++i)
	{
		lg->trace(pmb::logDebug, "\t- %d, opr -> 0x%08X\n", i, _operation[i]);
		_operation[i]->print(pmb::logDebug);
	}
}


bool CParserDoc::check_operation_table() const
{
	bool bRet = true;
	pmb::log* lg = pmb::log::instance();
	for (int i = 0; i < sizeof(_operation) / sizeof(*_operation); ++i)
	{
		if (!_operation[i]->check())
		{
			bRet = false;
			lg->trace(pmb::logError, "\t- %d, opr -> 0x%08X\n", i, _operation[i]);
			_operation[i]->print(pmb::logError);
		}
	}
	return bRet;
}



const tree* CParserDoc::getTree2() const
{
	return m_calculator.getTree();
}

// CParserDoc serialization

void CParserDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar.Write(m_expr.c_str(), m_expr.size());
	}
	else
	{
		m_expr.clear();
		const UINT bfsz = 1024;
		char buffer[bfsz];
		for (UINT br = ar.Read(buffer, bfsz); br; )
		{
			m_expr += std::string(buffer, br);
			br = br == bfsz ? ar.Read(buffer, bfsz) : 0;
			CString oldvalue = m_profile_back_entry;
			m_profile_back_entry.Empty();
			update();
			m_profile_back_entry = oldvalue;
		}
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




bool CParserDoc::load_configuration()
{
	CStringA cfgfile(AfxGetApp()->GetProfileString(L"Settings", L"document"));
	if (!cfgfile.IsEmpty())
	{
		pmb::configuration_file<symbol, pmb::calculator<block, operation_table>> cfile;

		cfile.open(cfgfile);

		return cfile.process(m_symbols, m_calculator);
	}
	else
	{
		m_symbols.add_dimension("L", "length");
		m_symbols.add_dimension("T", "time");
		m_symbols.add_dimension("M", "mass");
		m_symbols.add_dimension("I", "electric current");
		m_symbols.add_dimension("Theta", "temperature");
		m_symbols.add_dimension("N", "amount of substance");
		m_symbols.add_dimension("J", "luminous intensity");
		m_symbols.add_dimension("Crr", "currency");
		m_symbols.add_dimension("Mem", "memory");

		prefix_base* prefix = new prefix_base(10);
		prefix->insert("Y", 24, "yotta");
		prefix->insert("Z", 21, "zetta");
		prefix->insert("E", 18, "exa");
		prefix->insert("P", 15, "peta");
		prefix->insert("T", 12, "tera");
		prefix->insert("G", 9, "giga");
		prefix->insert("M", 6, "mega");
		prefix->insert("k", 3, "kilo");
		prefix->insert("h", 2, "hecto", false);
		prefix->insert("da", 1, "deka", false);
		prefix->insert("d", -1, "deci", false);
		prefix->insert("c", -2, "centi");
		prefix->insert("m", -3, "mili");
		prefix->insert("mhu", -6, "micro");
		prefix->insert("n", -9, "nano");
		prefix->insert("p", -12, "pico");
		prefix->insert("f", -15, "femto");
		prefix->insert("a", -18, "atto");
		prefix->insert("z", -21, "zepto");
		prefix->insert("y", -24, "yocto");
		m_symbols.add_prefix("decimal", prefix);

		prefix = new prefix_base(2);
		prefix->insert("E", 60, "exa");
		prefix->insert("P", 50, "peta");
		prefix->insert("T", 40, "tera");
		prefix->insert("G", 30, "giga");
		prefix->insert("M", 20, "mega");
		prefix->insert("k", 10, "kilo");
		m_symbols.add_prefix("memory", prefix);

		// adding system units without prefix
		m_symbols.add_system("No prefix");
		// adding Internation System of Units (SI):
		m_symbols.add_system("SI", "decimal");
		// adding system CGS:
		m_symbols.add_system("cgs", "decimal");
		// adding Imperial units (United States customary system):
		m_symbols.add_system("USCS"); // no prefix support
		// adding Memory system units:
		m_symbols.add_system("Memory", "memory");
		// adding Memory system units:
		m_symbols.add_system("Currency");

		// Initialized calculator:
		m_calculator.initialize();
		// set default system of units:

		print_operation_table();

		try
		{
			m_symbols.set_system_constants("Constants");
			check_operation_table();
			m_calculator.add_constant("Archimedes' constant", "pi = 4 atg 1");
			check_operation_table();
			m_calculator.add_constant("Euler's number", "e = 2.7182818284590452353602874");
			check_operation_table();
			m_calculator.add_constant("Golden ratio", "phi = (1 + 5^(1/2))/2");
			check_operation_table();
			m_calculator.add_constant("Euler–Mascheroni", "gamma = 0.5772156649015328606065120");
			check_operation_table();
			m_calculator.add_constant("Khinchin's constant", "Kh = 2.685452001065306445309714835481795");
			check_operation_table();
			m_calculator.add_constant("Glaisher–Kinkelin", "A = 1.282427129100622636875342568869791727");
			check_operation_table();
			m_calculator.add_constant("Liouville's constant", "Lc = .7656250596046447753906250000000000007");
			check_operation_table();
			m_symbols.set_system_constants();

			m_symbols.set_system("SI");
			m_calculator.add_unit("meter", "m = 1L");
			check_operation_table();
			m_calculator.add_unit("second", "s = 1T");
			check_operation_table();
			m_calculator.add_unit("gram", "g = 10^(-3)M");
			check_operation_table();
			m_calculator.add_unit("ampere", "A = 1I");
			check_operation_table();
			m_calculator.add_unit("kelvin", "K = 1Theta");
			check_operation_table();
			m_calculator.add_unit("mole", "mol = 1N");
			check_operation_table();
			m_calculator.add_unit("candela", "cd = 1J");
			check_operation_table();

			m_symbols.set_system("No prefix");
			m_calculator.add_unit("radian", "rad = 1", "plane angle");
			check_operation_table();
			m_calculator.add_unit("steradian", "sr = 1", "solid angle");
			check_operation_table();
			m_calculator.add_unit("minute", "min = 60s");
			check_operation_table();
			m_calculator.add_unit("hour", "h = 60min");
			check_operation_table();
			m_calculator.add_unit("day", "d = 24h");
			check_operation_table();
			m_calculator.add_unit("degree = pi/180 rad", false);
			check_operation_table();
			m_calculator.add_unit("ton", "t = 10^3kg", false);
			check_operation_table();
			m_calculator.add_unit("neper", "Np = 1");
			check_operation_table();


			m_symbols.set_system("SI");
			m_calculator.add_unit("hertz", "Hz = 1/s", "frecuency", false);
			check_operation_table();
			m_calculator.add_unit("newton", "N = kg m/s^2", "force");
			check_operation_table();
			m_calculator.add_unit("pascal", "Pa = N/m^2", "pressure, stress");
			check_operation_table();
			m_calculator.add_unit("joule", "J = N m", "energy, work, quantity of heat");
			check_operation_table();
			m_calculator.add_unit("watt", "W = J/s", "power, radiant flux");
			check_operation_table();
			m_calculator.add_unit("coulomb", "C = A s", "electric charge, quantity of electricity");
			check_operation_table();
			m_calculator.add_unit("volt", "V = W/A", "electric potential difference, electromotive force");
			check_operation_table();
			m_calculator.add_unit("farad", "F = C/V", "capacitance");
			check_operation_table();
			m_calculator.add_unit("ohm", "omega = V/A", "electric resistance");
			check_operation_table();
			m_calculator.add_unit("siemens", "S = 1/omega", "electric conductance");
			check_operation_table();
			m_calculator.add_unit("weber", "Wb = V s", "magnetic flux");
			check_operation_table();
			m_calculator.add_unit("tesla", "T = Wb/m^2", "magnetic flux density");
			check_operation_table();
			m_calculator.add_unit("henry", "H = W/A", "inductance");
			check_operation_table();
			// °C    = ???                   Celsius temperature
			m_calculator.add_unit("lumen", "lm = cd sr", "luminous flux");
			check_operation_table();
			m_calculator.add_unit("lux", "lx = lm/m^2", "illuminance");
			check_operation_table();
			m_calculator.add_unit("becquerel", "Bq = 1/s", "activity (of a radionuclide)", false);
			check_operation_table();
			m_calculator.add_unit("gray", "Gy = J/kg", "absorbed dose, specific energy (imparted), kerma");
			check_operation_table();
			m_calculator.add_unit("sievert", "Sv = J/kg", "dose equivalent");
			check_operation_table();
			m_calculator.add_unit("katal", "kat = mol/s", "catalytic activity");
			check_operation_table();

			m_calculator.add_unit("litre", "L = 1dm^3", "volume", false);
			check_operation_table();

			m_symbols.set_system("cgs");
			m_calculator.add_unit("gal", "Gal = cm/s^2", "acceleration");
			check_operation_table();
			m_calculator.add_unit("dyne", "dyn = g Gal");
			check_operation_table();
			m_calculator.add_unit("erg = dyn cm");
			check_operation_table();
			m_calculator.add_unit("barye", "Ba = dyn/cm^2");
			check_operation_table();
			m_calculator.add_unit("poise", "P = g/(cm s)", "dynamic viscosity");
			check_operation_table();
			m_calculator.add_unit("stockes", "St = cm^2/s", "kinematic viscosity");
			check_operation_table();
			m_calculator.add_unit("kayser", "kays = 1/cm", "wavenumber");
			check_operation_table();
			m_calculator.add_unit("statcoulomb", "Fr = cm dyn^(1/2)");
			check_operation_table();
			m_calculator.add_unit("statampere", "stA = Fr/s");
			check_operation_table();
			m_calculator.add_unit("abampere", "Bi = Fr/cm");
			check_operation_table();
			m_calculator.add_unit("abcoulomb", "abC = Bi s");
			check_operation_table();

			m_symbols.set_system("USCS");
			m_calculator.add_unit("point", "p = 127/360 mm");
			check_operation_table();
			m_calculator.add_unit("pica", "Pi = 12 p");
			check_operation_table();
			m_calculator.add_unit("inch", "in = 6 Pi");
			check_operation_table();
			m_calculator.add_unit("foot", "ft = 12 in");
			check_operation_table();
			m_calculator.add_unit("yard", "yd = 3 ft");
			check_operation_table();
			m_calculator.add_unit("mile", "mi = 1760 yd");
			check_operation_table();
			m_calculator.add_unit("link", "li = 33/50 ft");
			check_operation_table();
			m_calculator.add_unit("rod", "rd = 25 li");
			check_operation_table();
			m_calculator.add_unit("chain", "ch = 4 rd");
			check_operation_table();
			m_calculator.add_unit("furlong", "fur = 10 ch");
			check_operation_table();
			m_calculator.add_unit("league", "lea = 3 mi");
			check_operation_table();
			m_calculator.add_unit("fathom", "ftm = 2 yd");
			check_operation_table();
			m_calculator.add_unit("cable", "cb = 120 ftm");
			check_operation_table();
			m_calculator.add_unit("nautical_mile", "NM = nmi = 8439 cb");
			check_operation_table();
			m_calculator.add_unit("acres", "acre = 10 ch^2", "area");
			check_operation_table();
			m_calculator.add_unit("sections", "section = 640 acres");
			check_operation_table();
			m_calculator.add_unit("survey_township", "twp = 36 sections");
			check_operation_table();

			m_calculator.add_unit("grain", "gr = 64.79891 mg");
			check_operation_table();
			m_calculator.add_unit("dram", "dr = (27 + 11/32)gr");
			check_operation_table();
			m_calculator.add_unit("ounce", "oz = 16 dr");
			check_operation_table();
			m_calculator.add_unit("pound", "lb = 16 oz");
			check_operation_table();
			m_calculator.add_unit("hundredweicht", "cwt = 100 lb");
			check_operation_table();
			m_calculator.add_unit("short_ton", "ton = 20 cwt");
			check_operation_table();

			m_calculator.add_unit("pound_force", "lbf = lb * 9.80665m/s^2");
			check_operation_table();
			m_calculator.add_unit("psi = lbf/in^2");
			check_operation_table();
			//		m_symbols.add_by_name("survey_township", _block.tresult());

			m_calculator.add_unit("british_thermal_unit", "Btu = 1055 J");
			check_operation_table();
			m_calculator.add_unit("calorie", "cal = 4.184 J");
			check_operation_table();
			m_calculator.add_unit("large_calorie", "Cal = kcal = 1000 cal");
			check_operation_table();
			m_symbols.add_by_name("food_calorie", _block.tresult(), false);
			check_operation_table();
			m_calculator.add_unit("horse_power", "hp = 745.7 W");
			check_operation_table();

			m_symbols.set_system("No prefix");
			m_calculator.add_unit("atm = 101325 Pa");
			check_operation_table();
			//		m_symbols.add_by_name("survey_township", _block.tresult());

			m_symbols.set_system("Memory");
			m_calculator.add_unit("bit", "b = 1Mem");
			check_operation_table();
			m_calculator.add_unit("byte", "B = 8b");
			check_operation_table();

			m_symbols.set_system("Currency");
			m_calculator.add_unit("dollar = 1Crr");
			check_operation_table();

			m_symbols.set_system();
			m_symbols.set_system_constants("Physical Constants");
			m_calculator.add_constant("speed of light in a vacuum", "c = 299792458 m/s");
			check_operation_table();
			m_calculator.add_constant("gravitational constant", "G = 6.67408 10^(-11) N m^2/kg^2");
			check_operation_table();
			m_calculator.add_constant("planck constant", "h = 6.62607015 10^(-34) J s");
			check_operation_table();
			m_calculator.add_constant("h c", "hc = h c");
			check_operation_table();
			m_calculator.add_constant("elementary charge", "e.0 = 1.602176634 10^(-19) C");
			check_operation_table();
			m_calculator.add_constant("magnetic constant permeability of free space vacuum permeability", "mhu.0 = 4 pi 10^(-7) m/A");
			check_operation_table();
			m_calculator.add_constant("electric constant permitivitty of free space vacuum permitivitty", "epsilon.0 = 1 / (mhu.0 c^2)");
			check_operation_table();
			m_calculator.add_constant("avogadro constant", "NA = 6.02214076 10^23 1/mol");
			check_operation_table();
			m_calculator.add_constant("boltzmann constant", "k = 1.380649 10^(-23) J/K");
			check_operation_table();
			m_calculator.add_constant("gas constant", "R = NA k");
			check_operation_table();
			m_calculator.add_constant("wien displacement constant", "b = 2.897771955 mm/K");
			check_operation_table();
			m_calculator.add_constant("atomic mass constant", "m.u = 1.660 10^(-27) kg");
			check_operation_table();
			m_calculator.add_constant("electron mass", "m.e = 9.109 10^(-31) kg");
			check_operation_table();
			m_calculator.add_constant("proton mass", "m.p = 1.007 m.u");
			check_operation_table();
			m_calculator.add_constant("neutron mass", "m.n = 1.008 m.u");
			check_operation_table();
			m_calculator.add_constant("luminous efficacy", "Kcd = 683 lm/W");
			check_operation_table();
			m_calculator.add_constant("standard gravity", "g.0 = 9.80665 m/s^2");
			check_operation_table();
			m_calculator.add_constant("hubble constant", "H.0 = 2.25 10^(-18) 1/s");
			check_operation_table();
			m_symbols.set_system_constants();
		}
		catch (pmb::parser::exception<item>& ex)
		{
			m_expr = m_calculator.expression();
			pmb::log::instance()->trace(pmb::logError, "Exception \"%s\": %s\n", ex.message(m_expr.c_str()).c_str(), m_expr);
			m_error = ex;
			AfxGetMainWnd()->PostMessage(MM_CHARGENEWDOC, WPARAM(m_symbols.get()));
			return true;
		}
		m_symbols.set_default_system("SI");
	}
	return true;
}




void CParserDoc::result()
{
	try
	{
		const magnitude& val = _block.result();
		number_double number;
		std::string sunit;
		m_symbols.value(val, number, sunit, true);
		std::stringstream sres;
		sres << number;
		CStringA sn(sres.str().c_str());
		sn.Replace("e+", " 10^");
		int le = sn.Replace("e-", " 10^(-");
		sn.Replace("e", " 10^");
		if (0 < le)
			sn += ")";
		sres.str("");
		sres << sn << sunit;
		m_result = sres.str();
		m_calculator.parser_result(m_result.c_str());
	}
	catch (pmb::parser::exception<item>& ex)
	{
		m_result.clear();
	}
}



// CParserDoc commands
void CParserDoc::opr_positive(transporter_args& args)
{
	args.result()->positive(**args.left());
}

void CParserDoc::opr_negative(transporter_args& args)
{
	args.result()->negative(**args.left());
}

void CParserDoc::opr_factorial(transporter_args& args)
{
	if (!args.left()->integer())
		throw operation::exception("must be a integer");
	args.result()->factorial(**args.left());
}

void CParserDoc::opr_sqrroot(transporter_args& args)
{
	args.result()->sqrroot(**args.left());
}

void CParserDoc::opr_exponentiation(transporter_args& args)
{
	args.result()->pow(**args.left(), **args.right());
}

void CParserDoc::opr_root(transporter_args& args)
{
	args.result()->root(**args.left(), **args.right());
}

void CParserDoc::opr_multiplication(transporter_args& args)
{
	args.result()->multiplication(**args.left(), **args.right());
}

void CParserDoc::opr_division(transporter_args& args)
{
	if (args.right()->zero())
		throw operation::exception("divided by zero");
	args.result()->division(**args.left(), **args.right());
}

void CParserDoc::opr_modulo(transporter_args& args)
{
	args.result()->modulo(**args.left(), **args.right());
}

void CParserDoc::opr_addition(transporter_args& args)
{
	args.result()->addition(**args.left(), **args.right());
}

void CParserDoc::opr_subtraction(transporter_args & args)
{
	args.result()->substraction(**args.left(), **args.right());
}

void CParserDoc::opr_assignation(transporter_args& args)
{
	args.result() = args.left() = args.right();
}

void CParserDoc::opr_result(transporter_args& args)
{
	args.result() = args.left();
}

void CParserDoc::opr_result_modify(transporter_args& args)
{
	if (args.right()->zero())
		throw operation::exception("divided by zero");
	args.result()->division(**args.left(), **args.right());
}



void CParserDoc::binf_test(transporter_args & args)
{
	args.left()->addition(**args[0], **args[2]);
	args.left()->addition(**args[0], **args[3]);
	args.result()->addition(**args[0], **args[1]);
}


void CParserDoc::binf_abs(transporter_args& args)
{
	if (!args.left()->dimensionless())
		throw operation::exception("must be dimensionless");
	if (!args.left()->scalar())
		throw operation::exception("must be scaler");
	args.result()->abs(**args.left());
}

void CParserDoc::binf_sgn(transporter_args& args)
{
	if (!args.left()->dimensionless())
		throw operation::exception("The value must be dimensionless");
	if (!args.left()->scalar())
		throw operation::exception("The value must be scaler");
	if (!args.left()->complex())
		throw operation::exception("The value must be real or imaginary");
	args.result()->sgn(**args.left());
}

void CParserDoc::binf_rnd(transporter_args& args)
{
}

void CParserDoc::binf_ceil(transporter_args& args)
{
}

void CParserDoc::binf_floor(transporter_args& args)
{
}

void CParserDoc::binf_round(transporter_args& args)
{
}

void CParserDoc::binf_lg(transporter_args& args)
{
	args.result()->lg(**args[0]);
}

void CParserDoc::binf_ln(transporter_args& args)
{
	args.result()->ln(**args[0]);
}

void CParserDoc::binf_log(transporter_args& args)
{
	args.result()->log(**args[0], **args[1]);
}

void CParserDoc::binf_exp(transporter_args& args)
{
}

void CParserDoc::binf_sin(transporter_args& args)
{
	args.result()->sin(**args[0]);
}

void CParserDoc::binf_cos(transporter_args& args)
{
	args.result()->cos(**args[0]);
}

void CParserDoc::binf_tg(transporter_args& args)
{
	args.result()->tg(**args[0]);
}

void CParserDoc::binf_sec(transporter_args& args)
{
}

void CParserDoc::binf_cosec(transporter_args& args)
{
}

void CParserDoc::binf_cotg(transporter_args& args)
{
}

void CParserDoc::binf_asin(transporter_args& args)
{
	args.result()->asin(**args[0]);
}


void CParserDoc::binf_acos(transporter_args& args)
{
	args.result()->acos(**args[0]);
}

void CParserDoc::binf_atg(transporter_args& args)
{
	args.result()->atg(**args[0]);
}

void CParserDoc::binf_atg2(transporter_args& args)
{
	args.result()->atg2(**args[0], **args[1]);
}


void CParserDoc::binf_asec(transporter_args& args)
{
}

void CParserDoc::binf_acosec(transporter_args& args)
{
}

void CParserDoc::binf_acotg(transporter_args& args)
{
}

void CParserDoc::binf_if(transporter_args& args)
{
}



