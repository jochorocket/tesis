/*
 * EDIndividual.h
 *
 *  Created on: 02/10/2014
 *      Author: usuario
 */

#ifndef EDINDIVIDUAL_H_
#define EDINDIVIDUAL_H_

#include "ExtraFunctions.h"
#include <vector>

using namespace std;

/*	F: mutation rate control parameter
 * 	Must be between 0 and 2				*/
#define	F			0.3

class EDIndividual
{
	/*	Attributes	*/
	vector<double> 	_variables;
	int				_noVariables;
	int				_age;

	/*	Methods		*/
public:
	//	Default constructor
	EDIndividual(){}

	//	Main constructor
	EDIndividual(	int 			__noVariables,
					int 			__age,
					vector<double>	__minData,
					vector<double>	__maxData)
		:	_noVariables(__noVariables)
		, 	_age(__age)
	{
		_variables.resize(_noVariables);
		for (int _m=0; _m<_noVariables; ++_m)
			_variables[_m] =
					__minData[_m] + _EFrandDouble()*(__maxData[_m] - __minData[_m]);
	}

	//	Mutation Constructor: To generate noisy random vectors
	EDIndividual(	EDIndividual&	__indA,
					EDIndividual&	__indB,
					EDIndividual&	__indC)
		:	_noVariables(__indA._getNoVariables())
		, 	_age(__indA._getAge())
	{
		_variables.resize(_noVariables);
		for (int _m=0; _m<_noVariables; ++_m)
			_variables[_m] =
					__indC[_m] + (__indA[_m] - __indB[_m])*F;
	}

	int _getNoVariables()
	{
		return _noVariables;
	}

	int _getAge()
	{
		return _age;
	}

	void _increaseAge()
	{
		++_age;
	}

	double operator[] (int _index) const
	{
		return _variables[_index];
	}
};



#endif /* EDINDIVIDUAL_H_ */
