
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

		struct bnode;
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
		struct bnode : CRect
		{
		protected:
			bnode();
		public:
			virtual ~bnode();

			void clear();
			virtual void set(sset* ss) = 0;
			virtual void draw(sdraw* sd) const = 0;

			virtual bool empty() const = 0;

			virtual bnodetypes type() const = 0;

			virtual bool parentheses() const = 0;
			virtual short nparentheses() const = 0;

			bool is_left_parentheses(const bnode* parent) const;

			bnode& operator= (const CRect& right);

			bnode* node_mright();

			void rec_move(int dx, int dy);
			CRect rec_rect() const;
			void max_rect(CRect& r, const CRect& r2) const;

			void check_error(sset* ss);
			void end(sset* ss);

		public:
			int _middle;

		protected:
			bnode* _left;
			bnode* _right;
		};



		struct node : bnode
		{
		protected:
			node();
			node(const tnode* nd);

		public:
			virtual ~node();

			static node* new_instance(const tnode* nd);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			bool empty() const override;

			bool parentheses() const override;
			short nparentheses() const override;

		protected:
			item::SIZETP _ini;
			item::SIZETP _end;
		};


		struct node_space : node
		{
			node_space(const tnode* nd);

			bnodetypes type() const override;
		};


		struct node_alpha : node
		{
			node_alpha(const tnode* nd);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			bnodetypes type() const override;
		};


		struct node_function : node
		{
			node_function(const tnode* nd);

			bnodetypes type() const override;
		};


		struct node_buildin_function : node
		{
			node_buildin_function(const tnode* nd);

			bnodetypes type() const override;
		};


		struct node_number : node
		{
			node_number(const tnode* nd);

			bnodetypes type() const override;
		};


		struct node_string : node
		{
			node_string(const tnode* nd);

			bnodetypes type() const override;
		};


		struct node_list : node
		{
			node_list(const tnode* nd);

			bnodetypes type() const override;
		};


		struct node_parentheses : node
		{
			node_parentheses(const tnode* nd);

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
			node_unknown(const tnode* nd);

			bnodetypes type() const override;
		};


		struct node_operator_equal : node
		{
			node_operator_equal(const tnode* nd);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			bnodetypes type() const override;
		};


		struct node_operator_result : node
		{
			node_operator_result(const tnode* nd);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			bnodetypes type() const override;
		};


		struct node_result : node
		{
			node_result(const tnode* nd);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			bool empty() const override;

			bnodetypes type() const override;
		protected:
			bool _bNodes;
		};


		struct node_operator_root : node
		{
			node_operator_root(const tnode* nd);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			bnodetypes type() const override;

		protected:
			long rright;
		};


		struct node_operator_power : node
		{
			node_operator_power(const tnode* nd);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			bnodetypes type() const override;
		};


		struct node_operator_product : node
		{
			node_operator_product(const tnode* nd);

			void draw(sdraw* sd) const override;

			bnodetypes type() const override;
		};


		struct node_operator_division_inline : node
		{
			node_operator_division_inline(const tnode* nd);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			bnodetypes type() const override;
		};


		struct node_operator_division : node
		{
			node_operator_division(const tnode* nd);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			bnodetypes type() const override;
		};


		struct node_operator_plus : node
		{
			node_operator_plus(const tnode* nd);

			void draw(sdraw* sd) const override;

			bnodetypes type() const override;
		};


		struct node_operator_minus : node
		{
			node_operator_minus(const tnode* nd);

			void draw(sdraw* sd) const override;

			bnodetypes type() const override;
		};
#pragma endregion nodes

	public:
		line(CParserView* parent);
		~line();

		void set(CDC* pDC, int xo, int yo);
		void draw(CDC* pDC);

		CFont* font(bnodetypes type, short index);
		COLORREF color(bnodetypes type) const;
		COLORREF back_color() const;

	protected:
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


	line m_line;
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

		CFont* init(int pointSize, LPCTSTR fontName, COLORREF backColor, COLORREF color);

		void add_style(const std::string& sname, const LOGFONT* plf, COLORREF color);
		void add_style(const std::string& sname, int pointSize, LPCTSTR fontName, COLORREF color);

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
		short _idxSize;
		int _logpixelsy;
		resource* _src;
		mfonth _fonts;
		mstyle _style;
		CFont* _font;
		COLORREF _bckColor, _color;
	} m_resource;


	struct style
	{
		int maxHeight;
		int lineHeight;
		CPoint caretPos;
		CPoint caretPos0;
		int caret[2];
	} m_style;

	CPoint m_p0;
	bool m_bShowResult;


private:
	void drawNode(CDC* pDC, const item* nd, const CString& expr, bool bError);

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

