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
	double			_acumGeneratedFitness;
	int				_noGeneratedFitness;

	/*	Methods		*/
public:
	/*	Default Constructor	*/
	QIndividual	()
		:	_acumGeneratedFitness(0)
		,	_noGeneratedFitness(0)
	{
		_QInd.resize(N);
		for (int _i = 0;
			_i < N;
			++_i)
		{
			QGene _QG (0.5,0.5);
			_QInd[_i] = _QG;
		}
	}

	/*	Main Constructor
	 * 	QIndividual(double,double) for a entire domain coverage per ind
	 */
	QIndividual	(	double 	__centerVal,
					double 	__pulseWidth)
		:	_acumGeneratedFitness(0)
		,	_noGeneratedFitness(0)
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
	
	/*	Required operator
	*/
	QGene operator[]	(int _label)
	{
		return _QInd[_label];
	}

	/*	Method for calculating overall production of the qIndividual
	*/
	double _evalQInd()
	{
		return _acumGeneratedFitness / (double)_noGeneratedFitness;
	}


	/*	Method for clearing values of overall production
	*/
	void _clearQInd()
	{
		_acumGeneratedFitness = 0;
		_noGeneratedFitness = 0;
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
	vector<double> _generateCInd(	double(*__fitnessFunc)(vector<double>)	)
	{
		vector<double> _classicInd;
		_classicInd.resize(N);
		for (	int _i = 0;
					_i < N;
					++_i	)
		{
			_classicInd[_i] = _QInd[_i]._generateClassic();
		}

		//	Update values for acumulated fitnesss
		_acumGeneratedFitness += (__fitnessFunc(_classicInd));
		_noGeneratedFitness++;
		return _classicInd;
	}

};



#endif /* QINDIVIDUAL_H_ */
