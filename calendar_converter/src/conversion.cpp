#include "conversion.h"
#include "data.h"
#include <cmath>
#include <iostream>
#include <iomanip>
#include <ctime>

// External declarations from data.cpp
extern const char* WENGELAWI_LIST[4];

long dateToJDN(int d, int m, int y, bool isGregorian) {
    if (isGregorian) {
        // Fliegel-Van Flandern algorithm for Gregorian
        return (1461 * (y + 4800 + (m - 14) / 12)) / 4 +
               (367 * (m - 2 - 12 * ((m - 14) / 12))) / 12 -
               (3 * ((y + 4900 + (m - 14) / 12) / 100)) / 4 + d - 32075;
    } else {
        // Ethiopian JDN bridge
        return (y - 1) * 365 + (y / 4) + (m - 1) * 30 + d + 1724219;
    }
}

void jdnToEth(long jdn, int &d, int &m, int &y) {
    long era = 1724221;
    long r = (jdn - era) % 1461;
    long n = (jdn - era) / 1461;
    y = 4 * n + (r / 365);
    if (r == 1460) y = 4 * n + 3;
    long days = (r == 1460) ? 365 : r % 365;
    m = (days / 30) + 1;
    d = (days % 30) + 1;
    y += 1;
}

void jdnToIslamic(long jdn, int &d, int &m, int &y) {
    const long ISLAMIC_EPOCH = 1948439;
    long days = jdn - ISLAMIC_EPOCH;
    y = (30 * days + 10646) / 10631;
    long yearStart = ISLAMIC_EPOCH + 354 * (y - 1) + (3 + 11 * y) / 30;
    long dayOfYear = jdn - yearStart;
    int monthDays[12] = {30, 29, 30, 29, 30, 29, 30, 29, 30, 29, 30, 29};
    if ((11 * y + 14) % 30 < 11) monthDays[11] = 30;
    
    for (int i = 0; i < 12; i++) {
        if (dayOfYear < monthDays[i]) {
            m = i + 1;
            d = dayOfYear + 1;
            return;
        }
        dayOfYear -= monthDays[i];
    }
}

void jdnToGreg(long jdn, int &d, int &m, int &y) {
    long l = jdn + 68569;
    long n = (4 * l) / 146097;
    l = l - (146097 * n + 3) / 4;
    long i = (4000 * (l + 1)) / 1461001;
    l = l - (1461 * i) / 4 + 31;
    long j = (80 * l) / 2447;
    d = l - (2447 * j) / 80;
    l = j / 11;
    m = j + 2 - (12 * l);
    y = 100 * (n - 49) + i + l;
}

bool isValidDate(int d, int m, int y, CalendarType type) {
    if (y < 1 || y > 5000) return false;
    
    if (type == GREGORIAN) {
        if (m < 1 || m > 12) return false;
        int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (m == 2 && ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0))) {
            daysInMonth[1] = 29;
        }
        return d >= 1 && d <= daysInMonth[m - 1];
    } 
    else if (type == ETHIOPIAN) {
        if (m < 1 || m > 13) return false;
        if (m <= 12) return d >= 1 && d <= 30;
        // Pagume: 6 days if next year is leap (y%4 == 3)
        int maxPagume = (y % 4 == 3) ? 6 : 5;
        return d >= 1 && d <= maxPagume;
    }
    else if (type == ISLAMIC) {
        if (m < 1 || m > 12) return false;
        return d >= 1 && d <= 30; // Simplified for tabular
    }
    return false;
}

int getTewsak(int dayIndex) {
    const int tewsak[] = {3, 2, 1, 0, 5, 4, 3};
    return tewsak[dayIndex];
}

void calculateBahereHasab(int inputYear, BahereHasabResults& results) {
    results.year = inputYear;
    results.ameteAlem = results.year + 5500;
    results.wengelawi = WENGELAWI_LIST[results.ameteAlem % 4];
    results.wenber = (results.ameteAlem - 1) % 19;
    results.abekte = (results.wenber * 11) % 30;
    results.metqi = (results.abekte == 0) ? 30 : (30 - results.abekte);
}

void displayBahereHasabResults(const BahereHasabResults& res) {
    int rabeit = res.ameteAlem / 4;
    int meskerem1 = (res.ameteAlem + rabeit) % 7;
    int nMonth = (res.metqi > 14) ? 5 : 6;
    int dayIdx = (meskerem1 + (nMonth == 5 ? res.metqi - 1 : res.metqi + 29)) % 7;
    int nDay = res.metqi + getTewsak(dayIdx);

    if (nDay > 30) { 
        nDay -= 30; 
        nMonth++; 
    }

    std::cout << "\n--- " << res.year << " ዓ.ም የባሕረ ሐሳብ ውጤት ---" << std::endl;
    std::cout << "ዘመነ " << res.wengelawi << " | አበቅቴ: " << res.abekte 
              << " | መጥቅዕ: " << res.metqi << std::endl;

    const int offsets[] = {0, 14, 41, 62, 67, 69};
    const char* names[] = {"ጾመ ነነዌ", "ዐቢይ ጾም", "ደብረ ዘይት", "ሆሣዕና", "ስቅለት", "ትንሣኤ (ፋሲካ)"};
    
    for (int i = 0; i < 6; i++) {
        int d = nDay + offsets[i];
        int m = nMonth;
        while (d > 30) { 
            d -= 30; 
            m++; 
        }
        std::cout << std::left << std::setw(18) << names[i] << " : " 
                  << MONTHS_GEEZ[m-1] << " " << d << std::endl;
    }
}

void convertAndDisplayDate(int d, int m, int y, CalendarType sourceType) {
    if (!isValidDate(d, m, y, sourceType)) {
        std::cout << "⚠️ Invalid date!" << std::endl;
        return;
    }

    long jdn = dateToJDN(d, m, y, sourceType == GREGORIAN);
    
    int gd, gm, gy; // Gregorian
    int ed, em, ey; // Ethiopian
    int id, im, iy; // Islamic
    
    jdnToGreg(jdn, gd, gm, gy);
    jdnToEth(jdn, ed, em, ey);
    jdnToIslamic(jdn, id, im, iy);

    std::cout << "\n=== Date Conversion ===" << std::endl;
    std::cout << "Gregorian (GC)     : " << gd << "/" << gm << "/" << gy << std::endl;
    std::cout << "Ethiopian (EC)     : " << MONTHS_GEEZ[em-1] << " " << ed << ", " << ey << " ዓ.ም" << std::endl;
    std::cout << "Islamic (Hijri)    : " << id << " " << MONTHS_HIJRI[im-1] << " " << iy << " AH" << std::endl;
    std::cout << "Sidama Ayyaana     : " << SIDAMA_AYYAANA[jdn % 4] << std::endl;
}