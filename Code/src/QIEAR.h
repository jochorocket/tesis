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

#define		I		3;	//	Number of quantum individuals
#define		CI		5;	//	Number of classical individuals per generation
#define		F		0.3	//	Constant for individual recombination

template <int D>	//	Dimensionality of individuals
class QIEAR
{
	/*	Attributes	*/
	const static int		_I	= 	I;
	const static int		_CI =	CI;
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
			QIndividual<D> _opdQInd(	__centerVal - __pulseWidth/2				// start
											+ _i * __pulseWidth/__noPartitions		// minipulse width
											+  __pulseWidth / (__noPartitions*2),	// half missing
										__pulseWidth/__noPartitions	);
			_QPop[_i] = _opdQInd;
		}
	}

	/*	Algorithms	*/
	/*	Entangled crossover operator */
	void _entangledCrossover(bool __maxMin)
	{
		//	Evaluate best and worst individual - producing qInds
		map<double, int> _qIndEvaluation;
		for (unsigned int 	_m = 0;
							_m < _I;
							++_m)
		{
			_qIndEvaluation[_QPop[_m]._evalQInd()] = _m;
		}
		//	Apply it
		if (__maxMin)
		{
			_QPop[(_qIndEvaluation.begin())->second]._updateQInd
				(_QPop[(_qIndEvaluation.rbegin())->second]);
		}
		else
		{
			_QPop[(_qIndEvaluation.rbegin())->second]._updateQInd
				(_QPop[(_qIndEvaluation.begin())->second]);
		}
	}

	/*	Classic Algorithm*/
	vector<vector<double> > _genIndQIEAR ()
	{
		vector<vector<double> > _cInds;
		_cInds.resize(_I*_CI);
		for (	int _i = 0;
					_i < _I;
					++_i)
		{
			for (	int _ci = 0;
						_ci < _CI;
						++_ci)
				_cInds[_CI*_i + _ci] = _QPop[_i]._generateCInd(_fitnessFunc);
		}
		return _cInds;
	}

	void _execQIEAR	(	int	__genNumber
					,	bool __maxMin
					,	int	__crossoverIndex)
	{
		vector<vector<double> > _cInds = _genIndQIEAR(), _bInds;
		_bInds.resize(_I*_CI);
		map<double,vector<double> > _bestInds;
		//Generate first generation of individuals
		for (	int _ci = 0;
					_ci < _I*_CI;
					++_ci	)
		{
			_bInds[_ci] = _cInds[_ci];
		}
		_cInds.clear();
		for (	int _t = 0;
					_t < __genNumber;
					++_t)
		{
			cout<<_t<<'\t';
			//	Generate new generation of classical individuals
			double _avgFitness = 0;
			_cInds = _genIndQIEAR();
			for ( unsigned int 	_f = 0;
								_f < _I*_CI;
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
			_avgFitness /= (_I*_CI);

		
			map<double, vector<double> >::iterator prueba
				= _bestInds.begin();
			for (int ww = 0; ww != 6 ; ++ww)
			{
				cout << prueba->first << " ";
				++prueba;
			}
			cout << endl;
			

			//	Copy the I best individuals to _bInds
			//	Depends on __maxMin if the goal is to maximize or to minimize
			if (__maxMin)
			{
				map<double,vector<double> >::reverse_iterator _bestBInds
									= _bestInds.rbegin();
				for ( unsigned int 	_k = 0;
									_k < _I*_CI;
									++_k)
				{
					//cout<<_bestBInds->first<<" ";
					_bInds[_k] = _bestBInds->second;
					++_bestBInds;
				}
			}
			else
			{
				map<double,vector<double> >::iterator _bestBInds
										= _bestInds.begin();
				for ( unsigned int 	_k = 0;
									_k < _I*_CI;
									++_k)
				{
					//cout << _bestBInds->first << " ";
					_bInds[_k] = _bestBInds->second;
					++_bestBInds;
				}
			}
			_bestInds.clear();
			//cout<<endl;

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

			//	Apply entanglement crossover operator
			//	This is only done every five iterations
			if (__crossoverIndex > 0)
			{
				if ((_t+1) % __crossoverIndex == 0) _entangledCrossover(__maxMin);
			}
		}
	}

	/*	Universe-compound algorithm	*/
	vector<vector<double> > _genIndUQIEAR( int __noQInd)
	{
		vector<vector<double> > _cInds;
		_cInds.resize(_CI);
		for (int _ci = 0;
			_ci < _CI;
			++_ci)
		{
			_cInds[_ci] = _QPop[__noQInd]._generateCInd(_fitnessFunc);
		}
		return _cInds;
	}

	void _execUQIEAR(	int	__genNumber
					,	bool __maxMin
					,	int	__crossoverIndex)
	{
		vector<vector<vector<double> > > _cInds, _bInds;
		vector<map<double, vector<double> > > _bestInds;
		map<double, vector<double> > _allBestInds;
		_cInds.resize(_I);
		_bInds.resize(_I);
		_bestInds.resize(_I);
		//Generate first generation of individuals
		for (	int	_ci = 0;
					_ci < _I;
					++_ci)
		{
			_bInds[_ci] = _genIndUQIEAR(_ci);
		}
		for (int _t = 0;
			_t < __genNumber;
			++_t)
		{
			cout << _t << '\t';

			vector<double> _avgFitness(_I,0);
			for (int _i = 0; _i < _I; ++_i)
			{
				//	Generate new generation of classical individuals
				_cInds[_i] = _genIndUQIEAR(_i);
				for (unsigned int 	_f = 0;
					_f < _CI;
					++_f)
				{
					{
						//	Recombine new and previous classical individuals
						_cInds[_i][_f] = (_EFrand() < F)
						? _bInds[_i][_f] : _cInds[_i][_f];

						//	Evaluate both classical individual arrays
						_avgFitness[_i] += _fitnessFunc(_bInds[_i][_f]);
						_bestInds[_i][_fitnessFunc(_cInds[_i][_f])] = _cInds[_i][_f];
						_bestInds[_i][_fitnessFunc(_bInds[_i][_f])] = _bInds[_i][_f];
						_allBestInds[_fitnessFunc(_bInds[_i][_f])] = _bInds[_i][_f];
					}
					_avgFitness[_i] /= _CI;
				}

				/*map<double, vector<double> >::iterator prueba
					= _bestInds[_i].begin();
				cout << prueba->first << " ";
				++prueba;
				cout << prueba->first << " ";*/

				//	Copy the I best individuals to _bInds
				//	Depends on __maxMin if the goal is to maximize or to minimize
				if (__maxMin)
				{
					map<double, vector<double> >::reverse_iterator _bestBInds
						= _bestInds[_i].rbegin();
					for (unsigned int 	_k = 0;
						_k < _CI;
						++_k)
					{
						//cout<<_bestBInds->first<<" ";
						_bInds[_i][_k] = _bestBInds->second;
						++_bestBInds;
					}
				}
				else
				{
					map<double, vector<double> >::iterator _bestBInds
						= _bestInds[_i].begin();
					for (unsigned int 	_k = 0;
						_k < _CI;
						++_k)
					{
						//cout << _bestBInds->first << " ";
						_bInds[_i][_k] = _bestBInds->second;
						++_bestBInds;
					}
				}
				_bestInds[_i].clear();

				//	Update quantum individuals
				//	Update by altering centers
				_QPop[_i]._updateQInd(_bInds[_i][0]);

				int _totalImproved = 0;
				for (unsigned int	_l = 0;
					_l < _CI;
					++_l)
				{
					//	Sum-up percentage of improved classic individuals
					if (_avgFitness[_i] < _fitnessFunc(_bInds[_i][_l]))
					{
						++_totalImproved;
					}
				}

				//	Calculate 20% of total improvement
				if (__maxMin && _totalImproved > _CI / 5)
				{
					//	Update by altering pulsewidths
					_QPop[_i]._updateQInd(true);
				}
				else if (_totalImproved < _CI * 4 / 5)
				{
					//	Update by altering pulsewidths
					_QPop[_i]._updateQInd(true);
				}
				else
				{
					//	Update by altering pulsewidths
					_QPop[_i]._updateQInd(false);
				}
			}


			map<double, vector<double> >::iterator prueba
				= _allBestInds.begin();
			for (int ww = 0; ww != 6; ++ww)
			{
				cout << prueba->first << " ";
				++prueba;
			}
			cout << endl;


			//	Apply entanglement crossover operator
			//	This is only done every __crossoverIndex iterations
			if (__crossoverIndex > 0)
			{
				if ((_t + 1) % __crossoverIndex == 0) _entangledCrossover(__maxMin);
				for (int _m = 0; _m < _I; ++_m)
					_QPop[_m]._clearQInd();
			}
		}
	}
};



#endif /* QIEAR_H_ */
