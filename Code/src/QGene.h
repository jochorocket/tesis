/*
 * QGene.h
 *
 *  Created on: 24/11/2014
 *      Author: usuario
 */

#ifndef QGENE_H_
#define QGENE_H_

#include <vector>
#include "ExtraFunctions.h"

using namespace std;

#define			CVV		0,3		//	variation of _centerVal values
#define			PWV		0.4		//	variation of _pulseWidth values

class QGene
{
	/*	Attributes	*/
	vector<double> 	_centerVal;
	vector<double> 	_pulseWidth;

	/*	Methods		*/
public:
	/*	Default Constructor	*/
	QGene	()
	{
		_centerVal.push_back(0.5);
		_pulseWidth.push_back(0.5);
	}

	/*	Main Constructor	*/
	QGene(	double 	__centerVal,
			double	__pulseWidth	)
	{
		_centerVal.push_back(__centerVal);
		_pulseWidth.push_back(__pulseWidth);
	}

	/*	Methods for updating center(s) and pulse(s)
	 * 	They all share the same name, but have different characteristics
	 * 	_updateGene(bool) for modifying existing pulsewidths alone
	 * 	_updateGene(double) for modifying existing centers alone
	 * 	_updateGene(vector<double>,vector<double>) for adding new centers and pulsewidths
	 */
	void _updateGene(	bool __cond	)
	{
		for (	unsigned int 	_i = 0;
								_i < _pulseWidth.size();
								++_i	)
		{
			if (!__cond)
			{
				_pulseWidth[_i] *= PWV;
			}
			else
			{
				_pulseWidth[_i] /= PWV;
			}
		}
		/*
		vector<double>::iterator _it;
		for ( 	_it = _pulseWidth.begin();
				_it != _pulseWidth.end();
				++_it	)
		{
			// (*_it) *= (__cond)?PWV:PWV*-1;
			if (__cond)
			{
				(*_it) *= PWV;
			}
			else
				(*_it) *= (PWV*-1);
		}
		*/
	}

	void _updateGene(	double __currVal )
	{
		int _bestLocation = 0;
		/*	_closerPos with a default high value	*/
		double _closerPos = _centerVal[_centerVal.size()];
		for (	unsigned int 	_currLocation = 0;
								_currLocation < _centerVal.size();
								++_currLocation	)
		{
			_bestLocation = (_closerPos >= _centerVal[_currLocation]-__currVal)
								? _currLocation
								: _bestLocation;
			_closerPos = (_closerPos >= _centerVal[_currLocation]-__currVal)
							? _centerVal[_currLocation]-__currVal
							: _closerPos;
		}
		_centerVal[_bestLocation] +=
				CVV * (_centerVal[_bestLocation] - __currVal);
	}

	void _updateGene(	vector<double> 	__centerVal,
						vector<double>	__pulseWidth	)
	{
		_centerVal.insert(	_centerVal.end(),
							__centerVal.begin(),
							__centerVal.end()	);
		_pulseWidth.insert(	_pulseWidth.end(),
							__pulseWidth.begin(),
							__pulseWidth.end()	);
	}

	/*	Generate classic gene	*/
	double _generateClassic()
	{
		/*	Get boundaries	*/
		vector<double> _boundedAreas;
		_boundedAreas.resize(_pulseWidth.size());
		int _boundedTotal = 0;
		for (	unsigned int	_i = 0;
								_i < _pulseWidth.size();
								++_i	)
			_boundedTotal += 2*_pulseWidth[_i];
		for (	unsigned int 	_j = 0;
								_j < _pulseWidth.size();
								++_j)
		{
			_boundedAreas[_j] = 2*_pulseWidth[_j]/_boundedTotal;
			if (_j != 0)
				_boundedAreas[_j] += _boundedAreas[_j-1];
		}

		/*	Calculate in which bounded area will the
		 * 	classic gene be generated
		 */
		double _U = _EFrand(), _boundedU = _U;
		int _boundaryNo=0;
		for (	unsigned int 	_k = 0;
								_k < _pulseWidth.size();
								++_k	)
		{
			if (_boundedAreas[_k] > _U)
			{
				_boundaryNo = _k;
				_boundedU -= _boundedAreas[_k];
			}
			else break;
		}

		/*	Generate classic gene	*/
		return (_centerVal[_boundaryNo] - _pulseWidth[_boundaryNo])
				+ 2*_pulseWidth[_boundaryNo]*_boundedU;
	}

	/*	Return center values	*/
	vector<double> _returnCenterVal()
	{
		return _centerVal;
	}

	/*	Return pulseWidth values	*/
	vector<double> _returnPulseWid()
	{
		return _pulseWidth;
	}

};



#endif /* QGENE_H_ */
