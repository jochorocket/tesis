#ifndef QILGP_H
#define QILGP_H

__declspec(dllexport)
byte SetQIndiv(int _qIndivLen,
			   double _qNopProb,
			   byte _nFuncs,
			   byte _termsDim,
			   byte _termsCardins[],
			   byte _termIdxs[],
			   byte *_destReg0,
			   byte typedTerms,
			   byte effObserving,
			   byte *_mCodesLens,
			   byte _mCodeDim,
			   byte *_mCodeTable);

__declspec(dllexport)
void SetIndiv(byte *_header,
			  byte _headerLen,
			  byte *_footer,
			  byte _footerLen);

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
                   int _nConsts);

__declspec(dllexport)
void SetEvolution(double _lGateStep,
				  unsigned int _popSize, 
				  unsigned int _nDemes, 
				  unsigned int _nCycles,
				  unsigned int _nGenerations,
				  unsigned int _generationsFreq,
				  unsigned int _nExperiments);

__declspec(dllexport)
double RunQILGP();

double RunSimpleQILGP();

#endif