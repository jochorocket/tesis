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
#include "QIEAR.h"

using namespace std;

#define D	2

double _benchmarkFunc (vector<double> _in)
{
	return -20*exp(-0.2*sqrt(0.5*(pow(_in[0],2)+pow(_in[1],2))));
}

int main() {

	cout << "!!!Eso Perra!!!" << endl; // prints !!!Eso Perra!!!
	double(*_benchFunc)(vector<double>) = _benchmarkFunc;
	QIEAR<D> a(0,5,_benchFunc);
	a._execQIEAR(10,false);

	return 0;
}
