#pragma once

// Include necessary headers
#include "data.h"           // For CalendarType and other data structures
#include "conversion.h"     // For date conversion functions
#include <string>           // For std::string

/**
 * @file ui.h
 * @brief User Interface module for the Calendar Converter application
 * 
 * This module handles all user interactions, including displaying menus,
 * processing user input, and showing results. It serves as the bridge between
 * the user and the core functionality of the calendar conversion system.
 */

// UI Functions

/**
 * @brief Displays the welcome message and application title
 */
void showWelcome();

/**
 * @brief Shows the main menu with all available options
 * 
 * The menu includes options for date conversion, record management,
 * and other application features.
 */
void showMainMenu();

/**
 * @brief Displays help information and usage instructions
 */
void showHelp();

/**
 * @brief Shows the current date in all supported calendar systems
 * 
 * Displays today's date in Gregorian, Ethiopian, and Islamic calendars
 * along with the corresponding day in the Sidama calendar.
 */
void showCurrentDate();

/**
 * @brief Converts a date between different calendar systems
 * 
 * Allows the user to input a date in one calendar system and see
 * its equivalent in other supported calendars.
 */
void convertSpecificDate();

/**
 * @brief Adds a new date record to the system
 * 
 * Prompts the user for date information and stores it for future reference.
 */
void addRecord();

/**
 * @brief Searches for a previously saved record and displays its conversions
 */
void searchAndConvertRecord();

/**
 * @brief Updates an existing date record
 * 
 * Allows modification of previously saved date information.
 */
void updateRecord();

/**
 * @brief Removes a date record from the system
 */
void deleteRecord();

/**
 * @brief Displays the Bahere Hasab (ባሕረ ሐሳብ) calculation results
 * 
 * Shows important Ethiopian Orthodox Church dates for a given year,
 * including fasting periods and religious holidays.
 */
void showBahereHasab();

// Utility functions

/**
 * @brief Clears the input buffer to prevent input-related issues
 * 
 * This function should be called after reading input with std::cin
 * to ensure clean input handling.
 */
void clearInputBuffer();

/**
 * @brief Gets and validates integer input from the user
 * 
 * @param prompt The message to display to the user
 * @param min Minimum allowed value (inclusive)
 * @param max Maximum allowed value (inclusive)
 * @return int The validated user input
 * 
 * This function handles input validation and will repeatedly prompt
 * the user until a valid integer within the specified range is entered.
 */
int getIntInput(const std::string& prompt, int min, int max);

