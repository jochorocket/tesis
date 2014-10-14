#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "indiv.h"
#include "evolution.h"

void InitIndiv()
{
	register byte i;

	indiv = malloc(indivLen * sizeof(byte));

	for (i = 0; i < headerLen; i++)
	{
		indiv[i] = header[i];
	}
}

void PrintIndiv(byte *indiv, int indivLen)
{
	register int i;
	
	for (i = 0; (i <= indivLen) & (indiv[i-1] != 0xc3); i++)
	{
		printf("%x ", indiv[i]);
	}
	putchar('\n');

	return;
}

void RunCase(float *v)
{
	typedef void(* pFunction)(v);

	//static short unsigned int cw = 0x037a;
	_asm fninit	
	//_asm fldcw [cw]

	((pFunction) indiv)(v);
	if (!_finite(v[0]))
	{
		v[0]=0;
	}
}

void Assemble(struct tkIndiv *_tkIndiv)
{
	int i, j, b;
	int m;
    int len;
	byte funcToken;
	byte termToken = 0;
	byteList *terms;
	
	_tkIndiv->length = 0;
	m = headerLen;
    len = qIndivLen * 2;

	for (i = 0; i < len; i+=2)
	{
		funcToken = *(_tkIndiv->genome + i);
		if (funcToken > 0)
		{
			_tkIndiv->length++;
			termToken = *(_tkIndiv->genome + i + 1);
			terms = sintTable[funcToken];
			for (b = 0; b < terms[termToken].length; b++, m++)
			{
				indiv[m] = terms[termToken].list[b];
			}
		}
	}
	for (j = 0; j < footerLen; j++,m++)
	{
		indiv[m] = footer[j];
	}
}

double EvalIndiv(float *cases, int nSamples, struct tkIndiv *_tkIndiv)
{
	register int c;
	static int idx;
	float sError = 0.0;

	for (c = 0; c < nSamples; c++)
	{
		InitCase(c, cases);
		RunCase(caseVector);
		idx = c * dataRowDim + nVars;
		sError +=  fabs(caseVector[0] - cases[idx]); //MAE
		//sError +=  fabs((caseVector[0] - cases[idx]) / cases[idx]); //MAPE
        //sError +=  fabs(DiscipulusAssemblerFunction(caseVector) - cases[c * dataRowDim + nVars]);
        //sError +=  pow(caseVector[0] - cases[c * dataRowDim + nVars], 2); //MSE
	}

	return (sError / nSamples);
}

double EvalIndivHitRate(float *cases, int nSamples, struct tkIndiv *_tkIndiv)
{
	int c;
	int hits = 0;

	_tkIndiv->error = 0;
	for (c = 0; c < nSamples; c++)
	{
		InitCase(c, cases);
		RunCase(caseVector);
		if (cases[c * dataRowDim + nVars])
		{
			if (caseVector[0] >= 0.5) hits++;
		}
		else
		{
			if (caseVector[0] < 0.5) hits++;
		}
		_tkIndiv->error += fabs(caseVector[0] - 
			                    cases[c * dataRowDim + nVars]);
	}
	//nEvaluations++;
	_tkIndiv->error /= nSamples;

	return (1 - ((double)hits / nSamples));
}

// Copy the srcTkIndiv into dstTkIndiv
void CopyTkIndiv(struct tkIndiv *dstTkIndiv, struct tkIndiv *srcTkIndiv)
{
	register int i;
	register int indivLen = qIndivLen * 2;

	for (i = 0; i < indivLen; i++)
	{
		dstTkIndiv->genome[i] = srcTkIndiv->genome[i];
	}
	dstTkIndiv->fitness = srcTkIndiv->fitness;
	dstTkIndiv->validFitness = srcTkIndiv->validFitness;
	dstTkIndiv->error = srcTkIndiv->error;
	dstTkIndiv->length = srcTkIndiv->length;
	dstTkIndiv->shortest = srcTkIndiv->shortest;
}