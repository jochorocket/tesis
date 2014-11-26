/*
 * QIEAR.h
 *
 *  Created on: 25/11/2014
 *      Author: usuario
 */

#ifndef QIEAR_H_
#define QIEAR_H_

#include <vector>
#include "QIndividual.h"

using namespace std;

#define		D		3;	//	Dimensionality of individuals
#define		I		10;	//	Number of quantum individuals

class QIEAR
{
	/*	Attributes	*/
	int						_dimension;
	vector<QIndividual<D> >	_QPop;

	/*	Methods		*/
public:
	/*	Default Constructor	*/
	QIEAR	()
		: _dimension(2)
	{
	}

	/*	One-partition domain Constructor	*/
	QIEAR	(	int 	__dimension,
				double	__centerVal,
				double	__pulseWidth
			)
		: _dimension(__dimension)
	{
		for (	int _i = 0;
					_i < I;
					++_i	)
		{
			QIndividual<D> _opdQInd (	__centerVal,
										__pulseWidth	);
			_QPop[_i] = _opQInd;
		}
	}

	/*	N-partition domain Constructor	*/
	QIEAR	(	int 	__dimension,
				double	__centerVal,
				double	__pulseWidth,
				int		__noPartitions
			)
		: _dimension(__dimension)
	{
		for (	int _i = 0;
					_i < I;
					++_i	)
		{
			QIndividual<D> _opdQInd (	__centerVal,
										__pulseWidth,
										__noPartitions	);
			_QPop[_i] = _opQInd;
		}
	}

	/*	Algorithm	*/
	int _execQIEAR	()
	{

	}

};



#endif /* QIEAR_H_ */
