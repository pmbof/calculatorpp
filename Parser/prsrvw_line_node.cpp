
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes




CParserView::line::node::node(const tnode* ptnd, bnode* parent)
	: bnode(ptnd, parent)
{
}



CParserView::line::node::~node()
{
}




void CParserView::line::node::print_line(pmb::log_type tplg, const char* text) const
{
	if (this)
		pmb::log::instance()->trace(tplg, "%s = 0x%08X [%d, %d]\n", text, this, get_ini(), get_end());
	else
		bnode::print_line(tplg, text);
}





CParserView::line::node* CParserView::line::node::new_instance(bnode** ndLR, bnode* parent, const tnode* nd)
{
	pmb::log* lg = pmb::log::beginFunction(pmb::logDebug, __FUNCTION__);
	ASSERT(*ndLR == nullptr);
	node* nnd = nullptr;
	if (parent)
	{
		if (&static_cast<node*>(parent)->bnode::_left != ndLR && &static_cast<node*>(parent)->bnode::_right != ndLR)
		{
			lg->trace(pmb::logError, "Problem in tree generation, the new item child (0x%08X) of tree is not direct child of parent (0x%08X -> [l:0x%08X, r:0x%08X]), can not continue!\n", ndLR, parent, &static_cast<node*>(parent)->bnode::_left, &static_cast<node*>(parent)->bnode::_right);
			lg->endFunction(pmb::logError);
			return nullptr;
		}
	}

	switch (nd->getType())
	{
	case pmb::parser::ndSpace:
		*ndLR = nnd = new node_space(nd, parent);
		break;
	case pmb::parser::ndAlpha:
		if (nd->getParent() && nd->getParent()->getType() == pmb::parser::ndUnknown)
		{
			const pmb::parser::nodes::unknown<item, ndtype>* uk = static_cast<const pmb::parser::nodes::unknown<item, ndtype>*>(nd->getParent());
			if (uk->isBinary() && uk->isCallFunction() && (uk->isFirstLeft() && uk->getLeft() == nd || uk->isFirstRight() && uk->getRight() == nd))
				*ndLR = nnd = uk->isCallBuildInFunction() ? static_cast<node*>(new node_buildin_function(nd, parent)) : new node_function(nd, parent);
			else
				*ndLR = nnd = new node_alpha(nd, parent);
		}
		else
			*ndLR = nnd = new node_alpha(nd, parent);
		break;
	case pmb::parser::ndNumber:
		*ndLR = nnd = new node_number(nd, parent);
		break;
	case pmb::parser::ndString:
		*ndLR = nnd = new node_string(nd, parent);
		break;
	case pmb::parser::ndList:
		*ndLR = nnd = new node_list(nd, parent);
		break;
	case pmb::parser::ndParentheses:
		*ndLR = nnd = new node_parentheses(nd, parent);
		break;
	case pmb::parser::ndUnknown:
	{
		const pmb::parser::nodes::unknown<item, ndtype>* uk = static_cast<const pmb::parser::nodes::unknown<item, ndtype>*>(nd);
		if (uk->isValid() && !uk->isCallFunction())
		{
			const operation* opr = static_cast<const operation*>(reinterpret_cast<const ::operation*>(uk->pointer()));
			*ndLR = nnd = opr->new_instance(parent, nd);
		}
		else
			*ndLR = nnd = new node_unknown(nd, parent);
	}
		break;
	default:
		break;
	}

#ifdef DEBUG
	if (*ndLR)
	{
		lg->trace(pmb::logDebug, "new node generate = 0x%08X\n", *ndLR);
		(*ndLR)->debug_check();
		(*ndLR)->debug_check_all();
	}
#endif // DEBUG
	lg->endFunction(pmb::logDebug);
	return nnd;
}




void CParserView::line::node::set(sset* ss)
{
	const tnode* lnd = _ptnd->getLeft();
	const tnode* rnd = _ptnd->getRight();

	if (_parent)
		set_rect_fromparent();
	else
	{
		top = ss->pline->top;
		bottom = ss->pline->bottom;

		left = right = ss->pline->left;
		_middle = top + Height() / 2;
	}

	if (lnd)
	{
		ss->tnd = lnd;
		new_instance(&_left, this, lnd)->set(ss);
		ss->tnd = _ptnd;
	}

	CRect rl;
	if (lnd)
	{
		rl = _left->rect();
		left = rl.right;
		right = left;
		top = rl.top;
		bottom = rl.bottom;
		_middle = _left->_middle;
	}
	if (get_ini() < get_end())
	{
		CFont* pFont = ss->pline->font(type(), ss->index);
		CFont* oldFont = ss->pDC->SelectObject(pFont);

		CSize te;
		GetTextExtentPointA(ss->pDC->m_hDC, ss->pstr + get_ini(), get_length(), &te);

		right = left + te.cx;
		if (lnd && rl.Height() != te.cy)
		{
			top = _middle - te.cy / 2 - te.cy % 2;
			bottom = top + te.cy;
		}
		else if (!lnd)
		{
			top = _middle - te.cy / 2;
			bottom = top + te.cy;
		}
		ss->pDC->SelectObject(oldFont);
	}
	if (rnd)
	{
		ss->tnd = rnd;
		new_instance(&_right, this, rnd)->set(ss);
		ss->tnd = _ptnd;
	}
	check_error(ss);
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
	if (get_ini() < get_end())
	{
		COLORREF oldColor = sd->pDC->SetTextColor(sd->pline->color(type()));
		CFont* oldFont = sd->pDC->SelectObject(sd->pline->font(type(), sd->index));
		CString sn(sd->pstr + get_ini(), get_length());

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



bool CParserView::line::node::set_caret_pos(sdraw* sd, scaret& caret) const
{
	bool bOk = false;
	if (_left)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		bOk = _left->set_caret_pos(sd, caret);
		sd->pnd = pnd;
	}
	if (!bOk && get_ini() <= caret.spos[1] && caret.spos[1] <= get_end())
	{
		int cx;
		if (get_ini() < caret.spos[1])
		{
			CFont* oldFont = sd->pDC->SelectObject(sd->pline->font(type(), sd->index));
			CString sn(sd->pstr + get_ini(), caret.spos[1] - get_ini());

			cx = sd->pDC->GetTextExtent(sn).cx;
			sd->pDC->SelectObject(oldFont);
		}
		else
			cx = 0;
		caret.pos[1].x = left + cx;
		caret.pos[1].y = top;
		caret.height = Height();
		bOk = true;
	}
	if (!bOk && _right)
	{
		const bnode* pnd = sd->pnd;
		sd->pnd = this;
		bOk = _right->set_caret_pos(sd, caret);
		sd->pnd = pnd;
	}
	return bOk;
}





bool CParserView::line::node::empty() const
{
	return get_ini() == get_end() && !get_ini() && !_left && !_right;
}



bool CParserView::line::node::parentheses() const
{
	return false;
}



short CParserView::line::node::nparentheses() const
{
	return 0;
}


