//File: Printer.cpp
//Name: Murtaza & Yash
// ------------------------------------
#include "tools.hpp"
#include "Printer.hpp"

/**
 * Static instance initialization for the singleton Printer.<br>
 * Ensures a single Printer object manages all output logging.<br>
 */
Printer Printer::instance;

/**
 * Constructor for the Printer class.<br>
 * -------------------------------------------------------<br>
 * - Opens the `output.txt` file in write mode.<br>
 * - Used to initialize the singleton Printer instance.<br>
 * -------------------------------------------------------<br>
 */
Printer::Printer() {
    instance.file.open("output.txt", ios::out);
}

/**
 * Writes a message to both the output stream and the log file.<br>
 * -------------------------------------------------------<br>
 * @param message The message string to print.<br>
 * @param out Reference to the output stream (e.g., std::cout).<br>
 * -------------------------------------------------------<br>
 */
void Printer::write(const string& message , ostream& out) {
    out<<message;
    instance.file << message;
}


/**
 * Writes the contents of a stringstream to both the console and a log file.<br>
 * -------------------------------------------------------
 * - Outputs the contents of the stream to the given `ostream` (e.g., `cout`).
 * - Also writes the same content to an internal log file.
 * - Clears the stringstream after writing to avoid duplication.
 * -------------------------------------------------------
 * @param stream Reference to the stringstream holding the content.<br>
 * @param out Output stream to write to (e.g., `cout`, `cerr`).<br>
 */
void Printer::write(stringstream& stream , ostream& out) {
    out<<stream.str();
    instance.file << stream.str();
    stream.str("");
    stream.clear();
}


/**
 * Writes a single line message to both the console and a log file, followed by a newline.<br>
 * -------------------------------------------------------
 * - Outputs the provided message to the given `ostream` (e.g., `cout`) with a newline.
 * - Also appends the same message to the internal log file with a newline.
 * -------------------------------------------------------
 * @param message The message string to be written.
 * @param out Output stream to write to (e.g., `cout`, `cerr`).
 */
void Printer::writeln(const string& message , ostream& out) {
    out<<message<<'\n';
    instance.file << message<<'\n';
}

/**
 * Destructor for the Printer class.<br>
 * -------------------------------------------------------<br>
 * - Closes the log file stream to ensure all buffered output is written.<br>
 * - Automatically invoked when the `Printer` instance is destroyed.<br>
 * -------------------------------------------------------<br>
 */
Printer::~Printer()
{
    instance.file.close();
}
