#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <memory.h>
#include <time.h>
#include <math.h>
#include "qilgp.h"
#include "globals.h"
#include "qIndiv.h"
#include "indiv.h"
#include "evolution.h"
#include "dataIO.h"
#include "mt19937ar.h"

__declspec(dllexport)
byte SetQIndiv(int _qIndivLen,
			   double _qNopProb,
			   byte _nFuncs,
			   byte _termsDim,
			   byte _termsCardins[],
			   byte _termIdxs[],
			   byte *_destReg0,
			   byte _typedTerms,
			   byte _effObserving,
			   byte *_mCodesLens,
			   byte _mCodeDim,
			   byte *_mCodeTable)
{
	int i;
	int len;

	qIndivLen = _qIndivLen;
	qNopProb = _qNopProb;
	nFuncs = _nFuncs;
	termsDim = _termsDim;
	typedTerms = _typedTerms;
	effObserving = _effObserving;
	mCodeDim = _mCodeDim;
	qIndivInitLen = 8;

	destReg0 = malloc(nFuncs * sizeof(byte));
	for (i = 0; i < nFuncs; i++)
	{
		destReg0[i] = _destReg0[i];
	}

	termsCardins = malloc(nFuncs * sizeof(byte));
	for (i = 0; i < nFuncs; i++)
	{
		termsCardins[i] = _termsCardins[i];
	}

	if (_termIdxs != NULL)
	{
		termIdxs = malloc(nFuncs * sizeof(byte));
		for (i = 0; i < nFuncs; i++)
		{
			termIdxs[i] = _termIdxs[i];
		}
	}

	len = nFuncs * termsDim;
	mCodesLens = malloc(len * sizeof(byte));
	for (i = 0; i < len; i++)
	{
		mCodesLens[i] = _mCodesLens[i];
	}

	len *= mCodeDim;
	mCodeTable = malloc(len * sizeof(byte));
	for (i = 0; i < len; i++)
	{
		mCodeTable[i] = _mCodeTable[i];
	}

	return nFuncs;
}

__declspec(dllexport)
void SetIndiv(byte *_header,
			  byte _headerLen,
			  byte *_footer,
			  byte _footerLen)
{
	int i, j;
	
	headerLen = _headerLen;
	footerLen = _footerLen;

	header = malloc(headerLen * sizeof(byte));
	for (i = 0; i < headerLen; i++)
	{
		header[i] = _header[i];
	}

	footer = malloc(footerLen * sizeof(byte));
	for (j = 0; j< footerLen; j++)
	{
		footer[j] = _footer[j];
	}

}

__declspec(dllexport)
void SetInputCases(byte _classifProb,
				   double *_trainingData, 
				   double *_validatingData, 
				   double *_testingData, 
				   int _nSamplesTrain,
				   int _nSamplesValid,
				   int _nSamplesTest,
				   int _dataRowDim,
				   double *_consts,
                   int _nConsts)
{
	int i;
	int len;

	classifProb = _classifProb;
	nSamplesTrain = _nSamplesTrain;
	nSamplesValid = _nSamplesValid;
	nSamplesTest = _nSamplesTest;
	dataRowDim = _dataRowDim;
	nConsts = _nConsts;
	nVars = dataRowDim - 1;

	len = nSamplesTrain * dataRowDim;

	trainingData = malloc(len * sizeof(double));
	for (i = 0; i < len; i++)
	{
		trainingData[i] = _trainingData[i];
	}

	if (_validatingData)
	{
		len = nSamplesValid * dataRowDim;
		validatingData = malloc(len * sizeof(double));
		for (i = 0; i < len; i++)
		{
			validatingData[i] = _validatingData[i];
		}
	}

	if (_testingData)
	{
		len = nSamplesTest * dataRowDim;
		testingData = malloc(len * sizeof(double));
		for (i = 0; i < len; i++)
		{
			testingData[i] = _testingData[i];
		}
	}

	consts = malloc(nConsts * sizeof(double));
	for (i = 0; i < nConsts; i++)
	{
		consts[i] = _consts[i];
	}

	InitCaseVector();
}

__declspec(dllexport)
void SetEvolution(double _lGateStep,
				  unsigned int _popSize, 
				  unsigned int _nDemes, 
				  unsigned int _nCycles, 
				  unsigned int _nGenerations,
				  unsigned int _generationsFreq,
				  unsigned int _nExperiments)
{
	lGateStep = _lGateStep;
	popSize = _popSize;
	nDemes = _nDemes;
	nCycles = _nCycles;
	nGenerations = _nGenerations;
	generationsFreq = _generationsFreq;
	nExperiments = _nExperiments;
}

__declspec(dllexport)
double RunQILGP()
{
	unsigned int i;
	//srand((unsigned int)time(NULL));
	// 0 to 4,294,967,295 (4294967295)
	//srand(3221225472); 
	init_genrand((unsigned long)time(NULL));
	//init_genrand(0); //0//1073741824//2147483648//3221225472//-//536870912//1610612736//2684354560//3758096384

	writeFreq = nGenerations / 100;
	indivLen = (headerLen + qIndivLen * mCodeDim + footerLen);
	InitSintaticTable();
	InitIndiv();

	accumFit = malloc(nGenerations / writeFreq * sizeof(double));
	accumValFit = malloc(nGenerations / writeFreq * sizeof(double));
	accumLen = malloc(nGenerations / writeFreq * sizeof(double));
	for (i = 0; i < (nGenerations / writeFreq); i++)
	{
		accumFit[i] = 0;
		accumValFit[i] = 0;
		accumLen[i] = 0;
	}
	nEvaluations = 0;

	bestTkIndiv.error = HUGE_VAL;
	bestTkIndiv.fitness = HUGE_VAL;
	bestTkIndiv.validFitness = HUGE_VAL;
	bestTkIndiv.genome = malloc(qIndivLen * sizeof(int) * 2);

	return RunMultiQILGP();
}