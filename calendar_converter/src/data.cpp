// the definitions of the doxygen tags and their uses

/**
 * @brief   // Brief one-line description of what the function does
 *          // (Shows up in summaries and tooltips)
 * 
 * @param   // Documents a function parameter
 *          // Format: @param parameter_name Description
 * 
 * @return  // Describes the return value
 *          // Format: @return Description of return value
 * 
 * @note    // Important notes or additional information
 *          // Use for non-obvious implementation details
 * 
 * @warning // Highlights potential issues or gotchas
 *          // Use for critical information users should know
 * 
 * @see     // References to related functions/classes
 *          // Format: @see functionName(), ClassName
 * 
 * @details // Detailed explanation (longer than @brief)
 *          // Use for in-depth documentation
 * 
 * @code    // Start of a code example
 * int example = 42;
 * @endcode // End of code example
 * 
 * @tparam  // For template parameters
 *          // Format: @tparam T Description
 * 
 * @throws  // Documents exceptions that might be thrown
 *          // Format: @throws ExceptionType When condition
 */

#include "data.h"
#include <iostream>
#include <iomanip>    // For std::setw and other I/O manipulators

// ===========================================
// GLOBAL CONSTANTS AND VARIABLES
// ===========================================

// Maximum number of records that can be stored
const int MAX_RECORDS = 100;

// Static storage for calendar records
static CalendarRecord records[MAX_RECORDS];  // Array to store all calendar records
static int recordCount = 0;                 // Current number of records in the system
static int nextId = 1;                      // Auto-incrementing ID for new records

// ===========================================
// CALENDAR DATA
// ===========================================

// Ethiopian month names in Ge'ez script (13 months)
const char* MONTHS_GEEZ[13] = {
    "መስከረም", "ጥቅምት", "ኅዳር", "ታኅሣሥ", "ጥር", "የካቲት", "መጋቢት", 
    "ሚያዝያ", "ግንቦት", "ሰኔ", "ሐምሌ", "ነሐሴ", "ጳጉሜ"
};

// Islamic month names in English
const char* MONTHS_HIJRI[12] = {
    "Muharram", "Safar", "Rabi al-Awwal", "Rabi al-Thani",
    "Jumada al-Ula", "Jumada al-Akhira", "Rajab", "Sha'ban",
    "Ramadan", "Shawwal", "Dhu al-Qi'dah", "Dhu al-Hijjah"
};

// Ethiopian Gospel writers (used in calendar calculations)
const char* WENGELAWI_LIST[4] = {"ዮሐንስ", "ማቴዎስ", "ማርቆስ", "ሉቃስ"};

// Sidama day names (used in the Sidama calendar)
const char* SIDAMA_AYYAANA[4] = {"Dikko", "Bela", "Qawadoo", "Qululo"};

// ===========================================
// RECORD MANAGEMENT FUNCTIONS
// ===========================================

/**
 * @brief Adds a new calendar record to the system
 * @param record The record to be added (ID will be auto-assigned)
 */
void addRecord(const CalendarRecord& record) {
    // Check if storage is full
    if (recordCount >= MAX_RECORDS) {
        std::cerr << "⚠️ Storage full! Cannot add more records." << std::endl;
        return;
    }
    // Add record and assign next available ID
    records[recordCount] = record;
    records[recordCount].id = nextId++;
    recordCount++;
}

/**
 * @brief Deletes a record by its ID
 * @param id The ID of the record to delete
 * @return true if record was found and deleted, false otherwise
 */
bool deleteRecord(int id) {
    // Linear search for the record with matching ID
    for (int i = 0; i < recordCount; i++) {
        if (records[i].id == id) {
            // Shift all subsequent records left to fill the gap
            for (int j = i; j < recordCount - 1; j++) {
                records[j] = records[j + 1];
            }
            recordCount--;  // Decrease the count of records
            return true;    // Success
        }
    }
    return false;  // Record not found
}

/**
 * @brief Finds a record by its ID
 * @param id The ID of the record to find
 * @return Pointer to the record if found, nullptr otherwise
 */
CalendarRecord* findRecord(int id) {
    // Simple linear search through records
    for (int i = 0; i < recordCount; i++) {
        if (records[i].id == id) {
            return &records[i];  // Return address of the found record
        }
    }
    return nullptr;  // Record not found
}

/**
 * @brief Displays all stored records in a formatted table
 */
void showAllRecords() {
    // Check if there are any records to display
    if (recordCount == 0) {
        std::cout << "No records found." << std::endl;
        return;
    }

    // Print table header
    std::cout << "\n=== All Records ===\n";
    std::cout << std::left << std::setw(5) << "ID" 
              << std::setw(12) << "Type" 
              << "Date\n";
    std::cout << "----------------------------\n";

    // Print each record in a formatted row
    for (int i = 0; i < recordCount; i++) {
        // Convert calendar type enum to string
        std::string type;
        switch (records[i].calendarType) {
            case GREGORIAN: type = "Gregorian"; break;
            case ETHIOPIAN: type = "Ethiopian"; break;
            case ISLAMIC: type = "Islamic"; break;
        }
        // Format and print the record
        std::cout << std::left << std::setw(5) << records[i].id 
                  << std::setw(12) << type 
                  << records[i].day << "/" 
                  << records[i].month << "/" 
                  << records[i].year 
                  << std::endl;
    }
}

/**
 * @brief Gets the current number of stored records
 * @return The number of records in the system
 */
int getRecordCount() {
    return recordCount;
}