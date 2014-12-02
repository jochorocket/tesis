//============================================================================
// Name        : QIEAR.cpp
// Author      : José Carlos Delgado Ramos
// Version     :
// Copyright   : WTFPL (Do What the Fuck You Want to Licence)
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <math.h>       /* exp */
#include "ExtraFunctions.h"
#include "QIEAR.h"

using namespace std;

#define M_PI	3.14159265358979323846  /* pi */
#define D		2	//	Dimensionality of data

/*	Arckley's Function	*/
double _arckleyFunc (vector<double> _in)
{
	return -20*exp(-0.2*sqrt(0.5*(pow(_in[0],2)+pow(_in[1],2)))) 
			- exp(0.5*(cos(2*M_PI*_in[0])+cos(2*M_PI*_in[1]))) + exp(1) + 20;
}


/*	Booth's Function	*/
double _boothFunc(vector<double> _in)
{
	double a = pow(_in[0] + 2 * _in[1] - 7, 2) + pow(2 * _in[0] + _in[1] - 5, 2);
	//cout <<"Proceso: "<< _in[0]<<" "<<_in[1]<<" "<< a << endl;
	return a;
}

int main() {
	_EFinit();
	//double(*_benchFunc)(vector<double>) = _arckleyFunc;
	double(*_benchFunc)(vector<double>) = _boothFunc;
	double _centerVal = 0;
	double _pulseWidth = 5;
	int _noPartitions = 4;
	int _crossoverOccurence = 3;
	QIEAR<D> a(_centerVal, _pulseWidth, _benchFunc);
	QIEAR<D> b(_centerVal, _pulseWidth, _benchFunc, _noPartitions);
	QIEAR<D> c(_centerVal, _pulseWidth, _benchFunc);
	QIEAR<D> d(_centerVal, _pulseWidth, _benchFunc, _noPartitions);
	QIEAR<D> e(_centerVal, _pulseWidth, _benchFunc);
	QIEAR<D> f(_centerVal, _pulseWidth, _benchFunc, _noPartitions);
	QIEAR<D> g(_centerVal, _pulseWidth, _benchFunc);
	QIEAR<D> h(_centerVal, _pulseWidth, _benchFunc, _noPartitions);

	int _noIterations = 100;
	bool _maximize = false;
	//a._execQIEAR(_noIterations, _maximize, 0);
	b._execQIEAR(_noIterations, _maximize, 0);
	//c._execUQIEAR(_noIterations, _maximize, 0);
	d._execUQIEAR(_noIterations, _maximize, 0);
	//e._execQIEAR(_noIterations, _maximize, _crossoverOccurence);
	//f._execQIEAR(_noIterations, _maximize, _crossoverOccurence);
	//g._execUQIEAR(_noIterations, _maximize, _crossoverOccurence);
	//h._execUQIEAR(_noIterations, _maximize, _crossoverOccurence);
	int w;
	cin >> w;
	return 1;
}
