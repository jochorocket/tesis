#include <stdio.h>
#include <string.h>
#include "dataIO.h"
#include "evolution.h"

void PrintDblMatrix(unsigned int nRows, byte nCols, double **matrix)
{
	register unsigned int i,j;

	for (i = 0; i < nRows; i++)
	{
		for (j = 0; j < nCols; j++)
		{
			printf("%.4f ", matrix[i][j]);
		}
		putchar('\n');
	}
}

void PrintDblVector(int len, double *vector)
{
	register int i;

	for (i = 0; i < len; i++)
	{
		printf("%.4f ", vector[i]);
	}
	putchar('\n');
}

void PrintByteVector(int len, byte *vector)
{
	register int i;

	for (i = 0; i < len; i += 2)
	{
		printf("%x%x ", vector[i], vector[i + 1]);
	}
	putchar('\n');
}

void InitCaseVector()
{
	register int i, j;

	caseVector = (float*)malloc((nVars + nConsts) * sizeof(double));
	i = nVars;
	for (j = 0; j < nConsts; i++, j++)
	{
		caseVector[i] = consts[j];
	}
}

void InitCase(int dataRow, float *cases)
{
	register int i;

	for (i = 0; i < nVars; i++)
	{
		caseVector[i] = *(cases + dataRow * dataRowDim + i);
	}
}

void WriteOutCSV(float *trainingOut, byte *includeCycle)
{
	int e, c, f;
	byte printCycle;
	long unsigned int cIdx;
	double *tmpFits;
	FILE *fp;

	fp = fopen("QILGP_Out.csv", "w");
	tmpFits = (double*)malloc(nExperiments * sizeof(double));
	printCycle = 0;

	for (e = 0; e < nExperiments; e++)
	{
		tmpFits[e] = trainingOut[e];
	}

	for (c = 0; c < nCycles; c++)
	{
		cIdx = c * nExperiments;
		for (e = 0; e < nExperiments; e++)
		{
			if (includeCycle[cIdx + e])
			{
				tmpFits[e] = trainingOut[cIdx + e];
				printCycle = 1;
			}
		}
		if (printCycle)
		{
			fprintf(fp, "%d;", c);
			for (f = 0; f < nExperiments; f++)
			{
				fprintf(fp, "%.8f;", tmpFits[f]);
			}
			fputc('\n', fp);
			printCycle = 0;
		}
	}
	fprintf(fp, "%d;", c - 1);
	for (f = 0; f < nExperiments; f++)
	{
		fprintf(fp, "%.8f;", tmpFits[f]);
	}
	fputc('\n', fp);
}

void OutputData(float *outData)
{
	int c, eV;
	long unsigned int e, eTmp, dim;
	double *tmpFits;
	
	tmpFits = (double*)malloc(nExperiments * sizeof(double));

	dim = nExperiments * nCycles;
	for (c = 0; c < nCycles; c++)
	{
		for (e = 0; e < dim; e += nCycles)
		{
			// Check if this outData row must be included
			if (outData[c + e] != -1 || c == nCycles - 1)
			{
				for (eTmp = 0, eV = 0; eTmp < dim; eTmp += nCycles, eV++)
				{
					if (outData[c + eTmp] != -1)
					{
						tmpFits[eV] = outData[c + eTmp];
					}
					else
					{
						outData[c + eTmp] = tmpFits[eV];
					}
				}
				break;
			}
		}
	}
	WriteOutData(outData);
}

void WriteOutData(float *outData)
{
	int c;
	long unsigned int e, dim;
	FILE *fp;
	
	fp = fopen("QILGP_Out.csv", "w");

	dim = nExperiments * nCycles;
	for (c = 0; c < nCycles; c++)
	{
		if (outData[c] <= 0)
		{
			continue;
		}
		fprintf(fp, "%d;", c);
		for (e = 0; e < dim; e += nCycles)
		{
			fprintf(fp, "%.8f;", *(outData + e + c));
		}
		fputc('\n', fp);
	}

	fclose(fp);
	free(fp);
}

FILE *NewExperimOutFile(char *fName, int experimNo)
{
	char fileName[64];
	char expNoStr[4];
	static FILE *fp;

	itoa(experimNo, expNoStr, 10);
	//strcpy(fileName, "..\\..\\..\\..\\Outputs\\");
    //strcat(fileName, fName);
    strcpy(fileName, fName);
	strcat(fileName, expNoStr);
	strcat(fileName, ".txt\0");

	fp = fopen(fileName, "w");

	return fp;
}

void WriteBestProgOuts(char *fName, float *cases, int nSamples)
{
    static int i, j;
    FILE *fp;

    fp = fopen(fName, "w");
    for (i = 0; i < nSamples; i++)
    {
		InitCase(i, cases);
		RunCase(caseVector);
		fprintf(fp, "%f\n", caseVector[0]);
    }
    fclose(fp);
}

void WriteProgOuts(FILE *fp, float *cases, int nSamples)
{
    static int i, j;

    for (i = 0; i < nSamples; i++)
    {
		InitCase(i, cases);
		RunCase(caseVector);
		fprintf(fp, "%f\n", caseVector[0]);
    }
    fclose(fp);
}