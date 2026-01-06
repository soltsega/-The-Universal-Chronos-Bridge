#pragma once
#include <string>
#include <vector>

// Calendar type constants
enum CalendarType {
    GREGORIAN = 1,
    ETHIOPIAN = 2,
    ISLAMIC = 3
};

// Bahere Hasab results structure
struct BahereHasabResults {
    int year;
    int ameteAlem;
    int wenber;
    int abekte;
    int metqi;
    const char* wengelawi;
};

// Calendar record structure
struct CalendarRecord {
    int id;
    int day;
    int month;
    int year;
    CalendarType calendarType;
};

// Global data and functions
extern const char* MONTHS_GEEZ[13];
extern const char* MONTHS_HIJRI[12];
extern const char* WENGELAWI_LIST[4];
extern const char* SIDAMA_AYYAANA[4];

// Data management functions
void addRecord(const CalendarRecord& record);
bool deleteRecord(int id);
CalendarRecord* findRecord(int id);
void showAllRecords();
int getRecordCount();