/*
 * EDAlgorithm.h
 *
 *  Created on: 02/10/2014
 *      Author: usuario
 */

#ifndef EDALGORITHM_H_
#define EDALGORITHM_H_

#include "EDIndividual.h"

using namespace std;

/*	GR: recombination rate control parameter
 * 	Must be between 0 and 1
 * 	Base value = 0.3	*/
#define		GR						0.3
typedef		EDIndividual			EDInd;
typedef 	vector<EDIndividual>	VEDInd;

class EDAlgorithm
{
	/*	Attributes	*/
	int		_sizePop;
	int		_noVariables;
	VEDInd 	_pop;
	VEDInd 	_noisyPop;
	VEDInd 	_trialPop;

	/* 	Pointer Methods	*/
	/*	double	(EDAlgorithm::*_fitnessFunc)(VEDInd);	*/
	double	(*_fitnessFunc)(EDInd);

	/*	Methods		*/
public:
	//	Default constructor
	EDAlgorithm(){}

	//	Main constructor
	EDAlgorithm(	int 			__sizePop,
					int				__noVariables,
					vector<double>	__minData,
					vector<double>	__maxData,
					double			(*__fitnessFunc)(EDInd))
		:	_sizePop		(__sizePop)
		,	_noVariables	(__noVariables)
		,	_fitnessFunc	(__fitnessFunc)
	{
		_pop.resize(_sizePop);
		_noisyPop.resize(_sizePop);
		_trialPop.resize(_sizePop);
		// 	Initialize normal population
		for (int _p1 = 0; _p1 < _sizePop; ++_p1)
		{
			EDInd _newEDIndividual 	(_noVariables
									,0
									,__minData
									,__maxData);
			_pop[_p1] = _newEDIndividual;
		}
	}

	void _EDMutation ()
	{
		//	Initialize mutated population
		for (int _p1 = 0; _p1 < _sizePop; ++_p1)
		{
			EDInd _newEDIndividualM (_pop[_EFrandInt(_sizePop)]
									,_pop[_EFrandInt(_sizePop)]
									,_pop[_EFrandInt(_sizePop)]);
			_noisyPop[_p1] = _newEDIndividualM;
		}
	}

	void _EDRecombination ()
	{
		for (int _p1 = 0; _p1 < _sizePop; ++_p1)
		{
			_trialPop[_p1] = (_EFrandDouble() < GR) ?
								_noisyPop[_p1] : _pop[_p1];
		}
	}

	int _EDInit (int __noGeneration)
	{
		/*	Determine stop condition
		 * 	Here we're using a fixed number of iterations	*/
		while (_pop[0]._getAge() < __noGeneration)
		{
			for (int _p1 = 0; _p1 < _sizePop; ++_p1)
			{
				_EDMutation();
				_EDRecombination();
				_pop[_p1] =
						_fitnessFunc(_pop[_p1]) > _fitnessFunc(_trialPop[_p1]) ?
						_pop[_p1] : _trialPop[_p1];
				_pop[_p1]._increaseAge();
			}
		}
		return 1;
	}
};



#endif /* EDALGORITHM_H_ */
