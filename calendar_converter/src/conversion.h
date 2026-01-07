// This file is for the conversion functions
// it contains the functions that convert dates between different calendar systems

#pragma once  // prevent multiple inclusions
// ensures that the header file is included only once
#include "data.h"

// JDN conversion functions
long dateToJDN(int d, int m, int y, bool isGregorian = true);
void jdnToEth(long jdn, int &d, int &m, int &y);
void jdnToIslamic(long jdn, int &d, int &m, int &y);
void jdnToGreg(long jdn, int &d, int &m, int &y);

// Date validation
bool isValidDate(int d, int m, int y, CalendarType type);

// Date conversion
void convertAndDisplayDate(int d, int m, int y, CalendarType sourceType);

// Bahere Hasab functions

// get tewsak
int getTewsak(int dayIndex);
// calculate Bahere Hasab
void calculateBahereHasab(int inputYear, BahereHasabResults& results);
// display Bahere Hasab results
void displayBahereHasabResults(const BahereHasabResults& results);