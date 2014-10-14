#ifndef DATAIO_H
#define DATAIO_H

#include "globals.h"

byte classifProb;
float *trainingData;
float *validatingData;
float *testingData;
int nSamplesTrain;
int nSamplesValid;
int nSamplesTest;
int dataRowDim;
int nInputs;
float *consts;
int nConsts;
int nVars;
unsigned int writeFreq;
float *caseVector;
float *accumFit;
float *accumValFit;
unsigned int *accumLen;
FILE *averageOut;
FILE *hitsOut;
FILE *experimsOut;

void PrintDblMatrix(unsigned int nRows, byte nCols, double **matrix);
void PrintDblVector(int len, double *vector);
void PrintByteVector(int len, byte *vector);
double **ReadInputData(int nRows, byte nVars);
double *ReadConstants(byte nConsts);
void InitCaseVector();
void InitCase(int dataRow, float *cases);
void WriteOutCSV(float *trainingOut, byte *includeCycle);
void OutputData(float *outData);
void WriteOutData(float *outData);
FILE *NewExperimOutFile(char *fName, int experimNo);
void WriteProgOuts(FILE *fp, float *cases, int nSamples);

#endif