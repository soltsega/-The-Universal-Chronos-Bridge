

// This file is for the conversion functions
// it contains the functions that convert dates between different calendar systems

#include "conversion.h"  // include the header file
#include "data.h"  // include the data file
#include <cmath>  // include the math library
#include <iostream>  // include the input output stream library
#include <iomanip>  // include the input output manipulation library
#include <ctime>  // include the time library
#include <fstream>  // include the file stream library

// External declarations from data.cpp
extern const char* WENGELAWI_LIST[4];  // external declaration of the WENGELAWI_LIST array
// It is defined in data.cpp


// convert date to JDN
// JDN is the Julian Day Number which is used to convert dates between different calendar systems
// It uses 4713 BC as the epoch
// For Gregorian it uses the Fliegel-Van Flandern algorithm
// For Ethiopian it uses the Ethiopian JDN bridge
long dateToJDN(int days, int months, int years, bool isGregorian) {
    if (isGregorian) {
        // Fliegel-Van Flandern algorithm for Gregorian
        return (1461 * (years + 4800 + (months - 14) / 12)) / 4 +
               (367 * (months - 2 - 12 * ((months - 14) / 12))) / 12 -
               (3 * ((years + 4900 + (months - 14) / 12) / 100)) / 4 + days - 32075;
    } else {
        // Ethiopian JDN bridge
        return (years - 1) * 365 + (years / 4) + (months - 1) * 30 + days + 1724219;
    }
}


// To convert JDN to Ethiopian date
// It uses the Ethiopian JDN bridge
void jdnToEth(long jdn, int &days, int &months, int &years) {
    long era = 1724221;  //This is the Ethiopian JDN bridge: The number of days from the Ethiopian epoch to the JDN epoch
    long r = (jdn - era) % 1461;  //The remainder of the division of the JDN by 1461
    // 1461 is the number of days in a 4 year cycle
    long n = (jdn - era) / 1461;  //The number of years since the Ethiopian epoch
    years = 4 * n + (r / 365);  //The year
    if (r == 1460) years = 4 * n + 3;
    days = (r == 1460) ? 365 : r % 365;
    months = (days / 30) + 1;  //The month
    days = (days % 30) + 1;  //The day
    years += 1;
}


// To convert JDN to Islamic date
// It uses the Islamic JDN bridge
void jdnToIslamic(long jdn, int &days, int &months, int &years) {
    const long ISLAMIC_EPOCH = 1948439;  // JDN of Islamic epoch (July 16, 622 CE)
    long daysSinceEpoch = jdn - ISLAMIC_EPOCH;
    years = (30 * daysSinceEpoch + 10646) / 10631;  // Use daysSinceEpoch instead of days
    long yearStart = ISLAMIC_EPOCH + 354 * (years - 1) + (3 + 11 * years) / 30;
    long dayOfYear = jdn - yearStart;
    int monthDays[12] = {30, 29, 30, 29, 30, 29, 30, 29, 30, 29, 30, 29};
    
    // Handle leap year (30 days in Dhu al-Hijjah)
    if ((11 * years + 14) % 30 < 11) {
        monthDays[11] = 30;
    }
    
    for (int monthIndex = 0; monthIndex < 12; monthIndex++) {
        if (dayOfYear < monthDays[monthIndex]) {
            months = monthIndex + 1;
            days = dayOfYear + 1;  // Now we're setting the output parameter
            return;
        }
        dayOfYear -= monthDays[monthIndex];
    }
    
    // Handle case where date is in the 13th month (shouldn't happen in Islamic calendar)
    months = 12;
    days = dayOfYear + 1;
}


// To convert JDN to Gregorian date
// It uses the Fliegel-Van Flandern algorithm
void jdnToGreg(long jdn, int &days, int &months, int &years) {
    const long GREGORIAN_EPOCH_ADJUSTMENT = 68569;  // Days between JDN epoch and March 1, 1 BCE
    const long DAYS_IN_400_YEARS = 146097;         // 400 * 365 + 97 leap days
    const long DAYS_IN_4_YEARS = 1461;             // 4 * 365 + 1 leap day
    const int DAYS_IN_MARCH = 31;                  // Days in March

    // Adjust JDN to start from March 1, 1 BCE
    long adjustedJDN = jdn + GREGORIAN_EPOCH_ADJUSTMENT;
    
    // Calculate number of 400-year cycles
    long num400YearCycles = (4 * adjustedJDN) / DAYS_IN_400_YEARS;
    adjustedJDN = adjustedJDN - (DAYS_IN_400_YEARS * num400YearCycles + 3) / 4;
    
    // Calculate number of 4-year cycles
    long num4YearCycles = (4000 * (adjustedJDN + 1)) / 1461001;
    adjustedJDN = adjustedJDN - (DAYS_IN_4_YEARS * num4YearCycles) / 4 + DAYS_IN_MARCH;
    
    // Calculate month and day
    long dayOfYear = (80 * adjustedJDN) / 2447;
    days = adjustedJDN - (2447 * dayOfYear) / 80;
    
    // Calculate month and year
    long monthOffset = dayOfYear / 11;
    months = dayOfYear + 2 - (12 * monthOffset);
    years = 100 * (num400YearCycles - 49) + num4YearCycles + monthOffset;
}

// To validate a date
// It checks if the date is valid
// It returns true if the date is valid, false otherwise
bool isValidDate(int days, int months, int years, CalendarType type) {
    if (years < 1 || years > 5000) return false;
    
    if (type == GREGORIAN) {
        if (months < 1 || months > 12) return false;
        int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (months == 2 && ((years % 4 == 0 && years % 100 != 0) || (years % 400 == 0))) {
            daysInMonth[1] = 29;
        }
        return days >= 1 && days <= daysInMonth[months - 1];
    } 
    else if (type == ETHIOPIAN) {
        if (months < 1 || months > 13) return false;
        if (months <= 12) return days >= 1 && days <= 30;
        // Pagume: 6 days if next year is leap (y%4 == 3)
        int maxPagume = (years % 4 == 3) ? 6 : 5;
        return days >= 1 && days <= maxPagume;
    }
    else if (type == ISLAMIC) {
        if (months < 1 || months > 12) return false;
        return days >= 1 && days <= 30; // Simplified for tabular
    }
    return false;
}

// To get the tewsak
// It uses the tewsak array
// they are the indexes of each day in the week
int getTewsak(int dayIndex) {
    const int tewsak[] = {3, 2, 1, 0, 5, 4, 3};  //

    return tewsak[dayIndex];
}

// To calculate Bahere Hasab
// It uses the Bahere Hasab algorithm
// It takes the input year and returns the Bahere Hasab results
void calculateBahereHasab(int inputYear, BahereHasabResults& results) {
    results.year = inputYear;
    results.ameteAlem = results.year + 5500;
    results.wengelawi = WENGELAWI_LIST[results.ameteAlem % 4];
    results.wenber = (results.ameteAlem - 1) % 19;
    results.abekte = (results.wenber * 11) % 30;
    results.metqi = (results.abekte == 0) ? 30 : (30 - results.abekte);
}


// 
void writeHolidaysReport(const std::string& content) {
    std::ofstream outfile("holidays_report.txt", std::ios::app);
    if (outfile.is_open()) {
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        outfile << "[" << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "]\n"
                << content << "\n\n";
    } else {
        std::cerr << "Error: Could not open holidays_report.txt for writing\n";
    }
}

void displayBahereHasabResults(const BahereHasabResults& res) {
    // Calculate Bahere Hasab values
    int rabeit = res.ameteAlem / 4;
    int meskerem1 = (res.ameteAlem + rabeit) % 7;
    int nMonth = (res.metqi > 14) ? 5 : 6;
    int dayIdx = (meskerem1 + (nMonth == 5 ? res.metqi - 1 : res.metqi + 29)) % 7;
    int nDay = res.metqi + getTewsak(dayIdx);

    if (nDay > 30) { 
        nDay -= 30; 
        nMonth++; 
    }

    std::stringstream report;
    report << "--- " << res.year << " ዓ.ም የባሕረ ሐሳብ ውጤት ---\n"
           << "ዘመነ " << res.wengelawi << " | አበቅቴ: " << res.abekte 
           << " | መጥቅዕ: " << res.metqi << "\n";

    const int offsets[] = {0, 14, 41, 62, 67, 69};
    const char* names[] = {"ጾመ ነነዌ", "ዐቢይ ጾም", "ደብረ ዘይት", "ሆሣዕና", "ስቅለት", "ትንሣኤ (ፋሲካ)"};
    
    for (int i = 0; i < 6; i++) {
        int d = nDay + offsets[i];
        int m = nMonth;
        while (d > 30) { 
            d -= 30; 
            m++; 
        }
        std::string holiday = names[i] + std::string(" : ") + MONTHS_GEEZ[m-1] + " " + std::to_string(d);
        std::cout << std::left << std::setw(18) << names[i] << " : " 
                  << MONTHS_GEEZ[m-1] << " " << d << std::endl;
        report << holiday << "\n";
    }

    // Write the complete report to file
    writeHolidaysReport(report.str());
    std::cout << "\n✓ Holidays saved to holidays_report.txt\n";
}



// to record the user history
void writeToReport(const std::string& content) {
    std::ofstream outfile("report.txt", std::ios::app);
    if (outfile.is_open()) {
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        outfile << "[" << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "]\n"
                << content << "\n\n";
    } else {
        std::cerr << "Error: Could not open report.txt for writing\n";
    }
}

// To convert and display date
// It uses the dateToJDN function to convert the date to JDN
// It uses the jdnToGreg function to convert the JDN to Gregorian
// It uses the jdnToEth function to convert the JDN to Ethiopian
// It uses the jdnToIslamic function to convert the JDN to Islamic
void convertAndDisplayDate(int d, int m, int y, CalendarType sourceType) {
    if (!isValidDate(d, m, y, sourceType)) {
        std::cout << "⚠️ Invalid date!" << std::endl;
        return;
    }

    long jdn = dateToJDN(d, m, y, sourceType == GREGORIAN);
    
    int gd, gm, gy; // Gregorian
    int ed, em, ey; // Ethiopian
    int id, im, iy; // Islamic
    
    // calls all the three functions to convert the JDN to the three calendar systems
    jdnToGreg(jdn, gd, gm, gy);
    jdnToEth(jdn, ed, em, ey);
    jdnToIslamic(jdn, id, im, iy);

    std::cout << "\n=== Date Conversion ===" << std::endl;
    std::cout << "Gregorian (GC)     : " << gd << "/" << gm << "/" << gy << std::endl;
    std::cout << "Ethiopian (EC)     : " << MONTHS_GEEZ[em-1] << " " << ed << ", " << ey << " ዓ.ም" << std::endl;
    std::cout << "Islamic (Hijri)    : " << id << " " << MONTHS_HIJRI[im-1] << " " << iy << " AH" << std::endl;
    std::cout << "Sidama Ayyaana     : " << SIDAMA_AYYAANA[jdn % 4] << std::endl;


    // To record the conversions toa file for future uses
    std::stringstream report;
    report << "=== Date Conversion ===\n"
           << "Gregorian (GC)     : " << gd << "/" << gm << "/" << gy << "\n"
           << "Ethiopian (EC)     : " << MONTHS_GEEZ[em-1] << " " << ed << ", " << ey << " ዓ.ም\n"
           << "Islamic (Hijri)    : " << id << " " << MONTHS_HIJRI[im-1] << " " << iy << " AH\n"
           << "Sidama Ayyaana     : " << SIDAMA_AYYAANA[jdn % 4];
    
    writeToReport(report.str());
    std::cout << "✓ Conversion saved to report.txt\n";
}