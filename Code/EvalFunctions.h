/*
 * EvalFunctions.h
 *
 *  Created on: 02/12/2014
 *      Author: usuario
 */

#ifndef EVALFUNCTIONS_H_
#define EVALFUNCTIONS_H_

#include <vector>
#include <cmath>

using namespace std;

#define M_PI	3.14159265358979323846  /* pi */


/*	Arckley's Function	*/
double _arckleyFunc (vector<double> _in)
{
	return -20*exp(-0.2*sqrt(0.5*(pow(_in[0],2)+pow(_in[1],2))))
			- exp(0.5*(cos(2*M_PI*_in[0])+cos(2*M_PI*_in[1]))) + exp(1) + 20;
}


/*	Booth's Function	*/
double _boothFunc(vector<double> _in)
{
	return pow(_in[0] + 2 * _in[1] - 7, 2) + pow(2 * _in[0] + _in[1] - 5, 2);
}


/*	Holder Table Function	*/
double _holdertableFunc(vector<double> _in)
{
	return -abs(sin(_in[0])*cos(_in[1])*exp(abs(1-sqrt(pow(_in[0],2)+pow(_in[1],2))/M_PI)));
}


/*	Mayta's Function	*/
double _maytasFunc(vector<double> _in)
{
	return 0.26*(pow(_in[0],2)+pow(_in[1],2)) - 0.48*_in[0]*_in[1];
}

#endif /* EVALFUNCTIONS_H_ */
