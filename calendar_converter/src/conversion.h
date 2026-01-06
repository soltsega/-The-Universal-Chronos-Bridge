#pragma once
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
int getTewsak(int dayIndex);
void calculateBahereHasab(int inputYear, BahereHasabResults& results);
void displayBahereHasabResults(const BahereHasabResults& results);