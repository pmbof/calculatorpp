#pragma once

#include "pmb_parser_algorithm.h"


namespace pmb
{



class calculator: public parser::algorithm<parser::value<parser::number<double> > >
{
public:
	typedef double value;
	typedef parser::number<value> number;
	typedef parser::value<number> container;
	typedef parser::algorithm<container> _base;

public:
	calculator(void);
	~calculator(void);
};



}