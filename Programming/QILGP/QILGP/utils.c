#include <stdio.h>
#include <stdlib.h>
#include "indiv.h"
#include "evolution.h"
#include "utils.h"
#include "mt19937ar.h"

void InsertSort(struct tkIndiv *tkIndivs, unsigned int length)
{
    unsigned int i, j;
	static struct tkIndiv tkIndivAux;
	static byte *genome = NULL;

	if (genome == NULL)
	{
		genome = malloc(2 * qIndivLen * sizeof(int));
		tkIndivAux.genome = genome;
	}
 
    for (i = 1; i < length; i++)
	{
		CopyTkIndiv(&tkIndivAux, &(tkIndivs[i]));
        //tkIndivAux = tkIndivs[i];
		for (j = i;(j > 0) && (tkIndivs[j-1].fitness > tkIndivAux.fitness); j--)
		{
			CopyTkIndiv(&(tkIndivs[j]), &(tkIndivs[j - 1]));
            //tkIndivs[j] = tkIndivs[j - 1];
        }
		CopyTkIndiv(&(tkIndivs[j]), &tkIndivAux);
        //tkIndivs[j] = tkIndivAux;
    }
}

void InsertSortLen(struct tkIndiv *tkIndivs, unsigned int length)
{
    unsigned int i, j;
	static struct tkIndiv tkIndivAux;
	
    for (i = 1; i < length; i++)
	{
		tkIndivAux = tkIndivs[i];
		for (j = i;
			 //(j > 0) && IsTkIndivBetterError(&tkIndivs[j-1], &tkIndivAux);
             (j > 0) && IsTkIndivBetter(&tkIndivs[j-1], &tkIndivAux);
		     j--)
		{
			tkIndivs[j] = tkIndivs[j - 1];
        }
		tkIndivs[j] = tkIndivAux;
    }
}

byte IsTkIndivBetter(struct tkIndiv *refTkIndiv, struct tkIndiv *compTkIndiv)
{	
	static byte r = 0;
	if (refTkIndiv->fitness > compTkIndiv->fitness)
	{
		return 1; 
	}
	else if (refTkIndiv->fitness == compTkIndiv->fitness)
	{
        /*if (refTkIndiv->length > compTkIndiv->length)
        {
		    return 1;
        }
        else
        {*/
		  r = ((byte)(genrand_int32()) % 2);
	      return r;
			//return 0;
        //}
	}

	return 0;
}

byte IsTkIndivBetterError(struct tkIndiv *refTkIndiv, struct tkIndiv *compTkIndiv)
{
	static byte r = 0;
	if (refTkIndiv->fitness > compTkIndiv->fitness)
	{
		return 1; 
	}
	else if (refTkIndiv->fitness == compTkIndiv->fitness)
	{
		//if (refTkIndiv->error > compTkIndiv->error)
		//{
		//	return 1;
		//}
		//else 
		//if (refTkIndiv->error == compTkIndiv->error)
		//{
			//if (refTkIndiv->length > compTkIndiv->length) //TODO: check len comp
			//{
				//return 1;
			    r = (byte)(genrand_int32()) % 2;
				return r;
			//}
		//}
	}

	return 0;
}

void NoRepeatSortLen(struct tkIndiv *tkIndivs, unsigned int length)
{
	register byte i, j;
	
	tkIndivs[0].shortest = 1;
	for (i = 1; i < length; i++)
	{
		tkIndivs[i].shortest = 0;
	}
	j = 0;
	for (i = 0; i < length; i++)
	{
		i = j;
		for (j = i + 1; j < length; j++)
		{
			if (tkIndivs[i].fitness != tkIndivs[j].fitness ||
				tkIndivs[i].error != tkIndivs[j].error)
			{
				tkIndivs[j].shortest = 1;
				break;
			}
		}
	}
	j = 2;
	for (i = 1; i < popSize; i++)
	{
		if (!tkIndivs[i].shortest)
		{
			for (j = i + 1; j < length; j++)
			{
				if (tkIndivs[j].shortest)
				{
					CopyTkIndiv(&tkIndivs[i], &tkIndivs[j]);
                    //tkIndivs[i] = tkIndivs[j];
					tkIndivs[i].shortest = 1;
					tkIndivs[j].shortest = 0;
					j++;
					break;
				}
			}
		}
	}
}



void NoRepeat(struct tkIndiv *tkIndivs, unsigned int length)
{
	register byte i, j;
	static struct tkIndiv auxTkIndiv;

	for (i = 0; i < popSize-1; i++)
	{
		for (j = i+1; j < length; j++)
		{
			if (tkIndivs[j].fitness > tkIndivs[i].fitness)
			{
				auxTkIndiv = tkIndivs[i+1];
				tkIndivs[i+1] = tkIndivs[j];
				tkIndivs[j] = auxTkIndiv;
				break;
			}
		}
	}
}

void NoRepeatClass(struct tkIndiv *tkIndivs, unsigned int length)
{
	register byte i, j;
	static struct tkIndiv auxTkIndiv;

	for (i = 0; i < popSize-1; i++)
	{
		for (j = i+1; j < length; j++)
		{
			if (tkIndivs[j].fitness > tkIndivs[i].fitness ||
				(tkIndivs[j].fitness == tkIndivs[i].fitness &&
				 tkIndivs[j].error > tkIndivs[i].error))
			{
				auxTkIndiv = tkIndivs[i+1];
				tkIndivs[i+1] = tkIndivs[j];
				tkIndivs[j] = auxTkIndiv;
				break;
			}
		}
	}
}

void GenRandList(int *outList, int length)
{
	int i, j, r, tmpLen;
	int *tmpList;
	
	tmpLen = length;
	tmpList = malloc(length * sizeof(unsigned int));
	for(i = 0; i < length; i++)
	{
		tmpList[i] = i;
	}

	for(i = 0; i < length; i++, tmpLen--)
	{
		r = rand() % tmpLen;
		outList[i] = tmpList[r];
		for (j = r; j < (tmpLen - 1); j++)
		{
			tmpList[j] = tmpList[j + 1];
		}
	}
}
