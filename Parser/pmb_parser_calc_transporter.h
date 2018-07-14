#pragma once


namespace pmb
{
namespace parser
{
namespace calc
{






template <class _VALUE, typename _NREF>
class transporter
{
public:
	typedef typename transporter<_VALUE, _NREF> thisc;
	typedef _VALUE tpvalue;
	typedef _VALUE* ptr_value;
	typedef _NREF nref;

public:
	transporter();
	explicit transporter(bool varible);

	transporter(ptr_value pvalue);
	transporter(const thisc& rcopy);

	~transporter();

	thisc& operator =(thisc& right);
	thisc& operator =(ptr_value right);

	bool hasPlace() const;
	bool isNull() const;
	bool isNotNull() const;
	bool operator !() const;
	
	ptr_value operator *();
	ptr_value operator *() const;
	ptr_value operator ->();
	ptr_value operator ->() const;

	void clear();
	ptr_value release();

private:
	struct sref_val
	{
		bool variable;
		nref nref;
		ptr_value pvalue;
	}*_prv;
};





template <class _TVALUE, typename _NARGS>
class transporter_args
{
public:
	typedef typename transporter_args<_TVALUE, _NARGS> thisc;
	typedef typename _TVALUE transporter;
	typedef typename _NARGS nargs;
	typedef typename transporter::tpvalue tpvalue;
	typedef typename transporter::ptr_value ptr_tpvalue; 

public:
	transporter_args();
	~transporter_args();

	transporter_args& operator=(const transporter_args& right);

	void placeFor(nargs argsN);
	void placeForResult();
	transporter& operator[](nargs argN);
	transporter& left();
	transporter& right();
	void moveToLeft();
	void moveToRight();
	void removeLefts();
	void removeRights();
	transporter& result();
	nargs size() const;
	nargs nArgs() const;
	bool haveLeft() const;

	bool operator !() const;

	void join_l2r(transporter_args& targs, bool bOnlyFirst, bool bOnlyLast);
	void join_r2l(transporter_args& targs, bool bOnlyLast, bool bOnlyFirst);

	void add_back();
	void add_back(transporter& arg);
	void add_front();

	void clean();
protected:
	void _init();

private:
	struct node
	{
		node();

		transporter t;
		node* pnext;
	}
		* _root,
		* _last;
	nargs _size;
	transporter _result;
};







}
}
}