#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <memory.h>
#include <time.h>
#include <math.h>
#include "qilgp.h"
#include "evolution.h"
#include "dataIO.h"
#include "qIndiv.h"
#include "indiv.h"
#include "utils.h"

double RunMultiQILGP()
{
	unsigned int i;
	static unsigned int nTkIndivs;
	char fileTrnName[] = "trnOut.txt\0";
	char fileVldName[] = "vldOut.txt\0";
	char fileTstName[] = "tstOut.txt\0";

	bestFit = HUGE_VAL;
	nTkIndivs = 2 * popSize;

	qTokenAux = InitQToken(8);

	// Allocate memory to the best individual of the current experiment
    bestExpTkIndiv.genome = (byte*)malloc(qIndivLen * sizeof(int) * 2);

	// Allocate memory to the best individual of the QILGP's run (since then)
    bestTkIndiv.genome = (byte*)malloc(qIndivLen * sizeof(int) * 2);

	// Allocate memory to token individuals population
	tkPop = malloc(nTkIndivs * sizeof(struct tkIndiv));
	for (i = 0; i < nTkIndivs; i++)
	{
		tkPop[i].genome = malloc(2 * qIndivLen * sizeof(int));
		tkPop[i].error = HUGE_VAL;
		tkPop[i].fitness = HUGE_VAL;
        tkPop[i].validFitness = HUGE_VAL;
	}

	// Allocate memory to quantum individuals population
	qPop = malloc(popSize * qIndivLen * sizeof(qGene));

	extrons = malloc(qIndivLen * sizeof(byte));

	if (!validatingData)
	{
		hitsOut = fopen("hitsOut.txt", "w");
	}
	experimsOut = fopen("experimsOut.txt", "w");
	fprintf(experimsOut, "Gen;\t");
	if (validatingData)
	{
		fprintf(experimsOut, "BestValidIndiv;\t");
	}
	for (i = 0; i < popSize; i++)
	{
		fprintf(experimsOut, "Len Train Valid;\t");
	}
	fprintf(experimsOut, "\n");

	EvalIndivFunc = classifProb ? EvalIndivHitRate : EvalIndiv;

	if (effObserving) //Disabled!!!
	{
		//ObserveQIndivFunc = ObserveQIndivEff;
		ObserveQIndivFunc = ObserveQIndiv;
		for (i = 0; i < qIndivLen; i++)
		{
			extrons[i] = 1;
		}
	}
	else
	{
		ObserveQIndivFunc = ObserveQIndiv;
		for (i = 0; i < qIndivLen; i++)
		{
			extrons[i] = 1;
		}
	}

	demes = NULL;
	// Loop to run all the experiments
	for (i = 0; i < nExperiments; i++)
	{
		printf("Experiment %d:\n", i);
		RunExperiment(i);
		printf("\n\n");
	}

	if (!validatingData)
	{
		fclose(hitsOut);
	}
	fprintf(experimsOut, "\n#Evaluations = %d\n", nEvaluations);
	if (testingData)
	{
		fprintf(experimsOut, "Best evolved individual:\n");
		fprintf(experimsOut, "  Length = %d\n", bestTkIndiv.length);
		Assemble(&bestTkIndiv);
        fprintf(experimsOut, "  Training error   = %g\n", EvalIndivFunc(trainingData, nSamplesTrain, &bestTkIndiv)); //bestTkIndiv.fitness);  //
		fprintf(experimsOut, "  Validating error = %g\n", EvalIndivFunc(validatingData, nSamplesValid, &bestTkIndiv)); //bestTkIndiv.validFitness);  //
        fprintf(experimsOut, "  Testing error    = %g\n", EvalIndivFunc(testingData, nSamplesTest, &bestTkIndiv));
		fprintf(experimsOut, "  Genotype: ");
		for (i = 0; i < qIndivLen; i++)
		{
			fprintf(experimsOut, "%x,", bestTkIndiv.genome[2*i]);
			fprintf(experimsOut, "%x ", bestTkIndiv.genome[2*i+1]);
		}
	}
	fclose(experimsOut);

	averageOut = fopen("averageOut.txt", "w");
	for (i = 0; i < nGenerations / writeFreq; i++)
	{
		accumFit[i] /= nExperiments;
		accumLen[i] /= nExperiments;
		if (validatingData)
		{
			accumValFit[i] /= nExperiments;
			fprintf(averageOut, "%d %g %g %d\n", i * writeFreq, accumFit[i], accumValFit[i], accumLen[i]); //!!!
		}
		else
		{
			fprintf(averageOut, "%d %g %d\n", i * writeFreq, accumFit[i], accumLen[i]);
		}
	}
	if (validatingData)
	{
		fprintf(averageOut, "%d %g %g %d\n", i * writeFreq, accumFit[i-1], accumValFit[i-1], accumLen[i-1]);
	}
	else
	{
		fprintf(averageOut, "%d %g %d\n", i * writeFreq, accumFit[i-1], accumLen[i-1]);
	}
	fclose(averageOut);

	Assemble(&bestTkIndiv);
	WriteBestProgOuts(fileTrnName, trainingData, nSamplesTrain);
	WriteBestProgOuts(fileVldName, validatingData, nSamplesValid);
	WriteBestProgOuts(fileTstName, testingData, nSamplesTest);

    return bestTkIndiv.fitness;
}

//double RunSingleQILGP()
//{
//	unsigned int i;
//	static unsigned int nTkIndivs;
//
//	bestFit = HUGE_VAL;
//	nTkIndivs = 2 * popSize;
//
//	// Allocate memory to the best validation token individual
//	bestValidTkIndiv.genome = malloc(qIndivLen * sizeof(int) * 2);
//
//	// Allocate memory to the best observed token individual
//	observedTkIndiv.genome = malloc(qIndivLen * sizeof(int) * 2);
//
//	// Allocate memory to token individuals population
//	tkPop = malloc(nTkIndivs * sizeof(struct tkIndiv));
//	for (i = 0; i < nTkIndivs; i++)
//	{
//		tkPop[i].genome = malloc(qIndivLen * sizeof(int) * 2);
//	}
//
//	// Allocate memory to quantum individuals population
//	qPop = malloc(popSize * qIndivLen * sizeof(qGene));
//
//	hitsOut = fopen("hitsOut.txt", "w");
//	experimsOut = fopen("experimsOut.txt", "w");
//
//	// Loop to run all the experiments
//	for (i = 0; i < nExperiments; i++)
//	{
//		printf("Experiment %d:\n", i);
//		RunExperiment();
//		printf("\n\n");
//	}
//
//	fclose(hitsOut);
//	fclose(experimsOut);
//
//	averageOut = fopen("averageOut.txt", "w");
//	for (i = 0; i < nGenerations / writeFreq; i++)
//	{
//		accumFit[i] /= nExperiments;
//		fprintf(averageOut, "%d %.4f\n", i * writeFreq, accumFit[i]);
//	}
//	fprintf(averageOut, "%d %.4f\n", i * writeFreq, accumFit[i-1]);
//	fclose(averageOut);
//
//	return bestFit;
//}

void RunExperiment(int expNo)
{
	unsigned int i, j, wrFreqCount;
	double expBestFit = HUGE_VAL;
	double expBestValFit = HUGE_VAL;
    FILE *fpExp;
	
	//InitQPop();
	//InitTkPop();
	InitDemes();

	wrFreqCount = 0;
	gwi = 1;
    nEvalsExp = 0;
	i = 0;

    bestExpTkIndiv.fitness = HUGE_VAL;
    bestExpTkIndiv.validFitness = HUGE_VAL;
    bestExpTkIndiv.error = HUGE_VAL;
	
	//RunGeneration(i, &expBestFit);
	RunGenerationDemes(i, &expBestFit);
	accumFit[0] += tkPop[0].fitness; //expBestFit;
	accumLen[0] += tkPop[0].length;
    accumValFit[0] += bestTkIndiv.validFitness;

	for (i = 1, wrFreqCount = 1; i <= nGenerations; i++, wrFreqCount++) //!!!
	{
		//RunGeneration(i, &expBestFit);
		RunGenerationDemes(i, &expBestFit);
		if (wrFreqCount == writeFreq)
		{
			accumFit[i/writeFreq] += bestExpTkIndiv.fitness;//tkPop[0].fitness;//expBestFit;
			accumLen[i/writeFreq] += bestExpTkIndiv.length;//tkPop[0].length;
			fprintf(experimsOut, "%d;\t", i);
			if (validatingData)
			{
                accumValFit[i/writeFreq] += bestExpTkIndiv.validFitness;
				fprintf(experimsOut, "%d %g %g;\t", bestExpTkIndiv.length, bestExpTkIndiv.fitness, 
                    bestExpTkIndiv.validFitness);
			}
            // For no demes
			//for (j = 0; j < popSize; j++)
			//{
			//	fprintf(experimsOut, "%d %g %g;\t", tkPop[j].length, tkPop[j].fitness, tkPop[j].validFitness);
			//}
			
            // For demes
            for (j = 0; j < nDemes; j++)
			{
                fprintf(experimsOut, "%d %g %g;\t", demes[j].bestTkIndiv.length, demes[j].bestTkIndiv.fitness, 
                        demes[j].bestTkIndiv.validFitness);
			}
            
            fputc('\n', experimsOut);
			wrFreqCount = 0;
		}
	}
	if (expBestFit < bestFit)
	{
		bestFit = expBestFit;
	}

	if (validatingData)
	{
		printf("\nPrograms evaluated=%d", nEvalsExp);
        printf("\nBest Program: T=%g; V=%g; L=%d\n\n", bestExpTkIndiv.fitness, 
            bestExpTkIndiv.validFitness, bestExpTkIndiv.length);
	}
	
	Assemble(&bestExpTkIndiv); // Just to calculate test error

	fprintf(experimsOut, "Best: Len=%d; Trn=%g; Val=%g; Tst=%g;\n", 
		    bestExpTkIndiv.length, 
			//bestExpTkIndiv.fitness, bestExpTkIndiv.validFitness,
			EvalIndivFunc(trainingData, nSamplesTrain, &bestExpTkIndiv), // Maybe useless
			EvalIndivFunc(validatingData, nSamplesValid, &bestExpTkIndiv), // Maybe useless
		    EvalIndivFunc(testingData, nSamplesTest, &bestExpTkIndiv));
	fprintf(experimsOut, "  Genotype: ");
	for (i = 0; i < qIndivLen; i++)
	{
		fprintf(experimsOut, "%x,", bestTkIndiv.genome[2*i]);
		fprintf(experimsOut, "%x ", bestTkIndiv.genome[2*i+1]);
	}
	fprintf(experimsOut, "\n\n");

    fpExp = NewExperimOutFile("trainOut", expNo);
    WriteProgOuts(fpExp, trainingData, nSamplesTrain);
    fpExp = NewExperimOutFile("validOut", expNo);
    WriteProgOuts(fpExp, validatingData, nSamplesValid);
    fpExp = NewExperimOutFile("testOut", expNo);
    WriteProgOuts(fpExp, testingData, nSamplesTest);

    nEvalsExp = 0;
}

void InitQPop()
{
	register unsigned int i;

	for (i = 0; i < popSize; i++)
	{
		InitQIndiv(&qPop[i * qIndivLen]);
	}
}

void ResetQPop()
{
    register unsigned int i;
    for (i = 0; i < popSize; i++)
	{
		ResetQIndiv(&qPop[i * qIndivLen]);
	}
}

void InitTkPop()
{
	register unsigned int i;

	for (i = 0; i < popSize; i++)
	{
		ObserveQIndiv(qPop, &tkPop[i]);
        tkPop[i].fitness = EvalIndivFunc(trainingData, nSamplesTrain, &tkPop[i]); // HUGE_VAL;
        tkPop[i].validFitness = EvalIndivFunc(validatingData, nSamplesValid, &tkPop[i]); // HUGE_VAL;
	}
}

void InitTkPopLGP()
{
	unsigned int i, j, nInstrs, nTkIndivs, step, qIdx;
	static unsigned int genomeLen;
	struct tkIndiv *tkIndivs;

	nTkIndivs = 10 * popSize;
	genomeLen = qIndivLen * 2;
	tkIndivs = malloc(nTkIndivs * sizeof(struct tkIndiv));
	
	for (i = 0; i < nTkIndivs; i++)
	{
		tkIndivs[i].genome = malloc(qIndivLen * sizeof(int) * 2);
		for (j = 0; j < genomeLen; j++)
		{
			tkIndivs[i].genome[j] = 0;
		}
		//nInstrs = (rand() % (qIndivLen - 19) + 20); // E=0.1555!!!
		nInstrs = ((rand() % 40) + 20);
		step = 1 + (qIndivLen - nInstrs) / (nInstrs + 1);
		for (j = step - 1; j < genomeLen; j += step)
		{
			qIdx = j * 2;			
			tkIndivs[i].genome[qIdx] = (rand() % (nFuncs - 1)) + 1;
			tkIndivs[i].genome[qIdx + 1] = rand() % termsCardins[tkIndivs[i].genome[qIdx]];
		}
		Assemble(&tkIndivs[i]);
        tkIndivs[i].fitness = EvalIndivFunc(trainingData, nSamplesTrain, &tkIndivs[i]);
	}

	InsertSort(tkIndivs, nTkIndivs);
	
	for (i = 0; i < popSize; i++)
	{
		CopyTkIndiv(&tkPop[i], &tkIndivs[i]);
        //tkPop[i] = tkIndivs[i];
	}
}

//Sorting
void RunGeneration(unsigned int gen, double *expBestFit)
{
	register int i, j;
	unsigned int lastTkIndiv;
    int qTkIdx;
	static byte mustSort = 1;
    static byte foundBetter = 0;
	unsigned int nTkIndivs = 2 * popSize;
    static unsigned int qMutRange;
	static qGene *qGenePtr;
	
    lastTkIndiv = popSize - 1;
    foundBetter = 0;
    
    if (gwi >= 20000)
    {
		//qNopProb = 1 / (double)nFuncs;
		ResetQPop();
		//lGateStep = 1;
		//LGateNoNorm(&(qPop[0]), &bestExpTkIndiv);
		//for (i = 0; i < lastTkIndiv; i++) // popSize/2
		//{
		//	LGateNoNorm(&(qPop[i * qIndivLen]), &bestExpTkIndiv); // Best!
		//}
		//qNopProb = 0.9;
		//lGateStep = 0.004;
		//for (; i < popSize; i++)
		//{
		//	ResetQIndiv(&qPop[i * qIndivLen]);
		//}

        CopyTkIndiv(&tkPop[0], &bestExpTkIndiv);
        //tkPop[0] = bestExpTkIndiv;
		for (i = 1; i < popSize; i++)
		{
            tkPop[i].fitness = HUGE_VAL;
            tkPop[i].validFitness = HUGE_VAL;
			tkPop[i].error = HUGE_VAL;
        }

        printf("G=%d RST!  ", gen);
        gwi = 0;
    }
    
    for (i = 0, j = popSize; i < popSize; i++, j++)
	{
		ObserveQIndivFunc(&qPop[i * qIndivLen], &tkPop[j]);
		tkPop[j].fitness = EvalIndivFunc(trainingData, nSamplesTrain, &tkPop[j]);
        nEvaluations++;
        nEvalsExp++;

		if (tkPop[j].fitness <= tkPop[lastTkIndiv].fitness)
			mustSort = 1;
		
        if (tkPop[j].fitness <= bestExpTkIndiv.fitness) // *expBestFit)
		{
			if (validatingData)
			{
                tkPop[j].validFitness = EvalIndivFunc(validatingData, nSamplesValid, &tkPop[j]);

                // Check valid error
                if (tkPop[j].validFitness < bestExpTkIndiv.validFitness ||
                    (tkPop[j].fitness < bestExpTkIndiv.fitness &&
                     tkPop[j].validFitness < bestExpTkIndiv.fitness) ||
                    (tkPop[j].fitness == bestExpTkIndiv.fitness &&
                     tkPop[j].validFitness == bestExpTkIndiv.fitness &&
                     tkPop[j].length < bestExpTkIndiv.length))

      //          if (tkPop[j].validFitness < bestExpTkIndiv.validFitness ||
      //              (tkPop[j].validFitness > bestExpTkIndiv.validFitness &&
      //               tkPop[j].validFitness < tkPop[j].fitness &&
					 //tkPop[j].fitness < bestExpTkIndiv.fitness)) // Check valid error // It works so so!!!

			  //  if (tkPop[j].validFitness < bestExpTkIndiv.validFitness ||
					//(tkPop[j].fitness < bestExpTkIndiv.fitness &&
					// tkPop[j].validFitness < tkPop[j].fitness    )||
					//(tkPop[j].fitness == bestExpTkIndiv.fitness &&
					// tkPop[j].validFitness == bestExpTkIndiv.validFitness &&
					// tkPop[j].error < bestExpTkIndiv.error) ) // Check validation error
				{
					printf("G=%i L=%i T=%.4f BV=%.4f!  ", gen, tkPop[j].length, 
						tkPop[j].fitness, tkPop[j].validFitness);
					//printf("<=Best!  ");
                    CopyTkIndiv(&bestExpTkIndiv, &tkPop[j]);
                    if(bestExpTkIndiv.fitness < bestTkIndiv.fitness &&
                       (bestExpTkIndiv.validFitness < bestTkIndiv.validFitness/* ||
                        bestExpTkIndiv.validFitness < bestTkIndiv.fitness*/))
					{
						CopyTkIndiv(&bestTkIndiv, &bestExpTkIndiv);
                        //bestTkIndiv = bestExpTkIndiv;
					}
                    if (tkPop[j].fitness < *expBestFit)
				    {
                        *expBestFit = tkPop[j].fitness;
				    }
                    foundBetter = 1;
				}
				else
                {
                    if (tkPop[j].fitness < *expBestFit)
				    {
					    /*printf("G=%i L=%i T=%g V=%g; ", gen, tkPop[j].length, 
						       tkPop[j].fitness, tkPop[j].validFitness);*/
                        printf("G=%i; ", gen);
                        *expBestFit = tkPop[j].fitness;
				    }
                }
			}
			else
			{
				printf("G=%i L=%i T=%g;  ", gen, tkPop[j].length, tkPop[j].fitness);
			}
		}
		/*qTkIdx = (rand() % qMutRange) + (i * qIndivLen);
		qGenePtr = &qPop[qTkIdx];
		LGate2QTokenNoNorm(&qGenePtr->funcQT, 
			rand() % qGenePtr->funcQT.cardinality);
		LGate2QTokenNoNorm(&qGenePtr->termQTs[1],
			rand() % qGenePtr->termQTs[1].cardinality);
		LGate2QTokenNoNorm(&qGenePtr->termQTs[2],
			rand() % qGenePtr->termQTs[2].cardinality);*/
	}
	if (gwi == 0) // Pop has just reset
	{
		InsertSortLen(tkPop, nTkIndivs); // Both SymbRegr & Classif
	    mustSort = 0;
		gwi = 1;
	}
	else if (mustSort)
	{
		InsertSortLen(tkPop, nTkIndivs);
		NoRepeat(tkPop, nTkIndivs); // SymbRegr
		//NoRepeatClass(tkPop, nTkIndivs); // Classif
		mustSort = 0;
	}

    if (foundBetter)
    {
        gwi = 1;
    }
    else
        gwi++;
    
    for (i = 0; i < popSize; i++)
    {
        LGateNoNorm(&(qPop[i * qIndivLen]), &(tkPop[i]));
    }

    /*qTkIdx = rand() % qMutRange;
	qGenePtr = &qPop[qTkIdx];
	LGate2QTokenNoNorm(&qGenePtr->funcQT, 
		rand() % qGenePtr->funcQT.cardinality);
	LGate2QTokenNoNorm(&qGenePtr->termQTs[1],
		rand() % qGenePtr->termQTs[1].cardinality);
	LGate2QTokenNoNorm(&qGenePtr->termQTs[2],
		rand() % qGenePtr->termQTs[2].cardinality);*/

}

void InitDemes() //Stopped here! Todo: initQPop!
{
	register int i, j;
	static unsigned int nTkIndivs;
	
	if (!demes)
	{
		nTkIndivs = 2 * popSize;
		demes = malloc(nDemes * sizeof(deme));
		for (i = 0; i < nDemes; i++)
		{
			demes[i].qPop = malloc(popSize * qIndivLen * sizeof(qGene));
			demes[i].tkPop = malloc(nTkIndivs * sizeof(struct tkIndiv));
			demes[i].bestTkIndiv.genome = malloc(qIndivLen * sizeof(int) * 2);
			for (j = 0; j < nTkIndivs; j++)
			{
				demes[i].tkPop[j].genome = malloc(2 * qIndivLen * sizeof(int));
			}
		}
	}
	for (i = 0; i < nDemes; i++)
	{
		for (j = 0; j < popSize; j++)
		{
			InitQIndiv(&(demes[i].qPop[j * qIndivLen]));
			ObserveQIndiv(&demes[i].qPop[j * qIndivLen], &demes[i].tkPop[j]);
			demes[i].tkPop[j].fitness = EvalIndivFunc(trainingData, nSamplesTrain, &demes[i].tkPop[j]); // HUGE_VAL;
			demes[i].tkPop[j].validFitness = EvalIndivFunc(validatingData, nSamplesValid, &demes[i].tkPop[j]);
		}
		demes[i].foundBetter = 0;
		demes[i].gwi = 0;
		demes[i].gwiMigr = 1;
		demes[i].mustSort = 1;
		demes[i].lGateStep = lGateStep;
		demes[i].qNopProb = qNopProb;
		demes[i].bestTkIndiv.fitness = HUGE_VAL;
		demes[i].bestTkIndiv.validFitness = HUGE_VAL;
		demes[i].bestTkIndiv.error = HUGE_VAL;
		demes[i].tmpBestFit = HUGE_VAL;
		demes[i].tmpBestValFit = HUGE_VAL;
	}
}

//byte RunDemeMass(int dIdx)
//{
//	register int i, j, k;
//	static unsigned int lastTkIndiv;
//	static int nTkIndivs;
//
//	nTkIndivs = 2 * popSize;
//	lastTkIndiv = popSize - 1;
//
//	demes[dIdx].mustSort = 1;
//	demes[dIdx].foundBetter = 0;
//	if (demes[dIdx].gwiMigr >= 2001)
//	{
//		k = (dIdx + 1) % (nDemes);
//		CopyTkIndiv(&demes[dIdx].tkPop[lastTkIndiv], &demes[k].bestTkIndiv);
//		//ResetQIndiv(&demes[dIdx].qPop[lastTkIndiv]); // 18-01-2011
//		printf("D%d Mgr;  ", dIdx);
//		demes[dIdx].gwiMigr = 0;
//	}
//	else
//	if (demes[dIdx].gwi >= 5000)
//    {
//		k = (dIdx + 1) % (nDemes);
//		if (demes[dIdx].bestTkIndiv.fitness < demes[k].bestTkIndiv.fitness)
//		{
//			CopyTkIndiv(&demes[dIdx].tkPop[0], &demes[dIdx].bestTkIndiv);
//			CopyTkIndiv(&demes[dIdx].tkPop[1], &demes[k].bestTkIndiv);
//		}
//		else
//		{
//			CopyTkIndiv(&demes[dIdx].tkPop[0], &demes[k].bestTkIndiv);
//			CopyTkIndiv(&demes[dIdx].tkPop[1], &demes[dIdx].bestTkIndiv);
//		}
//
//		//CopyTkIndiv(&demes[dIdx].tkPop[0], &demes[dIdx].bestTkIndiv);
//
//		for (i = 2; i < popSize; i++)
//		//for (i = 1; i < popSize; i++)
//		{
//            demes[dIdx].tkPop[i].fitness = HUGE_VAL;
//            demes[dIdx].tkPop[i].validFitness = HUGE_VAL;
//			demes[dIdx].tkPop[i].error = HUGE_VAL;
//        }
//		qNopProb = 1 / (double)nFuncs;
//		ResetQIndiv(&demes[dIdx].qPop[0]);
//		ResetQIndiv(&demes[dIdx].qPop[qIndivLen]);
//		qNopProb = 0.9;
//		lGateStep = 1;
//		LGateNoNorm(&(demes[dIdx].qPop[0])        , &demes[dIdx].tkPop[0]);
//		LGateNoNorm(&(demes[dIdx].qPop[qIndivLen]), &demes[dIdx].tkPop[1]);
//		lGateStep = 0.004;
//		for (i = 2; i < popSize; i++)
//		//for (i = 1; i < popSize; i++)
//		{
//			ResetQIndiv(&demes[dIdx].qPop[i * qIndivLen]);
//			//LGateNoNorm(&(demes[dIdx].qPop[i * qIndivLen]), &demes[0].bestTkIndiv); // Best!
//            //LGateNoNorm(&(demes[dIdx].qPop[i * qIndivLen]), &demes[dIdx].bestTkIndiv); // YATry...
//		}
//
//        printf("D%d Rst;  ", dIdx);
//        demes[dIdx].gwi = 0;
//		demes[dIdx].gwiMigr = 0;
//		demes[dIdx].tmpBestFit = HUGE_VAL;
//		demes[dIdx].tmpBestValFit = HUGE_VAL;
//    }
//
//	//// Debugging code...
//	//ObserveQIndivFunc(&demes[0].qPop[0], &demes[0].tkPop[4]);
//	//demes[0].tkPop[4].fitness = EvalIndivFunc(validatingData, nSamplesValid, &demes[0].tkPop[4]);
//	//Assemble(&demes[0].tkPop[4]);
//	//demes[0].tkPop[4].fitness = EvalIndivFunc(validatingData, nSamplesValid, &demes[0].tkPop[4]);
//	//// ...
//
//    for (i = 0, j = popSize; i < popSize; i++, j++)
//	{
//		ObserveQIndivFunc(&demes[dIdx].qPop[i * qIndivLen], &demes[dIdx].tkPop[j]);
//		demes[dIdx].tkPop[j].fitness = EvalIndivFunc(trainingData, nSamplesTrain, &demes[dIdx].tkPop[j]);
//        nEvaluations++;
//        nEvalsExp++;
//
//		if (demes[dIdx].tkPop[j].fitness <= demes[dIdx].tkPop[lastTkIndiv].fitness)
//			demes[dIdx].mustSort = 1;
//		
//		if (demes[dIdx].tkPop[j].fitness <= demes[dIdx].bestTkIndiv.fitness)
//		{
//			if (validatingData)
//			{
//                demes[dIdx].tkPop[j].validFitness = EvalIndivFunc(validatingData, nSamplesValid, &demes[dIdx].tkPop[j]);
//				//TODO: isBetter()
//			    if ((demes[dIdx].tkPop[j].validFitness < demes[dIdx].bestTkIndiv.validFitness)/* ||
//				    (demes[dIdx].tkPop[j].fitness < demes[dIdx].bestTkIndiv.fitness &&
//					 demes[dIdx].tkPop[j].validFitness < demes[dIdx].bestTkIndiv.fitness)*/)
//				{
//					printf("D%i L=%i T=%.4f V=%.4f;  ", dIdx, demes[dIdx].tkPop[j].length, 
//						demes[dIdx].tkPop[j].fitness, demes[dIdx].tkPop[j].validFitness);
//                    CopyTkIndiv(&demes[dIdx].bestTkIndiv, &demes[dIdx].tkPop[j]);
//                    //demes[dIdx].bestTkIndiv = demes[dIdx].tkPop[j];
//                    demes[dIdx].foundBetter = 1;
//					demes[dIdx].tmpBestFit = HUGE_VAL;
//					demes[dIdx].tmpBestValFit = HUGE_VAL;
//				}
//				else
//				{
//					if (classifProb)
//					{
//						if (demes[dIdx].tkPop[j].fitness == demes[dIdx].bestTkIndiv.fitness &&
//					        demes[dIdx].tkPop[j].validFitness == demes[dIdx].bestTkIndiv.validFitness &&
//					        demes[dIdx].tkPop[j].error < demes[dIdx].bestTkIndiv.error) //Todo: len tst
//						{
//							printf("D%i L=%i T=%.4f V=%.4f;  ", dIdx, demes[dIdx].tkPop[j].length, 
//						    demes[dIdx].tkPop[j].fitness, demes[dIdx].tkPop[j].validFitness);
//                            CopyTkIndiv(&demes[dIdx].bestTkIndiv, &demes[dIdx].tkPop[j]);
//                            //demes[dIdx].bestTkIndiv = demes[dIdx].tkPop[j];
//                            demes[dIdx].foundBetter = 1;
//						}
//					}
//					else
//					{
//						if (demes[dIdx].tkPop[j].fitness == demes[dIdx].bestTkIndiv.fitness &&
//					        demes[dIdx].tkPop[j].validFitness == demes[dIdx].bestTkIndiv.validFitness &&
//							demes[dIdx].tkPop[j].length < demes[dIdx].bestTkIndiv.length)
//						{
//							printf("D%i L=%i;  ", dIdx, demes[dIdx].tkPop[j].length);
//                            CopyTkIndiv(&demes[dIdx].bestTkIndiv, &demes[dIdx].tkPop[j]);
//                            //demes[dIdx].foundBetter = 1;
//						}
//					}
//				}
//			}
//			else
//			{
//				printf("L=%i T=%g;  ", tkPop[j].length, tkPop[j].fitness);
//			}
//		}
//		else
//		{
//			if (demes[dIdx].tkPop[j].fitness <= demes[dIdx].tmpBestFit)
//			{
//				demes[dIdx].tkPop[j].validFitness = EvalIndivFunc(validatingData, nSamplesValid, &demes[dIdx].tkPop[j]);
//				if (demes[dIdx].tkPop[j].validFitness < demes[dIdx].tmpBestValFit)
//				{
//					demes[dIdx].tmpBestFit = demes[dIdx].tkPop[j].fitness;
//					demes[dIdx].tmpBestValFit = demes[dIdx].tkPop[j].validFitness;
//					demes[dIdx].gwi = 1;
//					demes[dIdx].gwiMigr = 1;
//					printf("D%i; ", dIdx);
//				}
//				else
//				{
//					demes[dIdx].gwi++;
//					demes[dIdx].gwiMigr++;
//				}
//			}
//			else
//			{
//				demes[dIdx].gwi++;
//				demes[dIdx].gwiMigr++;
//			}
//		}
//	}
//	if (demes[dIdx].gwi == 0)
//	{
//		InsertSortLen(demes[dIdx].tkPop, nTkIndivs);
//	    demes[dIdx].mustSort = 0;
//	}
//	else if (demes[dIdx].mustSort)
//	{
//		InsertSortLen(demes[dIdx].tkPop, nTkIndivs);
//		NoRepeat(demes[dIdx].tkPop, nTkIndivs);
//		demes[dIdx].mustSort = 0;
//	}
//
//    if (demes[dIdx].foundBetter)
//    {
//        demes[dIdx].gwi = 1;
//		demes[dIdx].gwiMigr = 1;
//    }
// //   else
//	//{
// //       demes[dIdx].gwi++;
//	//	demes[dIdx].gwiMigr++;
//	//}
//    
//    for (i = 0; i < popSize; i++)
//    {
//        LGateNoNorm(&(demes[dIdx].qPop[i * qIndivLen]), &(demes[dIdx].tkPop[i]));
//    }
//
//	return demes[dIdx].foundBetter;
//}


byte RunDeme(int dIdx)
{
	register int i, j, k;
	static unsigned int lastTkIndiv;
	static int nTkIndivs;

	nTkIndivs = 2 * popSize;
	lastTkIndiv = popSize - 1;

	demes[dIdx].mustSort = 0;
	demes[dIdx].foundBetter = 0;

	if (demes[dIdx].gwi >= 15000) //20000
    {
		k = (dIdx + 1) % (nDemes);
		if (demes[dIdx].bestTkIndiv.fitness < demes[k].bestTkIndiv.fitness)
		{
			CopyTkIndiv(&demes[dIdx].tkPop[0], &demes[dIdx].bestTkIndiv);
			CopyTkIndiv(&demes[dIdx].tkPop[1], &demes[k].bestTkIndiv);
		}
		else
		{
			CopyTkIndiv(&demes[dIdx].tkPop[0], &demes[k].bestTkIndiv);
			CopyTkIndiv(&demes[dIdx].tkPop[1], &demes[dIdx].bestTkIndiv);
		}
		ResetQIndiv(&demes[dIdx].qPop[0]);
		/*ResetQIndiv(&demes[dIdx].qPop[qIndivLen]);*/
		lGateStep = 1;
		LGateNoNorm(&(demes[dIdx].qPop[0])        , &demes[dIdx].tkPop[0]);
		//LGateNoNorm(&(demes[dIdx].qPop[qIndivLen]), &demes[dIdx].tkPop[1]);
		lGateStep = 0.004;
		//for (i = 2; i < popSize; i++)
		for (i = 1; i < popSize; i++)
		{
            demes[dIdx].tkPop[i].fitness = HUGE_VAL;
            demes[dIdx].tkPop[i].validFitness = HUGE_VAL;
			demes[dIdx].tkPop[i].error = HUGE_VAL;
			ResetQIndiv(&demes[dIdx].qPop[i * qIndivLen]);
        }
		printf("D%d Rst;  ", dIdx);
		demes[dIdx].gwi = 0;
    }

    for (i = 0, j = popSize; i < popSize; i++, j++)
	{
		ObserveQIndivFunc(&demes[dIdx].qPop[i * qIndivLen], &demes[dIdx].tkPop[j]);
		demes[dIdx].tkPop[j].fitness = EvalIndivFunc(trainingData, nSamplesTrain, &demes[dIdx].tkPop[j]);
        nEvaluations++;
        nEvalsExp++;

		demes[dIdx].mustSort = (demes[dIdx].tkPop[j].fitness <= demes[dIdx].tkPop[lastTkIndiv].fitness);
		
		if (demes[dIdx].tkPop[j].fitness <= demes[dIdx].bestTkIndiv.fitness)
		{
                demes[dIdx].tkPop[j].validFitness = EvalIndivFunc(validatingData, nSamplesValid, &demes[dIdx].tkPop[j]);
				//TODO: isBetter()
			    if (demes[dIdx].tkPop[j].validFitness < demes[dIdx].bestTkIndiv.validFitness ||
					(demes[dIdx].tkPop[j].fitness < demes[dIdx].bestTkIndiv.fitness && 
					 demes[dIdx].tkPop[j].validFitness < demes[dIdx].bestTkIndiv.fitness))
				{
					printf("D%i L=%i T=%.4f V=%.4f;  ", dIdx, demes[dIdx].tkPop[j].length, 
						demes[dIdx].tkPop[j].fitness, demes[dIdx].tkPop[j].validFitness);
                    CopyTkIndiv(&demes[dIdx].bestTkIndiv, &demes[dIdx].tkPop[j]);
                    demes[dIdx].foundBetter = 1;
					demes[dIdx].gwi = 0;
				}
				else
				{
					
						if (demes[dIdx].tkPop[j].fitness == demes[dIdx].bestTkIndiv.fitness &&
					        demes[dIdx].tkPop[j].validFitness == demes[dIdx].bestTkIndiv.validFitness &&
							demes[dIdx].tkPop[j].length < demes[dIdx].bestTkIndiv.length) //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
						{
							printf("D%i L=%i;  ", dIdx, demes[dIdx].tkPop[j].length);
                            CopyTkIndiv(&demes[dIdx].bestTkIndiv, &demes[dIdx].tkPop[j]);
                            demes[dIdx].foundBetter = 1;
							demes[dIdx].gwi = 0;
						}
					
				}
		}
	}
	if (demes[dIdx].mustSort)
	{
		InsertSortLen(demes[dIdx].tkPop, nTkIndivs);
		demes[dIdx].mustSort = 0;
	}
	
	NoRepeat(demes[dIdx].tkPop, nTkIndivs); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		
    for (i = 0; i < popSize; i++)
    {
        LGateNoNorm(&(demes[dIdx].qPop[i * qIndivLen]), &(demes[dIdx].tkPop[i]));
    }
	demes[dIdx].gwi++;

	return demes[dIdx].foundBetter;
}

void RunGenerationDemes(unsigned int gen, double *expBestFit)
{
	register int i;

	for (i = 0; i < nDemes; i++)
	{
		if(RunDeme(i))
		{
			if((demes[i].bestTkIndiv.fitness <= bestExpTkIndiv.fitness &&
                demes[i].bestTkIndiv.validFitness < bestExpTkIndiv.validFitness) ||
			   (demes[i].bestTkIndiv.fitness < bestExpTkIndiv.fitness &&
			    demes[i].bestTkIndiv.validFitness < demes[i].bestTkIndiv.fitness))
			{
				CopyTkIndiv(&bestExpTkIndiv, &demes[i].bestTkIndiv);
				printf("<= G=%i Best!  ", gen);
			}
		}
	}
	if((bestExpTkIndiv.fitness <= bestTkIndiv.fitness &&
	    bestExpTkIndiv.validFitness < bestTkIndiv.validFitness) ||
	   (bestExpTkIndiv.fitness < bestTkIndiv.fitness && 
	    bestExpTkIndiv.validFitness < bestTkIndiv.fitness))
	{
		CopyTkIndiv(&bestTkIndiv, &bestExpTkIndiv);
	}
}


// CEC_2009
//void RunGeneration(unsigned int gen, unsigned int *freqCount, 
//				   double *expBestFit)
//{
//	unsigned int i, r;
//	
//	for (i = 0; i < popSize; i++)
//	{
//		r = rand() % popSize;
//		ObserveQIndiv(&qPop[r * qIndivLen], &observedTkIndiv);
//		observedTkIndiv.fitness = EvalIndiv(trainingData);
//		if (observedTkIndiv.fitness <= tkPop[r].fitness)
//		{
//			CopyTkIndiv(&tkPop[r], &observedTkIndiv);
//		}
//		if (tkPop[r].fitness < *expBestFit)
//		{
//			*expBestFit = tkPop[r].fitness;
//			printf("G=%i I=%i F=%f;  ", gen, r, *expBestFit);
//		}
//		
//	}
//	for (i = 0; i < popSize; i++)
//	{
//		LGate(&qPop[i * qIndivLen], &tkPop[i]);
//	}
//}