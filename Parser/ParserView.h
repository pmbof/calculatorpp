
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
			bndOprMinus
		};

	public:
		struct bnode;

	private:
		typedef std::list<bnode*> lbnode;
		typedef std::map<line::bnodetypes, std::string> mstyletp;

		struct sset
		{
			line* pline;
			const tnode* nd;
			bnode* pnd;
			CDC* pDC;
			const char* pstr;
			bool bEditing;
			lbnode parents;
			short index;
		};

		struct sdraw
		{
			line* pline;
			const bnode* pnd;
			CDC* pDC;
			const char* pstr;
			bool bEditing;
			const char* postr;
			short index;

			void begin_expr(const bnode* pnd);
			void end_expr(const bnode* pnd);
		};

	
#pragma region nodes
	public:
		struct bnode : CRect
		{
		protected:
			bnode(bnode* parent);

		public:
			virtual ~bnode();

			virtual void print_line(pmb::log_type tplg, const char* text) const;
			void print() const;
			bool debug_check() const;
			void debug_check_all() const;

			virtual void set(sset* ss) = 0;
			virtual void draw(sdraw* sd) const = 0;

			virtual bool empty() const = 0;

			virtual bnodetypes type() const = 0;

			virtual bool parentheses() const = 0;
			virtual short nparentheses() const = 0;

			virtual item::SIZETP get_ini() const = 0;
			virtual item::SIZETP get_end() const = 0;

			bool is_left_parentheses(const bnode* parent) const;

			bnode& operator= (const CRect& right);

			bnode* node_mright();

			void rect_move(int dx, int dy);
			void _rect_move(int dx, int dy);
			CRect rect() const;
			void max_rect(CRect& r, const CRect& r2) const;

			void check_error(sset* ss);
			void end(sset* ss);

			const bnode* get_first() const;
			const bnode* get_root() const;
			const bnode* get_first_left() const;
			const bnode* get_next() const;

			bnode* get_first();
			bnode* get_root();
			bnode* get_first_left();
			bnode* get_next();

		public:
			int _middle;

		protected:
			bnode* _parent;
			bnode* _left;
			bnode* _right;
		};



		struct node : bnode
		{
		protected:
			node(bnode* parent);
			node(bnode* parent, const tnode* nd);

		public:
			virtual ~node();

			void print_line(pmb::log_type tplg, const char* text) const override;

			static node* new_instance(bnode** ndLR, bnode* parent, const tnode* nd);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			bool empty() const override;

			bool parentheses() const override;
			short nparentheses() const override;

			item::SIZETP get_ini() const override;
			item::SIZETP get_end() const override;

		protected:
			item::SIZETP _ini;
			item::SIZETP _end;
		};


		struct node_space : node
		{
			node_space(bnode* parent, const tnode* nd);

			bnodetypes type() const override;
		};


		struct node_alpha : node
		{
			node_alpha(bnode* parent, const tnode* nd);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			bnodetypes type() const override;
		};


		struct node_function : node
		{
			node_function(bnode* parent, const tnode* nd);

			bnodetypes type() const override;
		};


		struct node_buildin_function : node
		{
			node_buildin_function(bnode* parent, const tnode* nd);

			bnodetypes type() const override;
		};


		struct node_number : node
		{
			node_number(bnode* parent, const tnode* nd);

			bnodetypes type() const override;
		};


		struct node_string : node
		{
			node_string(bnode* parent, const tnode* nd);

			bnodetypes type() const override;
		};


		struct node_list : node
		{
			node_list(bnode* parent, const tnode* nd);

			bnodetypes type() const override;
		};


		struct node_parentheses : node
		{
			node_parentheses(bnode* parent, const tnode* nd);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			bool parentheses() const override;
			short nparentheses() const override;

			bnodetypes type() const override;

		protected:
			short _nparentheses;
		};


		struct node_unknown : node
		{
			node_unknown(bnode* parent, const tnode* nd);

			bnodetypes type() const override;
		};


		struct node_operator_equal : node
		{
			node_operator_equal(bnode* parent, const tnode* nd);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			bnodetypes type() const override;
		};


		struct node_operator_result : node
		{
			node_operator_result(bnode* parent, const tnode* nd);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			bnodetypes type() const override;
		};


		struct node_result : node
		{
			node_result(bnode* parent, const tnode* nd);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			bool empty() const override;

			bnodetypes type() const override;
		protected:
			bool _bNodes;
		};


		struct node_operator_root : node
		{
			node_operator_root(bnode* parent, const tnode* nd);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			bnodetypes type() const override;

		protected:
			long rright;
		};


		struct node_operator_power : node
		{
			node_operator_power(bnode* parent, const tnode* nd);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			bnodetypes type() const override;
		};


		struct node_operator_product : node
		{
			node_operator_product(bnode* parent, const tnode* nd);

			void draw(sdraw* sd) const override;

			bnodetypes type() const override;
		};


		struct node_operator_division_inline : node
		{
			node_operator_division_inline(bnode* parent, const tnode* nd);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			bnodetypes type() const override;
		};


		struct node_operator_division : node
		{
			node_operator_division(bnode* parent, const tnode* nd);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			bnodetypes type() const override;
		};


		struct node_operator_plus : node
		{
			node_operator_plus(bnode* parent, const tnode* nd);

			void draw(sdraw* sd) const override;

			bnodetypes type() const override;
		};


		struct node_operator_minus : node
		{
			node_operator_minus(bnode* parent, const tnode* nd);

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

	protected:
		CParserView* _parent;
		bnode* _root;
		node_result* _result;
		bnode* _ndres;
		const bnode* _nerror;

		mstyletp _style;
	};

public:
	struct operation : ::operation
	{
		operation(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction = false, bool canCreateLVariable = false, bool canCreateRVariable = false)
			: ::operation(symbol, precedence, leftToRight, binary, name, description, func, canCallFunction, canCreateLVariable, canCreateRVariable) {}

		virtual ~operation() {}

		virtual line::node* new_instance(line::bnode* parent, const tnode* nd) const {
			return new line::node_unknown(parent, nd);
		}
	};


	struct opr_equal : operation
	{
		opr_equal(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction = false, bool canCreateLVariable = false, bool canCreateRVariable = false)
			: operation(symbol, precedence, leftToRight, binary, name, description, func, canCallFunction, canCreateLVariable, canCreateRVariable) {}

		line::node* new_instance(line::bnode* parent, const tnode* nd) const override {
			return new line::node_operator_equal(parent, nd);
		}
	};


	struct opr_result : operation
	{
		opr_result(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction = false, bool canCreateLVariable = false, bool canCreateRVariable = false)
			: operation(symbol, precedence, leftToRight, binary, name, description, func, canCallFunction, canCreateLVariable, canCreateRVariable) {}

		line::node* new_instance(line::bnode* parent, const tnode* nd) const override {
			return new line::node_result(parent, nd);
		}
	};


	struct opr_product : operation
	{
		opr_product(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction = false, bool canCreateLVariable = false, bool canCreateRVariable = false)
			: operation(symbol, precedence, leftToRight, binary, name, description, func, canCallFunction, canCreateLVariable, canCreateRVariable) {}

		line::node* new_instance(line::bnode* parent, const tnode* nd) const override {
			return new line::node_operator_product(parent, nd);
		}
	};


	struct opr_plus : operation
	{
		opr_plus(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction = false, bool canCreateLVariable = false, bool canCreateRVariable = false)
			: operation(symbol, precedence, leftToRight, binary, name, description, func, canCallFunction, canCreateLVariable, canCreateRVariable) {}

		line::node* new_instance(line::bnode* parent, const tnode* nd) const override {
			return new line::node_operator_plus(parent, nd);
		}
	};


	struct opr_minus : operation
	{
		opr_minus(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction = false, bool canCreateLVariable = false, bool canCreateRVariable = false)
			: operation(symbol, precedence, leftToRight, binary, name, description, func, canCallFunction, canCreateLVariable, canCreateRVariable) {}

		line::node* new_instance(line::bnode* parent, const tnode* nd) const override {
			return new line::node_operator_minus(parent, nd);
		}
	};


	struct opr_division_inline : operation
	{
		opr_division_inline(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction = false, bool canCreateLVariable = false, bool canCreateRVariable = false)
			: operation(symbol, precedence, leftToRight, binary, name, description, func, canCallFunction, canCreateLVariable, canCreateRVariable) {}

		line::node* new_instance(line::bnode* parent, const tnode* nd) const override {
			return new line::node_operator_division_inline(parent, nd);
		}
	};


	struct opr_division : operation
	{
		opr_division(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction = false, bool canCreateLVariable = false, bool canCreateRVariable = false)
			: operation(symbol, precedence, leftToRight, binary, name, description, func, canCallFunction, canCreateLVariable, canCreateRVariable) {}

		line::node* new_instance(line::bnode* parent, const tnode* nd) const override {
			return new line::node_operator_division(parent, nd);
		}
	};


	struct opr_power : operation
	{
		opr_power(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction = false, bool canCreateLVariable = false, bool canCreateRVariable = false)
			: operation(symbol, precedence, leftToRight, binary, name, description, func, canCallFunction, canCreateLVariable, canCreateRVariable) {}

		line::node* new_instance(line::bnode* parent, const tnode* nd) const override {
			return new line::node_operator_power(parent, nd);
		}
	};


	struct opr_root : operation
	{
		opr_root(const char* symbol, int precedence, bool leftToRight, bool binary, const char* name, const char* description, tpFunc func, bool canCallFunction = false, bool canCreateLVariable = false, bool canCreateRVariable = false)
			: operation(symbol, precedence, leftToRight, binary, name, description, func, canCallFunction, canCreateLVariable, canCreateRVariable) {}

		line::node* new_instance(line::bnode* parent, const tnode* nd) const override {
			return new line::node_operator_root(parent, nd);
		}
	};
private:

	typedef std::vector<line> vline;

	vline m_line;
	bool m_bEditing;
	UINT m_fstyle;
	std::string m_expr;
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
		int height;
		CPoint pos[2];
		item::SIZETP spos[2];
	} m_caret;

	CPoint m_p0;
	bool m_bShowResult;


private:
	void draw_line(CDC* pDC, bool bCalc = false, int* x_pos = nullptr);

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

