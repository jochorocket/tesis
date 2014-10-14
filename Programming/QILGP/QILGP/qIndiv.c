#include <stdio.h>
#include <math.h>
#include "qIndiv.h"
#include "indiv.h"
#include "globals.h"
#include "mt19937ar.h"

// Create the sintatic table.
void InitSintaticTable()
{
	register byte f, t, b;
	int termIdx;
	byte* lenPtr;

	// Pointer to terminals: used during sintatic table's reading
	byteList *terms;

	termIdx = termsDim * mCodeDim;
	sintTable = malloc(nFuncs * sizeof(byteList*));
	sintTable[0] = NULL; // QNOP has no terminal

	for (f = 1; f < nFuncs; f++)
	{
		sintTable[f] = malloc(termsCardins[f] * sizeof(byteList));
		terms = sintTable[f];
		for (t = 0; t < termsCardins[f]; t++)
		{
			lenPtr = mCodesLens + (f * termsDim) + t;
			terms[t].length = *lenPtr;
			terms[t].list = malloc(terms[t].length * sizeof(byte));
			for (b = 0; b < *lenPtr; b++)
			{
				terms[t].list[b] = *(mCodeTable + (f * termIdx) + (t * mCodeDim) + b);
			}
		}
	}
}

// Create and return a Q-token
qToken InitQToken(byte cardinality)
{
	qToken qT;
	register byte i;
	double probStep; // Probabilities step

	qT.alphas = malloc(cardinality * sizeof(double));
	qT.cardinality = cardinality;

	if (cardinality == 0)
	{
		probStep = 1.0;
	}
	else if (cardinality == nVars + nConsts)
	{
		probStep = 0.5 / nVars;
		qT.alphas[0] = probStep;
		for (i = 1; i < nVars; i++)
		{
			qT.alphas[i] = qT.alphas[i - 1] + probStep;
		}
		probStep = 0.5 / nConsts;
		for (; i < cardinality; i++)
		{
			qT.alphas[i] = qT.alphas[i - 1] + probStep;
		}
		return qT;
	}
	else
	{
		probStep = (1.0 / cardinality);
	}
	qT.alphas[0] = probStep;
	for (i = 1; i < cardinality; i++)
	{
		qT.alphas[i] = qT.alphas[i - 1] + probStep;
	}
	
	return qT;
} 

// Create and return a function Q-token
// qNopProb: QNOP probability
qToken InitFuncQToken(byte _nFuncs, double nopProb)
{
	qToken qT;
	register byte i;
	double _qNopProb = (1 - nopProb) / (_nFuncs - 1);

	qT.alphas = malloc(_nFuncs * sizeof(double));
	qT.cardinality = _nFuncs;
	qT.alphas[0] = nopProb;
	for (i = 1; i < nFuncs; i++)
	{
		qT.alphas[i] = qT.alphas[i - 1] + _qNopProb;
	}
	
	return qT;
}

// Observe Q-token.
// Return the observed token.
byte ObserveQToken(qToken *qT)
{
	register byte i;
	static double randNum;
	
	if (qT == NULL)
		return 0;
	
	//randNum = (double)rand() / RAND_MAX; // For normalized qtoken
	//randNum = qT->alphas[qT->cardinality - 1] * (double)rand() / RAND_MAX; // For non normalized
	randNum = qT->alphas[qT->cardinality - 1] * genrand_real1();

	for (i = 0; i < qT->cardinality - 1; i++)
	{
		if (randNum < qT->alphas[i])
		{
			return i;
		}
	}

	return i;
}

byte ObserveQTokenEff(qToken *qT)
{
	register byte i;
	double randNum;
	
	qTokenAux.alphas[0] = qT->alphas[0];
	for (i = 1; i < 8; i++)
	{
		qTokenAux.alphas[i] = effRegs[i] ? (qTokenAux.alphas[i-1] + qT->alphas[i] - qT->alphas[i-1]): 
			qTokenAux.alphas[i-1];
	}

	randNum = ((double)rand() / RAND_MAX) * qTokenAux.alphas[7];

	for (i = 0; i < 8; i++)
	{
		if (randNum < qTokenAux.alphas[i])
		{
			return i;
		}
	}

	return 7;
}

void ResetFuncQToken(qToken *qT, byte _nFuncs, double nopProb)
{
	register byte i;
	static double _qNopProb;

	_qNopProb = (1 - nopProb) / (_nFuncs - 1);
	qT->alphas[0] = nopProb;
	for (i = 1; i < nFuncs; i++)
	{
		qT->alphas[i] = qT->alphas[i - 1] + _qNopProb;
	}
}

void ResetQToken(qToken *qT, byte cardinality)
{
	register byte i;
	static double probStep;

	if (cardinality == 0)
	{
		probStep = 1.0;
	}
	else if (cardinality == nVars + nConsts)
	{
		probStep = 0.5 / nVars;
		qT->alphas[0] = probStep;
		for (i = 1; i < nVars; i++)
		{
			qT->alphas[i] = qT->alphas[i - 1] + probStep;
		}
		probStep = 0.5 / nConsts;
		for (; i < cardinality; i++)
		{
			qT->alphas[i] = qT->alphas[i - 1] + probStep;
		}
	}
	else
	{
		probStep = (1.0 / cardinality);
		qT->alphas[0] = probStep;
		for (i = 1; i < cardinality; i++)
		{
			qT->alphas[i] = qT->alphas[i - 1] + probStep;
		}
	}
	
}

qGene InitQLinGene(double nopProb)
{
	qGene qGene;
	register byte i;

	qGene.funcQT = InitFuncQToken(nFuncs, nopProb);
	qGene.termQTs = malloc(nFuncs * sizeof(qToken));
	for (i = 0; i < nFuncs; i++)
	{
		qGene.termQTs[i] = InitQToken(termsCardins[i]);
	}
	return qGene;
}

void ResetQGene(qGene *qG, double nopProb) //Stopped Here!!!
{
    register byte i;

	ResetFuncQToken(&qG->funcQT, nFuncs, nopProb);
	for (i = 0; i < nFuncs; i++)
	{
		 ResetQToken(&qG->termQTs[i], termsCardins[i]);
	}
}

// Create a Q-Individual and points qIndiv to it.
void InitQIndiv(qGene *qIndiv) 
{
	register int i;

	for (i = 0; i < qIndivLen; i++)
	{
		qIndiv[i] = InitQLinGene(qNopProb);
	}
}

void ResetQIndiv(qGene *qIndiv)
{
    register int i;

	for (i = 0; i < qIndivLen; i++)
	{
        ResetQGene(&qIndiv[i], qNopProb);
	}
}

//void SeedQIndiv()
//{
//int i;
//
//	for (i = 0; i < qIndivLen; i++)
//	{
//		
//			LGate2QTokenNoNorm(&(qIndiv[i].funcQT), _tkIndiv->genome[i * 2]); 
//			if (qIndiv[i].termQTs[_tkIndiv->genome[i * 2]].cardinality > 1)
//			{
//				
//					if (termIdxs[_tkIndiv->genome[i * 2]] > 0)
//					{
//						LGate2QTokenNoNorm(&(qIndiv[i].termQTs[termIdxs[_tkIndiv->genome[i * 2]]]),
//										   _tkIndiv->genome[i * 2 + 1]);
//						/*LGate2QTokenNoNormLtd(&(qIndiv[i].termQTs[termIdxs[_tkIndiv->genome[i * 2]]]),
//										   _tkIndiv->genome[i * 2 + 1]);*/
//					}
//				
//			}
//	}
//}

void InitQIndivExit(qGene *qIndiv) 
{
	register int i, j;
	double prod, sum;
	int secLen;

	secLen = qIndivLen - qIndivInitLen;
	sum = 3 * (secLen - 1) * secLen / 2;
	prod = qIndivInitLen;
	qIndiv[0] = InitQLinGene(0);
	for (i = 1; i < qIndivInitLen; i++)
	{
		prod *= (1 - qIndiv[i-1].funcQT.alphas[0]);
		qIndiv[i] = InitQLinGene(1/prod);
	}
	qIndiv[i] = InitQLinGene(0);
	prod = 2*(secLen - 1);
	for (j = prod, i++; i < qIndivLen; i++, j--)
	{
		//qIndiv[i] = InitQLinGene((j/sum)/);
	}
}

void ObserveQIndiv(qGene *qLinIndiv, struct tkIndiv *_tkIndiv)
{
	int i, j, b, qIdx;
	int m;
	byte funcToken;
	byte termToken = 0;
	byteList *terms;
	
	_tkIndiv->length = 0;

	m = headerLen;
	for (i = 0; i < qIndivLen; i++)
	{
		funcToken = ObserveQToken(&qLinIndiv[i].funcQT);
		qIdx = i * 2;
		if (funcToken > 0)
		{
			_tkIndiv->length++;
			if (!typedTerms)
			{
			    termToken = ObserveQToken(&qLinIndiv[i].termQTs[funcToken]);
			}
			else
			{
				if (termIdxs[funcToken] == 0)
				{
					termToken = 0;
				}
				else
				{
					termToken = ObserveQToken(&qLinIndiv[i].termQTs[termIdxs[funcToken]]);	
				}
			}
			terms = sintTable[funcToken];
			for (b = 0; b < terms[termToken].length; b++, m++)
			{
				indiv[m] = terms[termToken].list[b];
			}
			*(_tkIndiv->genome + qIdx + 1) = termToken;
		}
		//qIdx = i * 2;
		*(_tkIndiv->genome + qIdx) = funcToken;
		//*(_tkIndiv->genome + qIdx + 1) = termToken;
	}
	for (j = 0; j < footerLen; j++,m++)
	{
		indiv[m] = footer[j];
	}
}


void ObserveQIndivNoNop(qGene *qLinIndiv, struct tkIndiv *_tkIndiv)
{
	int i, j, b, qIdx;
	int m;
	byte funcToken;
	byte termToken = 0;
	byteList *terms;
	
	m = headerLen;
	for (i = 0, j = 0; i < qIndivLen; i++)
	{
		funcToken = ObserveQToken(&qLinIndiv[i].funcQT);
		if (funcToken > 0)
		{
			if (!typedTerms)
			{
			    termToken = ObserveQToken(&qLinIndiv[i].termQTs[funcToken]);
			}
			else
			{
				if (termIdxs[funcToken] == 0)
				{
					termToken = 0;
				}
				else
				{
					termToken = ObserveQToken(&qLinIndiv[i].termQTs[termIdxs[funcToken]]);	
				}
			}
			terms = sintTable[funcToken];
			for (b = 0; b < terms[termToken].length; b++, m++)
			{
				indiv[m] = terms[termToken].list[b];
			}
			qIdx = j * 2;
			*(_tkIndiv->genome + qIdx) = funcToken;
			*(_tkIndiv->genome + qIdx + 1) = termToken;
			j++;
		}
	}
	_tkIndiv->length = j / 2;
	for (j = 0; j < footerLen; j++,m++)
	{
		indiv[m] = footer[j];
	}

	return;
}

void CopyQToken(qToken *qTkDst, qToken *qTkSrc)
{
	int i;

	qTkDst->cardinality = qTkSrc->cardinality;
	for(i = 0; i < qTkSrc->cardinality; i++)
	{
		qTkDst->alphas[i] = qTkSrc->alphas[i];
	}
}

qToken *ZeroGate(qToken *qTk, int tkId)
{
	register int i;
	static int lastTkIdx;
	static qToken outQTk;
	double dec = 0;
	static double *alphas = NULL;

	if (alphas == NULL)
	{
		alphas = malloc(nFuncs * sizeof(double));
		outQTk.cardinality = nFuncs;
		outQTk.alphas = alphas;
		lastTkIdx = qTk->cardinality - 1;
	}
	
	CopyQToken(&outQTk, qTk);

	if (tkId == 0)
	{
		dec = outQTk.alphas[0];
		outQTk.alphas[0] = 0;
	}
	else
	{
		dec = outQTk.alphas[tkId] - outQTk.alphas[tkId -1];
		outQTk.alphas[tkId] = outQTk.alphas[tkId -1];
	}

	for (i = tkId + 1; i < outQTk.cardinality; i++)
	{
		outQTk.alphas[i] -= dec;
	}
	
	for (i = 0; i < outQTk.cardinality; i++)
	{
		outQTk.alphas[i] /= outQTk.alphas[lastTkIdx];
	}

	return &outQTk;
}

void ObserveQIndiv1Gene(qGene *qIndiv, struct tkIndiv *_tkIndiv)
{
	int g, gIdx;
	byte funcToken;
	byte termToken = 0;
	qToken *qT;

	g = rand() % qIndivLen;
	gIdx = g * 2;

	//funcToken = ObserveQToken(&qIndiv[g].funcQT);

	funcToken = ObserveQToken(&qIndiv[g].funcQT);
	if (funcToken == _tkIndiv->genome[gIdx] && funcToken != 0)
	{
		qT = ZeroGate(&qIndiv[g].funcQT, _tkIndiv->genome[gIdx]);
		funcToken = ObserveQToken(qT);
	}

	if (funcToken > 0)
	{
		termToken = ObserveQToken(&qIndiv[g].termQTs[funcToken]);
		//qT = ZeroGate(&qIndiv[g].termQTs[funcToken], _tkIndiv->genome[gIdx + 1]);
		//termToken = ObserveQToken(qT);
	}	
	*(_tkIndiv->genome + gIdx) = funcToken;
	*(_tkIndiv->genome + gIdx + 1) = termToken;

	return;
}

byte LGate2QToken(qToken *qT, byte token)
{
	int i;
	int lastTkIdx;
	double inc;
	
	if (token > 0 && 
		(qT->alphas[token] - qT->alphas[token - 1] < 0.95))
	{
		//inc = lGateStep * (qT->alphas[token] - qT->alphas[token - 1]);
		//inc = cos(qT->alphas[token] - qT->alphas[token - 1]);
		inc = lGateStep * (1 - (qT->alphas[token] - qT->alphas[token - 1]));
		//inc = sin(HALF_PI * (qT->alphas[token] - qT->alphas[token - 1]));
		//inc = lGateStep * (1 - (qT->alphas[token] - qT->alphas[token - 1])) / qT->cardinality;
	}
	else
	if (token == 0 &&
		 qT->alphas[0] < 0.95)
	{
		//inc = lGateStep * qT->alphas[token];
		//inc = cos(qT->alphas[token]);
		inc = lGateStep * (1 - qT->alphas[token]);
		//inc = sin(HALF_PI * qT->alphas[token]);
		//inc = lGateStep * (1 - qT->alphas[token]) / qT->cardinality;
	}
	else
	{
		return 1;
	}

	lastTkIdx = qT->cardinality - 1;

	for (i = token; i < qT->cardinality; i++)
	{
		qT->alphas[i] += inc;
	}
	for (i = 0; i < qT->cardinality; i++)
	{
		qT->alphas[i] /= qT->alphas[lastTkIdx];
	}
	
	return 0;
}

void LGate(qGene *qIndiv, struct tkIndiv *_tkIndiv)
{
	int i;
	int convTksCount = 0;

	for (i = 0; i < qIndivLen; i++)
	{
		convTksCount += LGate2QToken(&(qIndiv[i].funcQT), _tkIndiv->genome[i * 2]);
		if (qIndiv[i].termQTs[_tkIndiv->genome[i * 2]].cardinality > 1)
		{
			if (!typedTerms)
			{
				convTksCount += LGate2QToken(&(qIndiv[i].termQTs[_tkIndiv->genome[i * 2]]), 
                    _tkIndiv->genome[i * 2 + 1]);
			}
			else
			{
				if (termIdxs[_tkIndiv->genome[i * 2]] > 0)
				{
					convTksCount += LGate2QToken(&(qIndiv[i].termQTs[termIdxs[_tkIndiv->genome[i * 2]]]),
                        _tkIndiv->genome[i * 2 + 1]);
				}
			}
		}
	}

	// Todo: verify to change the initial qNop prob here...
	//if (convTksCount >= qIndivLen) // 1000 * qIndivLen) // 1.4, -> 1.7 <-, 1.1
	//{
	//	InitQIndiv(qIndiv);
	//	printf("r! ");
	//	convTksCount = 0;
	//}
}

//byte LGate2QTokenNoNorm(qToken *qT, byte token) // P to Linear!!! (06-10-2011)
//{
//	register int i;
//	static int lastTkIdx;
//	static double inc;
//	static byte;
//
//	lastTkIdx = qT->cardinality - 1;
//
// 	if (token == 0)
//	{
//		inc = lGateStep * (qT->alphas[lastTkIdx] - qT->alphas[0]);
//	}
//    else
//    {
//        inc = lGateStep * (qT->alphas[lastTkIdx] - (qT->alphas[token] - qT->alphas[token-1]));
//    }
//
//	for (i = token; i < qT->cardinality; i++)
//	{
//		qT->alphas[i] += inc;
//	}
//	
//	return 0;
//}

byte LGate2QTokenNoNorm(qToken *qT, byte token) // P Ltd!!! (07/10/2011)
{
	register int i;
	static int lastTkIdx;
	static double inc;
	//static byte limit, card;

	lastTkIdx = qT->cardinality - 1;

 	if (token == 0)
	{
		//limit = (qT->alphas[0] / qT->alphas[lastTkIdx]) < 0.98;
		inc = lGateStep * (qT->alphas[lastTkIdx] - qT->alphas[0]);
		//if (!limit) printf(" f gt 0.98 ");
	}
    else 
    {
		//limit = ((qT->alphas[token] - qT->alphas[token-1]) / qT->alphas[lastTkIdx]) < 0.98;
        inc = lGateStep * (qT->alphas[lastTkIdx] - (qT->alphas[token] - qT->alphas[token-1]));
		//if (!limit)
		//{
		//	printf(" t gt 0.98 ");
		//}
    }
	//card = limit * qT->cardinality;
	for (i = token; i < qT->cardinality; i++)
	{
		qT->alphas[i] += inc;
	}
	
	return 0;
}

byte LGate2QTokenNoNormLtd2(qToken *qT, byte token)
{
	register int i;
	static int lastTkIdx;
	static double inc;
	static byte gt, card;

	lastTkIdx = qT->cardinality - 1;

	// For the first token (a0): Official!!!
 	if (token == 0)
	{
		gt = (((qT->alphas[lastTkIdx] - qT->alphas[0]) / qT->alphas[lastTkIdx]) < 0.99);
		//if (gt == 0) printf("Top!\n");
		inc = lGateStep * (qT->alphas[lastTkIdx] - qT->alphas[0]);// / qT->cardinality;
		//inc = lGateStep * qT->alphas[0];
	}
    else
    {
		gt = (((qT->alphas[lastTkIdx] - (qT->alphas[token] - qT->alphas[token-1])) / qT->alphas[lastTkIdx]) < 0.99);
        inc = lGateStep * (qT->alphas[lastTkIdx] - (qT->alphas[token] - qT->alphas[token-1]));
		//inc = lGateStep * (qT->alphas[token] - qT->alphas[token-1]);
    }

	card = gt * qT->cardinality;
	for (i = token; i < card; i++)
	{
		qT->alphas[i] += inc;
		//qT->alphas[i] += lGateStep;
	}
	
	return 0;
}

byte MGate2QToken(qToken *qT, byte token)
{
	register int i;
	static int lastTkIdx;
	static double inc, acc;
	static double stp[17][17] = {
		{1, 0.25, 0.25, 0.111111111, 0, 0, 0.111111111, 0, 0, 0.111111111, 0, 0, 0, 0, 0, 0, 0},
		{0.111111111, 1, 0.25, 0, 0.0625, 0.04, 0, 0.0625, 0.04, 0, 0.0625, 0.04, 0, 0, 0, 0, 0},
		{0.111111111, 0.25, 1, 0, 0.04, 0.0625, 0, 0.04, 0.0625, 0, 0.04, 0.0625, 0, 0, 0, 0, 0},
		{0.111111111, 0, 0, 1, 0.25, 0.25, 0.111111111, 0, 0, 0.111111111, 0, 0, 0, 0, 0, 0, 0},
		{0, 0.0625, 0.04, 0.111111111, 1, 0.25, 0, 0.0625, 0.04, 0, 0.0625, 0.04, 0, 0, 0, 0, 0},
		{0, 0.04, 0.0625, 0.111111111, 0.25, 1, 0, 0.04, 0.0625, 0, 0.04, 0.0625, 0, 0, 0, 0, 0},
		{0.111111111, 0, 0, 0.111111111, 0, 0, 1, 0.25, 0.25, 0.111111111, 0, 0, 0, 0, 0, 0, 0},
		{0, 0.0625, 0.04, 0, 0.0625, 0.04, 0.111111111, 1, 0.25, 0, 0.0625, 0.04, 0, 0, 0, 0, 0},
		{0, 0.04, 0.0625, 0, 0.04, 0.0625, 0.111111111, 0.25, 1, 0, 0.04, 0.0625, 0, 0, 0, 0, 0},
		{0.111111111, 0, 0, 0.111111111, 0, 0, 0.111111111, 0, 0, 1, 0.25, 0.25, 0, 0, 0, 0, 0},
		{0, 0.0625, 0.04, 0, 0.0625, 0.04, 0, 0.0625, 0.04, 0.111111111, 1, 0.25, 0, 0, 0, 0, 0},
		{0, 0.04, 0.0625, 0, 0.04, 0.0625, 0, 0.04, 0.0625, 0.111111111, 0.25, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0.25, 0.25, 0.25},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.25, 1, 0.25, 0.25},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.111111111, 0.111111111, 1, 0.25},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.111111111, 0.111111111, 0.25, 1}
	};
	
	lastTkIdx = qT->cardinality - 1;

	if (token == 0)
	{
		inc = lGateStep * (qT->alphas[lastTkIdx] - qT->alphas[0]);
		for (i = 0; i < qT->cardinality; i++)
		{
			qT->alphas[i] += inc;
		}
	}
    else
	{
		acc = 0;
		inc = lGateStep * (qT->alphas[lastTkIdx] - (qT->alphas[token] - qT->alphas[token-1]));
		for (i = 1; i < qT->cardinality; i++)
		{
			//inc = stp[token-1][i-1] * (qT->alphas[lastTkIdx] - (qT->alphas[token] - qT->alphas[token-1]));
			acc += (0.1 * inc * stp[token-1][i-1]);
			//acc += inc;
			qT->alphas[i] += acc;
			//qT->alphas[i] += inc;
		}
	}

	return 0;
}

byte LGate2QTokenNoNormLtd(qToken *qT, byte token)
{
	register int i;
	static int lastTkIdx;
	static double inc;
    static double limit;
    static double alpha;

	inc = 0;
    lastTkIdx = qT->cardinality - 1;
    limit = 0.90 * qT->alphas[lastTkIdx];

	// For the first token (a0)
 	if (token == 0)
	{
        if (qT->alphas[0] < limit)
        {
		    inc = lGateStep * (qT->alphas[lastTkIdx] - qT->alphas[0]) / qT->cardinality;
        }
	}
	// For the last token
	else
	if (token == lastTkIdx)
	{
        if ((qT->alphas[lastTkIdx] - qT->alphas[lastTkIdx-1]) < limit)
        {
		    inc = lGateStep * qT->alphas[lastTkIdx-1] / qT->cardinality;
        }
	}
	// For a token in the middle
	else
	{
        alpha = qT->alphas[token] - qT->alphas[token-1];
        if (alpha < limit)
        {
		    inc = lGateStep * (qT->alphas[lastTkIdx] - alpha) / qT->cardinality;
        }
	}

    if (inc == 0)
        return 1;

	for (i = token; i < qT->cardinality; i++)
	{
		qT->alphas[i] += inc;
	}
	
	return 0;
}

byte LGate2FuncQTokenNoNormLtd(qToken *qT, byte token)
{
	register int i;
	static int lastTkIdx;
	static double inc;
    static double limit;
    static double alpha;

	inc = 0;
    lastTkIdx = qT->cardinality - 1;
    //limit = 0.90 * qT->alphas[lastTkIdx];

	// For the first token (a0)
 	if (token == 0)
	{
        limit = 0.90 * qT->alphas[lastTkIdx];
        if (qT->alphas[0] < limit)
        {
		    inc = lGateStep * (qT->alphas[lastTkIdx] - qT->alphas[0]) / qT->cardinality;
        }
	}
	// For the last token
	else
	if (token == lastTkIdx)
	{
        limit = 0.95 * qT->alphas[lastTkIdx];
        if ((qT->alphas[lastTkIdx] - qT->alphas[lastTkIdx-1]) < limit)
        {
		    inc = lGateStep * qT->alphas[lastTkIdx-1] / qT->cardinality;
        }
	}
	// For a token in the middle
	else
	{
        limit = 0.95 * qT->alphas[lastTkIdx];
        alpha = qT->alphas[token] - qT->alphas[token-1];
        if (alpha < limit)
        {
		    inc = lGateStep * (qT->alphas[lastTkIdx] - alpha) / qT->cardinality;
        }
	}

    if (inc == 0)
        return 1;

	for (i = token; i < qT->cardinality; i++)
	{
		qT->alphas[i] += inc;
	}
	
	return 0;
}

void LGateNoNorm(qGene *qIndiv, struct tkIndiv *_tkIndiv)
{
	int i;

	for (i = 0; i < qIndivLen; i++)
	{
		if (extrons[i] != 0)
		{
			LGate2QTokenNoNorm(&(qIndiv[i].funcQT), _tkIndiv->genome[i * 2]); 
			if (qIndiv[i].termQTs[_tkIndiv->genome[i * 2]].cardinality > 1)
			{
				if (!typedTerms)
				{
					LGate2QTokenNoNorm(&(qIndiv[i].termQTs[_tkIndiv->genome[i * 2]]),
									   _tkIndiv->genome[i * 2 + 1]);
					/*LGate2QTokenNoNormLtd(&(qIndiv[i].termQTs[_tkIndiv->genome[i * 2]]),
									   _tkIndiv->genome[i * 2 + 1]);*/
				}
				else
				{
					if (termIdxs[_tkIndiv->genome[i * 2]] > 0)
					{
						LGate2QTokenNoNorm(&(qIndiv[i].termQTs[termIdxs[_tkIndiv->genome[i * 2]]]),
										   _tkIndiv->genome[i * 2 + 1]);
						/*LGate2QTokenNoNormLtd(&(qIndiv[i].termQTs[termIdxs[_tkIndiv->genome[i * 2]]]),
										   _tkIndiv->genome[i * 2 + 1]);*/
					}
				}
			}
		}
		else
		{
			LGate2QTokenNoNorm(&(qIndiv[i].funcQT), 0);
		}
	}
}

void LGateNoNop(qGene *qIndiv, struct tkIndiv *_tkIndiv)
{
	int i, j;
	int convTksCount = 0;

	for (i = 0, j = 0; i < qIndivLen; i++)
	{
		if (_tkIndiv->genome[i * 2] != 0)
		{
			convTksCount += LGate2QToken(&(qIndiv[j].funcQT), _tkIndiv->genome[i * 2]);
			if (qIndiv[i].termQTs[_tkIndiv->genome[i * 2]].cardinality > 1)
			{
				if (!typedTerms)
				{
					LGate2QToken(&(qIndiv[j].termQTs[_tkIndiv->genome[i * 2]]),
								 _tkIndiv->genome[i * 2 + 1]);
				}
				else
				{
					if (termIdxs[_tkIndiv->genome[i * 2]] > 0)
					{
						LGate2QToken(&(qIndiv[j].termQTs[termIdxs[_tkIndiv->genome[i * 2]]]),
									 _tkIndiv->genome[i * 2 + 1]);
					}
				}
			}
			j++;
		}
	}
	//for (i = j+1; i < qIndivLen; i++)
	//{
	//	LGate2QToken(&(qIndiv[i].funcQT), 0);
	//}
}

void LGate1Gene(qGene *qIndiv, struct tkIndiv *_tkIndiv)
{
	int gene;

	gene = rand() % qIndivLen;

	LGate2QToken(&(qIndiv[gene].funcQT), _tkIndiv->genome[gene * 2]);
	if (qIndiv[gene].termQTs[_tkIndiv->genome[gene * 2]].cardinality > 1)
	{
		LGate2QToken(&(qIndiv[gene].termQTs[_tkIndiv->genome[gene * 2]]),
		_tkIndiv->genome[gene * 2 + 1]);
	}
}