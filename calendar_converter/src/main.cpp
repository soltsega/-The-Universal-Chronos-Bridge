#include "ui.h"
#include "data.h"
#include "conversion.h"
#include <iostream>
#include <locale>

int main() {
    // Set up console for UTF-8
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif
    
    std::locale::global(std::locale(""));
    std::cout.imbue(std::locale());
    
    showWelcome();
    
    int choice;
    do {
        showMainMenu();
        std::cin >> choice;
        clearInputBuffer();
        
        switch (choice) {
            case 1: showCurrentDate(); break;
            case 2: showBahereHasab(); break;
            case 3: showHelp(); break;
            case 4: convertSpecificDate(); break;
            case 5: addRecord(); break;
            case 6: showAllRecords(); break;
            case 7: searchAndConvertRecord(); break;
            case 8: updateRecord(); break;
            case 9: deleteRecord(); break;
            case 10:
                std::cout << "\nበሰላም ቆዩ! ስለተጠቀሙ እናመሰግናለን።" << std::endl;
                break;
            default:
                std::cout << "ያልተፈቀደ ምርጫ!" << std::endl;
        }
    } while (choice != 10);
    
    return 0;
}