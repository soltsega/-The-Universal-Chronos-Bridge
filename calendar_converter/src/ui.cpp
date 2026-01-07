#include "ui.h"
#include "data.h"
#include "conversion.h"
#include <iostream>
#include <ctime>
#include <limits>


// To clear the input buffer
// it clears teh user input if he/she enters invalid input and loops back
void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

//
void searchAndConvertRecord() {
    if (getRecordCount() == 0) {
        std::cout << "No records available to search." << std::endl;
        return;
    }

    showAllRecords();
    int searchId = getIntInput("\nEnter record ID to convert: ", 1, getRecordCount());
    
    CalendarRecord* record = findRecord(searchId);
    if (!record) {
        std::cout << "Record not found!" << std::endl;
        return;
    }

    // Convert the found record
    std::cout << "\n=== Converting Record ID: " << searchId << " ===" << std::endl;
    convertAndDisplayDate(record->day, record->month, record->year, record->calendarType);
}

int getIntInput(const std::string& prompt, int min, int max) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= min && value <= max) {
            clearInputBuffer();
            return value;
        }
        std::cout << "⚠️ Invalid input. Please enter a number between " 
                  << min << " and " << max << ".\n";
        clearInputBuffer();
    }
}


//gives a warm welcome message to the user when the user first opens the program
void showWelcome() {
    std::cout << "\n";
    std::cout << "====================================================\n";
    std::cout << "     ባሕረ ሐሳብና የቀን መቁጠሪያ (Calendar Bridge System)\n";
    std::cout << "====================================================\n";
    std::cout << std::endl;
}


// shows the options menu to the user
void showMainMenu() {
    std::cout << "\n";
    std::cout << "[1] Today's Date      [6] Manage Records\n";
    std::cout << "[2] Bahere Hasab       [7] Search & Convert\n";
    std::cout << "[3] Help Center       [8] Update Record\n";
    std::cout << "[4] Date Converter    [9] Delete Record\n";
    std::cout << "[5] Add Record       [10] Exit\n";
    std::cout << "----------------------------------------------------\n";
    std::cout << "Choice: ";
    std::cout.flush();
}

void showBahereHasab() {
    std::cout << "\n=== Bahere Hasab (ባሕረ ሐሳብ) ===" << std::endl;
    std::cout << "Calculate important Ethiopian Orthodox Church dates for a given year." << std::endl;
    
    // Get current Ethiopian year as default
    std::time_t now = std::time(nullptr);
    std::tm* tm_now = std::localtime(&now);
    int current_year = tm_now->tm_year + 1900;
    
    // Convert to Ethiopian year (Gregorian year - 8 years, +1 if after September)
    int default_eth_year = current_year - 8;
    if (tm_now->tm_mon >= 8) { // September or later
        default_eth_year++;
    }
    
    std::cout << "\nEnter Ethiopian year (e.g., " << default_eth_year << "): ";
    
    int year;
    if (!(std::cin >> year)) {
        // If input fails, use default year
        std::cin.clear();
        year = default_eth_year;
        std::cout << "Using default year: " << year << std::endl;
    }
    clearInputBuffer();
    
    if (year < 1 || year > 9999) {
        std::cout << "Invalid year. Please enter a year between 1 and 9999." << std::endl;
        return;
    }
    
    BahereHasabResults results;
    calculateBahereHasab(year, results);
    displayBahereHasabResults(results);
}

void showHelp() {
    std::cout << R"(
=== Help Center ===
1. Today's Date: Shows current date in all calendar systems
2. Bahere Hasab: Calculate Ethiopian calendar events
3. Help Center: Show this help message
4. Date Converter: Convert between different calendar systems
5. Add Record: Save a date for future reference
6. Manage Records: View all saved dates
7. Search & Convert: Find and convert a specific record
8. Update Record: Modify an existing record
9. Delete Record: Remove a saved date
10. Exit: Close the application


Other Features:

1. ዓመተ ዓለም፡ ዓመተ ምሕረት + 5500
2. ወንበር፡ በ19 የጨረቃ ዑደት ውስጥ ያለንበት ደረጃ
3. አበቅቴ፡ በፀሐይና በጨረቃ ዓመታት መካከል ያለው የልዩነት ቀን
4. መጥቅዕ፡ ለትንሣኤና ለዐቢይ ጾም መነሻ ቁጥር
5. ሲዳማ፡ የ4 ቀናት የአያና ዑደት (Dikko, Bela, Qawadoo, Qululo)


)" << std::endl;
}

void showCurrentDate() {
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
    
    int gd = now->tm_mday;
    int gm = now->tm_mon + 1;
    int gy = now->tm_year + 1900;
    
    std::cout << "\n=== Current Date ===" << std::endl;
    convertAndDisplayDate(gd, gm, gy, GREGORIAN);
}

void convertSpecificDate() {
    std::cout << "\n=== Date Converter ===" << std::endl;
    std::cout << "Select source calendar:\n";
    std::cout << "1. Gregorian\n2. Ethiopian\n3. Islamic\n";
    
    int choice = getIntInput("Enter choice (1-3): ", 1, 3);
    CalendarType type = static_cast<CalendarType>(choice);
    
    std::string calendarName = (type == GREGORIAN) ? "Gregorian" : 
                             (type == ETHIOPIAN) ? "Ethiopian" : "Islamic";
    
    std::cout << "\nEnter " << calendarName << " date (DD MM YYYY): ";
    int d, m, y;
    std::cin >> d >> m >> y;
    clearInputBuffer();
    
    convertAndDisplayDate(d, m, y, type);
}

void addRecord() {
    std::cout << "\n=== Add New Record ===" << std::endl;
    std::cout << "Select calendar type:\n";
    std::cout << "1. Gregorian\n2. Ethiopian\n3. Islamic\n";
    
    int choice = getIntInput("Enter choice (1-3): ", 1, 3);
    CalendarType type = static_cast<CalendarType>(choice);
    
    std::string calendarName = (type == GREGORIAN) ? "Gregorian" : 
                             (type == ETHIOPIAN) ? "Ethiopian" : "Islamic";
    
    std::cout << "\nEnter " << calendarName << " date (DD MM YYYY): ";
    int d, m, y;
    std::cin >> d >> m >> y;
    clearInputBuffer();
    
    if (!isValidDate(d, m, y, type)) {
        std::cout << "⚠️ Invalid date for " << calendarName << " calendar!\n";
        return;
    }
    
    CalendarRecord record{0, d, m, y, type};
    addRecord(record);
    std::cout << "✅ Record added successfully! (ID: " << (getRecordCount()) << ")\n";
}


void updateRecord() {
    if (getRecordCount() == 0) {
        std::cout << "No records available to update." << std::endl;
        return;
    }

    showAllRecords();
    int id = getIntInput("\nEnter record ID to update: ", 1, getRecordCount());
    
    CalendarRecord* record = findRecord(id);
    if (!record) {
        std::cout << "Record not found!" << std::endl;
        return;
    }

    std::cout << "\n=== Updating Record ID: " << id << " ===" << std::endl;
    std::cout << "Select new calendar type:\n";
    std::cout << "1. Gregorian\n2. Ethiopian\n3. Islamic\n";
    
    int choice = getIntInput("Enter choice (1-3): ", 1, 3);
    CalendarType newType = static_cast<CalendarType>(choice);
    
    std::string calendarName = (newType == GREGORIAN) ? "Gregorian" : 
                             (newType == ETHIOPIAN) ? "Ethiopian" : "Islamic";
    
    std::cout << "\nEnter new " << calendarName << " date (DD MM YYYY): ";
    int d, m, y;
    std::cin >> d >> m >> y;
    clearInputBuffer();
    
    if (!isValidDate(d, m, y, newType)) {
        std::cout << "⚠️ Invalid date for " << calendarName << " calendar!\n";
        return;
    }
    
    // Update the record
    record->day = d;
    record->month = m;
    record->year = y;
    record->calendarType = newType;
    
    std::cout << "✅ Record updated successfully!\n";
}

void deleteRecord() {
    if (getRecordCount() == 0) {
        std::cout << "No records available to delete." << std::endl;
        return;
    }

    showAllRecords();
    int id = getIntInput("\nEnter record ID to delete: ", 1, getRecordCount());
    
    if (::deleteRecord(id)) {
        std::cout << "✅ Record deleted successfully!\n";
    } else {
        std::cout << "⚠️ Failed to delete record. ID not found.\n";
    }
}

// ... (other UI functions implementation)