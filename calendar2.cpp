#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <ctime>
#include <limits>

using namespace std;

// --- 1. GLOBAL DATA & LABELS / ዓለም አቀፍ መረጃዎች ---

const char* MONTHS_GEEZ[] = { "መስከረም", "ጥቅምት", "ኅዳር", "ታኅሣሥ", "ጥር", "የካቲት", "መጋቢት", "ሚያዝያ", "ግንቦት", "ሰኔ", "ሐምሌ", "ነሐሴ", "ጳጉሜ" };
const char* WENGELAWI_LIST[] = {"ዮሐንስ", "ማቴዎስ", "ማርቆስ", "ሉቃስ"};
const char* SIDAMA_AYYAANA[] = {"Dikko", "Bela", "Qawadoo", "Qululo"};

// --- 2. STRUCTURE DEFINITIONS / የመረጃ ቅርጾች ---

struct BahereHasabResults {
    int year, ameteAlem, wenber, abekte, metqi;
    string wengelawi;
};

struct TripleDate {
    int gDay, gMonth, gYear;
    int eDay, eMonth, ethYear;
    string sidamaAyyaana;
};

// --- 3. FUNCTION PROTOTYPES ---
void showWelcome();
void showHelpCenter();
void calculateAndDisplay(int year);
void showCurrentDate();
void convertSpecificDate(); 
int getTewsak(int dayIndex);
int getMenuChoice();
void clearInputBuffer();

long dateToJDN(int d, int m, int y, bool isGregorian);
void jdnToEth(long jdn, int &d, int &m, int &y);

// --- 4. MAIN FUNCTION ---
int main() {
    int choice;
    showWelcome();

    do {
        choice = getMenuChoice();

        switch (choice) {
            case 1: showCurrentDate(); break;
            case 2:
                int inputYear;
                cout << "የኢትዮጵያ ዓመተ ምሕረትን ያስገቡ (1000-3000): ";
                while (!(cin >> inputYear) || inputYear < 1000 || inputYear > 3000) {
                    clearInputBuffer();
                    cout << "⚠️ ስህተት! እባክዎ ከ1000 እስከ 3000 ያሉ ትክክለኛ ዓመት ያስገቡ: ";
                }
                calculateAndDisplay(inputYear);
                break;
            case 3: showHelpCenter(); break;
            case 4: convertSpecificDate(); break;
            case 5:
                cout << "\nበሰላም ቆዩ! ስለተጠቀሙ እናመሰግናለን።" << endl;
                break;
            default:
                cout << "ያልተፈቀደ ምርጫ!" << endl;
        }
    } while (choice != 5);

    return 0;
}

// --- 5. LOGIC IMPLEMENTATION / ዝርዝር ተግባራት ---

/**
 * dateToJDN: The Universal Bridge logic.
 * የጁሊያን ቀን ቁጥር (JDN) በሁለቱ የቀን መቁጠሪያዎች መካከል እንደ ድልድይ ሆኖ ያገለግላል።
 * It converts specific dates into a continuous count of days to ensure 
 * mathematical precision during conversion.
 */
long dateToJDN(int d, int m, int y, bool isGregorian) {
    if (isGregorian) {
        // Standard Gregorian JDN math
        return (1461 * (y + 4800 + (m - 14) / 12)) / 4 +
               (367 * (m - 2 - 12 * ((m - 14) / 12))) / 12 -
               (3 * ((y + 4900 + (m - 14) / 12) / 100)) / 4 + d - 32075;
    } else {
        // Precise Ethiopian JDN formula (Epoch: 1724219)
        // የኢትዮጵያ ዘመን አቆጣጠር መነሻን (Epoch) በመጠቀም የሚሰላ ቀመር።
        return (y - 1) * 365 + (y / 4) + (m - 1) * 30 + d + 1724219;
    }
}

/**
 * jdnToEth: Reconstructs the Ethiopian date from the day count.
 * ከቀናት ድምር (JDN) ተነስቶ የኢትዮጵያን ቀን፣ ወርና ዓመት መልሶ መገንቢያ።
 * This specifically handles the 4-year cycle (1461 days) to identify Leap Years.
 */
void jdnToEth(long jdn, int &d, int &m, int &y) {
    long era = 1724221; 
    long r = (jdn - era) % 1461;
    long n = (jdn - era) / 1461;
    
    y = 4 * n + (r / 365);
    // Handling the extra day (Pagume 6) in the 4th year of the cycle.
    // በአራት ዓመት አንድ ጊዜ የሚመጣውን የጳጉሜ 6 ቀንን በትክክል ለማስላት።
    if (r == 1460) y = 4 * n + 3; 
    
    long days = (r == 1460) ? 365 : r % 365;
    
    m = (days / 30) + 1;
    d = (days % 30) + 1;
    y += 1; 
}

/**
 * convertSpecificDate: Enhanced with Robust Input Handling
 * ማንኛውንም ቀን በመፈለግ ወደ ሶስቱም የቀን መቁጠሪያዎች መለወጫ።
 * 1. Validates the Source Calendar choice.
 * 2. Ensures Day/Month/Year are numeric and within logical bounds.
 * 3. Displays result in Gregorian, Ethiopian, and Sidama calendars.
 */
void convertSpecificDate() {
    int d, m, y, type;
    
    // --- 1. Select Calendar Type with Validation ---
    cout << "\nየምንጭ የቀን መቁጠሪያ ይምረጡ (Select Source Calendar):\n";
    cout << "[1] Gregorian (GC)\n[2] Ethiopian (EC)\nምርጫ: ";
    while (!(cin >> type) || (type != 1 && type != 2)) {
        clearInputBuffer();
        cout << "⚠️ ስህተት! እባክዎ 1 ወይም 2 ብቻ ያስገቡ: ";
    }

    // --- 2. Input and Validate Date Components ---
    // Note: We use clearInputBuffer() to prevent crashes if a user types a letter.
    cout << "ዓመት ያስገቡ (Enter Year) [1000-3000]: ";
    while (!(cin >> y) || y < 1000 || y > 3000) {
        clearInputBuffer();
        cout << "⚠️ የተሳሳተ ዓመት! እባክዎ ከ1000-3000 ያስገቡ: ";
    }

    cout << "ወር ያስገቡ (Enter Month) [1-13]: ";
    while (!(cin >> m) || m < 1 || (type == 1 && m > 12) || (type == 2 && m > 13)) {
        clearInputBuffer();
        cout << "⚠️ የተሳሳተ ወር! እባክዎ በድጋሚ ያስገቡ: ";
    }

    cout << "ቀን ያስገቡ (Enter Day) [1-31]: ";
    while (!(cin >> d) || d < 1 || d > 31) {
        clearInputBuffer();
        cout << "⚠️ የተሳሳተ ቀን! እባክዎ በድጋሚ ያስገቡ: ";
    }

    // --- 3. The Universal Bridge Calculation ---
    // We convert the input to a Julian Day Number (JDN) first.
    long jdn = dateToJDN(d, m, y, (type == 1));

    // Now we extract the other calendars from that JDN.
    int ed, em, ey, gd, gm, gy;
    
    // Get Ethiopian components from JDN
    jdnToEth(jdn, ed, em, ey);
    
    // To get Gregorian components, we use a reverse JDN-to-Gregorian algorithm.
    // Logic: JDN -> Gregorian Date conversion formula.
    long l = jdn + 68569;
    long n = (4 * l) / 146097;
    l = l - (146097 * n + 3) / 4;
    long i = (4000 * (l + 1)) / 1461001;
    l = l - (1461 * i) / 4 + 31;
    long j = (80 * l) / 2447;
    gd = l - (2447 * j) / 80;
    l = j / 11;
    gm = j + 2 - (12 * l);
    gy = 100 * (n - 49) + i + l;

    // --- 4. Display Result in All Three Calendars ---
    cout << "\n========================================" << endl;
    cout << "   ሙሉ የቀን መረጃ (Full Date Conversion)" << endl;
    cout << "========================================" << endl;
    cout << left << setw(20) << "Gregorian (GC)" << ": " << gd << "/" << gm << "/" << gy << endl;
    cout << left << setw(20) << "Ethiopian (EC)" << ": " << MONTHS_GEEZ[em-1] << " " << ed << ", " << ey << " ዓ.ም" << endl;
    cout << left << setw(20) << "Sidama Ayyaana" << ": " << SIDAMA_AYYAANA[jdn % 4] << endl;
    cout << "========================================" << endl;
}

void showCurrentDate() {
    time_t t = time(0);
    tm* now = localtime(&t);
    long jdn = dateToJDN(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900, true);
    int ed, em, ey;
    jdnToEth(jdn, ed, em, ey);

    cout << "\n--- የዛሬ ቀን መረጃ ---" << endl;
    cout << "Gregorian (GC)  : " << now->tm_mday << "/" << now->tm_mon + 1 << "/" << now->tm_year + 1900 << endl;
    cout << "Ethiopian (EC)  : " << MONTHS_GEEZ[em-1] << " " << ed << ", " << ey << " ዓ.ም" << endl;
    cout << "Sidama Ayyaana  : " << SIDAMA_AYYAANA[jdn % 4] << endl;
}

void calculateAndDisplay(int inputYear) {
    BahereHasabResults res;
    res.year = inputYear;
    res.ameteAlem = res.year + 5500;
    res.wengelawi = WENGELAWI_LIST[res.ameteAlem % 4];
    res.wenber = (res.ameteAlem - 1) % 19;
    res.abekte = (res.wenber * 11) % 30;
    res.metqi = (res.abekte == 0) ? 30 : (30 - res.abekte);

    int rabeit = res.ameteAlem / 4;
    int meskerem1 = (res.ameteAlem + rabeit) % 7; 
    int nMonth = (res.metqi > 14) ? 5 : 6; 
    int dayIdx = (meskerem1 + (nMonth == 5 ? res.metqi - 1 : res.metqi + 29)) % 7;
    int nDay = res.metqi + getTewsak(dayIdx);
    
    if (nDay > 30) { nDay -= 30; nMonth++; }

    cout << "\n--- " << res.year << " ዓ.ም የባሕረ ሐሳብ ውጤት ---" << endl;
    cout << "ዘመነ " << res.wengelawi << " | አበቅቴ: " << res.abekte << " | መጥቅዕ: " << res.metqi << endl;

    int offsets[] = {0, 14, 41, 62, 67, 69};
    const char* names[] = {"ጾመ ነነዌ", "ዐቢይ ጾም", "ደብረ ዘይት", "ሆሣዕና", "ስቅለት", "ትንሣኤ (ፋሲካ)"};
    for (int i = 0; i < 6; i++) {
        int d = nDay + offsets[i];
        int m = nMonth;
        while (d > 30) { d -= 30; m++; }
        cout << left << setw(18) << names[i] << " : " << MONTHS_GEEZ[m-1] << " " << d << endl;
    }
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void showWelcome() {
    cout << "****************************************************" << endl;
    cout << "     ባሕረ ሐሳብና የቀን መቁጠሪያ (Calendar Bridge System)" << endl;
    cout << "****************************************************" << endl;
}

int getMenuChoice() {
    int choice;
    while (true) {
        cout << "\n========================================" << endl;
        cout << "  ዋና ማውጫ (Main Menu)" << endl;
        cout << "  [1] የዛሬ ቀን (Today's Date)" << endl;
        cout << "  [2] የባሕረ ሐሳብ ስሌት (Bahere Hasab)" << endl;
        cout << "  [3] መመሪያ (Help Center)" << endl;
        cout << "  [4] የቀን መለወጫ (Converter/Search)" << endl;
        cout << "  [5] መውጫ (Exit)" << endl;
        cout << "========================================" << endl;
        cout << "ምርጫዎን ያስገቡ (1-5): ";
        if (cin >> choice && choice >= 1 && choice <= 5) {
            clearInputBuffer();
            return choice;
        } else {
            clearInputBuffer();
            cout << "⚠️ ስህተት! እባክዎ ቁጥር (1-5) ብቻ ያስገቡ።" << endl;
        }
    }
}

void showHelpCenter() {
    cout << "\n--- የመረጃ ማዕከል (Help Center) ---" << endl;
    cout << "\n========================================================" << endl;
    cout << "   መመሪያና የቃላት ትርጓሜ (Help Center & Definitions)" << endl;
    cout << "========================================================" << endl;
    
    cout << "1. ዓመተ ዓለም (Amete Alem - Era of the World):" << endl;
    cout << "   - The total years since creation (Ethiopian Year + 5500)." << endl;
    cout << "   - በኢትዮጵያ አቆጣጠር ከዓለም ፍጥረት ጀምሮ ያለው የዓመታት ድምር (ዓ.ም + 5500)።" << endl;

    cout << "\n2. ወንበር (Wenber - The Chair/Metonic Cycle):" << endl;
    cout << "   - Position of the year in the 19-year lunar cycle." << endl;
    cout << "   - በ19 ዓመት የጨረቃ ዑደት ውስጥ የዓመቱ መለያ ቁጥር (ዓመተ ዓለም - 1) % 19።" << endl;

    cout << "\n3. አበቅቴ (Abekte - Epact):" << endl;
    cout << "   - The 11-day difference between solar and lunar years accumulated annually." << endl;
    cout << "   - በፀሐይና በጨረቃ ዓመታት መካከል ያለው የ11 ቀናት ልዩነት ድምር (ወንበር * 11) % 30።" << endl;

    cout << "\n4. መጥቅዕ (Metqi - The Call):" << endl;
    cout << "   - The day used to announce the start of the fasting season. (30 - Abekte)." << endl;
    cout << "   - የአጽዋማትና በዓላት ማውጫ ቀን (30 - አበቅቴ)።" << endl;

    cout << "\n5. ጁሊያን ቀን (JDN - Julian Day Number):" << endl;
    cout << "   - A continuous count of days used as a mathematical bridge between calendars." << endl;
    cout << "   - በተለያዩ የቀን መቁጠሪያዎች መካከል ቀናትን ያለምንም ስህተት ለመቀየር የሚያገለግል ድምር።" << endl;

    cout << "\n6. ሲዳማ አያና (Sidama Ayyaana):" << endl;
    cout << "   - An unbroken 4-day cycle (Dikko, Bela, Qawadoo, Qululo)." << endl;
    cout << "   - በየ4 ቀኑ የሚደጋገም የሲዳማ የዕለታት ስያሜ።" << endl;
    
    cout << "========================================================" << endl;
}

int getTewsak(int dayIndex) {
    int tewsakTable[] = {5, 2, 1, 9, 8, 7, 6};
    return tewsakTable[dayIndex];
}