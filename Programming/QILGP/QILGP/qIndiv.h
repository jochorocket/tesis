#ifndef QINDIV_H
#define QINDIV_H

#include <stdlib.h>
#include "globals.h"
#include "indiv.h"

typedef struct _qToken
{
	byte cardinality;
	double *alphas;
} qToken;

typedef struct _qGene
{
	qToken funcQT;
	qToken *termQTs;
} qGene;

qToken qTokenAux;

qGene *qPop;

int qIndivLen;

int qIndivInitLen;

// No. of functions (including QNOP)
byte nFuncs;

double qNopProb;

double lGateStep;

byte termsDim;

// Terminals cardinalities
// byte termsCardins[];
byte *termsCardins;

byte *termIdxs;

byte *destReg0;
byte typedTerms;
byte effObserving;

// The sintatic table
byteList **sintTable;

// Machine codes lengths [function_token][terminal_token]
// byte mCodesLens[][256];
byte *mCodesLens;

byte mCodeDim;

// Machine code table [function_token][terminal_token][byte_idx]
// byte mCodeTable[][256][4];
byte *mCodeTable;

void InitSintaticTable();

qToken InitQToken(byte cardinality);
qToken InitFuncQToken();
byte ObserveQToken(qToken *qT);
byte ObserveQTokenEff(qToken *qT);

qGene InitQLinGene(double nopProb);
void InitQIndiv(qGene *qIndiv);

void ResetFuncQToken(qToken *qT, byte nFuncs, double nopProb);
void ResetQToken(qToken *qT, byte cardinality);
void ResetQIndiv(qGene *qIndiv);
void ResetQGene(qGene *qG, double nopProb);

void ObserveQIndiv(qGene *qLinIndiv, struct tkIndiv *_tkIndiv);
void CopyQToken(qToken *qTkDst, qToken *qTkSrc);
qToken *ZeroGate(qToken *qTk, int tkId);
void ObserveQIndiv1Gene(qGene *qIndiv, struct tkIndiv *__tkIndiv);

byte LGate2QToken(qToken *qT, byte token);

void LGate(qGene *qIndiv, struct tkIndiv *_tkIndiv);
void LGateNoNop(qGene *qIndiv, struct tkIndiv *_tkIndiv);
void LGate1Gene(qGene *qIndiv, struct tkIndiv *_tkIndiv);
void LGateNoNorm(qGene *qIndiv, struct tkIndiv *_tkIndiv);
byte LGate2QTokenNoNorm(qToken *qT, byte token);

#endif