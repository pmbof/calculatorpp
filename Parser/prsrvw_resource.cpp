
#pragma region includes
#include "stdafx.h"

#include "ParserDoc.h"
#include "ParserView.h"
#pragma endregion includes


CParserView::resource::style::style(CFont* pF, COLORREF col)
	: font(pF), color(col)
{
}



CParserView::resource::resource(CWnd* pwnd)
	: _src(nullptr), _pwnd(pwnd)
{
}


CParserView::resource::resource(resource* rsc)
	: _src(rsc), _pwnd(nullptr)
{
	while (_src->_src)
		_src = _src->_src;
}


CParserView::resource::~resource()
{
	if (!_src)
	{
		for (mstyle::const_iterator s = _style.begin(); s != _style.end(); ++s)
			delete s->second;
		for (mfonth::const_iterator f = _fonts.begin(); f != _fonts.end(); ++f)
			delete f->first.pFont;
	}
}


void CParserView::resource::clear()
{
	if (_src)
		_src->clear();
	else
	{
		for (mstyle::const_iterator s = _style.begin(); s != _style.end(); ++s)
			delete s->second;
		_style.clear();

		for (mfonth::const_iterator f = _fonts.begin(); f != _fonts.end(); ++f)
			delete f->first.pFont;
		_fonts.clear();
		_font = nullptr;
	}
}


CFont* CParserView::resource::init(bool bPretty, int pointSize, LPCTSTR fontName, COLORREF backColor, COLORREF color)
{
	clear();
	if (_src)
		return _src->init(bPretty, pointSize, fontName, backColor, color);

	_bPretty = bPretty;
	_idxSize = 25;
	CDC* pDC = _pwnd->GetDC();
	_logpixelsy = pDC->GetDeviceCaps(LOGPIXELSY);
	_font = _get_font(std::string(CStringA(fontName)), pointSize, false, false, pDC);
	_pwnd->ReleaseDC(pDC);

	_bckColor = backColor;
	_color = color;
	return _font;
}





void CParserView::resource::add_style(const std::string& sname, const LOGFONT* plf, COLORREF color)
{
	CDC* pDC = _pwnd->GetDC();
	CFont* pFont = _get_font(plf, pDC);
	_add_style(sname, pFont, pDC, color);
	_pwnd->ReleaseDC(pDC);
}




void CParserView::resource::add_style(const std::string& sname, int pointSize, LPCTSTR fontName, COLORREF color)
{
	CDC* pDC = _pwnd->GetDC();
	CFont* pFont = _get_font(std::string(CStringA(fontName)), pointSize, false, false, pDC);
	_add_style(sname, pFont, pDC, color);
	_pwnd->ReleaseDC(pDC);
}




bool CParserView::resource::pretty() const
{
	return _bPretty;
}





CFont* CParserView::resource::font(const std::string& sname, short index)
{
	if (_src)
		return _src->font(sname, index);

	mstyle::iterator s = _style.find(sname);
	if (s == _style.end())
	{
		if (index)
		{
			LOGFONT lf;
			_font->GetLogFont(&lf);
			int pointSize = -MulDiv(lf.lfHeight, 72, _logpixelsy) * 10;

			add_style(sname, pointSize, lf.lfFaceName, _color);
			s = _style.find(sname);
		}
		else
			return _font;
	}

	if (index)
	{
		short nidx = index < 0 ? -index : index;
		mapidxfont::iterator f = s->second->idxfont.find(nidx);
		if (f != s->second->idxfont.end())
			return f->second;

		LOGFONT lf;
		s->second->font->GetLogFont(&lf);
		int pointSize = -MulDiv(lf.lfHeight, 72, _logpixelsy) * 10;
		pointSize -= _idxSize * nidx;
		if (pointSize < 50)
			pointSize = 50;
		CDC* pDC = _pwnd->GetDC();
		CFont* pFont = _get_font(std::string(CStringA(lf.lfFaceName)), pointSize, lf.lfItalic, lf.lfWeight == FW_BOLD, pDC);
		_pwnd->ReleaseDC(pDC);
		s->second->idxfont[nidx] = pFont;
		return pFont;
	}
	return s->second->font;
}





short CParserView::resource::height(CFont* pFont) const
{
	if (_src)
		return _src->height(pFont);

	for (mfonth::const_iterator f = _fonts.begin(); f != _fonts.end(); ++f)
	{
		if (f->first.pFont == pFont)
			return f->second;
	}
	return 0;
}






COLORREF CParserView::resource::color(const std::string& sname) const
{
	if (_src)
		return _src->color(sname);

	mstyle::const_iterator s = _style.find(sname);
	return s == _style.end() ? _color : s->second->color;
}



CFont* CParserView::resource::font(short index)
{
	if (_src)
		return font(index);

	if (!index || !_font)
		return _font;

	short nidx = index < 0 ? -index : index;

	LOGFONT lf;
	_font->GetLogFont(&lf);
	int pointSize = -MulDiv(lf.lfHeight, 72, _logpixelsy) * 10;
	pointSize -= _idxSize * nidx;
	if (pointSize < 50)
		pointSize = 50;
	CDC* pDC = _pwnd->GetDC();
	CFont* pFont = _get_font(std::string(CStringA(lf.lfFaceName)), pointSize, lf.lfItalic, lf.lfWeight == FW_BOLD, pDC);
	_pwnd->ReleaseDC(pDC);

	return pFont;
}



COLORREF CParserView::resource::color() const
{
	if (_src)
		return _src->color();
	return _color;
}

COLORREF CParserView::resource::back_color() const
{
	if (_src)
		return _src->back_color();
	return _bckColor;
}


short CParserView::resource::max_height() const
{
	short mh = 0;
	for (mfonth::const_iterator fh = _fonts.begin(); fh != _fonts.end(); ++fh)
	{
		if (fh == _fonts.begin())
			mh = fh->second;
		else if (mh < fh->second)
			mh = fh->second;
	}
	return mh;
}



CParserView::resource::style* CParserView::resource::_add_style(const std::string& sname, CFont* pFont, CDC* pDC, COLORREF color)
{
	if (_src)
		return _src->_add_style(sname, pFont, pDC, color);

	mstyle::iterator s = _style.find(sname);
	if (s == _style.end())
	{
		style* ns = new style(pFont, color);
		_style[sname] = ns;
		return ns;
	}

	s->second->font = pFont;
	s->second->color = color;

	LOGFONT lf;
	pFont->GetLogFont(&lf);
	update_childs(pDC, &lf, s->second->idxfont);
	return s->second;
}



CFont* CParserView::resource::_find_font(const std::string& fontName, short pointFont, bool italic, bool bold) const
{
	sfont sf;
	sf.name = fontName;
	sf.pointSize = pointFont;
	sf.italic = italic;
	sf.bold = bold;
	mfonth::const_iterator f = _fonts.find(sf);
	return f == _fonts.end() ? nullptr : f->first.pFont;
}


CFont* CParserView::resource::_find_font(const LOGFONT* plf) const
{
	sfont sf;
	sf.name = CStringA(plf->lfFaceName);
	sf.pointSize = -MulDiv(plf->lfHeight, 72, _logpixelsy) * 10;
	sf.italic = plf->lfItalic;
	sf.bold = plf->lfWeight == FW_BOLD;
	mfonth::const_iterator f = _fonts.find(sf);
	return f == _fonts.end() ? nullptr : f->first.pFont;
}


CFont* CParserView::resource::_get_font(const std::string& fontName, short pointFont, bool italic, bool bold, CDC* pDC)
{
	sfont sf;
	sf.name = fontName;
	sf.pointSize = pointFont;
	sf.italic = italic;
	sf.bold = bold;
	mfonth::const_iterator f = _fonts.find(sf);
	if (f == _fonts.end())
	{
		sf.pFont = new CFont;
		sf.pFont->CreatePointFont(sf.pointSize, CString(sf.name.c_str()));
		if (sf.italic || sf.bold)
		{
			LOGFONT lf;
			sf.pFont->GetLogFont(&lf);
			lf.lfItalic = sf.italic;
			lf.lfWeight = sf.bold ? FW_BOLD : FW_NORMAL;
			sf.pFont->DeleteObject();
			sf.pFont->CreateFontIndirect(&lf);
		}
		short h;
		if (pDC)
		{
			CFont* oldFont = pDC->SelectObject(sf.pFont);
			TEXTMETRIC tm;
			pDC->GetTextMetrics(&tm);
			h = tm.tmHeight;
			pDC->SelectObject(oldFont);
		}
		else
			h = 0;
		_fonts[sf] = h;
		return sf.pFont;
	}
	return f->first.pFont;
}



CFont* CParserView::resource::_get_font(const LOGFONT* plf, CDC* pDC)
{
	sfont sf;
	sf.name = CStringA(plf->lfFaceName);
	sf.pointSize = -MulDiv(plf->lfHeight, 72, _logpixelsy) * 10;
	sf.italic = plf->lfItalic;
	sf.bold = plf->lfWeight == FW_BOLD;
	mfonth::const_iterator f = _fonts.find(sf);
	if (f == _fonts.end())
	{
		sf.pFont = new CFont;
		sf.pFont->CreateFontIndirect(plf);
		short h;
		if (pDC)
		{
			CFont* oldFont = pDC->SelectObject(sf.pFont);
			TEXTMETRIC tm;
			pDC->GetTextMetrics(&tm);
			h = tm.tmHeight;
			pDC->SelectObject(oldFont);
		}
		else
			h = 0;
		_fonts[sf] = h;
		return sf.pFont;
	}

	return f->first.pFont;
}



void CParserView::resource::update_childs(CDC* pDC, const LOGFONT* plf, mapidxfont& idxfont)
{
	int pointSize = -MulDiv(plf->lfHeight, 72, pDC->GetDeviceCaps(LOGPIXELSY));
	for (mapidxfont::iterator cf = idxfont.begin(); cf != idxfont.end(); ++cf)
		cf->second = _get_font(std::string(CStringA(plf->lfFaceName)), pointSize - _idxSize * cf->first, plf->lfItalic, plf->lfWeight == FW_BOLD, pDC);
}

bool CParserView::resource::sfont_less::operator()(const sfont& left, const sfont& right) const
{
	return left.name == right.name ? left.pointSize == right.pointSize ? left.italic == right.italic ? left.bold == false && right.bold == true :
		left.italic == false && right.bold == true :
		left.pointSize < right.pointSize :
		left.name < right.name;
}
