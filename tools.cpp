//                    -*- mode:c++; tab-width:4 -*-
// File: tools.cpp                                  the tools library.
// Assorted utility routines for use in C++ programs.
// -------------------------------------------------------------------
#include "tools.hpp"
#include "Printer.hpp"

// -------------------------------------------------------------------
// I/O Manipulators.
// -------------------------------------------------------------------
// Used to discard the remainder of the current input line
istream&
cleanline( istream& is ) {
	return is.ignore(numeric_limits<streamsize>::max(), '\n');
}

// -------------------------------------------------------------------
// Used to flush an input stream buffer
//    as in myin >> x >> flush;  or cin >> flush;
istream&
flush( istream& is ) {
    return is.seekg( 0, ios::end );
}

// -------------------------------------------------------------------
// Reset formatting style from fixed or scientific back to default.
// Use: cout <<fixed <<x <<general <<y;
ostream& general( ostream& os ){
	os.unsetf( ios::floatfield );
	return os;
}

// -------------------------------------------------------------------
// Routine output labeling. ------------------------------------------
// -------------------------------------------------------------------
void
fbanner( ostream& fout ) {
    string date, time;
    when(date, time);
    fout << "\n----------------------------------------------------------------\n"
         << "\t" << NAME
         << "\n\t" << CLASS << "\n\t" <<  date << "\t" << time << '\n'
         << "----------------------------------------------------------------\n";
}

void banner() { fbanner( cout ); }

void bye() { Printer::write("\nNormal termination.\n",cerr) ; }

// -------------------------------------------------------------------
//  Error handling and error recovery functions.
// -------------------------------------------------------------------
// This function is for error messages.
//    It takes a format argument priored by any number of data arguments.
//    It formats and prints an error message, then exits.
void
fatal (const string& msg) {
    cout << flush;
    cerr << msg;
    cerr << "\nError exit\n";
    exit(1);
}

// -------------------------------------------------------------------
//  Routines for handling the time and date.
// -------------------------------------------------------------------
// Store the current date and time in the arguments.
//      System's date format is: "Fri Jun  9 10:15:55 1995\n"
//      After extraction, date is: "Fri Jun  9 1995"    hour is: "10:15:55"
void
when( string& date, string& hour ) {
    time_t now = time(nullptr);                // Get encoded date, time.
    string nowstring = ctime(&now);            // Convert to string form.
    date = nowstring.substr(0, 10); // Extract day, month, date.
    date.append(nowstring.substr(19, 5));      // Extract space and year.
    hour = nowstring.substr(11, 8); // Copy hour:minutes:seconds.
}

// -------------------------------------------------------------------
// Store the current date in the argument and return a pointer to it.
//      date format is: "Fri Jun  9 1995"
// -------------------------------------------------------------------
string
today() {
    time_t now = time(nullptr);                // Get encoded date, time.
    string nowstring = ctime(&now);            // Convert to string form.
    string date = nowstring.substr(0, 10);     // Copy hour:minutes:seconds.
    date.append(nowstring.substr(19, 5));      // Copy hour:minutes:seconds.
    return date;
}

// -------------------------------------------------------------------
// Store the current time in hour and return a pointer to it.
//      hour format is: "10:15:55"
// -------------------------------------------------------------------
string
oclock() {
    time_t now = time(nullptr);                // Get encoded date, time.
    string nowstring = ctime(&now);            // Convert to string form.
    string hour = nowstring.substr(11, 8);     // Extract hour, minutes, seconds.
    return hour;
}

// ---------------------------------------------------------------------
//  Menu handling
// ---------------------------------------------------------------------
// Display a menu then read an alphabetic menu choice character.
char menu_c( string title, int n, const string menu[], const string valid ){
    int k;
    char choice;

    for(;;) {
        Printer::write("\n" +title +"\n",cout);
        for( k=0; k<n; ++k ) Printer::write("\t "+menu[k]+"\n",cout);
        Printer::write("\n Enter code of desired item: ",cout);
        cin >> choice;
        if (valid.find(static_cast<char>(tolower(choice)))!=string::npos) break;
        Printer::write("Illegal entry, try again.\n",cout);
    }
    choice = static_cast<char>(tolower(choice));
    string st;
    st+=choice;
    Printer::write(st+"\n",cout);
    return choice;
}

// ----------------------------------------------------------------------------
// Display a menu then read and validate a numeric menu choice.
// ----------------------------------------------------------------------------
int menu( const char* title, int n, const string menu[] ){
    int choice;

    cout << '\n' << title << '\n';//she wanted this
    for(;;) {
        for( int k=0; k<n; ++k )
            cout << "\t " << (k+1) << ". " << menu[k] <<'\n';

        cout << "\n Enter number of desired item: ";
        cin >> choice >> flush;
        if ( 0 < choice && choice <= n) break;
        cout << "\n Illegal choice or input error; try again.";
    }
    return choice;
}


//----------------------------------------------------------------------------
// Case Insensitivity Checker
//-----------------------------------------------------------------------------
bool caseInsensitiveEquals(const string& str1, const string& str2) {
    if (str1.length() != str2.length()) {
        return false;
    }
    string lowerStr1 = str1;
    string lowerStr2 = str2;
    transform(lowerStr1.begin(), lowerStr1.end(), lowerStr1.begin(), ::tolower);
    transform(lowerStr2.begin(), lowerStr2.end(), lowerStr2.begin(), ::tolower);
    return lowerStr1 == lowerStr2;
}


void printSockInfo( const char* who, sockInfo sock ){
    cout <<who <<" socket is  {\n\t"
    <<" sin_family      = " <<sock.sin_family <<"\n\t"
    <<" sin_addr.s_addr = " <<inet_ntoa (sock.sin_addr) <<"\n\t" //Linux: ntop
    <<" sin_port (!!!)  = " <<ntohs(sock.sin_port) <<"\n\t};\n";
}
