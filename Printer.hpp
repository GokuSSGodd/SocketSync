// File: Printer.hpp<br>
// Name: Murtaza & Yash<br>
// -------------------------------------------------------------
#pragma once
#include "tools.hpp"

/**
 * @class Printer<br>
 * Singleton class for unified printing to both terminal and file output.<br>
 * -------------------------------------------------------------<br>
 * - Ensures all console messages are duplicated to `output.txt`.<br>
 * - Supports direct string output and buffered stringstream output.<br>
 * - Useful for debugging, logging, and consistent message tracking.<br>
 */
class Printer {
private:
    Printer();                    ///< Constructor that opens the output file<br>
    ~Printer();                   ///< Destructor that closes the file stream<br>
    ofstream file;               ///< Output file stream<br>
    static Printer instance;     ///< Singleton instance of the Printer class<br>

public:
    /**
     * Assigns file control to the singleton instance.<br>
     * @param out An rvalue reference to an ofstream object<br>
     */
    static void getControl(ofstream&& out);

    /**
     * Writes a plain string to both the file and the provided ostream.<br>
     * @param message Message to be printed<br>
     * @param out Output stream (e.g., std::cout)<br>
     */
    static void write(const string& message, ostream& out);

    /**
     * Writes a stringstream's content to both the file and provided ostream.<br>
     * Automatically clears the stream afterward.<br>
     * @param stream String stream buffer containing the message<br>
     * @param out Output stream (e.g., std::cout)<br>
     */
    static void write(stringstream& stream, ostream& out);

    /**
     * Writes a message with a newline to both the file and the provided ostream.<br>
     * @param message The message to be written<br>
     * @param out Output stream (e.g., std::cout)<br>
     */
    static void writeln(const string& message, ostream& out);
};
