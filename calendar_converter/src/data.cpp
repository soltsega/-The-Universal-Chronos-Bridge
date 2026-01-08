#include "data.h"

// In data.cpp
const char* WENGELAWI_LIST[4] = {"ዮሐንስ", "ማቴዎስ", "ማርቆስ", "ሉቃስ"};
const char* MONTHS_GEEZ[13] = {"መስከረም", "ጥቅምት", "ኅዳር", "ታህሳስ", "ጥር", "የካቲት", "መጋቢት", "ሚያዝያ", "ግንቦት", "ሰኔ", "ሐምሌ", "ነሐሴ", "ጳጉሜ"};
const char* MONTHS_HIJRI[12] = {"ሙሀረም", "ሰፈር", "ረቢእ አውዋል", "ረቢእ ሰነ", "ጁማዳ አውዋል", "ጁማዳ ሰነ", "ረጃብ", "ሻዕባን", "ረመዳን", "ሻዋል", "ዙልቃዕዳ", "ዙልሐጅ"};
const char* SIDAMA_AYYAANA[4] = {"ዶራ", "ኮር", "ቦካ", "ሸፈሮ"};

// Internal storage
const int MAX_RECORDS = 100;
static CalendarRecord records[MAX_RECORDS];
static int recordCount = 0;
static int nextId = 1;
