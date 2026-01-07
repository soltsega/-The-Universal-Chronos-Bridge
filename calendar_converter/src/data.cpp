#include "data.h"
#include <iostream>
#include <iomanip>

// Internal storage
const int MAX_RECORDS = 100;
static CalendarRecord records[MAX_RECORDS];
static int recordCount = 0;
static int nextId = 1;

void addRecord(const CalendarRecord& record) {
    if (recordCount >= MAX_RECORDS) {
        std::cerr << "⚠️ Storage full!" << std::endl;
        return;
    }
    records[recordCount] = record;
    records[recordCount].id = nextId++; // Professional: ID is independent of index
    recordCount++;
}

bool deleteRecord(int id) {
    for (int i = 0; i < recordCount; i++) {
        if (records[i].id == id) {
            for (int j = i; j < recordCount - 1; j++) {
                records[j] = records[j + 1];
            }
            // Professional: Wipe the last record to prevent stale data
            records[recordCount - 1] = {0, 0, 0, 0, GREGORIAN};
            recordCount--;
            return true;
        }
    }
    return false;
}

CalendarRecord* findRecord(int id) {
    for (int i = 0; i < recordCount; i++) {
        if (records[i].id == id) return &records[i];
    }
    return nullptr;
}

void showAllRecords() {
    if (recordCount == 0) {
        std::cout << "No records found." << std::endl;
        return;
    }

    std::cout << "\n" << std::setfill('=') << std::setw(30) << "" << std::setfill(' ') << "\n";
    std::cout << std::left << std::setw(6) << "ID" << std::setw(12) << "Type" << "Date\n";
    std::cout << std::setfill('-') << std::setw(30) << "" << std::setfill(' ') << "\n";

    for (int i = 0; i < recordCount; i++) {
        std::string typeStr;
        if (records[i].calendarType == GREGORIAN) typeStr = "Gregorian";
        else if (records[i].calendarType == ETHIOPIAN) typeStr = "Ethiopian";
        else typeStr = "Islamic";

        std::cout << std::left << std::setw(6) << records[i].id 
                  << std::setw(12) << typeStr 
                  << records[i].day << "/" << records[i].month << "/" << records[i].year << "\n";
    }
}

int getRecordCount() { 
    return recordCount; 
}