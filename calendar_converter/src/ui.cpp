#include "ui.h"
#include "data.h"
#include "conversion.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <locale>
#include <cstdio>

// To clear the input buffer
// it clears teh user input if he/she enters invalid input and loops back
void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
    std::cout << "[1] Today's Date" << std::endl;
    std::cout << "[2] Bahere Hasab" << std::endl;
    std::cout << "[3] Help Center" << std::endl;
    std::cout << "[4] Date Converter" << std::endl;
    std::cout << "[5] Report/history Management" << std::endl;
    std::cout << "[6] Exit\n";
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
    
    int gregorianDays = now->tm_mday;
    int gregorianMonths = now->tm_mon + 1;
    int gregorianYears = now->tm_year + 1900;
    
    std::cout << "\n=== Current Date ===" << std::endl;
    convertAndDisplayDate(gregorianDays, gregorianMonths, gregorianYears, GREGORIAN);
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
    int days, month, year;
    std::cin >> days >> month >> year;
    clearInputBuffer();
    
    convertAndDisplayDate(days, month, year, type);
}

void manageReportFiles() {
    while (true) {
        std::cout << "\n=== Report File Management ===" << std::endl;
        std::cout << "[1] View Conversion Report (report.txt)" << std::endl;
        std::cout << "[2] View Holidays Report (holidays_report.txt)" << std::endl;
        std::cout << "[3] Clear Conversion History" << std::endl;
        std::cout << "[4] Clear Holidays History" << std::endl;
        std::cout << "[5] Back to Main Menu" << std::endl;
        std::cout << "Choice: ";
        
        int choice;
        std::cin >> choice;
        clearInputBuffer();
        
        if (choice == 5) break;
        
        std::string filename;
        std::string fileDescription;
        
        switch (choice) {
            case 1:
                filename = "report.txt";
                fileDescription = "Conversion Report";
                break;
            case 2:
                filename = "holidays_report.txt";
                fileDescription = "Holidays Report";
                break;
            case 3:
                if (std::remove("report.txt") == 0) {
                    std::cout << "✓ Conversion history cleared successfully!" << std::endl;
                } else {
                    std::cout << "⚠️ No conversion history found or error occurred." << std::endl;
                }
                continue;
            case 4:
                if (std::remove("holidays_report.txt") == 0) {
                    std::cout << "✓ Holidays history cleared successfully!" << std::endl;
                } else {
                    std::cout << "⚠️ No holidays history found or error occurred." << std::endl;
                }
                continue;
            default:
                std::cout << "⚠️ Invalid choice. Please try again." << std::endl;
                continue;
        }
        
        // For viewing files
        std::ifstream file(filename);
        if (file.is_open()) {
            std::cout << "\n=== " << fileDescription << " ===" << std::endl;
            std::string line;
            while (std::getline(file, line)) {
                std::cout << line << std::endl;
            }
            file.close();
            
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cout << "⚠️ No " << fileDescription << " found or file is empty." << std::endl;
        }
    }
}

// ... (other UI functions implementation)