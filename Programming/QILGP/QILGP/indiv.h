#ifndef INDIV_H
#define INDIV_H

#include "dataIO.h"
#include "qIndiv.h"
#include "globals.h"

struct tkIndiv
{
	byte *genome;
	double fitness;
	double validFitness;
	double error;
	byte length;
	byte shortest;
};

struct tkIndiv bestValidTkIndiv;
struct tkIndiv observedTkIndiv;
struct tkIndiv *tkPop;

//struct tkIndiv bestExpTkIndiv;

byte effRegs[8];
byte *extrons;

byte *indiv;
int indivLen;
byte *header;
byte headerLen;
byte *footer;
byte footerLen;

void InitIndiv();
void PrintIndiv(byte *indiv, int indivLen);
void RunCase(float *v);
void Assemble(struct tkIndiv *_tkIndiv);
double (*EvalIndivFunc)(float *cases, int nSamples, struct tkIndiv *_tkIndiv);
double        EvalIndiv(float *cases, int nSamples, struct tkIndiv *_tkIndiv);
double EvalIndivHitRate(float *cases, int nSamples, struct tkIndiv *_tkIndiv);
void CopyTkIndiv(struct tkIndiv *dstTkIndiv, struct tkIndiv *srcTkIndiv);

#endif