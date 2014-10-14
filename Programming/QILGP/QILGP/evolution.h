#ifndef EVOLUTION_H
#define EVOLUTION_H

#include "qIndiv.h"
#include "indiv.h"

unsigned int popSize;
unsigned int nDemes;
unsigned int nCycles;
unsigned int nGenerations;
unsigned int generationsFreq;
unsigned int nExperiments;
double bestFit;
unsigned long int nEvaluations;
unsigned long int nEvalsExp;
unsigned int gwi;

typedef struct _deme
{
	qGene *qPop;
	struct tkIndiv *tkPop;
	struct tkIndiv bestTkIndiv;
	byte mustSort;
    byte foundBetter;
	unsigned int gwi;
	unsigned int gwiMigr;
	double qNopProb;
	double lGateStep;
	double tmpBestFit;
	double tmpBestValFit;
} deme;

// Best tk individual of all the experiments
struct tkIndiv bestTkIndiv;
// Best tk individual of the current experiment
struct tkIndiv bestExpTkIndiv;

deme *demes;

void (*ObserveQIndivFunc)(void *, void *);

double RunMultiQILGP();

void RunExperiment(int expNo);

void InitQPop();

void ResetQPop();

void InitTkPop();

void InitTkPopLGP();

void RunGeneration(unsigned int gen, double *expBestFit);

void InitDemes();

byte RunDeme(int dIdx);

void RunGenerationDemes(unsigned int gen, double *expBestFit);

void NoRepeatSort(struct tkIndiv *tkIndivs, unsigned int length);

#endif