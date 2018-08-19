
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
#include "pmb_calculator.cpp"

#include "pmb_parser_operation.cpp"
#include "pmb_parser_symbol.cpp"

#include "pmb_parser_tree.cpp"
#include "pmb_parser_nodes_calc.cpp"
#include "pmb_parser_nodes_parentheses.cpp"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CParserDoc
const operation CParserDoc::_operation[] = {
	operation("+", 250, false, false, "positive", "positive", &CParserDoc::opr_positive),
	operation("-", 250, false, false, "negative", "negative", &CParserDoc::opr_negative),
	operation("!", 250, true, false, "factorial", "factorial", &CParserDoc::opr_factorial),
	operation("^", 200, true, true, "power", "exponentiation", &CParserDoc::opr_exponentiation),
	operation("¨", 200, true, true, "root", "root", &CParserDoc::opr_root),
	operation("*", 110, true, true, "product", "multiplication", &CParserDoc::opr_multiplication),
	operation("/", 110, true, true, "cocient", "division", &CParserDoc::opr_division),
	operation("", 100, true, true, "product implicit", "multiplication implicit or call function", &CParserDoc::opr_multiplication, true),
	operation(" ", 100, true, true, "product space", "multiplication space or call function", &CParserDoc::opr_multiplication, true),
	operation(" ", 100, false, true, "product space inverse", "multiplication space or call function right to left", &CParserDoc::opr_multiplication, true),
	operation("\\", 100, true, true, "modulo", "congruence relation", &CParserDoc::opr_modulo),
	operation("-", 50, true, true, "substract", "substraction", &CParserDoc::opr_subtraction),
	operation("+", 50, true, true, "add", "addition", &CParserDoc::opr_addition),
	operation("=", 10, false, true, "assignation", "assignation", &CParserDoc::opr_assignation, false, true, false),
	operation("=", 0, true, false, "result", "result", &CParserDoc::opr_result)
};


const build_in_function CParserDoc::_build_in_function[] = {
	build_in_function("test", "for test", &CParserDoc::binf_test, 4),
	build_in_function("abs", "absolute value", &CParserDoc::binf_abs),
	build_in_function("sgn", "sign", &CParserDoc::binf_sgn),
	build_in_function("rnd", "random", &CParserDoc::binf_rnd),
	build_in_function("ceil", "ceil", &CParserDoc::binf_ceil),
	build_in_function("floor", "sign", &CParserDoc::binf_floor),
	build_in_function("round", "round", &CParserDoc::binf_round),

	build_in_function("lg", "decimal logarithm", &CParserDoc::binf_lg),
	build_in_function("ln", "neperian logarithm", &CParserDoc::binf_ln),
	build_in_function("log", "n-esim logarithm", &CParserDoc::binf_log, 2),
	build_in_function("exp", "exponential", &CParserDoc::binf_exp),

	build_in_function("sin", "sine", &CParserDoc::binf_sin),
	build_in_function("cos", "cosine", &CParserDoc::binf_cos),
	build_in_function("tg", "tangent", &CParserDoc::binf_tg),
	build_in_function("sec", "secant", &CParserDoc::binf_sec),
	build_in_function("cosec", "cosecant", &CParserDoc::binf_cosec),
	build_in_function("cotg", "cotangent", &CParserDoc::binf_cotg),
	build_in_function("asin", "arc sine", &CParserDoc::binf_asin),
	build_in_function("acos", "arc cosine", &CParserDoc::binf_acos),
	build_in_function("atg", "arc tangent", &CParserDoc::binf_atg),
	build_in_function("atg", "tangent", &CParserDoc::binf_atg2, 2),
	build_in_function("asec", "arc secant", &CParserDoc::binf_asec),
	build_in_function("acosec", "arc cosecant", &CParserDoc::binf_acosec),
	build_in_function("acotg", "arc cotangent", &CParserDoc::binf_acotg),

	build_in_function("if", "conditional function", &CParserDoc::binf_if, 3)
};



IMPLEMENT_DYNCREATE(CParserDoc, CDocument)

BEGIN_MESSAGE_MAP(CParserDoc, CDocument)
END_MESSAGE_MAP()


// CParserDoc construction/destruction

CParserDoc::CParserDoc()
	: _operation_table(_operation, sizeof(_operation) / sizeof(*_operation)),
		_build_in_fnc_table(_build_in_function, sizeof(_build_in_function) / sizeof(*_build_in_function)),
		_block(&_build_in_fnc_table, &m_symbols),
		m_parser(&_operation_table, &_block),
		m_calculator(&_operation_table, &_block)
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
	if (!CDocument::OnNewDocument())
	{
		log->endFunction();
		return FALSE;
	}

	m_parser.initialize();
	m_calculator.initialize();

	m_iterators = m_parser.getIterators();
	m_iterator = m_parser.getIterator();
	m_countIterators = 1;

	m_expr = "a (sin(b + 3, zl), 4 ^ 7 * z^i * 2^3 + 1*2 + 3 + 4 +5, beta(c-3)alpha,  (8*(9*(h + 5))^(j-k)) )(2c) = - a*b+c";
	m_expr = "a = 2^3";
//	m_expr = "a acos(6x, -3y, z) = -5k";
//	m_expr = "a acos (";

	typedef std::tuple<const char*, bool, double> tuple;
	typedef std::vector<tuple> vector;

	const double pi = 4 * atan(1);
	vector test;
	test.push_back(tuple("f(x, y, z) = z + 4 x^2 + y", false, 0));
	test.push_back(tuple("f(2, 3, 1)", true, 1 + 4 * 2 * 2 + 3));
	test.push_back(tuple("f(x) = 1 + 4 x^2 + x", false, 0));
	test.push_back(tuple("a0 = f(5) - f(2)", true, 1 + 4 * 5 * 5 + 5 - (1 + 4 * 2 * 2 + 2)));
	test.push_back(tuple("k = (2 * 3) ^ (1 + 1) / (5 + 4) + 8 * (((1 + 1)(5 + 7)(2 + 1)) / (6 + 6)) / 12", true, 8));
	test.push_back(tuple("a1 = 5 test(9 - 8, 1 + 1, 6 - 3, 2 * 2)", true, 50));
	test.push_back(tuple("a2 = 5 test(1, 2, 3, 4)", true, 50));
	test.push_back(tuple("10 - 10 * 10 + 10", true, 10 - 10 * 10 + 10));
	test.push_back(tuple("pi = 4 atg 1", true, pi));
	test.push_back(tuple("a3 = sin(3.141592/6)", true, sin(3.141592 / 6)));
	test.push_back(tuple("a4 = sin pi/6", true, sin(pi / 6)));
	test.push_back(tuple("a5 = 5 test(9 - 8, 1 + 1, 6 - 3, 2 * 2)", true, 50));
	test.push_back(tuple("k1 = (2 * 3) ^ (1 + 1) / (5 + 4) + 8 * (((1 + 1)(5 + 7)(2 + 1)) / (6 + 6)) / 12", true, 8));
	test.push_back(tuple("a6 = sin pi/6", true, sin(pi / 6)));
	test.push_back(tuple("g(x) = 1 + 4 x^2 + x", false, 0));
	test.push_back(tuple("a = f(5) - f(-1)", true, 1 + 4 * 5 * 5 + 5 - (1 + 4 - 1)));
	test.push_back(tuple("f(1)", true, 1 + 4 + 1));

	int errors = 0;
	m_symbols.addSetVariable("Constants");
	m_symbols.selectSearch("Constants");
	for (int i = 0; i < test.size(); ++i)
	{
		bool bResult = false;
		try
		{
			m_calculator.parser(m_expr = std::get<0>(test[i]));
			bResult = true;
			if (_block.result() != std::get<2>(test[i]) || !std::get<1>(test[i]))
			{
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
			if (!bResult || std::get<1>(test[i]))
			{
				log->trace(pmb::logError, "Exception \"%s\": %s = <NULL> =! %f\n", ex.message(m_expr).c_str(), m_expr, std::get<2>(test[i]));
				m_error = ex;
				++errors;
			}
			else
				log->trace(pmb::logWarning, "%s == <NULL>\n", m_expr);
		}
	}
	if (errors)
		log->trace(pmb::logError, "%d/%d errors found\n", errors, test.size());
	else
		log->trace(pmb::logWarning, "%d tests successful!!!\n", test.size());

	AfxGetMainWnd()->PostMessage(MM_CHARGENEWDOC, WPARAM(m_symbols.get()));
	return true;
	try
	{
		m_calculator.parser(m_expr = "f(x) = 4 x");
		m_calculator.parser(m_expr = "a = 3 f(2)");
		m_calculator.parser(m_expr = "e = 2.71828182");
	}
	catch (pmb::parser::exception<item>& ex)
	{
		pmb::log::instance()->trace(pmb::logError, "Error: %s\n", ex.message(m_expr).c_str());
		m_error = ex;
		AfxGetMainWnd()->PostMessage(MM_CHARGENEWDOC, WPARAM(m_symbols.get()));
		return true;
	}
	m_symbols.selectSearch("Variables");

	m_parser.parser(m_expr);

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
		m_calculator.parser(m_expr);
	}
	catch (pmb::parser::exception<item>& ex)
	{
		pmb::log::instance()->trace(pmb::logError, "Error: %s\n", ex.message(m_expr).c_str());
	}
//	m_expr = "a = y(pi)";
//	m_calculator.parser(m_expr);
	GetSystemTime(&m_time_end);

	AfxGetMainWnd()->PostMessage(MM_CHARGENEWDOC, WPARAM(m_symbols.get()));
	log->endFunction(pmb::logDebug);
	return TRUE;
}


bool CParserDoc::nextStep()
{
	return m_parser.nextStep();
}

const tree* CParserDoc::getTree() const
{
	return m_parser.getTree();
}

const tnode* CParserDoc::getNodeRoot() const
{
	return m_calculator.getTree() ? m_calculator.getTree()->getRootNode() : nullptr;
}

const tnode* CParserDoc::getNewNode() const
{
	return m_parser.getNewNode();
}

const tnode* CParserDoc::getNewNodeUnknow() const
{
	return m_parser.getNewNodeUnknow();
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



const tree* CParserDoc::getTree2() const
{
	return m_calculator.getTree();
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

void CParserDoc::opr_exponentiation(transporter_args& args)
{
	args.result()->exponentiation(**args.left(), **args.right());
}

void CParserDoc::opr_root(transporter_args& args)
{
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
}

void CParserDoc::binf_ln(transporter_args& args)
{
}

void CParserDoc::binf_log(transporter_args& args)
{
}

void CParserDoc::binf_exp(transporter_args& args)
{
}

void CParserDoc::binf_sin(transporter_args& args)
{
	if (!args.left()->dimensionless())
		throw operation::exception("must be dimensionless");
	if (!args.left()->scalar())
		throw operation::exception("must be scaler");
	args.result()->sin(**args[0]);
}

void CParserDoc::binf_cos(transporter_args& args)
{
}

void CParserDoc::binf_tg(transporter_args& args)
{
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
}


void CParserDoc::binf_acos(transporter_args& args)
{
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
