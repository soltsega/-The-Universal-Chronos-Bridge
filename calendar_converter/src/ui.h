#pragma once
#include "data.h"
#include "conversion.h"

// UI Functions
void showWelcome();
void showMainMenu();
void showHelp();
void showCurrentDate();
void convertSpecificDate();
void addRecord();
void searchAndConvertRecord();
void updateRecord();
void deleteRecord();
void showBahereHasab();

// Utility functions
void clearInputBuffer();
int getIntInput(const std::string& prompt, int min, int max);