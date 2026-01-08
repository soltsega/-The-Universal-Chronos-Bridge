#include "ui.h"
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
            case 5: manageReportFiles(); break;
            case 6:
                std::cout << "\nበሰላም ቆዩ! ስለተጠቀሙ እናመሰግናለን።" << std::endl;
                break;
            default:
                std::cout << "ያልተፈቀደ ምርጫ!" << std::endl;
        }
    } while (choice != 6);
    
    return 0;
}