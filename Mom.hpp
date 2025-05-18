#pragma once
#include "tools.hpp"
#include "JobTable.hpp"
#include "Kid.hpp"

#define MAXCLIENTS 4
typedef struct pollfd toPoll;

/**
 * @class Mom<br>
 * Controller class for the server (Mom) in the client-server simulation.<br>
 * Handles job table management, socket communication, client processing, and job assignment.<br>
 */
class Mom {
private:
    JobTable table;                        ///< Shared table containing the list of jobs<br>
    const string kidNames[4] = {"Ali", "Cory", "Lee", "Pat"}; ///< Names of connected kids<br>
    vector<Job> completedJobs;            ///< Stores completed jobs for post-run analysis<br>
    time_t startTime;                     ///< Start time of simulation<br>
    time_t currentTime;                   ///< Current time during simulation<br>
    int fd = -1;                          ///< File descriptor for Mom's main socket<br>
    sockaddr_in info;                     ///< Socket address info<br>
    short entireJT[60];                   ///< Encoded job table array for transmission<br>
    short message;                        ///< Message buffer for socket communication<br>
    short kidID;                          ///< ID assigned to each connected kid<br>
    short nCli;                           ///< Number of currently active client connections<br>
    int welcomeFd;                        ///< File descriptor for the welcome socket<br>
    int status;                           ///< Return value from system calls (poll, read, write)<br>
    toPoll ufds[MAXCLIENTS + 1];          ///< Poll file descriptors array (1 welcome + MAXCLIENTS)<br>
    toPoll* const welcome = &ufds[0];     ///< Pointer to the welcome socket entry<br>
    toPoll* const worker = &ufds[1];      ///< Pointer to the array of worker socket entries<br>

    /**
     * Accepts new connections from kids and sets up poll descriptors.<br>
     */
    void addClient();

    /**
     * Handles job assignment logic for a given kid.<br>
     * @param kidIndex Index of the kid in the poll array<br>
     * @param jobChoiceIndex Index of the selected job in the job table<br>
     */
    void jobRequest(short kidIndex, short jobChoiceIndex);

    /**
     * Processes incoming messages from a specific kid.<br>
     * @param kidIndex Index of the kid in the poll array<br>
     */
    void processMessage(short kidIndex);

public:
    /**
     * Default constructor<br>
     */
    Mom() = default;

    /**
     * Default destructor<br>
     */
    ~Mom() = default;

    /**
     * Initializes the job table with 10 new jobs.<br>
     */
    void initializeJobTable();

    /**
     * Scans the job table and replaces completed jobs with new ones.<br>
     */
    void scanJobTable();

    /**
     * Main run loop for the Mom server.<br>
     * Starts server, accepts connections, polls for messages, and manages job flow.<br>
     */
    void run();

    /**
     * Custom print function to identify Mom in the output.<br>
     * @param os Output stream<br>
     * @return Reference to the output stream<br>
     */
    ostream& print(ostream& os) const;

    /**
     * Updates Mom's socket information using getsockname.<br>
     */
    void refresh();

    /**
     * Creates, binds, and listens on a socket at a given port.<br>
     * @param port Port number to bind and listen on<br>
     */
    void listen(int port);

    /**
     * Sends the current job table to a connected kid.<br>
     * @param fd File descriptor of the kid's socket<br>
     */
    void sendJobTable(int fd);
};

/**
 * Overloaded operator for printing Mom objects using streams.<br>
 * @param out Output stream<br>
 * @param sn Mom object<br>
 * @return Reference to output stream<br>
 */
inline ostream& operator <<(ostream& out, const Mom& sn) {
    return sn.print(out);
}

