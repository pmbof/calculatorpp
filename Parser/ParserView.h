
// ParserView.h : interface of the CParserView class
//

#pragma once

#define PMB_STYLE_CPPEDIT_MULTILINE				0x0001
#define PMB_STYLE_CPPEDIT_INDEX					0x0002
#define PMB_STYLE_CPPEDIT_SUPRESSPARENTHESIS	0x0004






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
	tnode* m_pNdUnk;
	tnode* m_pNd;
	CRect m_error;
	CMFCToolTipCtrl m_tooltip;
	int m_tooltipId;


#pragma region nodes declare
public:
	struct operation;
	struct opr_equal;
	struct opr_result;
	struct opr_product;
	struct opr_plus;
	struct opr_minus;
	struct opr_division_inline;
	struct opr_division;
	struct opr_power;
	struct opr_root;
	struct opr_not;
	struct opr_and;
	struct opr_or;
#pragma endregion nodes declare
private:

	struct scaret;


	struct line : CRect
	{
	private:
		enum bnodetypes : char
		{
			bndOther,
			bndSpace,
			bndAlpha,
			bndFunction,
			bndBIFunction,
			bndNumber,
			bndString,
			bndList,
			bndParentheses,
			bndUnknow,
			bndOprEqual,
			bndOprResult,
			bndResult,
			bndOprRoot,
			bndOprPower,
			bndOprProduct,
			bndOprDivisionIl,
			bndOprDivision,
			bndOprPlus,
			bndOprMinus,
			bndOprNot,
			bndOprAnd,
			bndOprOr
		};

		struct bnode;

	private:
		typedef std::map<line::bnodetypes, std::string> mstyletp;

#pragma region nodes
	private:
		struct sbase
		{
			sbase(line* pl, CDC* pdc, const char* pExpr, bool bediting);

			line* pline;
			CDC* pDC;
			const char* pstr;
			short index;
			bool bEditing;
		};

		struct sset : sbase
		{
			sset(line* pl, const tnode* ptnd, CDC* pdc, const char* pExpr, bool bediting);

			const tnode* tnd;
		};

		struct sdraw : sbase
		{
			sdraw(line* pl, const bnode* pbnd, CDC* pdc, const char* pExpr, bool bediting, item::SIZETP carpos, const char* pExpr2 = nullptr);

			const bnode* pnd;
			const char* postr;
			item::SIZETP _carpos;

			void begin_expr(const bnode* pnd);
			void end_expr(const bnode* pnd);
		};

	
		struct bnode : CRect
		{
		protected:
			bnode(const tnode* ptnd, bnode* parent);

		public:
			virtual ~bnode();

			virtual void print_line(pmb::log_type tplg, const char* text) const;
			void print() const;
			bool debug_check() const;
			void debug_check_all() const;

			virtual void set(sset* ss) = 0;
			virtual void draw(sdraw* sd) const = 0;
			virtual bool set_caret_pos(sdraw* sd, scaret& caret) const = 0;

			virtual bool empty() const = 0;

			virtual bnodetypes type() const = 0;

			virtual bool parentheses() const = 0;
			virtual short nparentheses() const = 0;

			virtual item::SIZETP get_ini() const;
			virtual item::SIZETP get_end() const;
			virtual item::SIZETP get_length() const;

			void set_rect_fromparent();

			bool is_left_parentheses(const bnode* parent) const;

			bnode& operator= (const CRect& right);

			bnode* node_mright();

			void rect_move(int dx, int dy);
			void _rect_move(int dx, int dy);
			CRect rect() const;
			void max_rect(CRect& r, const CRect& r2) const;

			void check_error(sset* ss);
			void end(sset* ss);

			const tnode* get_tnode() const;
			const bnode* get_first() const;
			const bnode* get_root() const;
			const bnode* get_first_child() const;
			const bnode* get_next() const;

			bnode* get_first();
			bnode* get_root();
			bnode* get_first_child();
			bnode* get_next();

		public:
			int _middle;

		protected:
			const tnode* _ptnd;
			bnode* _parent;
			bnode* _left;
			bnode* _right;
		};



		struct node : bnode
		{
		protected:
			node(const tnode* ptnd, bnode* parent);

		public:
			virtual ~node();

			void print_line(pmb::log_type tplg, const char* text) const override;

			static node* new_instance(bnode** ndLR, bnode* parent, const tnode* nd);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;
			bool set_caret_pos(sdraw* sd, scaret& caret) const override;

			bool empty() const override;

			bool parentheses() const override;
			short nparentheses() const override;
		};


		struct node_space : node
		{
			node_space(const tnode* nd, bnode* parent);

			bnodetypes type() const override;
		};


		struct node_alpha : node
		{
			node_alpha(const tnode* nd, bnode* parent);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;
			bool set_caret_pos(sdraw* sd, scaret& caret) const override;

			bnodetypes type() const override;

			item::SIZETP get_ini() const override;
			item::SIZETP get_end() const override;
			item::SIZETP get_length() const override;

		protected:
			item::SIZETP _ini;
			item::SIZETP _end;
		};


		struct node_function : node_alpha
		{
			node_function(const tnode* nd, bnode* parent);

			bnodetypes type() const override;
		};


		struct node_buildin_function : node_alpha
		{
			node_buildin_function(const tnode* nd, bnode* parent);

			bnodetypes type() const override;
		};


		struct node_number : node
		{
			node_number(const tnode* nd, bnode* parent);

			void set(sset* ss) override;

			bnodetypes type() const override;
		};


		struct node_string : node
		{
			node_string(const tnode* nd, bnode* parent);

			bnodetypes type() const override;
		};


		struct node_list : node
		{
			node_list(const tnode* nd, bnode* parent);

			bnodetypes type() const override;
		};


		struct node_parentheses : node
		{
			node_parentheses(const tnode* nd, bnode* parent);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;
			bool set_caret_pos(sdraw* sd, scaret& caret) const override;

			bool parentheses() const override;
			short nparentheses() const override;

			bnodetypes type() const override;

		protected:
			bool get_np(sbase* sb, const bnode* pnd, short& np, bool bDraw) const;

		};


		struct node_unknown : node
		{
			node_unknown(const tnode* nd, bnode* parent);

			bnodetypes type() const override;
		};


		struct node_operator_equal : node
		{
			node_operator_equal(const tnode* nd, bnode* parent);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;
			bool set_caret_pos(sdraw* sd, scaret& caret) const override;

			bnodetypes type() const override;
		};


		struct node_operator_result : node
		{
			node_operator_result(const tnode* nd, bnode* parent);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;
			bool set_caret_pos(sdraw* sd, scaret& caret) const override;

			bnodetypes type() const override;

		protected:
			item::SIZETP _ini;
			item::SIZETP _end;
		};


		struct node_result : node
		{
			node_result(const tnode* nd, bnode* parent);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;
			bool set_caret_pos(sdraw* sd, scaret& caret) const override;

			item::SIZETP get_ini() const override;
			item::SIZETP get_end() const override;
			item::SIZETP get_length() const override;

			bool empty() const override;

			bnodetypes type() const override;

		protected:
			bool _bNodes;
		};


		struct node_operator_root : node
		{
			node_operator_root(const tnode* nd, bnode* parent);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;
			bool set_caret_pos(sdraw* sd, scaret& caret) const override;

			bnodetypes type() const override;

		protected:
			long rright;
		};


		struct node_operator_power : node
		{
			node_operator_power(const tnode* nd, bnode* parent);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;
			bool set_caret_pos(sdraw* sd, scaret& caret) const override;

			bnodetypes type() const override;
		};


		struct node_operator_product : node
		{
			node_operator_product(const tnode* nd, bnode* parent);

			void draw(sdraw* sd) const override;

			bnodetypes type() const override;
		};


		struct node_operator_division_inline : node
		{
			node_operator_division_inline(const tnode* nd, bnode* parent);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			bnodetypes type() const override;
		};


		struct node_operator_division : node
		{
			node_operator_division(const tnode* nd, bnode* parent);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;
			bool set_caret_pos(sdraw* sd, scaret& caret) const override;

			bnodetypes type() const override;
		};


		struct node_operator_plus : node
		{
			node_operator_plus(const tnode* nd, bnode* parent);

			void draw(sdraw* sd) const override;

			bnodetypes type() const override;
		};


		struct node_operator_minus : node
		{
			node_operator_minus(const tnode* nd, bnode* parent);

			void draw(sdraw* sd) const override;

			bnodetypes type() const override;
		};


		struct node_operator_not : node
		{
			node_operator_not(const tnode* nd, bnode* parent);

			void draw(sdraw* sd) const override;

			bnodetypes type() const override;
		};


		struct node_operator_and : node
		{
			node_operator_and(const tnode* nd, bnode* parent);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			bnodetypes type() const override;
		};


		struct node_operator_or : node
		{
			node_operator_or(const tnode* nd, bnode* parent);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			bnodetypes type() const override;
		};

	private:
#pragma endregion nodes

	public:
		line(CParserView* parent);
		~line();

		void set(CDC* pDC, int xo, int yo);
		void draw(CDC* pDC);

		CFont* font(bnodetypes type, short index);
		COLORREF color(bnodetypes type) const;
		COLORREF back_color() const;

		bool operator()(scaret& caret) const;

	protected:
		void clear();
		void _swith_expr_begin(const bnode* pnd, sdraw* sd) const;
		void _swith_expr_end(const bnode* pnd, sdraw* sd) const;

		void normalize(int xo, int yo);
		void check_error(const tnode* nd, const bnode* nnd);
		void draw_error(CDC* pDC);

		bool editing() const;

	protected:
		CParserView* _parent;
		bnode* _root;
		node_result* _result;
		bnode* _ndres;
		const bnode* _nerror;

		mstyletp _style;

		friend operation;
		friend opr_equal;
		friend opr_result;
		friend opr_product;
		friend opr_plus;
		friend opr_minus;
		friend opr_division_inline;
		friend opr_division;
		friend opr_power;
		friend opr_root;
		friend opr_not;
		friend opr_and;
		friend opr_or;
	};

public:
#pragma region operations view
	struct operation : ::operation
	{
		operation(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction = false, bool canCreateLVariable = false, bool canCreateRVariable = false);
		operation(const char* symbol, int precedence, bool leftToRight, const char* name, const char* description, tpFunc func, tpFuncCheck funcCheck, bool canCallFunction = false, bool canCreateLVariable = false, bool canCreateRVariable = false);

		virtual ~operation();

		virtual line::node* new_instance(line::bnode* parent, const tnode* nd) const;
	};


	struct opr_equal : operation
	{
		opr_equal(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction = false, bool canCreateLVariable = false, bool canCreateRVariable = false);

		line::node* new_instance(line::bnode* parent, const tnode* nd) const override;
	};


	struct opr_result : operation
	{
		opr_result(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction = false, bool canCreateLVariable = false, bool canCreateRVariable = false);

		line::node* new_instance(line::bnode* parent, const tnode* nd) const override;
	};


	struct opr_product : operation
	{
		opr_product(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction = false, bool canCreateLVariable = false, bool canCreateRVariable = false);

		line::node* new_instance(line::bnode* parent, const tnode* nd) const override;
	};


	struct opr_plus : operation
	{
		opr_plus(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction = false, bool canCreateLVariable = false, bool canCreateRVariable = false);

		line::node* new_instance(line::bnode* parent, const tnode* nd) const override;
	};


	struct opr_minus : operation
	{
		opr_minus(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction = false, bool canCreateLVariable = false, bool canCreateRVariable = false);

		line::node* new_instance(line::bnode* parent, const tnode* nd) const override;
	};


	struct opr_division_inline : operation
	{
		opr_division_inline(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction = false, bool canCreateLVariable = false, bool canCreateRVariable = false);

		line::node* new_instance(line::bnode* parent, const tnode* nd) const override;
	};


	struct opr_division : operation
	{
		opr_division(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction = false, bool canCreateLVariable = false, bool canCreateRVariable = false);

		line::node* new_instance(line::bnode* parent, const tnode* nd) const override;
	};


	struct opr_power : operation
	{
		opr_power(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction = false, bool canCreateLVariable = false, bool canCreateRVariable = false);

		line::node* new_instance(line::bnode* parent, const tnode* nd) const override;
	};


	struct opr_root : operation
	{
		opr_root(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction = false, bool canCreateLVariable = false, bool canCreateRVariable = false);

		line::node* new_instance(line::bnode* parent, const tnode* nd) const override;
	};


	struct opr_not : operation
	{
		opr_not(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction = false, bool canCreateLVariable = false, bool canCreateRVariable = false);

		line::node* new_instance(line::bnode* parent, const tnode* nd) const override;
	};


	struct opr_and : operation
	{
		opr_and(const char* symbol, int precedence, bool leftToRight, const char* name, const char* description, tpFunc func, tpFuncCheck funcCheck, bool canCallFunction = false, bool canCreateLVariable = false, bool canCreateRVariable = false);

		line::node* new_instance(line::bnode* parent, const tnode* nd) const override;
	};


	struct opr_or : operation
	{
		opr_or(const char* symbol, int precedence, bool leftToRight, const char* name, const char* description, tpFunc func, tpFuncCheck funcCheck, bool canCallFunction = false, bool canCreateLVariable = false, bool canCreateRVariable = false);

		line::node* new_instance(line::bnode* parent, const tnode* nd) const override;
	};

#pragma endregion operations view

private:

	typedef std::vector<line> vline;

	vline m_line;
	bool m_bEditing;
	UINT m_fstyle;
	std::string m_result;


	struct resource
	{
		resource(CWnd* pwnd);
		resource(resource* rsc);
		~resource();

		void clear();

		CFont* init(bool bPretty, int pointSize, LPCTSTR fontName, COLORREF backColor, COLORREF color);

		void add_style(const std::string& sname, const LOGFONT* plf, COLORREF color);
		void add_style(const std::string& sname, int pointSize, LPCTSTR fontName, COLORREF color);

		bool pretty() const;

		CFont* font(const std::string& sname, short index);
		short height(CFont* pFont) const;
		COLORREF color(const std::string& sname) const;
		CFont* font(short index);
		COLORREF color() const;
		COLORREF back_color() const;

		short max_height() const;

	protected:
		struct sfont
		{
			std::string name;
			short pointSize;
			bool italic;
			bool bold;
			CFont* pFont;
		};
		struct sfont_less
		{
			bool operator()(const sfont& left, const sfont& right) const;
		};

		typedef std::map<sfont, short, sfont_less> mfonth;
		typedef std::map<int, CFont*>  mapidxfont;
		struct style
		{
			style(CFont* pF, COLORREF col);

			CFont* font;
			mapidxfont idxfont;
			COLORREF color;
		};
		typedef std::map<std::string, style*> mstyle;

	protected:
		style* _add_style(const std::string& sname, CFont* pFont, CDC* pDC, COLORREF color);

		CFont* _find_font(const std::string& fontName, short pointFont, bool italic, bool bold) const;
		CFont* _find_font(const LOGFONT* plf) const;

		CFont* _get_font(const std::string& fontName, short pointFont, bool italic, bool bold, CDC* pDC);
		CFont* _get_font(const LOGFONT* plf, CDC* pDC);


		void update_childs(CDC* pDC, const LOGFONT* plf, mapidxfont& idxfont);

	protected:
		CWnd* _pwnd;
		bool _bPretty;
		short _idxSize;
		int _logpixelsy;
		resource* _src;
		mfonth _fonts;
		mstyle _style;
		CFont* _font;
		COLORREF _bckColor, _color;
	} m_resource;


	struct scaret
	{
		CDC* pDC;
		int height;
		CPoint pos[2];
		item::SIZETP spos[2];
	} m_caret;

	CPoint m_p0;
	bool m_bShowResult;

	CSize _scaleNum;
	CSize _scaleDen;

private:
	void draw_line(CDC* pDC, bool bCalc = false, int* x_pos = nullptr);
	void set_caret();
	const std::string& getExpression() const;

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
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in ParserView.cpp
inline CParserDoc* CParserView::GetDocument() const
   { return reinterpret_cast<CParserDoc*>(m_pDocument); }
#endif

