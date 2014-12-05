/*
 * ExtraFunctions.h
 *
 *  Created on: 02/10/2014
 *      Author: usuario
 */

#ifndef EXTRAFUNCTIONS_H_
#define EXTRAFUNCTIONS_H_

#include <cstdlib>
#include <time.h>

using namespace std;

void _EFinit()
{
	/*
	Declare variable to hold seconds on clock.
	*/
	time_t seconds;
	/*
	Get value from system clock and
	place in seconds variable.
	*/
	time(&seconds);
	/*
	Convert seconds to a unsigned
	integer.
	*/
	//srand((unsigned int)seconds);

	srand(time(NULL));
}

double _EFrand()
{
	/*
	Output random values.
	*/
	return (double)rand() / (double)RAND_MAX;
}

int _EFrandBounded(int __upperBoundary)
{
	/*
	Output random values.
	*/
	return (int)rand() % __upperBoundary;
}



#endif /* EXTRAFUNCTIONS_H_ */
