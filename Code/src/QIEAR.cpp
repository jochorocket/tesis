//============================================================================
// Name        : QIEAR.cpp
// Author      : José Carlos Delgado Ramos
// Version     :
// Copyright   : WTFPL (Do What the Fuck You Want to Licence)
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "ExtraFunctions.h"
#include "EvalFunctions.h"
#include "QIEAR.h"

using namespace std;

#define D		2	//	Dimensionality of data


int main(int argc, char *argv[]) {
	_EFinit();
	double(*_benchFunc)(vector<double>) = _arckleyFunc;
	//double(*_benchFunc)(vector<double>) = _boothFunc;
	//double(*_benchFunc)(vector<double>) = _easomFunc;

	double _centerVal = 0;
	double _pulseWidth = 5;	//	modify for 10 when Booth, 5 when Arckley

	int _noPartitions = 5;
	//int _noPartitions = atoi(argv[1]);
	int _crossoverOccurence = 5;
	//int _noPartitions = atoi(argv[2]);

	int _noIterations = 100;
	bool _maximize = false;


	for (int _noIt = 0; _noIt < 20; ++_noIt)
	{
		stringstream _ss;
		_ss << _noIt;
		string _noVersion = _ss.str();

		QIEAR<D> a(_centerVal, _pulseWidth, _benchFunc);
		QIEAR<D> b(_centerVal, _pulseWidth, _benchFunc, _noPartitions);
		QIEAR<D> c(_centerVal, _pulseWidth, _benchFunc);
		QIEAR<D> d(_centerVal, _pulseWidth, _benchFunc, _noPartitions);
		QIEAR<D> e(_centerVal, _pulseWidth, _benchFunc);
		QIEAR<D> f(_centerVal, _pulseWidth, _benchFunc, _noPartitions);
		QIEAR<D> g(_centerVal, _pulseWidth, _benchFunc);
		QIEAR<D> h(_centerVal, _pulseWidth, _benchFunc, _noPartitions);

		string _aFile = ("qiear" + _noVersion);
		string _bFile = ("qiear_p" + _noVersion);
		string _cFile = ("uqiear" + _noVersion);
		string _dFile = ("uqiear_p" + _noVersion);
		string _eFile = ("qiear_co" + _noVersion);
		string _fFile = ("qiear_pco" + _noVersion);
		string _gFile = ("uqiear_co" + _noVersion);
		string _hFile = ("uqiear_pco" + _noVersion);

		ofstream _aofs(_aFile.c_str(),std::ofstream::out);
		ofstream _bofs(_bFile.c_str(),std::ofstream::out);
		ofstream _cofs(_cFile.c_str(),std::ofstream::out);
		ofstream _dofs(_dFile.c_str(),std::ofstream::out);
		ofstream _eofs(_eFile.c_str(),std::ofstream::out);
		ofstream _fofs(_fFile.c_str(),std::ofstream::out);
		ofstream _gofs(_gFile.c_str(),std::ofstream::out);
		ofstream _hofs(_hFile.c_str(),std::ofstream::out);

		a._execQIEAR(_noIterations, _maximize, _aofs, 0);
		b._execQIEAR(_noIterations, _maximize, _bofs, 0);
		c._execUQIEAR(_noIterations, _maximize, _cofs, 0);
		d._execUQIEAR(_noIterations, _maximize, _dofs, 0);
		e._execQIEAR(_noIterations, _maximize, _eofs, _crossoverOccurence);
		f._execQIEAR(_noIterations, _maximize, _fofs, _crossoverOccurence);
		g._execUQIEAR(_noIterations, _maximize, _gofs, _crossoverOccurence);
		h._execUQIEAR(_noIterations, _maximize, _hofs, _crossoverOccurence);

		_aofs.close();
		_bofs.close();
		_cofs.close();
		_dofs.close();
		_eofs.close();
		_fofs.close();
		_gofs.close();
		_hofs.close();
	}

	return 1;
}
