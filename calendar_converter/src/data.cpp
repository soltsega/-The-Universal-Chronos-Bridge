#include "data.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

// Constants
const int MAX_RECORDS = 100;
static CalendarRecord records[MAX_RECORDS];
static int recordCount = 0;
static int nextId = 1;

// Calendar data
const char* MONTHS_GEEZ[13] = {
    "መስከረም", "ጥቅምት", "ኅዳር", "ታኅሣሥ", "ጥር", "የካቲት", "መጋቢት", 
    "ሚያዝያ", "ግንቦት", "ሰኔ", "ሐምሌ", "ነሐሴ", "ጳጉሜ"
};

const char* MONTHS_HIJRI[12] = {
    "Muharram", "Safar", "Rabi al-Awwal", "Rabi al-Thani",
    "Jumada al-Ula", "Jumada al-Akhira", "Rajab", "Sha'ban",
    "Ramadan", "Shawwal", "Dhu al-Qi'dah", "Dhu al-Hijjah"
};

const char* WENGELAWI_LIST[4] = {"ዮሐንስ", "ማቴዎስ", "ማርቆስ", "ሉቃስ"};

const char* SIDAMA_AYYAANA[4] = {"Dikko", "Bela", "Qawadoo", "Qululo"};

// Implementation of data management functions
void addRecord(const CalendarRecord& record) {
    if (recordCount >= MAX_RECORDS) {
        std::cerr << "⚠️ Storage full! Cannot add more records." << std::endl;
        return;
    }
    records[recordCount] = record;
    records[recordCount].id = nextId++;
    recordCount++;
}

bool deleteRecord(int id) {
    for (int i = 0; i < recordCount; i++) {
        if (records[i].id == id) {
            // Shift remaining elements
            for (int j = i; j < recordCount - 1; j++) {
                records[j] = records[j + 1];
            }
            recordCount--;
            return true;
        }
    }
    return false;
}

CalendarRecord* findRecord(int id) {
    for (int i = 0; i < recordCount; i++) {
        if (records[i].id == id) {
            return &records[i];
        }
    }
    return nullptr;
}

void showAllRecords() {
    if (recordCount == 0) {
        std::cout << "No records found." << std::endl;
        return;
    }

    std::cout << "\n=== All Records ===\n";
    std::cout << std::left << std::setw(5) << "ID" 
              << std::setw(12) << "Type" 
              << "Date\n";
    std::cout << "----------------------------\n";

    for (int i = 0; i < recordCount; i++) {
        std::string type;
        switch (records[i].calendarType) {
            case GREGORIAN: type = "Gregorian"; break;
            case ETHIOPIAN: type = "Ethiopian"; break;
            case ISLAMIC: type = "Islamic"; break;
        }
        std::cout << std::left << std::setw(5) << records[i].id 
                  << std::setw(12) << type 
                  << records[i].day << "/" << records[i].month << "/" << records[i].year 
                  << std::endl;
    }
}

int getRecordCount() {
    return recordCount;
}