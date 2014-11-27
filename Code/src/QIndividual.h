/*
 * QIndividual.h
 *
 *  Created on: 25/11/2014
 *      Author: usuario
 */

#ifndef QINDIVIDUAL_H_
#define QINDIVIDUAL_H_

#include <vector>
#include "QGene.h"

using namespace std;

template <int N>
class QIndividual
{
	/*	Attributes	*/
	vector<QGene>	_QInd;

	/*	Methods		*/
public:
	/*	Default Constructor	*/
	QIndividual	()
	{
		_QInd.resize(N);
		for (	int _i = 0;
					_i < N;
					++_i	)
		{
			QGene _QG (0.5,0.5);
			_QInd[_i] = _QG;
		}
	}

	/*	Main Constructors
	 * 	QIndividual(double,double) for a entire domain coverage per ind
	 * 	QIndividual(double,double,int) to partition the domain
	 */
	QIndividual	(	double 	__centerVal,
					double 	__pulseWidth	)
	{
		_QInd.resize(N);
		for (	int _i = 0;
					_i < N;
					++_i	)
		{
			QGene _QG (	__centerVal,
						__pulseWidth);
			_QInd[_i] = _QG;
		}
	}

	QIndividual	(	double 	__centerVal,
					double 	__pulseWidth,
					int		__noPartitions	)
	{
		_QInd.resize(N);
		for (	int _i = 0;
					_i < N;
					++_i	)
		{
			QGene _QG (	__centerVal - __pulseWidth +
						_i*2*__pulseWidth/__noPartitions,
						__pulseWidth/__noPartitions);
			_QInd[_i] = _QG;
		}
	}

	/*	Methods for updating center(s) and pulse(s)
	 * 	They all share the same name, but have different characteristics
	 * 	_updateQInd(bool) for modifying existing pulsewidths alone
	 * 	_updateQInd(vector<double>) for modifying existing centers alone
	 * 	_updateQInd(QIndividual<N>) for crossover
	 */
	void _updateQInd (	bool __cond	)
	{
		for (	int _i = 0;
					_i < N;
					++_i	)
			_QInd[_i]._updateGene(__cond);
	}

	void _updateQInd (	vector<double> __bestCInd	)
	{
		for (	int _i = 0;
					_i < N;
					++_i	)
			_QInd[_i]._updateGene(__bestCInd[_i]);
	}

	void _updateQInd (	QIndividual<N> __QInd	)
	{
		for (	int _i = 0;
					_i < N;
					++_i	)
			_QInd[_i]._updateGene(	__QInd[_i]._returnCenterVal(),
									__QInd[_i]._returnPulseWid()	);
	}

	/*	Generate classic individual	*/
	vector<double> _generateCInd ()
	{
		vector<double> _classicInd;
		_classicInd.resize(N);
		for (	int _i = 0;
					_i < N;
					++_i	)
		{
			_classicInd[_i] = _QInd[_i]._generateClassic();
		}
		return _classicInd;
	}

};



#endif /* QINDIVIDUAL_H_ */
