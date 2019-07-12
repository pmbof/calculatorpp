
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
		struct bnode;
		typedef std::list<bnode*> lbnode;

		struct sset
		{
			line* pline;
			const tnode* nd;
			bnode* pnd;
			CDC* pDC;
			const char* pstr;
			bool bEditing;
			lbnode parents;
		};

		struct sdraw
		{
			line* pline;
			const bnode* pnd;
			CDC* pDC;
			const char* pstr;
			bool bEditing;
			const char* postr;

			void begin_expr(const bnode* pnd);
			void end_expr(const bnode* pnd);
		};


		struct bnode : CRect
		{
		protected:
			bnode();
		public:
			virtual ~bnode();

			void clear();
			virtual void set(sset* ss) = 0;
			virtual void draw(sdraw* sd) const = 0;

			virtual short font() const = 0;
			virtual short color() const = 0;

			virtual bool empty() const = 0;

			virtual bool parenthesis() const = 0;
			virtual short nparenthesis() const = 0;

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

			bool parenthesis() const override;
			short nparenthesis() const override;

		protected:
			item::SIZETP _ini;
			item::SIZETP _end;
		};


		struct node_space : node
		{
			node_space(const tnode* nd);

			short font() const override;
			short color() const override;
		};


		struct node_alpha : node
		{
			node_alpha(const tnode* nd);

			short font() const override;
			short color() const override;
		};


		struct node_function : node
		{
			node_function(const tnode* nd);

			short font() const override;
			short color() const override;
		};


		struct node_buildin_function : node
		{
			node_buildin_function(const tnode* nd);

			short font() const override;
			short color() const override;
		};


		struct node_number : node
		{
			node_number(const tnode* nd);

			short font() const override;
			short color() const override;
		};


		struct node_string : node
		{
			node_string(const tnode* nd);

			short font() const override;
			short color() const override;
		};


		struct node_list : node
		{
			node_list(const tnode* nd);

			short font() const override;
			short color() const override;
		};


		struct node_parenthesis : node
		{
			node_parenthesis(const tnode* nd);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			bool parenthesis() const override;
			short nparenthesis() const override;

			short font() const override;
			short color() const override;

		protected:
			short _nparenthesis;
		};


		struct node_unknown : node
		{
			node_unknown(const tnode* nd);

			short font() const override;
			short color() const override;
		};


		struct node_operator_equal : node
		{
			node_operator_equal(const tnode* nd);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			short font() const override;
			short color() const override;
		};


		struct node_operator_result : node
		{
			node_operator_result(const tnode* nd);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			short font() const override;
			short color() const override;
		};


		struct node_result : node
		{
			node_result(const tnode* nd);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			bool empty() const override;

			short font() const override;
			short color() const override;

		protected:
			bool _bNodes;
		};


		struct node_operator_root : node
		{
			node_operator_root(const tnode* nd);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			short font() const override;
			short color() const override;

		protected:
			long rright;
		};


		struct node_operator_power : node
		{
			node_operator_power(const tnode* nd);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			short font() const override;
			short color() const override;
		};


		struct node_operator_product : node
		{
			node_operator_product(const tnode* nd);

			void draw(sdraw* sd) const override;

			short font() const override;
			short color() const override;
		};


		struct node_operator_division_inline : node
		{
			node_operator_division_inline(const tnode* nd);

			void draw(sdraw* sd) const override;

			short font() const override;
			short color() const override;
		};


		struct node_operator_division : node
		{
			node_operator_division(const tnode* nd);

			void set(sset* ss) override;
			void draw(sdraw* sd) const override;

			short font() const override;
			short color() const override;
		};


		struct node_operator_plus : node
		{
			node_operator_plus(const tnode* nd);

			void draw(sdraw* sd) const override;

			short font() const override;
			short color() const override;
		};


		struct node_operator_minus : node
		{
			node_operator_minus(const tnode* nd);

			void draw(sdraw* sd) const override;

			short font() const override;
			short color() const override;
		};

	public:
		line(CParserView* parent);
		~line();

		void set(CDC* pDC, int xo, int yo);
		void draw(CDC* pDC);

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
	};


	line m_line;
	bool m_bEditing;
	UINT m_fstyle;
	std::string m_expr;
	std::string m_result;

	struct style
	{
		CFont font[9];
		int   height[9];

		COLORREF color[9];
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

