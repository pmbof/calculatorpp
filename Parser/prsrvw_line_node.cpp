
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::line::node::node()
	: bnode()
{
}



CParserView::line::node::node(const tnode* nd)
	: bnode(), _ini(nd->getIni()), _end(nd->getEnd())
{
}



CParserView::line::node::~node()
{
}



CParserView::line::node* CParserView::line::node::new_instance(const tnode* nd)
{
	node* nnd = nullptr;
	switch (nd->getType())
	{
	case pmb::parser::ndSpace:
		nnd = new node_space(nd);
		break;
	case pmb::parser::ndAlpha:
		if (nd->getParent() && nd->getParent()->getType() == pmb::parser::ndUnknown)
		{
			const pmb::parser::nodes::unknown<item, ndtype>* uk = static_cast<const pmb::parser::nodes::unknown<item, ndtype>*>(nd->getParent());
			if (uk->isBinary() && uk->isCallFunction() && (uk->isFirstLeft() && uk->getLeft() == nd || uk->isFirstRight() && uk->getRight() == nd))
				nnd = uk->isCallBuildInFunction() ? static_cast<node*>(new node_buildin_function(nd)) : new node_function(nd);
			else
				nnd = new node_alpha(nd);
		}
		else
			nnd = new node_alpha(nd);
		break;
	case pmb::parser::ndNumber:
		nnd = new node_number(nd);
		break;
	case pmb::parser::ndString:
		nnd = new node_string(nd);
		break;
	case pmb::parser::ndList:
		nnd = new node_list(nd);
		break;
	case pmb::parser::ndParentheses:
		nnd = new node_parentheses(nd);
		break;
	case pmb::parser::ndUnknown:
	{
		const pmb::parser::nodes::unknown<item, ndtype>* uk = static_cast<const pmb::parser::nodes::unknown<item, ndtype>*>(nd);
		if (uk->isValid() && !uk->isCallFunction())
		{
			const operation* opr = reinterpret_cast<const operation*>(uk->pointer());
			if (!strcmp(opr->getSymbol(), "="))
				nnd = new node_operator_equal(nd);
			else if (!strcmp(opr->getSymbol(), "=."))
				nnd = new node_operator_result(nd);
			else if (!strcmp(opr->getSymbol(), "*"))
				nnd = new node_operator_product(nd);
			else if (!strcmp(opr->getSymbol(), "+"))
				nnd = new node_operator_plus(nd);
			else if (!strcmp(opr->getSymbol(), "-"))
				nnd = new node_operator_minus(nd);
			else if (!strcmp(opr->getSymbol(), "/."))
				nnd = new node_operator_division_inline(nd);
			else if (!strcmp(opr->getSymbol(), "/"))
				nnd = new node_operator_division(nd);
			else if (!strcmp(opr->getSymbol(), "^"))
				nnd = new node_operator_power(nd);
			else if (!strcmp(opr->getSymbol(), "\\"))
				nnd = new node_operator_root(nd);
			else
				nnd = new node_unknown(nd);
		}
		else
			nnd = new node_unknown(nd);
	}
	break;
	default:
		break;
	}
	return nnd;
}




void CParserView::line::node::set(sset* ss)
{
	const tnode* lnd = ss->nd->getLeft();
	const tnode* rnd = ss->nd->getRight();

	ss->parents.push_back(this);
	if (ss->pnd)
	{
		top = ss->pnd->top;
		bottom = ss->pnd->bottom;
		left = right = ss->pnd->right;
		_middle = ss->pnd->_middle;
	}
	else
	{
		top = ss->pline->top;
		bottom = ss->pline->bottom;

		left = right = ss->pline->left;
		_middle = top + Height() / 2;
	}

	if (lnd)
	{
		const tnode* nd = ss->nd;
		ss->nd = lnd;
		ss->pnd = this;
		_left = new_instance(lnd);
		_left->set(ss);
		ss->nd = nd;
	}

	CRect rl;
	if (lnd)
	{
		rl = _left->rec_rect();
		left = rl.right;
		right = left;
		top = rl.top;
		bottom = rl.bottom;
		_middle = _left->_middle;
	}
	if (_ini < _end)
	{
		CFont* pFont = ss->pline->font(type(), ss->index);
		CFont* oldFont = ss->pDC->SelectObject(pFont);
		CString sn(ss->pstr + _ini, ss->nd->len());

		CRect cr(this);
		ss->pDC->DrawText(sn, cr, DT_CALCRECT | DT_LEFT | DT_TOP | DT_SINGLELINE);
		right = left + cr.Width();
		if (lnd && rl.Height() != cr.Height())
		{
			top = _middle - cr.Height() / 2 - cr.Height() % 2;
			bottom = top + cr.Height();
		}
		else if (!lnd)
		{
			top = _middle - cr.Height() / 2;
			bottom = top + cr.Height();
		}
		LOGFONT lf;
		pFont->GetLogFont(&lf);
		if (lf.lfItalic)
			right += 2;
		ss->pDC->SelectObject(oldFont);
	}
	if (rnd)
	{
		const tnode* nd = ss->nd;
		ss->nd = rnd;
		ss->pnd = this;
		_right = new_instance(rnd);
		_right->set(ss);
		ss->nd = nd;
	}
	check_error(ss);
	ss->parents.pop_back();
}



void CParserView::line::node::draw(sdraw* sd) const
{
	sd->begin_expr(this);
	if (_left)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_left->draw(sd);
		sd->pnd = pnd;
	}
	if (_ini < _end)
	{
		COLORREF oldColor = sd->pDC->SetTextColor(sd->pline->color(type()));
		CFont* oldFont = sd->pDC->SelectObject(sd->pline->font(type(), sd->index));
		CString sn(sd->pstr + _ini, _end - _ini);

		sd->pDC->DrawText(sn, const_cast<CRect*>(static_cast<const CRect*>(this)), DT_LEFT | DT_TOP | DT_SINGLELINE);
		sd->pDC->SetTextColor(oldColor);
		sd->pDC->SelectObject(oldFont);
	}
	if (_right)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		_right->draw(sd);
		sd->pnd = pnd;
	}
	sd->end_expr(this);
}




bool CParserView::line::node::empty() const
{
	return _ini == _end && !_ini && !_left && !_right;
}



bool CParserView::line::node::parentheses() const
{
	return false;
}



short CParserView::line::node::nparentheses() const
{
	return 0;
}



