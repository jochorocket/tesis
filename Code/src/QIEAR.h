/*
 * QIEAR.h
 *
 *  Created on: 25/11/2014
 *      Author: usuario
 */

#ifndef QIEAR_H_
#define QIEAR_H_

#include <vector>
#include <map>
#include "QIndividual.h"

using namespace std;

#define		D		3;	//	Dimensionality of individuals
#define		I		10;	//	Number of quantum individuals

class QIEAR
{
	/*	Attributes	*/
	const static int			_D	=	D;
	const static int			_I	= 	I;
	int							_dimension;
	vector<QIndividual<_D> >	_QPop;

	/* 	Pointer Methods	*/
	/*	double	(EDAlgorithm::*_fitnessFunc)(VEDInd);	*/
	double	(*_fitnessFunc)(vector<double>);

	/*	Methods		*/
public:
	/*	Default Constructor	*/
	QIEAR	()
		:	_dimension(2)
	{
	}

	/*	One-partition domain Constructor	*/
	QIEAR	(	int 	__dimension,
				double	__centerVal,
				double	__pulseWidth,
				double	(*__fitnessFunc)(vector<double>)
			)
		:	_dimension(__dimension)
		,	_fitnessFunc(__fitnessFunc)
	{
		int	_i;
		for (	_i = 0;
				_i < _I;
				++_i	)
		{
			QIndividual<_D> _opdQInd (	__centerVal,
										__pulseWidth	);
			_QPop[_i] = _opdQInd;
		}
	}

	/*	N-partition domain Constructor	*/
	QIEAR	(	int 	__dimension,
				double	__centerVal,
				double	__pulseWidth,
				double	(*__fitnessFunc)(vector<double>),
				int		__noPartitions
			)
		: 	_dimension(__dimension)
		,	_fitnessFunc(__fitnessFunc)
	{
		for (	int _i = 0;
					_i < _I;
					++_i	)
		{
			QIndividual<_D> _opdQInd (	__centerVal,
										__pulseWidth,
										__noPartitions	);
			_QPop[_i] = _opdQInd;
		}
	}

	/*	Algorithm	*/
	int _execQIEAR	(	int	__T	)
	{
		map<double,vector<double> > _bestInd;

		for (	int _t = 0;
					_t < __T;
					++_t)
		{

		}
	}

};



#endif /* QIEAR_H_ */
