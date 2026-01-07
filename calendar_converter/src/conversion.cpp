#include "conversion.h" //header file for conversion of the calendars to each other
#include "data.h"  //header file for 
#include <cmath>  //directive for mathematical operations
#include <iostream>  //for input and outpt streams
#include <iomanip>  //for formatting output
#include <ctime>  //for time operations
#include <fstream>  //for file operations
#include <sstream>  //for string operations

// External list defined in data.cpp
extern const char* WENGELAWI_LIST[4];

// Convert date to Julian Day Number (JDN)
// Gregorian: Fliegel-Van Flandern algorithm
// Ethiopian: Fixed epoch offset bridge
long dateToJDN(int days, int months, int years, bool isGregorian) {
    if (isGregorian) {
        // Integer division handles months < 3 by treating them as months 13/14 of previous year
        return (1461 * (years + 4800 + (months - 14) / 12)) / 4 +
               (367 * (months - 2 - 12 * ((months - 14) / 12))) / 12 -
               (3 * ((years + 4900 + (months - 14) / 12) / 100)) / 4 + days - 32075;
    } else {
        // 1724219 is the JDN for 0/0/0 Ethiopian (approx 7.5 years before Gregorian epoch)
        return (years - 1) * 365 + (years / 4) + (months - 1) * 30 + days + 1724219;
    }
}

// Convert JDN to Ethiopian Date
void jdnToEth(long jdn, int &days, int &months, int &years) {
    const long era = 1724221;
    long r = (jdn - era) % 1461;
    long n = (jdn - era) / 1461;
    
    years = 4 * n + (r / 365);
    if (r == 1460) years = 4 * n + 3; // Handle leap year boundary
    
    days = (r == 1460) ? 365 : r % 365;
    months = (days / 30) + 1;
    days = (days % 30) + 1;
    years += 1;
}

// Convert JDN to Islamic (Hijri) Date 
// Uses the Tabular Islamic Calendar (30-year cycle)
void jdnToIslamic(long jdn, int &days, int &months, int &years) {
    const long ISLAMIC_EPOCH = 1948439;
    long daysSinceEpoch = jdn - ISLAMIC_EPOCH;
    
    years = (30 * daysSinceEpoch + 10646) / 10631;
    // Calculate JDN of the start of this Hijri year
    long yearStart = ( (years - 1) * 354 ) + ( (11 * years + 3) / 30 ) + ISLAMIC_EPOCH;
    long dayOfYear = jdn - yearStart;
    
    // Tabular month lengths: alternating 30 and 29
    int monthDays[12] = {30, 29, 30, 29, 30, 29, 30, 29, 30, 29, 30, 29};
    
    // Check if current year is a Hijri leap year (11 years in a 30-year cycle)
    if (((11 * years + 14) % 30) < 11) {
        monthDays[11] = 30; // Dhu al-Hijjah gets 30 days
    }
    
    for (int i = 0; i < 12; i++) {
        if (dayOfYear < monthDays[i]) {
            months = i + 1;
            days = (int)dayOfYear + 1;
            return;
        }
        dayOfYear -= monthDays[i];
    }
}

// Convert JDN to Gregorian Date
void jdnToGreg(long jdn, int &days, int &months, int &years) {
    long l = jdn + 68569;
    long n = (4 * l) / 146097;
    l = l - (146097 * n + 3) / 4;
    long i = (4000 * (l + 1)) / 1461001;
    l = l - (1461 * i) / 4 + 31;
    long j = (80 * l) / 2447;
    days = (int)(l - (2447 * j) / 80);
    l = j / 11;
    months = (int)(j + 2 - (12 * l));
    years = (int)(100 * (n - 49) + i + l);
}

// Validate date inputs for specific calendar systems
bool isValidDate(int days, int months, int years, CalendarType type) {
    if (years < 1 || years > 5000) return false;
    
    if (type == GREGORIAN) {
        if (months < 1 || months > 12) return false;
        int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (months == 2 && ((years % 4 == 0 && years % 100 != 0) || (years % 400 == 0)))
            daysInMonth[1] = 29;
        return days >= 1 && days <= daysInMonth[months - 1];
    } 
    else if (type == ETHIOPIAN) {
        if (months < 1 || months > 13) return false;
        if (months <= 12) return days >= 1 && days <= 30;
        // Pagume (Month 13) has 6 days in leap years, else 5
        int maxPagume = (years % 4 == 3) ? 6 : 5;
        return days >= 1 && days <= maxPagume;
    }
    else if (type == ISLAMIC) {
        if (months < 1 || months > 12) return false;
        // Accurate tabular validation: Odd months 30, Even months 29
        int maxDay = (months % 2 != 0) ? 30 : 29;
        // Dhu al-Hijjah (Month 12) check for leap year
        if (months == 12 && ((11 * years + 14) % 30 < 11)) maxDay = 30;
        return days >= 1 && days <= maxDay;
    }
    return false;
}

// Get day index offset for Bahere Hasab calculations
int getTewsak(int dayIndex) {
    const int tewsak[] = {3, 2, 1, 0, 5, 4, 3};
    return tewsak[dayIndex % 7];
}

// Calculate ecclesiastical metadata for the Ethiopian year
void calculateBahereHasab(int inputYear, BahereHasabResults& results) {
    results.year = inputYear;
    results.ameteAlem = results.year + 5500;
    results.wengelawi = WENGELAWI_LIST[results.ameteAlem % 4];
    results.wenber = (results.ameteAlem - 1) % 19;
    results.abekte = (results.wenber * 11) % 30;
    results.metqi = (results.abekte == 0) ? 30 : (30 - results.abekte);
}

// Log holiday data to file
void writeHolidaysReport(const std::string& content) {
    std::ofstream outfile("holidays_report.txt", std::ios::app);
    if (outfile.is_open()) {
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        outfile << "[" << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "]\n"
                << content << "\n\n";
    }
}

// Process and print moveable Ethiopian holidays
void displayBahereHasabResults(const BahereHasabResults& res) {
    int rabeit = res.ameteAlem / 4;
    int meskerem1 = (res.ameteAlem + rabeit) % 7;
    int nMonth = (res.metqi > 14) ? 5 : 6; // Tikimt vs Meskerem
    int dayIdx = (meskerem1 + (nMonth == 5 ? res.metqi - 1 : res.metqi + 29)) % 7;
    int nDay = res.metqi + getTewsak(dayIdx);

    while (nDay > 30) { nDay -= 30; nMonth++; }

    std::stringstream report;
    report << "--- " << res.year << " ዓ.ም የባሕረ ሐሳብ ውጤት ---\n"
           << "ዘመነ " << res.wengelawi << " | አበቅቴ: " << res.abekte 
           << " | መጥቅዕ: " << res.metqi << "\n";

    const int offsets[] = {0, 14, 41, 62, 67, 69};
    const char* names[] = {"ጾመ ነነዌ", "ዐቢይ ጾም", "ደብረ ዘይት", "ሆሣዕና", "ስቅለት", "ትንሣኤ (ፋሲካ)"};
    
    for (int i = 0; i < 6; i++) {
        int d = nDay + offsets[i];
        int m = nMonth;
        while (d > 30) { d -= 30; m++; }
        std::cout << std::left << std::setw(18) << names[i] << " : " 
                  << MONTHS_GEEZ[m-1] << " " << d << std::endl;
        report << names[i] << " : " << MONTHS_GEEZ[m-1] << " " << d << "\n";
    }

    writeHolidaysReport(report.str());
    std::cout << "\n✓ Holidays saved to holidays_report.txt\n";
}

// Log conversion history to file
void writeToReport(const std::string& content) {
    std::ofstream outfile("report.txt", std::ios::app);
    if (outfile.is_open()) {
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        outfile << "[" << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "]\n"
                << content << "\n\n";
    }
}

// Master function to convert and display dates across systems
void convertAndDisplayDate(int d, int m, int y, CalendarType sourceType) {
    if (!isValidDate(d, m, y, sourceType)) {
        std::cout << "⚠️ Invalid date!" << std::endl;
        return;
    }

    long jdn = dateToJDN(d, m, y, sourceType == GREGORIAN);
    
    int gd, gm, gy, ed, em, ey, id, im, iy; //the variables are for gregorian, ethiopian, and islamic dates
    
    jdnToGreg(jdn, gd, gm, gy);
    jdnToEth(jdn, ed, em, ey);
    jdnToIslamic(jdn, id, im, iy);

    std::cout << "\n=== Date Conversion ===" << std::endl;
    std::cout << "Gregorian (GC)     : " << gd << "/" << gm << "/" << gy << std::endl;
    std::cout << "Ethiopian (EC)     : " << MONTHS_GEEZ[em-1] << " " << ed << ", " << ey << " ዓ.ም" << std::endl;
    std::cout << "Islamic (Hijri)    : " << id << " " << MONTHS_HIJRI[im-1] << " " << iy << " AH" << std::endl;
    std::cout << "Sidama Ayyaana     : " << SIDAMA_AYYAANA[jdn % 4] << std::endl;

    std::stringstream report;
    report << "=== Date Conversion ===\n"
           << "Gregorian (GC)     : " << gd << "/" << gm << "/" << gy << "\n"
           << "Ethiopian (EC)     : " << MONTHS_GEEZ[em-1] << " " << ed << ", " << ey << " ዓ.ም\n"
           << "Islamic (Hijri)    : " << id << " " << MONTHS_HIJRI[im-1] << " " << iy << " AH\n"
           << "Sidama Ayyaana     : " << SIDAMA_AYYAANA[jdn % 4];
    
    writeToReport(report.str());
}