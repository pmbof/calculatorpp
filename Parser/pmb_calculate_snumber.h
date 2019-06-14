#pragma once
#include "pmb_calculate_magnitude.h"


namespace pmb
{
namespace calculate
{



template<class _TYPE, typename _INT, typename _CHAR, typename _SZSTR>
struct snumber: pmb::calculate::units::magnitude<_TYPE, _INT, _CHAR, _SZSTR>
{
	typedef unsigned long vidx;
	typedef unsigned short aidx;

	enum tpData : char {
		tpNoSet,
		tpBoolean,
		tpVectorBoolean,
		tpArrayBoolean,
		tpReal,
		tpImaginary,
		tpComplex,
		tpVectorReal,
		tpVectorImaginary,
		tpVectorComplex,
		tpArrayReal,
		tpArrayImaginary,
		tpArrayComplex,
		tpTensorReal,
		tpTensorReal,
		tpTensorComplex
	};


	snumber();
	explicit snumber(const _TYPE& src, bool imaginary = false);
	explicit snumber(const _MyT* src);
	explicit snumber(const _CHAR* str, const _SZSTR& len);
	explicit snumber(const dimension* dim);
	explicit snumber(const _TYPE& n, const unit& u);

	explicit snumber(_TYPE& real, _TYPE& imag);
	explicit snumber(_TYPE* vector, vidx size, bool imaginary);
	explicit snumber(_TYPE* vector, vidx size);
	explicit snumber(_TYPE* array, aidx rows, aidx cols, bool imaginary);
	explicit snumber(_TYPE* array, aidx rows, aidx cols);

	~snumber();
	void clear();
	bool copy(const snumber& right, const _TYPE& factor);

	// Operators:
	void positive(const snumber& right);
	void negative(const snumber& right);

	void factorial(const snumber& right);
	void multiplication(const snumber& left, const snumber& right);
	void division(const snumber& left, const snumber& right);
	void modulo(const snumber& left, const snumber& right);

	void addition(const snumber& left, const snumber& right);
	void substraction(const snumber& left, const snumber& right);

	void exponentiation(const snumber& left, const snumber& right);
	void root(const snumber& left, const snumber& right);

	void assignation(const snumber& left, const snumber& right);


	// check:
	bool zero() const;
	bool positive() const;
	bool negative() const;
	bool integer() const;
	bool dimensionless() const;
	bool one_dimension() const;
	bool scalar() const;
	bool real() const;
	bool imaginary() const;
	bool complex() const;
	bool vector() const;
	bool matrix() const;

	vidx dimension() const;
	aidx rows() const;
	aidx cols() const;


protected:
	void _product_lboolean(const snumber& left, const snumber& right);
	void _product_lvectorboolean(const snumber& left, const snumber& right);
	void _product_larrayboolean(const snumber& left, const snumber& right);

	void _product_lreal(const snumber& left, const snumber& right);
	void _product_limaginary(const snumber& left, const snumber& right);
	void _product_lcomplex(const snumber& left, const snumber& right);
	
	void _product_lvectorreal(const snumber& left, const snumber& right);
	void _product_lvectorimaginary(const snumber& left, const snumber& right);
	void _product_lvectorcomplex(const snumber& left, const snumber& right);
	
	void _product_larrayreal(const snumber& left, const snumber& right);
	void _product_larrayimaginary(const snumber& left, const snumber& right);
	void _product_larraycomplex(const snumber& left, const snumber& right);





	tpData _type;
	_TYPE* _data;
};




}
}

#include "pmb_calculate_snumber.hpp"