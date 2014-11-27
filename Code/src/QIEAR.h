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
#include <iostream>
#include "ExtraFunctions.h"
#include "QIndividual.h"

using namespace std;

#define		I		4;	//	Number of quantum individuals
#define		F		0.3	//	Constant for individual recombination

template <int D>	//	Dimensionality of individuals
class QIEAR
{
	/*	Attributes	*/
	const static int		_I	= 	I;
	vector<QIndividual<D> >	_QPop;

	/* 	Pointer Methods	*/
	double	(*_fitnessFunc)(vector<double>);

	/*	Methods		*/
public:
	/*	Default Constructor	*/
	QIEAR	()
	{
	}

	/*	One-partition domain Constructor	*/
	QIEAR	(	double	__centerVal,
				double	__pulseWidth,
				double	(*__fitnessFunc)(vector<double>)
			)
		:	_fitnessFunc(__fitnessFunc)
	{
		_QPop.resize(_I);
		int	_i;
		for (	_i = 0;
				_i < _I;
				++_i	)
		{
			QIndividual<D> _opdQInd (	__centerVal,
										__pulseWidth	);
			_QPop[_i] = _opdQInd;
		}
	}

	/*	N-partition domain Constructor	*/
	QIEAR	(	double	__centerVal,
				double	__pulseWidth,
				double	(*__fitnessFunc)(vector<double>),
				int		__noPartitions
			)
		: 	_fitnessFunc(__fitnessFunc)
	{
		_QPop.resize(_I);
		for (	int _i = 0;
					_i < _I;
					++_i	)
		{
			QIndividual<D> _opdQInd (	__centerVal,
										__pulseWidth,
										__noPartitions	);
			_QPop[_i] = _opdQInd;
		}
	}

	/*	Algorithm	*/
	vector<vector<double> > _genIndQIEAR ()
	{
		vector<vector<double> > _cInds;
		_cInds.resize(_I);
		for (	int _i = 0;
					_i < _I;
					++_i)
		{
			_cInds[_i] = _QPop[_i]._generateCInd();
		}
		return _cInds;
	}

	void _execQIEAR	(	int	__genNumber
					,	bool __maxMin )
	{
		vector<vector<double> > _cInds = _genIndQIEAR(), _bInds;
		_bInds.resize(_I);
		map<double,vector<double> > _bestInds;
		//Generate first generation of individuals
		for (	int _i = 0;
					_i < _I;
					++_i	)
		{
			_bInds[_i] = _cInds[_i];
		}
		_cInds.clear();
		for (	int _t = 0;
					_t < __genNumber;
					++_t)
		{
			cout<<_t<<endl;
			//	Generate new generation of classical individuals
			double _avgFitness = 0;
			_cInds = _genIndQIEAR();
			for ( unsigned int 	_f = 0;
								_f < _I;
								++_f )
			{
				//	Recombine new and previous classical individuals
				_cInds[_f] = (_EFrand() < F)
								? _bInds[_f]:_cInds[_f];

				//	Evaluate both classical individual arrays
				_avgFitness += _fitnessFunc(_bInds[_f]);
				_bestInds[_fitnessFunc(_cInds[_f])] = _cInds[_f];
				_bestInds[_fitnessFunc(_bInds[_f])] = _bInds[_f];
			}
			_avgFitness /= _I;

			//	Copy the I best individuals to _bInds
			//	Depends on __maxMin if the goal is to maximize or to minimize
			if (__maxMin)
			{
				map<double,vector<double> >::reverse_iterator _bestBInds
									= _bestInds.rbegin();
				for ( unsigned int 	_k = 0;
									_k < _I;
									++_k)
				{
					cout<<_bestBInds->first<<" ";
					_bInds[_k] = _bestBInds->second;
					++_bestBInds;
				}
			}
			else
			{
				map<double,vector<double> >::iterator _bestBInds
										= _bestInds.begin();
				for ( unsigned int 	_k = 0;
									_k < _I;
									++_k)
				{
					_bInds[_k] = (*_bestBInds).second;
					++_bestBInds;
					cout<<_bestBInds->first<<" ";
				}
			}
			cout<<endl;

			//	Update quantum individuals
			int _totalImproved = 0;
			for (	unsigned int	_l = 0;
									_l < _I;
									++_l	)
			{
				//	Update by altering centers
				_QPop[_l]._updateQInd(_bInds[_l]);
				//	Sum-up percentage of improved classic individuals
				if (_avgFitness < _fitnessFunc(_bInds[_l]) )
				{
					++_totalImproved;
				}
			}
			//	Calculate 20% of total improvement
			if (__maxMin && _totalImproved > _I/5)
			{
				//	Update by altering pulsewidths
				for (	unsigned int 	_m = 0;
										_m < _I;
										++_m)
					_QPop[_m]._updateQInd(true);
			}
			else if (_totalImproved < _I*4/5)
			{
				//	Update by altering pulsewidths
				for (	unsigned int 	_m = 0;
										_m < _I;
										++_m)
					_QPop[_m]._updateQInd(true);
			}
			else
			{
				//	Update by altering pulsewidths
				for (	unsigned int 	_m = 0;
										_m < _I;
										++_m)
					_QPop[_m]._updateQInd(false);
			}
		}
	}

};



#endif /* QIEAR_H_ */
