#ifndef UTILS_H
#define UTILS_H

byte IsTkIndivBetter(struct tkIndiv * refTkIndiv, struct tkIndiv * compTkIndiv);
byte IsTkIndivBetterError(struct tkIndiv *refTkIndiv, struct tkIndiv *compTkIndiv);
void InsertSort(struct tkIndiv *tkPop, unsigned int length);
void InsertSortLen(struct tkIndiv *tkIndivs, unsigned int length);
void NoRepeatSortLen(struct tkIndiv *tkIndivs, unsigned int length);
void NoRepeat(struct tkIndiv *tkIndivs, unsigned int length);

#endif